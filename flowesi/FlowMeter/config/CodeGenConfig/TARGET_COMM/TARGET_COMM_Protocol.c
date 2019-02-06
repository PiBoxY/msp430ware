/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

//*****************************************************************************
//! TARGET_COMM_Protocol.c
//
//! A packet generator and interpretor that conforms to the FlowESI
//! communication protocol.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//! \author Modified by: MSP430 Embedded Software
//
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup TARGET_COMM_Protocol
//! @{
//
//*****************************************************************************
#include "TARGET_COMM_Protocol.h"
#include "main.h"

// Global variables used by TARGET_COMM_buildTSMPacket to determine TSM delay
// by FlowESI GUI
extern uint8_t gTSMCH0aclk;
extern uint8_t gTSMCH0Esi0;
extern uint8_t gTSMCH0Esi1;
extern uint8_t gTSMCH0Esi2;
extern uint8_t gTSMCH0Esi3;
extern uint8_t gTSMCH0Esi4;
extern uint8_t gTSMCH1aclk;
extern uint8_t gTSMCH1Esi0;
extern uint8_t gTSMCH1Esi1;
extern uint8_t gTSMCH1Esi2;
extern uint8_t gTSMCH1Esi3;
extern uint8_t gTSMCH1Esi4;
extern uint8_t gTSMCH2aclk;
extern uint8_t gTSMCH2Esi0;
extern uint8_t gTSMCH2Esi1;
extern uint8_t gTSMCH2Esi2;
extern uint8_t gTSMCH2Esi3;
extern uint8_t gTSMCH2Esi4;


//*****************************************************************************
//
//! This API build a Sensor Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************

uint16_t TARGET_COMM_buildSensorPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 7;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here. (Showing 28 bytes for example)
	//
	addByte(pNextBufferPos, 0x70);
	addByte(pNextBufferPos, ESICNT0_H);
	addByte(pNextBufferPos, ESICNT0_L);
	addByte(pNextBufferPos, ESICNT1_H);
	addByte(pNextBufferPos, ESICNT1_L);
	addByte(pNextBufferPos, ESICNT2_H);
	addByte(pNextBufferPos, ESICNT2_L);


	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}

//*****************************************************************************
//
//! This API build a TSM Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************

uint16_t TARGET_COMM_buildTSMPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 19;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here.
	//
	addByte(pNextBufferPos, 0x87);
	addByte(pNextBufferPos,gTSMCH0aclk);
	addByte(pNextBufferPos,gTSMCH0Esi0);
	addByte(pNextBufferPos,gTSMCH0Esi1);
	addByte(pNextBufferPos,gTSMCH0Esi2);
	addByte(pNextBufferPos,gTSMCH0Esi3);
	addByte(pNextBufferPos,gTSMCH0Esi4);
	addByte(pNextBufferPos,gTSMCH1aclk);
	addByte(pNextBufferPos,gTSMCH1Esi0);
	addByte(pNextBufferPos,gTSMCH1Esi1);
	addByte(pNextBufferPos,gTSMCH1Esi2);
	addByte(pNextBufferPos,gTSMCH1Esi3);
	addByte(pNextBufferPos,gTSMCH1Esi4);
	addByte(pNextBufferPos,gTSMCH2aclk);
	addByte(pNextBufferPos,gTSMCH2Esi0);
	addByte(pNextBufferPos,gTSMCH2Esi1);
	addByte(pNextBufferPos,gTSMCH2Esi2);
	addByte(pNextBufferPos,gTSMCH2Esi3);
	addByte(pNextBufferPos,gTSMCH2Esi4);

	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}

