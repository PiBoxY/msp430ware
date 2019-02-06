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
//! \brief  This demonstration implements a single capacitive touch button
//!         with a minimalistic application layer that simply controls 2 LEDs.
//!
//!         This very basic example is intended to be used with the
//!         CAPTIVATE-FR2633 processor module and the CAPTIVATE-BSWP sensing
//!         panel.  It demonstrates the simplest, most code-efficient way
//!         to set up the MCU with traditional MSP-style register access.
//!
//!         When Button 1 on the CAPTIVATE-BSWP panel is pressed,
//!         LED1 will illuminate and LED2 will toggle.  LED1 will stay
//!         illuminated as long as the touch is held; LED2 only changes state
//!         at the beginning of a new touch.
//!
//!         Button: CAP0.0
//!         LEDs:    P1.7, P1.6
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

#include <msp430.h>
#include "captivate.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief The bit mask for LED1 on port P1.
//!
#define LED1 0x80

//! \brief The bit mask for LED2 on port P1.
//!
#define LED2 0x40

//! \brief The ratio between the DCOCLK and the FLLREF.
//!
#define FLL_RATIO 0x01E7

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Event handler for the keypad sensor.
//!        This handler updates LED1 and LED2 based on
//!        the status of the keypad sensor.
//! \param Pointer to the keypad sensor
static void buttonHandler(tSensor* pSensor);

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
	// SMCLK = OFF
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
	CSCTL5 = SMCLKOFF | VLOAUTOOFF;

	//
	// Set up the CapTIvate peripheral
	// and timer.  The timer will periodically
	// set the g_bConvTimerFlag.
	//
	CAPT_initUI(&g_uiApp);
	CAPT_calibrateUI(&g_uiApp);
	MAP_CAPT_registerCallback(&button, &buttonHandler);
	MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
	MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod));
	MAP_CAPT_startTimer();
	MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);
	__bis_SR_register(GIE);

	//
	// Start the application
	//
	while(1)
	{
		if (g_bConvTimerFlag == true)
		{
			//
			// If it is time to update the button,
			// update it here with the generic library call.
			//
			g_bConvTimerFlag = false;
			CAPT_updateUI(&g_uiApp);
		}

		//
		// Go to sleep when finished.
		// The CapTIvate timer will wake the application from sleep.
		//
		__bis_SR_register(g_uiApp.ui8AppLPM | GIE);
	}
} // End main()

void buttonHandler(tSensor* pSensor)
{
	//
	// If the button is currently being touched,
	// illuminate LED1.
	//
	if (pSensor->bSensorTouch==true)
	{
		P1OUT |= LED1;

		//
		// If a completely new touch was detected,
		// toggle the state of LED2.
		//
		if (pSensor->bSensorPrevTouch==false)
		{
			P1OUT ^= LED2;
		}
	}
	else
	{
		P1OUT &= ~LED1;
	}
}
