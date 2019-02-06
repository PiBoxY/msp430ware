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
//! \file   main.c
//
//! \brief  This is the top level application for the BOOSTXL-CAPKEYPAD.
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
#include "system.h"
#include "host_interface.h"
#include "boostxl-capkeypad.h"

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief System_topLevel is the top level system control and status
//! structure.  It contains the control and status words for the
//! BOOSTXL-CAPKEYPAD.
//! For information on the fields in this structure, see system.h.
//!
volatile System_controlAndStatus System_topLevel;

//! \brief If a shutdown was requested, lock the applicaiton in LPM4 with
//! the keypad powered down until the shutdown request is cleared.  After the
//! shutdown request is cleared,the keypad is re-initialized and the shutdown
//! function will exit.
static inline void shutdownHandler(void);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main(void)
{
    // Initialize all of the needed submodules.
    // The watchdog timer is disabled before CINIT.
	Board_init();
	HostInterface_init();
	LEDControl_init();
	BXLCapKeypad_init();

	// Set default top level system status
	g_uiApp.state = eUIActive;
    System_topLevel.statusWord = SYSTEM__STAT_ACTIVE;
    System_topLevel.lastStatusWord = SYSTEM__STAT_ACTIVE;
	System_topLevel.controlWord =\
	        SYSTEM__CTRL_WOP_ENABLE |\
	        SYSTEM__CTRL_UART_ENABLE;

	// This is the background execution loop.
	// BXLCapKeypad_run checks to see if it is time to update
	// the keypad status.  If it is, an update will be performed.
	// This function also handles entering and exiting a wake-on-prox state.
	// BXLCapKeypad_updateCapTIvateComm checks to see if there is any incoming
	// data from the CapTIvate Design Center via the UART interface that needs
	// to be handled.
	// BXLCapKeypad_sleep is a safe sleep function that ensures no CapTIvate
	// flags are pending when a low power sleep mode is entered.
	while(1)
	{
	    shutdownHandler();
	    BXLCapKeypad_run();
	    BXLCapKeypad_updateCapTIvateComm();
	    BXLCapKeypad_sleep();
	}
}

void shutdownHandler(void)
{
    __bic_SR_register(GIE);
    if (System_topLevel.controlWord & SYSTEM__CTRL_SHUTDOWN)
    {
        BXLCapKeypad_stop();
        do
        {
            __bis_SR_register(LPM4_bits | GIE);
            __bic_SR_register(GIE);
        } while(System_topLevel.controlWord & SYSTEM__CTRL_SHUTDOWN);
        BXLCapKeypad_init();
    }
    __bis_SR_register(GIE);
}
