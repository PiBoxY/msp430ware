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
//
//! TouchProGUI.c
//
//! TouchProGUI enables interrupt-driven UART communication
//! to the Touch Pro GUI platform tool.  The protocol is defined in
//! SLAU486 (The TouchPro GUI User's Guide).
//! The baud rate is fixed at 9600B.
//
//! Resources: UCA0 in UART Mode, USCIAB0TX_VECTOR
//!  - UCA0 BR and MOD settings need to be placed in the header file for the
//!    to provide 9600B with a given input clock.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//
//! \version 1.0 - W. Schnoor, 1/7/14: Initial Release
//
//*****************************************************************************

#include "TouchProGUI.h"

uint8_t g_ui8TouchProGUITXBuffer[TOUCHPROGUI_BUFFER_LENGTH] = { TOUCHPROGUI_SYNC_UPPER, TOUCHPROGUI_SYNC_LOWER };
uint8_t g_ui8TouchProGUITXLength = 0;

//*****************************************************************************
//
//! TouchProGUI_init()
//! This API will configure the USCI UCA0 in UART mode based upon the
//! definitions UART_BR_LOWER, UART_BR_UPPER, and UART_MOD.
//
//! /param none.
//! /return none.
//
//*****************************************************************************
void TouchProGUI_init(void)
{
	//
	// Configure Pin for UART Transmit Functionality
	//
	P1SEL |= BIT2;
	P1SEL2 |= BIT2;

	//
	// Configure USCIA0 UART based upon header file definitions
	//
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = UART_BR_LOWER;
	UCA0BR1 = UART_BR_UPPER;
	UCA0MCTL = UART_MOD;

	//
	// Release State Machine Reset
	//
	UCA0CTL1 &= ~UCSWRST;
}

//*****************************************************************************
//
//! TouchProGUI_sendData(uint16_t *ui16DataBuffer, uint8_t ui8Length)
//! This API will send a set of data to TouchPro GUI.
//
//! /param ui16DataBuffer is a pointer to the data array to send.
//! /param ui8Length specifies how many words in the ui16DataBuffer array
//! that there are to be sent.
//! /return none.
//
//*****************************************************************************
void TouchProGUI_sendData(uint16_t *ui16DataBuffer, uint8_t ui8Length)
{
	uint8_t *pTxBuffer;
	uint8_t ui8CurrentChannel;
	uint8_t ui8Checksum;

	//
	// Check to make sure the user is not trying to send more channels
	// than the buffer or TouchPro GUI supprt
	//
	if (ui8Length > TOUCHPROGUI_MAX_CHANNELS)
	{
		ui8Length = TOUCHPROGUI_MAX_CHANNELS;
	}

	//
	// Start buffer pointer at position 2 (since positions 0 and 1
	// are utilized for the sync word and are not to be altered).
	// NOTE: The check sum includes sync word.
	//
	pTxBuffer = &g_ui8TouchProGUITXBuffer[2];
	ui8Checksum = TOUCHPROGUI_SYNC_UPPER + TOUCHPROGUI_SYNC_LOWER;

	//
	// Calculate packet length (without sync word) in bytes
	// = (Length * 3) + 1
	//
	g_ui8TouchProGUITXLength = ui8Length * 3 + 1;
	*(pTxBuffer++) = g_ui8TouchProGUITXLength;
	ui8Checksum += g_ui8TouchProGUITXLength;

	//
	// Fill buffer with each channel's data
	//
	for (ui8CurrentChannel = 0; ui8CurrentChannel < ui8Length; ui8CurrentChannel++)
	{
		//
		// Channel Number
		//
		*pTxBuffer = (uint8_t)ui8CurrentChannel;
		ui8Checksum += *(pTxBuffer++);

		//
		// Channel Data Upper Byte
		//
		*pTxBuffer = (uint8_t)(ui16DataBuffer[ui8CurrentChannel] >> 8);
		ui8Checksum += *(pTxBuffer++);

		//
		// Channel Data Lower Byte
		//
		*pTxBuffer = (uint8_t)ui16DataBuffer[ui8CurrentChannel];
		ui8Checksum += *(pTxBuffer++);
	}

	//
	// Add checksum to buffer
	//
	*pTxBuffer = ui8Checksum;

	//
	// Increase packet length by three to include the sync word (0x55AA)
	// and length byte
	//
	g_ui8TouchProGUITXLength += 3;

	//
	// Start USCI_A0 transmit interrupt to
	// send the data via the USCI_A0 peripheral
	//
	IE2 |= UCA0TXIE;

	//
	// Go to LPM0 for completion of byte transmission
	//
	LPM0;

	//
	// If revived from LPMx, ensure packet got sent
	// before continuing
	//
	if (g_ui8TouchProGUITXLength != 0)
	{
		LPM0;
	}
	else
	{
		return;
	}
}

//*****************************************************************************
//
//! USCIAB0TX_VECTOR
//
//*****************************************************************************
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI_ISR(void)
{
	static uint8_t g_ui8CurrentByteIndex = 0;

	//
	// Send the next byte in the transmit buffer
	//
	UCA0TXBUF = g_ui8TouchProGUITXBuffer[g_ui8CurrentByteIndex++];

	//
	// If sending the last byte, clear interrupt enable, reset byte
	// counters, and wake back to application from LPMx.
	//
	if (g_ui8CurrentByteIndex == g_ui8TouchProGUITXLength)
	{
		g_ui8CurrentByteIndex = 0;
		g_ui8TouchProGUITXLength = 0;
		IE2 &= ~UCA0TXIE;
		LPM3_EXIT;
	}
}
