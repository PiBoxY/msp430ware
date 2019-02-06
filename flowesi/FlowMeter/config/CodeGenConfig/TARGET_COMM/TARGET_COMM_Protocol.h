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
//! TARGET_COMM_Protocol.h
//
//! A packet generator and interpretor that conforms to the FlowESI
//! communication protocol.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//! \author Modified by: MSP430 Embedded Software
//
//*****************************************************************************
#ifndef TARGET_COMM_PROTOCOL_H_
#define TARGET_COMM_PROTOCOL_H_

#include <stdio.h>
#include <stdbool.h>

//
// Include ByteQueue for queue APIs
//
#include "TARGET_COMM_ByteQueue.h"

#define addByte(buffer, byte)	(*(buffer++) = (byte))

//*****************************************************************************
//
//! Captivate Protocol Control Bytes
//
//! \def TL_PROTOCOL_SYNC defines the Captivate protocol sync byte
//! \def TL_PROTOCOL_BLANK defines a blanking byte.  This can be anything
//! other than the sync byte.
//! \def TL_PROTOCOL_READ defines the Captivate protocol read flag
//! \def TL_PROTOCOL_WRITE defines the Captivate protocol write flag
//
//*****************************************************************************
#define TL_PROTOCOL_SYNC					(0x55)
#define TL_PROTOCOL_BLANK					(0xAA)
#define TL_PROTOCOL_READ					(0x00)
#define TL_PROTOCOL_WRITE					(0x01)

//*****************************************************************************
//
//! Captivate Protocol Control Bytes
//
//! \def TL_PCKT_SERIAL_OVRHD_LENGTH specifies the serial overhead length.
//! this includes the sync, blank, and length bytes (3).
//! \def TL_PCKT_CHKSUM_LENGTH defines the length of the checksum in bytes.
//! \def TL_PARAMETER_PACKET_LENGTH defines the length of the parameter packet
//! \def TL_PARAMETER_DATA_LENGTH defines the length of the data field in the
//! parameter packet.
//
//*****************************************************************************
#define TL_PCKT_SERIAL_OVRHD_LENGTH			(0x03)
#define TL_PCKT_CHKSUM_LENGTH				(0x02)
#define TL_PARAMETER_PACKET_LENGTH			(0x0C)
#define TL_PARAMETER_DATA_LENGTH			(0x07)

//*****************************************************************************
//
//! \typedef tTLProtocolProcessingVariables holds "static" variables that the
//! protocol layer uses during packet framing.  These variables are used by 
//! the TARGET_COMM_processReceivedBytes API.
//
//*****************************************************************************
typedef struct
{
	int8_t i8currPacketIndex;
	bool bStuffedByteDetected;
} tTLProtocolProcessingVariables;

//*****************************************************************************
//
//! \typedef tParameterPacket is a union structure that stores a Captivate
//! parameter packet.  The union allows the low-level serial driver to
//! interface with a buffer, while higher-level applications can access
//! specific portions of the packet.
//
//*****************************************************************************
typedef union
{
	struct
	{
		uint8_t ui8Sync;
		uint8_t ui8Blank;
		uint8_t ui8Length;
		uint8_t ui8Command;
		uint8_t ui8Write;
		uint8_t ui8SensorID;
		uint8_t ui8Param0;
		uint8_t ui8Param1;
		uint8_t ui8Param2;
		uint8_t ui8Param3;
		uint16_t ui16Checksum;
	};
	uint8_t ui8Packet[TL_PARAMETER_PACKET_LENGTH];
} tParameterPacket;

//
// TL Protocol API Prototypes
//
extern uint16_t TARGET_COMM_buildRecalValuesPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_buildRecalStartedPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_buildSensorPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_buildTSMPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_buildDACPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_buildDebugPacket(uint8_t *pBuffer);

extern uint16_t TARGET_COMM_calculateChecksum(const uint8_t *pBuffer,
		uint16_t ui16Length);

extern bool TARGET_COMM_verifyChecksum(uint8_t *pBuffer, uint16_t ui16Length,
		uint16_t ui16Checksum);

extern uint16_t TARGET_COMM_stuffSyncBytes(uint8_t *pBuffer,
		uint16_t ui16Length);

extern bool TARGET_COMM_processingVariablesInit(
		tTLProtocolProcessingVariables *pVariables);

extern bool TARGET_COMM_processReceivedBytes(tByteQueue *pReceiveQueue,
		tParameterPacket *packet, tTLProtocolProcessingVariables *pVariables);

extern bool TARGET_COMM_parameterPacketInit(tParameterPacket *packet);

#endif /* TARGET_COMM_PROTOCOL_H_ */
