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
//! \file   board.c
//!
//! \brief  This is the board file for the BOOSTXL-CAPKEYPAD.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "board.h"

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Board_init(void)
{
    // Configure FRAM wait state (set to 1 to support 16MHz MCLK)
	FRAMCtl_configureWaitStateControl(FRAMCTL_ACCESS_TIME_CYCLES_1);
	
	// Initialize Clock Signals
    CS_initClockSignal(CS_FLLREF, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);

    // Tune the DCO parameters
    CS_initFLL((MCLK_FREQ/1000), FLL_RATIO);
    CS_clearAllOscFlagsWithTimeout(1000);

    // Re-map UCA0 to secondary location
    SYSCFG3 |= USCIARMP;

    // Re-map UCB0 to secondary location
    SYSCFG2 |= USCIBRMP;

    // P1.0: OUTPUT LOW
    // P1.1: OUTPUT LOW
    // P1.2: OUTPUT LOW
    // P1.3: OUTPUT LOW
    // P1.4: OUTPUT LOW
    // P1.5: OUTPUT LOW
    // P1.6: OUTPUT LOW
    // P1.7: OUTPUT LOW
    P1OUT  = (0);
    P1DIR  = (GPIO_PIN_ALL8);
    P1SEL0 = (0);
    P1SEL1 = (0);

    // P2.0: UCA0 UART TXD
    // P2.1: UCA0 UART RXD
    // P2.2: I2C MASTER SDA (Software)
    // P2.3: I2C MASTER SCL (Software)
    // P2.4: IRQ (HIZ idle, active-low style)
    // P2.5: I2C SLAVE SDA (UCB0)
    // P2.6: I2C SLAVE SCL (UCB0)
    P2OUT  =  (0);
    P2DIR  =  (0);
    P2SEL0 =  (GPIO_PIN0 | GPIO_PIN1);
    P2SEL1 =  (GPIO_PIN5 | GPIO_PIN6);

    // Clear port lock
    PM5CTL0 &= ~LOCKLPM5;

    // Enable global interrupts
    __enable_interrupt();
}

int _system_pre_init(void)
{
    // Disable watchdog timer before CINIT.
    WDTCTL = WDTPW | WDTHOLD;
    return 1;
}
