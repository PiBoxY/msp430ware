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
//!
//! \brief  This demo contains a 7x4 mutual capacitance matrix that includes 
//!         keypads, sliders, and a wheel.  There is also a self-capacitance 
//!         proximity and guard sensing channel.
//!
//!         Haptic effects are fired to the LRA on-board when a valid touch
//!         is detected.
//!
//!         When the guard channel is inactive and a valid touch is detected, 
//!         LED1 will illuminate.
//!         When the guard channel is activated, LED2 will illuminate.
//
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>                      // Generic MSP430 Device Include
#include "driverlib.h"                   // MSPWare Driver Library
#include "captivate.h"                   // CapTIvate Touch Software Library
#include "CAPT_App.h"                    // CapTIvate Application Code
#include "CAPT_BSP.h"                    // CapTIvate EVM Board Support Package
#include "CAPTIVATE_PHONE_Demo.h"        // CAPTIVATE-PHONE PCB Demo

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main(void)
{
	//
	// Initialize everything that is required for the demo.
	// The standard CAPTIVATE-FR2633 BSP is used to bring up the MCU.
	// The CAPTIVATE-PHONE-Demo is set up with Demo_init(), which sets up
	// the haptic driver and sensor callbacks.  The CapTIvate application
	// is started with CAPT_appStart(), which calibrates the sensors and
	// configures the CapTIvate timer to provide the requested scan period.
	//
	WDTCTL = WDTPW | WDTHOLD;
	BSP_configureMCU();
	__bis_SR_register(GIE);
	Demo_init();
	CAPT_appStart();

	//
	// The CapTIvate application handler will refresh the user interface
	// when requested to by the CapTIvate timer flag.  It will also handle
	// responding to communication on the UART interface via the CapTIvate
	// protocol.  If there is nothing left to do, the loop will sleep.
	//
	while(1)
	{
		CAPT_appHandler();
		Demo_checkForValidTouch();
		CAPT_appSleep();
	} // End background loop
} // End main()
