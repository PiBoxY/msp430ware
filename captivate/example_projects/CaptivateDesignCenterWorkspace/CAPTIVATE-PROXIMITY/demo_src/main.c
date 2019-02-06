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
//! \brief  This is the demo project for the CAPTIVATE-PROXIMITY PCB.
//!         The CAPTIVATE_PROXIMITY_Demo files contain callbacks that drive 
//!         LEDs on the panel via muxed IOs.  These LEDs indicate proximity 
//!         detection status, as well as proximity swipe left/right detection.
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
#include "CAPTIVATE_PROXIMITY_Demo.h"    // CAPTIVATE-PROXIMITY PCB Demo

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main(void)
{
	//
	// Initialize everything that is required for the demo.
	// The standard CAPTIVATE-FR2633 BSP is used to bring up the MCU.
	// The CAPTIVATE-PROXIMITY-Demo is setup with Demo_init(), which sets up
	// the LEDs and sensor callbacks.  The CapTIvate application
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
	// responding to communication on the I2C interface via the CapTIvate
	// protocol.  If there is nothing left to do, the loop will sleep.
	//
	while(1)
	{
		//
		// Run the captivate application handler.
		// Set LED1 while the app handler is running,
		// and set LED2 if proximity is detected
		// on any sensor.
		//
		LED1_ON;
		if(CAPT_appHandler()==true)
			LED2_ON;
		else
			LED2_OFF;
		LED1_OFF;
		
		//
		// This is a great place to add in any 
		// background application code.
		//
		__no_operation();

		//
		// End of background loop iteration
		// Go to sleep if there is nothing left to do
		//
		CAPT_appSleep();
		
	} // End background loop
} // End main()
