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
#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "TARGET_COMM_ByteQueue.h"

//*****************************************************************************
//
//! \def EUSCI_A_SELECTION indicates the base address of the UART peripheral
//! to use.  EUSCI_A0_BASE and EUSCI_A1_BASE are the valid options, and are
//! defined in the MSP430 Driver Library.
//
//*****************************************************************************
#define EUSCI_A_SELECTION			(APP_EUSCI_A_SELECTION)

//*****************************************************************************
//
//! The definitions below configure the timing of the eUSCI_A peripheral.  The
//! web page below can calculate the appropriate values.
//! http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/
//! MSP430BaudRateConverter/index.html
//
//*****************************************************************************

//*****************************************************************************
//
//! \def UART_PRESCALER defines the eUSCI_A pre-scaler.
//
//*****************************************************************************
#define UART_PRESCALER				(APP_UART_PRESCALER)

//*****************************************************************************
//
//! \def UART_FIRST_STAGE_MOD defines the eUSCI_A first stage modulation.
//
//*****************************************************************************
#define UART_FIRST_STAGE_MOD		(APP_FIRST_STAGE_MOD)

//*****************************************************************************
//
//! \def UART_SECOND_STAGE_MOD defines the eUSCI_A second stage modulation.
//
//*****************************************************************************
#define UART_SECOND_STAGE_MOD		(APP_UART_SECOND_STAGE_MOD)


//*****************************************************************************
//
//! \def UART_SECOND_STAGE_MOD defines the eUSCI_A second stage modulation.
//
//*****************************************************************************
#define UART_OVERSAMPLING			(APP_UART_OVERSAMPLING)


//*****************************************************************************
//
//! \var g_bUARTTransmitting is a boolean value that indicates to the
//! application of a transmission is in progress.
//
//*****************************************************************************
extern bool g_bUARTTransmitting;

//
// eUSCI_UART APIs
//
extern void EUSCI_UART_init(tByteQueue* receiveQueue);

extern void EUSCI_UART_write(uint8_t *pBuffer, uint16_t ui16Length);

#endif /* UART_H_ */