//*****************************************************************************
//
//! This API build a DAC Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************
uint16_t TARGET_COMM_buildDACPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 17;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here.
	//
	addByte(pNextBufferPos, 0x88);
	addByte(pNextBufferPos, ESIDAC1R0_H);
	addByte(pNextBufferPos, ESIDAC1R0_L);
	addByte(pNextBufferPos, ESIDAC1R1_H);
	addByte(pNextBufferPos, ESIDAC1R1_L);
	addByte(pNextBufferPos, ESIDAC1R2_H);
	addByte(pNextBufferPos, ESIDAC1R2_L);
	addByte(pNextBufferPos, ESIDAC1R3_H);
	addByte(pNextBufferPos, ESIDAC1R3_L);
	addByte(pNextBufferPos, ESIDAC1R4_H);
	addByte(pNextBufferPos, ESIDAC1R4_L);
	addByte(pNextBufferPos, ESIDAC1R5_H);
	addByte(pNextBufferPos, ESIDAC1R5_L);
	addByte(pNextBufferPos, ESIDAC1R6_H);
	addByte(pNextBufferPos, ESIDAC1R6_L);
	addByte(pNextBufferPos, ESIDAC1R7_H);
	addByte(pNextBufferPos, ESIDAC1R7_L);


	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}

//*****************************************************************************
//
//! This API build a Debug Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************
uint16_t TARGET_COMM_buildDebugPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 11;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here.
	//
	addByte(pNextBufferPos, 0x89);
	addByte(pNextBufferPos, ESIDEBUG1_H);
	addByte(pNextBufferPos, ESIDEBUG1_L);
	addByte(pNextBufferPos, ESIDEBUG2_H);
	addByte(pNextBufferPos, ESIDEBUG2_L);
	addByte(pNextBufferPos, ESIDEBUG3_H);
	addByte(pNextBufferPos, ESIDEBUG3_L);
	addByte(pNextBufferPos, ESIDEBUG4_H);
	addByte(pNextBufferPos, ESIDEBUG4_L);
	addByte(pNextBufferPos, ESIDEBUG5_H);
	addByte(pNextBufferPos, ESIDEBUG5_L);


	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}

//*****************************************************************************
//
//! This API build a Recalibration Started Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************
uint16_t TARGET_COMM_buildRecalStartedPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 1;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here.
	//
	addByte(pNextBufferPos, 0x8A);


	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}

//*****************************************************************************
//
//! This API builds a Recalibration DAC Packet
//
//! \param pBuffer is a pointer to buffer of bytes to be send.
//
//! \return length of the buffer.
//
//*****************************************************************************
uint16_t TARGET_COMM_buildRecalValuesPacket(uint8_t *pBuffer)
{
	uint8_t *pNextBufferPos;
	uint16_t ui16Checksum;
	uint16_t ui16PayloadLength;

	pNextBufferPos = pBuffer;
	ui16PayloadLength = 17;
	addByte(pNextBufferPos, TL_PROTOCOL_SYNC);
	addByte(pNextBufferPos, TL_PROTOCOL_BLANK);
	addByte(pNextBufferPos, ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH);

	//
	// Add packet generation code here.
	//
	addByte(pNextBufferPos, 0x8C);
	addByte(pNextBufferPos, ESIDAC1R0_H);
	addByte(pNextBufferPos, ESIDAC1R0_L);
	addByte(pNextBufferPos, ESIDAC1R1_H);
	addByte(pNextBufferPos, ESIDAC1R1_L);
	addByte(pNextBufferPos, ESIDAC1R2_H);
	addByte(pNextBufferPos, ESIDAC1R2_L);
	addByte(pNextBufferPos, ESIDAC1R3_H);
	addByte(pNextBufferPos, ESIDAC1R3_L);
	addByte(pNextBufferPos, ESIDAC1R4_H);
	addByte(pNextBufferPos, ESIDAC1R4_L);
	addByte(pNextBufferPos, ESIDAC1R5_H);
	addByte(pNextBufferPos, ESIDAC1R5_L);
	addByte(pNextBufferPos, ESIDAC1R6_H);
	addByte(pNextBufferPos, ESIDAC1R6_L);
	addByte(pNextBufferPos, ESIDAC1R7_H);
	addByte(pNextBufferPos, ESIDAC1R7_L);


	//
	// Calculate and include the checksum.  The checksum is calculated from the payload start to the payload end.
	//
	ui16Checksum = TARGET_COMM_calculateChecksum((pBuffer + TL_PCKT_SERIAL_OVRHD_LENGTH), ui16PayloadLength);
	addByte(pNextBufferPos, ((uint8_t)ui16Checksum));
	addByte(pNextBufferPos, ((uint8_t)(ui16Checksum >> 8)));
	return (ui16PayloadLength + TL_PCKT_CHKSUM_LENGTH + TL_PCKT_SERIAL_OVRHD_LENGTH);
}



