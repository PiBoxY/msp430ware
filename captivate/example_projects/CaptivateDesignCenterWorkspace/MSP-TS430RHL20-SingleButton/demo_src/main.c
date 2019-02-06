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
//! \brief  This demonstration utilizes the MSP-TS430RHL20 target socket board
//!         to demonstrate a simple capacitive touch application using the
//!         MSP430FR2522 CapTIvate MCU.
//
//! Pin 17 of the MSP430FR2522 (CAP0.0) is used as the capacitive sensing IO.
//! Pin 2 (P1.0) is used to control the green LED (D1) on the PCB.
//! Pin 12 (P2.3) is used to control the blue LED (D2) on the PCB.
//! The green LED illuminates whenever a user is present.
//! The blue LED toggles whenever a new touch is asserted.
//
//! For best results, attach a wire to Pin 17 (CAP0.0) to form the sensor,
//! or wire Pin 17 off to another sensor board, such as the CAPTIVATE-BSWP.
//! It is possible to connect this demo to the CapTIvate Design Center
//! in real time by connecting the UART pins (pin 7 and 8) to a
//! CAPTIVATE-PGMR debug module.
//! It may be necessary to adjust the touch threshold to respond well
//! to the sensor that is attached to CAP0.0.
//
//  Group:          MSP
//  Target Devices: MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
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

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void demoButtonEventHandler(tSensor *pSensor)
{
    if (pSensor->bSensorTouch==true)
    {
        P1OUT |= BIT0;
        if (pSensor->bSensorPrevTouch==false)
        {
            P2OUT ^= BIT3;
        }
    }
    else
    {
        P1OUT &= ~BIT0;
    }
}

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

	//
	// Start the CapTIvate application
	//
	MAP_CAPT_registerCallback(&demoButton, &demoButtonEventHandler);
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
