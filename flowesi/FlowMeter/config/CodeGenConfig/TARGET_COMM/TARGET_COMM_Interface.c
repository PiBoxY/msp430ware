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
//! TL_Communications.c
//
//! This is the top-level  communications layer.  It interfaces
//! between the application level, protocol layer, and the low-level 
//! serial interface.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//! \author Modified by: MSP430 Embedded Software
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup TARGET_COMM_Interface
//! @{
//
//*****************************************************************************
#include "TARGET_COMM_Interface.h"

//*****************************************************************************
//
//! \def TL_TRANSMIT_BUFFER_SIZE defines the size of the transmit buffer.  Note
//! that 2x this size will be allocated, since ping-pong buffering is used.
//! This should be at least 2x the size of the largest packet, to allow for
//! byte stuffing.
//
//*****************************************************************************
#define TL_TRANSMIT_BUFFER_SIZE			(48)

//*****************************************************************************
//
//! \def TL_RECEIVE_BUFFER_SIZE defines the size of the receive buffer.
//
//*****************************************************************************
#define TL_RECEIVE_BUFFER_SIZE			(96)

extern uint16_t gCommFlags;

//*****************************************************************************
//
//! \var g_ui8TransmitBuffer_ping is the transmit "ping" buffer, used in
//! conjunction with g_ui8TransmitBuffer_pong by g_TransmitPPBuffer.
//
//*****************************************************************************
uint8_t g_ui8TransmitBuffer_ping[TL_TRANSMIT_BUFFER_SIZE];

//*****************************************************************************
//
//! \var g_ui8TransmitBuffer_pong is the transmit "pong" buffer, used in
//! conjunction with g_ui8TransmitBuffer_ping by g_TransmitPPBuffer.
//
//*****************************************************************************
uint8_t g_ui8TransmitBuffer_pong[TL_TRANSMIT_BUFFER_SIZE];

//*****************************************************************************
//
//! \var g_ui8TransmitBuffer_pong is the receive buffer, used by g_ReceiveQueue
//! for incoming serial buffering.
//
//*****************************************************************************
uint8_t g_ui8ReceiveBuffer[TL_RECEIVE_BUFFER_SIZE];

//*****************************************************************************
//
//! \var g_TransmitPPBuffer is the transmit ping-pong buffer.
//
//*****************************************************************************
tPingPongBuffer g_TransmitPPBuffer;

//*****************************************************************************
//
//! \var g_ReceiveQueue is the receive byte queue.
//
//*****************************************************************************
tByteQueue g_ReceiveQueue;

//*****************************************************************************
//
//! \var g_parameterPacket is a structure for holding a parameter packet both
//! during framing when received, and during processing.
//
//*****************************************************************************
tParameterPacket g_parameterPacket;

//*****************************************************************************
//
//! \var tTLProtocolProcessingVariables contains the global variables used by
//! the TL_Protocol layer during incoming data processing.
//
//*****************************************************************************
tTLProtocolProcessingVariables g_protocolProcessingVars;

//*****************************************************************************
//
//! Initialize TL communications as a whole. This API will start the low-level
//! serial driver.
//!
//! \param pSensorArray is a pointer to the global array of pointers to sensors
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_init(void)
{
	//
	// Initialize receive queue
	//
	TARGET_COMM_byteQueueInit(&g_ReceiveQueue, g_ui8ReceiveBuffer,
			TL_RECEIVE_BUFFER_SIZE);

	//
	// Init the transmit ping pong buffer
	//
	TARGET_COMM_pingPongBufferInit(&g_TransmitPPBuffer, g_ui8TransmitBuffer_ping,
			g_ui8TransmitBuffer_pong);

	//
	// Init the parameter packet
	//
	TARGET_COMM_parameterPacketInit(&g_parameterPacket);
	TARGET_COMM_processingVariablesInit(&g_protocolProcessingVars);

	//
	// Start the selected serial peripheral
	// Note that after Serial_init() is called,
	// the serial peripheral will begin to buffer
	// incoming bytes in the receive queue
	//
	Serial_init(&g_ReceiveQueue);
}

//*****************************************************************************
//
//! Transmit all sensor-specific data for a given sensor.
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendSensorData(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildSensorPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Transmit all TSM-specific data.
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendTSMData(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildTSMPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Transmit all DAC-specific data.
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendDACData(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildDACPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Transmit all Debug-specific data.
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendDebugData(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildDebugPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Transmit command which informs tool recalibration has started
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendRecalStartCmd(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildRecalStartedPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Transmit command which send recalibration values
//!
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_sendRecalValues(void)
{

	uint16_t ui16Length;

	ui16Length = TARGET_COMM_buildRecalValuesPacket(g_TransmitPPBuffer.pEditBuffer);
	ui16Length = TARGET_COMM_stuffSyncBytes(g_TransmitPPBuffer.pEditBuffer,
			ui16Length);

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Transmit new sensor packet
	// (via a call into the low-level serial COM driver)
	//
	TARGET_COMM_pingPongBufferToggle(&g_TransmitPPBuffer);
	Serial_write(g_TransmitPPBuffer.pTransmitBuffer, ui16Length);
}

//*****************************************************************************
//
//! Call into the protocol layer to determine if any parameter packets have
//! been received.  THis API should be called periodically in the background
//! by the application layer.
//!
//! \param none.
//! \return none.
//
//*****************************************************************************
void TARGET_COMM_checkForReceivedPacket(void)
{
	bool bGotPacket;

	bGotPacket = TARGET_COMM_processReceivedBytes(&g_ReceiveQueue,
			&g_parameterPacket, &g_protocolProcessingVars);

	if (bGotPacket == true)
	{

		switch(g_parameterPacket.ui8Packet[3]){

			case 0x80:
					gCommFlags |= START_CONFIG_ESI_MODULE;
				break;

			case 0x81:
				if(gCommFlags & COMM_CAL_DONE){
					gCommFlags |= PERIODIC_COUNTER_FLUSH_ENABLED;

					Timer_A_startCounter(TIMER_TX_SELECTION,
									     TIMER_A_CONTINUOUS_MODE);
				}
				break;

			case 0x82:
				if(gCommFlags & COMM_CAL_DONE){
					gCommFlags &= ~PERIODIC_COUNTER_FLUSH_ENABLED;

					if(!(gCommFlags & DEBUG_REGISTER_FLUSH_ENABLED)){
						Timer_A_stop(TIMER_TX_SELECTION);
					}
				}
				break;
			case 0x83:
				if(gCommFlags & COMM_CAL_DONE){
					gCommFlags |= DEBUG_REGISTER_FLUSH_ENABLED;

					Timer_A_startCounter(TIMER_TX_SELECTION,
										 TIMER_A_CONTINUOUS_MODE);
				}
				break;

			case 0x84:
				if(gCommFlags & COMM_CAL_DONE){
					gCommFlags &= ~DEBUG_REGISTER_FLUSH_ENABLED;

					if(!(gCommFlags & PERIODIC_COUNTER_FLUSH_ENABLED)){
						Timer_A_stop(TIMER_TX_SELECTION);
					}
				}
				break;
			default:
				break;

		}

	}
}

//*****************************************************************************
//
//! Close the doxygen group for TARGET_COMM_Interface
//! @}
//
//*****************************************************************************
