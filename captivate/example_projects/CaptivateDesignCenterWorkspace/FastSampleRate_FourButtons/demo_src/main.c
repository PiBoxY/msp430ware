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
//! \brief  This example demonstrates how to achieve very fast sample rates
//!         with the CapTIvate peripheral.  The project answers these
//!         main questions:
//!           1. How to achieve a 1000sps report rate (1ms active scan period)
//!              for 1-4 buttons with bulk I2C slave reporting
//!           2. How to achieve a 8196sps scan rate (0.12ms active scan period)
//!              for 1-4 buttons when just looking at the raw data of a single
//!              time cycle
//!           3. How to use low-level CapTIvate library calls to set up a
//!              single cycle for repeated, fast raw measurements
//!
//!         This very basic example is intended to be used with the
//!         CAPTIVATE-FR2633 processor module and the CAPTIVATE-PHONE sensing
//!         panel.  It has one sensor with one cycle and 4 elements.  The
//!         four elements correspond with the mode keypad sensor in the
//!         CAPTIVATE-PHONE demonstration project.
//!
//!         Buttons: END CALL, HEADSET, MUTE, SPEAKER; Rx: CAP0.0, CAP1.0,
//!                  CAP2.0, CAP3.0; Tx: CAP2.1
//!         LEDs:    P1.7, P1.6
//!
//!         To switch from the default 1000sps mode with full processing and
//!         communication to the 8196sps mode with no processing or
//!         communication, un-comment the __RAW_DATA_UPDATE_ONLY__ definition.
//!         The raw data may be viewed by accessing the .pRawCount[0] member
//!         of the element structures which are declared in the global
//!         variable section.
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
#include "captivate.h"                   // CapTIvate Touch Software Library

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief The bit mask for LED1 on port P1.
//!
#define LED1 0x80

//! \brief The bit mask for LED2 on port P1.
//!
#define LED2 0x40

//! \brief The LED port
//!
#define LEDOUT P1OUT

//! \brief The ratio between the DCOCLK and the FLLREF.
//!
#define FLL_RATIO 0x01E7

//! \brief The 0.000122s sample period based on a 32768 Hz clock source,
//!        providing for a 8.196 kHz sample rate
//!
#define RAW_UPDATE_PERIOD (4 - 1)

//! \brief Define to update raw data of the single cycle only, with no COMMs
//!
#define __RAW_DATA_UPDATE_ONLY__

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief The time cycle that contains the 4 elements in the application.
//!
extern tCycle modeKeypadSensor_C00;

//! \brief The END CALL element.
//!
extern tElement modeKeypadSensor_E00;

//! \brief The HEADSET element
//!
extern tElement modeKeypadSensor_E01;

//! \brief The MUTE element
//!
extern tElement modeKeypadSensor_E02;

//! \brief The SPEAKER element
//!
extern tElement modeKeypadSensor_E03;

//*****************************************************************************
// Function Declarations
//*****************************************************************************

#ifndef __RAW_DATA_UPDATE_ONLY__
//! \brief Standard touch application that calls the three top-level CapTIvate
//!        functions: CAPT_initUI(), CAPT_calibrateUI() and CAPT_updateUI().
//!        The bulk I2C communication interface is used for cycle packet
//!        transmission to the CapTIvate Design Center.
static void typicalApplication(void);
#else
//! \brief Low-level raw data acquisition application that utilizes HAL
//!        and touch layer functions to update just the raw count values
//!        of the single time cycle (modeKeypadSensor_C00).
static void rawDataUpdateApplication(void);
#endif

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main(void)
{
	//
	// Stop the watchdog timer
	//
	WDTCTL = WDTPW | WDTHOLD;

	//
	// Set the port muxing:
	//  - P1.1: IRQ (OPEN DRAIN)
	//  - P1.2: UCB0 I2C SDA
	//  - P1.3: UCB0 I2C SCL
	//  - P1.4: UCA0 UART TXD
	//  - P1.5: UCA0 UART RXD
	//  - P1.6: LED2
	//  - P1.7: LED1
	//  - P2.0: XOUT
	//  - P2.1: XIN
	//
	PAOUT  = 0x0000;
	PADIR  = 0xFCC1;
	PASEL0 = 0x033C;
	P3OUT  = 0x00;
	P3DIR  = 0x07;
	P3SEL0 = 0x00;
	PM5CTL0 &= ~LOCKLPM5;

	//
	// Set up FRAM wait state to allow for a 16 MHz MCLK.
	// Note that CapTIvate ROM functions execute without
	// any wait states at 16 MHz.
	//
	FRCTL0 = FRCTLPW | NWAITS_1;

	//
	// Set up the system clocks.
	// MCLK = DCO = 16 MHz
	// ACLK = XT1 = 32 kHz
	// SMCLK = MCLK / 8 = 2 MHz
	//
	do
	{
		CSCTL7 &= ~(XT1OFFG | DCOFFG);
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);
	CSCTL3 = SELREF__XT1CLK;
	__bis_SR_register(SCG0);
	CSCTL0 = 0;
	CSCTL1 &= ~(DCORSEL_7);
	CSCTL1 |= DCORSEL_5;
	CSCTL2 = FLLD_0 + FLL_RATIO;
	__delay_cycles(3);
	__bic_SR_register(SCG0);
	while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));
	CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;
	CSCTL5 = VLOAUTOOFF | DIVS__8;

	//
	// Run the selected demonstration
	//
