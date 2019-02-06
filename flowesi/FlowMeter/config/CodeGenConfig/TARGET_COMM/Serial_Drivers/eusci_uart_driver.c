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
//! eusci_uart_driver.h
//
//! This driver enables UART communication via MSP430 Driver Library API calls
//! into the selected eUSCI_A UART peripheral.
//! Transmit and receive functions are fully interrupt driven. For receive,
//! a ByteQueue structure is utilized to provide ring buffering.  This is also
//! the interface to the application.  For transmit, an API is provided that
//! will send a buffer of a given length over the UART interface in 
//! the background. 
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//
//! \version Release 1
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup eusci_uart_driver
//! @{
//
//*****************************************************************************
#include "eusci_uart_driver.h"

//*****************************************************************************
//
//! \var g_bUARTTransmitting is a boolean value that indicates to the
//! application of a transmission is in progress.  When a transmission is in
//! progress, a new transmission should not be started.
//
//*****************************************************************************
bool g_bUARTTransmitting;

//*****************************************************************************
//
//! \var g_pTransmitBuffer is a pointer to the current transmit buffer, so
//! that the interrupt service routine knows where to find it.
//
//*****************************************************************************
uint8_t *g_pTransmitBuffer;

//*****************************************************************************
//
//! \var g_ui16TransmitLength holds the length of the transmit buffer pointed
//! to by g_pTransmitBuffer, so that the interrupt service routine knows
//! how long the transmit buffer is.
//
//*****************************************************************************
uint16_t g_ui16TransmitLength;

//*****************************************************************************
//
//! \var g_pReceiveQueue is a pointer to the receive ByteQueue to use to buffer
//! incoming bytes.
//
//*****************************************************************************
tByteQueue *g_pReceiveQueue;

//*****************************************************************************
//
//! Initialize the eUSCI_A UART peripheral to be used.
//
//! \param pReceiveQueue is a pointer to the receive ByteQueue to be used for 
//! buffering incoming bytes.
//
//! \return none.
//
//*****************************************************************************
void EUSCI_UART_init(tByteQueue* pReceiveQueue)
{
	//
	// Store off pointer to receive queue
	//
	g_pReceiveQueue = pReceiveQueue;

	//
	// Clear transmitting flag
	//
	g_bUARTTransmitting = false;

	//
	// Configure the selected eUSCI UART (UCAx) via DriverLib calls
	// Enable UART receive interrupts.  Bytes received over UART go
	// into the UART Rx Queue (g_ReceiveQueue);
	GPIO_setAsPeripheralModuleFunctionInputPin(
			UART_TX_PORT,
			UART_TX_PIN,
			GPIO_PRIMARY_MODULE_FUNCTION);

	GPIO_setAsPeripheralModuleFunctionInputPin(
			UART_RX_PORT,
			UART_RX_PIN,
			GPIO_PRIMARY_MODULE_FUNCTION);

	EUSCI_A_UART_initParam uartParam =
	{
		EUSCI_A_UART_CLOCKSOURCE_SMCLK,
		UART_PRESCALER,
		UART_FIRST_STAGE_MOD,
		UART_SECOND_STAGE_MOD,
		EUSCI_A_UART_NO_PARITY,
		EUSCI_A_UART_LSB_FIRST,
		EUSCI_A_UART_ONE_STOP_BIT,
		EUSCI_A_UART_MODE,
		UART_OVERSAMPLING
	};

	EUSCI_A_UART_init(EUSCI_A_SELECTION, &uartParam);
	EUSCI_A_UART_enable(EUSCI_A_SELECTION);
	EUSCI_A_UART_enableInterrupt(EUSCI_A_SELECTION,
			EUSCI_A_UART_RECEIVE_INTERRUPT);
}

//*****************************************************************************
//
//! Transmit a buffer via the eUSCI_A UART peripheral.  This is a non-blocking
//! API that operates in the background.
//
//! \param pBuffer is a pointer to the buffer to transmit.
//! \param ui16Length specifies the number of valid bytes to transmit in
//! pBuffer. 
//
//! \return none.
//
//*****************************************************************************
void EUSCI_UART_write(uint8_t *pBuffer, uint16_t ui16Length)
{
	//
	// Break out if no valid packet data exists
	//
	if ((ui16Length == 0) || (pBuffer == 0))
	{
		return;
	}

	//
	// Wait for any previous writes to finish transmission
	//
	while (g_bUARTTransmitting == true)
	{
		LPM0;
	}

	//
	// Store off transmit buffer, transmit length
	//
	g_pTransmitBuffer = pBuffer;
	g_ui16TransmitLength = ui16Length;

	//
	// Start transmission
	//
	g_bUARTTransmitting = true;
	EUSCI_A_UART_enableInterrupt(EUSCI_A_SELECTION,
			EUSCI_A_UART_TRANSMIT_INTERRUPT);
}

//*****************************************************************************
//
//! EUSCI_VECTOR Interrupt Handler (UART Transmit and Receive)
//
//*****************************************************************************
#if (EUSCI_A_SELECTION == EUSCI_A0_BASE)
#define EUSCI_VECTOR     (USCI_A0_VECTOR)
#elif (EUSCI_A_SELECTION == EUSCI_A1_BASE)
#define EUSCI_VECTOR     (USCI_A1_VECTOR)
#else
#error INVALID_EUSCI_A_SELECTION
#endif
#pragma vector=EUSCI_VECTOR
__interrupt void UART_ISR(void)
{
	static uint16_t g_ui16TransmitIndex = 0;
	uint8_t ui8Data;
#if (EUSCI_A_SELECTION == EUSCI_A0_BASE)
	switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
#elif (EUSCI_A_SELECTION == EUSCI_A1_BASE)
	switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
#else
#error INVALID_EUSCI_A_SELECTION
#endif
	{
	//
	// New data received, push it onto the receive queue
	//
	case USCI_UART_UCRXIFG:
		ui8Data = EUSCI_A_UART_receiveData(EUSCI_A_SELECTION);
		TARGET_COMM_byteQueuePush(g_pReceiveQueue, ui8Data);
		LPM3_EXIT;
		break;

		//
		// Transmit buffer available, send next byte
		//
	case USCI_UART_UCTXIFG:
		EUSCI_A_UART_transmitData(EUSCI_A_SELECTION,
				g_pTransmitBuffer[g_ui16TransmitIndex++]);

		//
		// If this is the last byte in the packet, clear transmit interrupt enable and
		// wake from sleep in case application is sleeping
		//
		if (g_ui16TransmitIndex == g_ui16TransmitLength)
		{
			g_ui16TransmitIndex = 0;
			g_ui16TransmitLength = 0;
			EUSCI_A_UART_disableInterrupt(EUSCI_A_SELECTION,
					EUSCI_A_UART_TRANSMIT_INTERRUPT);
			g_bUARTTransmitting = false;
			LPM3_EXIT;
		}
		break;

	default:
		break;
	}
}

//*****************************************************************************
//
//! Close the doxygen group for eusci_uart_driver
//! @}
//
//*****************************************************************************
