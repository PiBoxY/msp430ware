/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//#############################################################################
//
//! \file   board.h
//!
//! \brief  This is the board file for the BOOSTXL-CAPKEYPAD.
//
// *** CLOCK CONFIGURATION ***
//   MCLK is 16 MHz, sourced from the DCO
//   SMCLK is 2 MHz, sourced from MCLK
//   ACLK is 32kHz, sourced from REFO
//   FLLREF is 32kHz, sourced from REFO
//   VLO is 10kHz
//
// *** SERIAL INTERFACES ***
//   UART: UCA0 is muxed in UART mode to P2.0 and P2.1 (secondary mapping)
//   I2C SLAVE: UCB0 is muxed in I2C mode to P2.5 and P2.6 (secondary mapping)
//   I2C MASTER: Software I2C on P2.2 and P2.3
//   IRQ: An active low IRQ signal is made available on P2.4
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef BOARD_H_
#define BOARD_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>
#include "driverlib.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief MCLK_FREQ defines the main clock frequency in Hz.
//!
#define MCLK_FREQ 16000000

//! \brief SMCLK_FREQ defines the sub-main clock frequency in Hz.
//!
#define SMCLK_FREQ 2000000

//! \brief ACLK_FREQ defines the auxiliary clock frequency in Hz.
//!
#define ACLK_FREQ 32768

//! \brief FLLREF_FREQ defines the FLL reference clock frequency in Hz.
//!
#define FLLREF_FREQ ACLK_FREQ

//! \brief FLL_RATIO defines ratio of MCLK to the FLL reference clock.
//!
#define FLL_RATIO (MCLK_FREQ / FLLREF_FREQ)

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Initialize the BOOSTXL-CAPKEYPAD board for operation.
//! This includes configuring the port muxing, clock system, SYS, and PMM.
//! \param none
//! \return none
extern void Board_init(void);

#endif /* BOARD_H_ */
