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
//! TouchProGUI.h
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

#ifndef TOUCHPROGUI_H_
#define TOUCHPROGUI_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
//! \def TOUCHPROGUI_MAX_CHANNELS is user-selectable and defines the maximum
//! number of channels that can be sent to TouchPro GUI.  This effects the
//! RAM requirements.
//
//*****************************************************************************
#define TOUCHPROGUI_MAX_CHANNELS	(10)
#define UART_BR_LOWER				(0x71)
#define UART_BR_UPPER				(0x02)
#define UART_MOD					(0x00)

//*****************************************************************************
//
//! \def TOUCHPROGUI_SYNC_UPPER defines the upper byte of the sync word.
//
//! \def TOUCHPROGUI_SYNC_LOWER defines the lower byte of the sync word.
//
//! \def TOUCHPROGUI_BUFFER_LENGTH defines the required length of the transmit
//! buffer.  This is based upn the TOUCHPROGUI_MAX_CHANNELS definition.
//
//*****************************************************************************
#define TOUCHPROGUI_SYNC_UPPER	(0x55)
#define TOUCHPROGUI_SYNC_LOWER	(0xAA)
#define TOUCHPROGUI_BUFFER_LENGTH 	(TOUCHPROGUI_MAX_CHANNELS * 3 + 3)

void TouchProGUI_init(void);
void TouchProGUI_sendData(uint16_t *ui16DataBuffer, uint8_t ui8Length);

#endif /* TOUCHPROGUI_H_ */
