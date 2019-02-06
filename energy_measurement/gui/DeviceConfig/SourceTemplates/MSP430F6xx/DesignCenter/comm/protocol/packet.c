/* --COPYRIGHT--,BSD
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * --/COPYRIGHT--*/
//#############################################################################
//
//! \file   packet.c
//!
//! \brief
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include "protocol/checksum.h"
#include "packet.h"

static inline uint8_t* Packet_stuffByte(uint8_t *nextPosition, uint8_t byte);

uint8_t Packet_appendChecksum(uint8_t *sourceBuffer, uint8_t sourceLength)
{
	uint16_t checksum;

	checksum = Checksum_computeChecksum(sourceBuffer, sourceLength);
	sourceBuffer[sourceLength] = (uint8_t)(checksum);
	sourceBuffer[sourceLength+1] = (uint8_t)(checksum >> 8);

	return sourceLength + 2;
}

uint8_t Packet_convertToStream(uint8_t *streamBuffer, const uint8_t *sourceBuffer,
    uint8_t sourceLength)
{
	uint16_t checksum;
	uint8_t *streamPtr;
	uint8_t sourceIndex;

	//
	// Load the serial packet information
	//
	streamPtr = streamBuffer;
	*(streamPtr++) = PACKET_SYNC;
	*(streamPtr++) = PACKET_BLANK;
	*(streamPtr++) = sourceLength + PACKET_CHECKSUM_SZ;

	//
	// Add the packet
	//
	for (sourceIndex=0; sourceIndex<sourceLength; sourceIndex++)
	{
		streamPtr = Packet_stuffByte(streamPtr, sourceBuffer[sourceIndex]);
	}

	//
	// Add the checksum
	//
	checksum = Checksum_computeChecksum(sourceBuffer, sourceLength);
	streamPtr = Packet_stuffByte(streamPtr, (uint8_t)(checksum));
	streamPtr = Packet_stuffByte(streamPtr, (uint8_t)(checksum >> 8));

	return streamPtr - streamBuffer;
}

bool Packet_extractFromStream(tByteQueue *queue, Packet_t *packet)
{
	static int8_t currIndex = -1;
	static bool detectedStuffedByte = false;
	uint16_t checksum;
	uint8_t x;
	bool checksumVerified = false;

	//
	// If no packet has started framing, check to see if
	// bytes are ready to be checked
	//
	if (currIndex < 0)
	{
		while (ByteQueue_getLength(queue) > 2)
		{
			ByteQueue_pull(queue, &x);
			if (x != PACKET_SYNC)
			{
				continue;
			}
			ByteQueue_pull(queue, &x);
			if (x == PACKET_SYNC)
			{
				continue;
			}
			ByteQueue_pull(queue, &(packet->length));
			if (packet->length > PACKET_PAYLOAD_MAX_SZ)
			{
				currIndex = -1;
				return false;
			}
			currIndex = 0;
			packet->length -= PACKET_CHECKSUM_SZ;
			break;
		}
	}

	//
	// If packet started, start framing
	//
	if (currIndex >= 0)
	{
		while (ByteQueue_getLength(queue) > 0)
		{
			ByteQueue_pull(queue, &(packet->payload[currIndex]));
			if (detectedStuffedByte == true)
			{
				if (packet->payload[currIndex]
						== PACKET_SYNC)
				{
					currIndex++;
					detectedStuffedByte = false;
				}
				else
				{
					currIndex = -1;
					detectedStuffedByte = false;
					return false;
				}
			}
			else if (packet->payload[currIndex]
					== PACKET_SYNC)
			{
				detectedStuffedByte = true;
			}
			else
			{
				currIndex++;

			}
			if (currIndex == packet->length + PACKET_CHECKSUM_SZ)
			{
				currIndex = -1;
				checksum = packet->payload[packet->length];
				checksum |= ((uint16_t)(packet->payload[packet->length+1])) << 8;
				checksumVerified = Checksum_verifyChecksum(
						packet->payload,
						packet->length,
						checksum
					);
				break;
			}
		}
	}

	return checksumVerified;
}

uint8_t* Packet_stuffByte(uint8_t *nextPosition, uint8_t byte)
{
	//
	// Copy the byte to the next position
	//
	*(nextPosition++) = byte;

	//
	// If the byte is a sync byte, copy it again (stuff it...)
	//
	if (byte == PACKET_SYNC)
	{
		*(nextPosition++) = PACKET_SYNC;
	}

	//
	// Return the next position pointer
	//
	return nextPosition;
}
