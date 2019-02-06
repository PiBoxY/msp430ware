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
//! \brief  This project implements a self-capacitance button group with 3 
//!         elements. It exposes CAP1.2, CAP1.3, and CAP0.0, which are 
//!         available for prototyping on the BOOSTXL-CAPKEYPAD via Jumper J10.
//
//! To experiment with your own sensor designs, remove all 3 jumpers on J10
//! from the BOOSTXL-CAPKEYPAD.  This will expose CAP1.2, CAP1.3, and CAP0.0
//! on the MSP430FR2522 CapTIvate MCU.  Wire in your prototype sensor designs
//! to these pins, and run this project to view the data in CapTIvate Design
//! Center!  The MCU pins are the 3 pins in the top row of J10.
//
//! To view data, connect a CAPTIVATE-PGMR module to J5 on the BoosterPack
//! and connect the CAPTIVATE-PGMR to your host PC via USB.  Then run
//! CapTIvate Design Center and open the BOOSTXL-CAPKEYPAD-Prototyping project.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#include <msp430.h>                      // Generic MSP430 Device Include
#include "driverlib.h"                   // MSPWare Driver Library
#include "captivate.h"                   // CapTIvate Touch Software Library
#include "board.h"

void main(void)
{
	// Set up the MCU
	WDTCTL = WDTPW | WDTHOLD;
	Board_init();

	// Start CapTIvate, and use the CapTIvate timer to generate a periodic
	// flag to know when to refresh the sensors
	CAPT_initUI(&g_uiApp);
	CAPT_calibrateUI(&g_uiApp);
	MAP_CAPT_stopTimer();
    MAP_CAPT_clearTimer();
    MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
    MAP_CAPT_selectTimerSourceDivider(CAPT_TIMER_CLKDIV__1);
    MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod));
    MAP_CAPT_startTimer();
    MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);

	while(1)
	{
		// The CapTIvate timer will set g_bConvTimerFlag, indicating
		// when it is time to refresh the sensor data.
	    if (g_bConvTimerFlag == true)
        {
	        g_bConvTimerFlag = false;
	        CAPT_updateUI(&g_uiApp);
        }

		// This periodically checks for data from the CapTIvate Design Center.
	    CAPT_checkForInboundPacket();
	    if (CAPT_checkForRecalibrationRequest() == true)
        {
	        CAPT_calibrateUI(&g_uiApp);
        }
	} // End background loop
} // End main()
