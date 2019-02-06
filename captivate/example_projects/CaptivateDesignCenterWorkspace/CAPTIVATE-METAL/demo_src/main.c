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
//! \brief  This demo utilizes 8 CapTIvate IO's in self-capacitance mode.
//!  The CAPTIVATE-METAL panel demonstrates an alternative use of capacitive
//!  touch by using the deflection in the grounded metal overlay to cause a
//!  change in the capacitance between the electrodes below and the metal
//!  overlay. Compared to traditional capacitive sensing through plastics or
//!  glass, capacitive metal touch is only sensitive to an applied force.
//!  For this reason metal touch is an excellent choice for harsh or hostile
//!  environments where there are liquids, grease, mud or grime present and
//!  where a user may be wearing gloves.

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

#include <msp430.h>              // Generic MSP430 Device Include
#include <driverlib.h>           // MSPWare Driver Library
#include <captivate.h>           // CapTIvate Touch Software Library
#include "CAPT_UserConfig.h"     // CapTIvate Auto-Generated User Configuration
#include "CAPT_App.h"            // CapTIvate Application Code
#include "CAPT_BSP.h"            // CapTIvate EVM Board Support Package
#include "FourDigitDisplay.h"
#include "CAPTIVATE_METAL_Demo.h"

bool g_bUpdateDisplayFlag = false;


//*****************************************************************************
// Function Implementations
//*****************************************************************************
void main(void)
{
	//
	// Initialize the MCU
	// BSP_configureMCU() sets up the device IO and clocking
	// The global interrupt enable is set to allow peripherals
	// to wake the MCU.
	//
	WDTCTL = WDTPW | WDTHOLD;
	BSP_configureMCU();

	__bis_SR_register(GIE);

	Demo_Init();

	//
	// Start the CapTIvate application
	//
	CAPT_appStart();

	//
	// Background Loop
	//
	while(1)
	{
		//
		// Run the captivate application handler.
		//
		CAPT_appHandler();
		
		//
		// End of background loop iteration
		// Go to sleep if there is nothing left to do
		//
		CAPT_appSleep();
		
	} // End background loop
} // End main()
