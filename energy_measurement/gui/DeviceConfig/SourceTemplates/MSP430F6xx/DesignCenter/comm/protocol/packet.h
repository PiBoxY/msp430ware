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
#ifndef PACKET_H_
#define PACKET_H_
//#############################################################################
//
//! \file   packet.h
//!
//! \brief COMM library packet API.
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include "utils/byte_queue.h"

//! \def Packet synchronization byte in serial stream
#define PACKET_SYNC (0x55)

//! \def Packet blank byte in serial stream
#define PACKET_BLANK (0xAA)

//! \def Packet payload start offset
#define PACKET_PAYLOAD_START (0x03)

//! \def Max packet payload size 62 byte ( 60 byte + 2 checksum bytes)
#define PACKET_PAYLOAD_MAX_SZ (0x3E)

//! \def Max packet payload size
#define PACKET_CHECKSUM_SZ (0x02)

//! \typedef Packet object definition
typedef struct
{
	uint8_t payload[PACKET_PAYLOAD_MAX_SZ];
	uint8_t length;
} Packet_t;

//! \brief Calculate and append the checksum to a source buffer
//! \param[in] sourceBuffer is a pointer to the source data
//! \param[in] sourceLength is the length of the source data in bytes
//! \return the new size of the data buffer with the appended checksum, in bytes
extern uint8_t Packet_appendChecksum(uint8_t *sourceBuffer, uint8_t sourceLength);

//! \brief Convert the source buffer into a outbound stream.
//!        This adds the serial transmission header, byte stuffing, and the
//!        checksum to the data and loads it into the stream buffer.
//! \param[out] streamBuffer is a pointer to the outbound data stream buffer to
//!        save the results to
//! \param[in] sourceBuffer is a pointer to the source data
//! \param[in] sourceLength is the size of the source data, in bytes
//! \return the size of the data saved into the stream buffer, in bytes
extern uint8_t Packet_convertToStream(uint8_t *streamBuffer,
		const uint8_t *sourceBuffer, uint8_t sourceLength);

//! \brief Attempt to extract a packet from the input stream (queue).
//!        The passed packet structure is used for framing, and should
//!        not be modified between calls to this function until a valid
//!        packet is framed.  This function should be called periodically
//!        to service the inbound queue, looking for packets in the stream.
//! \param[in] queue is the inbound stream byte queue
//! \param[out] packet is a pointer to the packet structure to save incoming data
//!       as it is being framed.
//! \return true if a packet was extracted, else false
extern bool Packet_extractFromStream(tByteQueue *queue, Packet_t *packet);

#endif /* PACKET_H_ */