#ifndef __RAW_DATA_UPDATE_ONLY__
	typicalApplication();
#else
	rawDataUpdateApplication();
#endif /* __RAW_DATA_UPDATE_ONLY__ */
} // End main()

#ifndef __RAW_DATA_UPDATE_ONLY__

void typicalApplication(void)
{
	//
	// CapTIvate setup and calibration
	//
	CAPT_initUI(&g_uiApp);
	CAPT_calibrateUI(&g_uiApp);

	//
	// Set up the CapTivate peripheral interrupt 
	// timer for a wakeup period of 1ms (1kSPS)
	//
	MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
	MAP_CAPT_writeTimerCompRegister(
			CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod)
		);
	MAP_CAPT_startTimer();
	MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);

	//
	// Application Loop
	//
	while(1)
	{
		//
		// Check for timer trigger flag
		// If set, measure the UI
		//
		if (g_bConvTimerFlag == true)
		{
			//
			// Clear the timer flag.
			// Toggle LED1 to indicate a new measurement frame.
			// Set LED2 to indicate that the conversion is starting.
			//
			g_bConvTimerFlag = false;
			LEDOUT ^= LED1;
			LEDOUT |= LED2;

			//
			// Update the UI with the top-level update call.
			// This will refresh raw and processed values,
			// and will transmit new data via the bulk I2C
			// interface to the CapTIvate Design Center.
			//
			CAPT_updateUI(&g_uiApp);

			//
			// Clear LED2 to indicate that the conversion is finished.
			//
			LEDOUT &= ~LED2;
		}
		
		//
		// Service the communications layer
		// to ensure that incoming packets
		// are handled.
		//
#if ((CAPT_INTERFACE==__CAPT_UART_INTERFACE__) ||\
	(CAPT_INTERFACE==__CAPT_BULKI2C_INTERFACE__))

		CAPT_checkForInboundPacket();
		if (CAPT_checkForRecalibrationRequest() == true)
		{
			CAPT_calibrateUI(&g_uiApp);
		}
#endif

		//
		// Safely go to sleep if the timer flag is not yet set.
		//
		__bic_SR_register(GIE);
		if (!g_bConvTimerFlag)
		{
			__bis_SR_register(g_uiApp.ui8AppLPM | GIE);
		}
		else
		{
			__bis_SR_register(GIE);
		}
	}
}

#else

void rawDataUpdateApplication(void)
{
	//
	// CapTIvate setup and calibration
	//
	CAPT_initUI(&g_uiApp);
	CAPT_calibrateUI(&g_uiApp);

	//
	// Set up the CapTIvate peripheral for the first time cycle of the 
	// modeKeypadSensor, before starting the application.
	//
	// The finite state machine is bypassed, since processed data will
	// not be used in this application.  The sensor parameters and
	// conversion frequency settings are appled to the CapTIvate peripheral.
	// The cycle compensation (gain ratio and offset settings from the
	// calibration routine) are applied for the cycle, and the cycle IO
	// are set to prepare for the conversion.
	//
	// These settings  will not be removed from the peripheral,
	// as this is the only time cycle being measured.
	//
	MAP_CAPT_bypassFSM();
	MAP_CAPT_applySensorParams(&modeKeypadSensor);
	MAP_CAPT_applySensorFreq(
			CAPT_OSC_FREQ_DEFAULT, 
			&modeKeypadSensor
		);
	MAP_CAPT_applyCycleComp(
			CAPT_OSC_FREQ_DEFAULT, 
			&modeKeypadSensor_C00
		);
	MAP_CAPT_setCycleIO(
			&modeKeypadSensor, 
			&modeKeypadSensor_C00
		);

	//
	// Set up the CapTivate peripheral interrupt timer for a wakeup period
	// of 122 microseconds (about 8196 Hz, or ~8.2kSPS) as defined in
	// RAW_UPDATE_PERIOD.
	//
	MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
	MAP_CAPT_writeTimerCompRegister(RAW_UPDATE_PERIOD);
	MAP_CAPT_startTimer();
	MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);

	//
	// Application Loop
	//
	while(1)
	{
		//
		// Check for timer trigger flag
		// If set, measure the cycle and store the raw results
		//
		if (g_bConvTimerFlag == true)
		{
			//
			// Clear the timer flag.
			// Toggle LED1 to indicate a new measurement frame.
			// Set LED2 to indicate that the conversion is starting.
			//
			g_bConvTimerFlag = false;
			LEDOUT ^= LED1;
			LEDOUT |= LED2;

			//
			// Start the conversion and wait until it is
			// complete.  Then, save the raw results
			// back into the data structures.
			//
			CAPT_startConversionAndWaitUntilDone(&g_bEndOfConversionFlag, CPUOFF);
			MAP_CAPT_saveCycleRawResults(
					CAPT_OSC_FREQ_DEFAULT,
					&modeKeypadSensor,
					&modeKeypadSensor_C00
				);

			//
			// Clear LED2 to indicate that the conversion
			// is finished.
			//
			LEDOUT &= ~LED2;

			//
			// Pause here to view raw data in the debugger
			//
			__no_operation();
		}
	}
}

#endif /* __RAW_DATA_UPDATE_ONLY__ */