//*****************************************************************************
//
//! This API calculate the checksum of a byte buffer of given length.
//
//! \param pBuffer is a pointer to buffer of bytes to calculate from.
//! \param ui16Length indicates the number of valid bytes in the buffer.
//
//! \return the checksum of the buffer.
//
//*****************************************************************************
uint16_t TARGET_COMM_calculateChecksum(const uint8_t *pBuffer, uint16_t ui16Length)
{
	uint16_t ui16Checksum = 0;
	uint8_t ui8Index;

	for (ui8Index = 0; ui8Index < ui16Length; ui8Index++)
	{
		ui16Checksum += pBuffer[ui8Index];
	}

	return ui16Checksum;
}

//*****************************************************************************
//
//! This API verifies the checksum of a byte buffer of given length.
//
//! \param pBuffer is a pointer to buffer of bytes to verify.
//! \param ui16Length indicates the number of valid bytes in the buffer.
//! \param ui16Checksum is the expected checksum to verify against.
//
//! \return true of the checksum is valid, else false.
//
//*****************************************************************************
bool TARGET_COMM_verifyChecksum(uint8_t *pBuffer, uint16_t ui16Length,
		uint16_t ui16Checksum)
{
	uint16_t ui16CalcChecksum;

	ui16CalcChecksum = TARGET_COMM_calculateChecksum(pBuffer, ui16Length);

	if (ui16CalcChecksum == ui16Checksum)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//*****************************************************************************
//
//! This API parses a buffer and inserts stuffed bytes at each occurrence of the
//! sync byte in the buffer, except the first position.
//
//! \param pBuffer is a pointer to buffer of bytes to parse.
//! \param ui16Length indicates the number of valid bytes in the buffer.
//
//! \return the new length of the buffer
//
//*****************************************************************************
uint16_t TARGET_COMM_stuffSyncBytes(uint8_t *pBuffer, uint16_t ui16Length)
{
	uint16_t ui16BufferIndex;
	uint16_t ui16ShiftIndex;

	//
	// Start at index 1, since index 0 will always be a sync byte
	//
	for (ui16BufferIndex = 1; ui16BufferIndex < ui16Length; ui16BufferIndex++)
	{
		if (pBuffer[ui16BufferIndex] == TL_PROTOCOL_SYNC)
		{
			for (ui16ShiftIndex = ui16Length; ui16ShiftIndex > ui16BufferIndex;
					ui16ShiftIndex--)
			{
				pBuffer[ui16ShiftIndex] = pBuffer[ui16ShiftIndex - 1];
			}
			pBuffer[ui16BufferIndex++] = TL_PROTOCOL_SYNC;
			ui16Length++;
		}
	}

	return ui16Length;
}

//*****************************************************************************
//
//! Initialize processing variables to their default values.
//
//! \param pVariables is a pointer to the processing variable structure.
//
//! \return true if pVariables is != 0, else false.
//
//*****************************************************************************
bool TARGET_COMM_processingVariablesInit(
		tTLProtocolProcessingVariables *pVariables)
{
	if (pVariables == 0)
	{
		return false;
	}

	pVariables->bStuffedByteDetected = false;
	pVariables->i8currPacketIndex = -1;

	return true;
}

//*****************************************************************************
//
//! Parse incoming bytes from the low level serial driver, and identify
//! valid Captivate packets to be processed.  Stuffed bytes are removed here.
//
//! \param pReceiveQueue is a pointer to the queue to pull incoming bytes from
//! \param pPacket is a pointer to the parameter packet to fill
//! \param pVariables is a pointer to the processing variable structure.
//
//! \return true if a valid Captivate packet was found, else false.
//
//*****************************************************************************
bool TARGET_COMM_processReceivedBytes(tByteQueue *pReceiveQueue,
		tParameterPacket *pPacket, tTLProtocolProcessingVariables *pVariables)
{
	bool bChecksumVerified = false;

	//
	// If no packet has started framing, check to see if
	// bytes are ready to be checked
	//
	if (pVariables->i8currPacketIndex < 0)
	{
		while (TARGET_COMM_byteQueueSize(pReceiveQueue) > 2)
		{
			if (TARGET_COMM_byteQueuePull(pReceiveQueue) != TL_PROTOCOL_SYNC)
			{
				__no_operation();
				continue;
			}
			if (TARGET_COMM_byteQueuePull(pReceiveQueue) == TL_PROTOCOL_SYNC)
			{
				__no_operation();
				continue;
			}
			if (TARGET_COMM_byteQueuePull(pReceiveQueue)
					== (TL_PARAMETER_PACKET_LENGTH
							- TL_PCKT_SERIAL_OVRHD_LENGTH))
			{
				pVariables->i8currPacketIndex = 3;
				break;
			}
		}
	}

	//
	// If packet started, start framing
	//
	if (pVariables->i8currPacketIndex >= 0)
	{
		while (TARGET_COMM_byteQueueSize(pReceiveQueue) > 0)
		{
			pPacket->ui8Packet[pVariables->i8currPacketIndex] =
					TARGET_COMM_byteQueuePull(pReceiveQueue);
			if (pVariables->bStuffedByteDetected == true)
			{
				if (pPacket->ui8Packet[pVariables->i8currPacketIndex]
						== TL_PROTOCOL_SYNC)
				{
					pVariables->i8currPacketIndex++;
					pVariables->bStuffedByteDetected = false;
				}
				else
				{
					pVariables->i8currPacketIndex = -1;
					pVariables->bStuffedByteDetected = false;
					return false;
				}
			}
			else if (pPacket->ui8Packet[pVariables->i8currPacketIndex]
					== TL_PROTOCOL_SYNC)
			{
				pVariables->bStuffedByteDetected = true;
			}
			else
			{
				pVariables->i8currPacketIndex++;

			}
			if (pVariables->i8currPacketIndex == TL_PARAMETER_PACKET_LENGTH)
			{
				pVariables->i8currPacketIndex = -1;
				bChecksumVerified = TARGET_COMM_verifyChecksum(
						&(pPacket->ui8Packet[TL_PCKT_SERIAL_OVRHD_LENGTH]),
						TL_PARAMETER_DATA_LENGTH, pPacket->ui16Checksum);
				break;
			}
		}
	}
	__no_operation();
	return bChecksumVerified;
}

//*****************************************************************************
//
//! Initialize a parameter packet buffer with the sync/blank/length (overhead)
//! field values.
//
//! \param pPacket is a pointer to the parameter packet to init.
//
//! \return true if pPacket != 0, else false.
//
//*****************************************************************************
bool TARGET_COMM_parameterPacketInit(tParameterPacket *pPacket)
{
	if (pPacket == 0)
	{
		return false;
	}

	pPacket->ui8Sync = TL_PROTOCOL_SYNC;
	pPacket->ui8Blank = TL_PROTOCOL_BLANK;
	pPacket->ui8Length = TL_PARAMETER_PACKET_LENGTH
			- TL_PCKT_SERIAL_OVRHD_LENGTH;

	return true;
}

//*****************************************************************************
//
//! Close the doxygen group for TARGET_COMM_Protocol
//! @}
//
//*****************************************************************************
