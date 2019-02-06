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
//! \brief  This example demonstrates how to implement a basic proximity
//!         range sensor.
//!
//!         This example is intended to be used with the CAPTIVATE-FR2633  
//!         processor module and the CAPTIVATE-PROXIMITY sensing
//!         panel.  It measures only the center proximity channel on that
//!         board, and terminates other channels as shields to lower 
//!         parasitic capacitance.
//
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2016, Texas Instruments, Inc.
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
#include "CAPT_BSP.h"   

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Defines the left LED POUT register
//!
#define LED_A1__POUT (P3OUT)

//! \brief Defines the left LED PDIR register
//!
#define LED_A1__PDIR (P3DIR)

//! \brief Defines the left LED pin
//!
#define LED_A1__PIN (BIT2)

//! \brief Turn off the left LED
//!
#define LED_A1__OFF (LED_A1__POUT |= LED_A1__PIN)

//! \brief Turn on the left LED
//!
#define LED_A1__ON  (LED_A1__POUT &= ~LED_A1__PIN)

//! \brief Toggle the left LED
//!
#define LED_A1__TOGGLE (LED_A1__POUT ^= LED_A1__PIN)

//! \brief Defines the center LED POUT register
//!
#define LED_A2__POUT (P2OUT)

//! \brief Defines the center LED PDIR register
//!
#define LED_A2__PDIR (P2DIR)

//! \brief Defines the center LED pin
//!
#define LED_A2__PIN (BIT7)

//! \brief Turn off the center LED
//!
#define LED_A2__OFF (LED_A2__POUT |= LED_A2__PIN)

//! \brief Turn on the center LED
//!
#define LED_A2__ON (LED_A2__POUT &= ~LED_A2__PIN)

//! \brief Toggle the center LED
//!
#define LED_A2__TOGGLE (LED_A2__POUT ^= LED_A2__PIN)

//! \brief Defines the right LED POUT register
//!
#define LED_A3__POUT (P2OUT)

//! \brief Defines the right LED PDIR register
//!
#define LED_A3__PDIR (P2DIR)

//! \brief Defines the right LED pin
//!
#define LED_A3__PIN (BIT5)

//! \brief Turn off the right LED
//!
#define LED_A3__OFF (LED_A3__POUT |= LED_A3__PIN)

//! \brief Turn on the right LED
//!
#define LED_A3__ON (LED_A3__POUT &= ~LED_A3__PIN)

//! \brief Toggle the right LED
//!
#define LED_A3__TOGGLE (LED_A3__POUT ^= LED_A3__PIN)

//! \brief Defines the ring LED POUT register
//!
#define LED_A4__POUT (P2OUT)

//! \brief Defines the ring LED PDIR register
//!
#define LED_A4__PDIR (P2DIR)

//! \brief Defines the ring LED pin
//!
#define LED_A4__PIN (BIT4)

//! \brief Turn off the ring LED
//!
#define LED_A4__OFF (LED_A4__POUT |= LED_A4__PIN)

//! \brief Turn on the ring LED
//!
#define LED_A4__ON (LED_A4__POUT &= ~LED_A4__PIN)

//! \brief Toggle the ring LED
//!
#define LED_A4__TOGGLE (LED_A4__POUT ^= LED_A4__PIN)

//! \brief Defines the swipe left LED POUT register
//!
#define LED_G1__POUT (P3OUT)

//! \brief Defines the swipe left LED PDIR register
//!
#define LED_G1__PDIR (P3DIR)

//! \brief Defines the swipe left LED pin
//!
#define LED_G1__PIN (BIT1)

//! \brief Turn off the swipe left LED
//!
#define LED_G1__OFF (LED_G1__POUT |= LED_G1__PIN)

//! \brief Turn on the swipe left LED
//!
#define LED_G1__ON (LED_G1__POUT &= ~LED_G1__PIN)

//! \brief Toggle the swipe left LED
//!
#define LED_G1__TOGGLE (LED_G1__POUT ^= LED_G1__PIN)

//! \brief Defines the swipe right LED POUT register
//!
#define LED_G2__POUT          (P2OUT)

//! \brief Defines the swipe right LED PDIR register
//!
#define LED_G2__PDIR          (P2DIR)

//! \brief Defines the swipe right LED pin
//!
#define LED_G2__PIN           (BIT3)

//! \brief Turn off the swipe right LED
//!
#define LED_G2__OFF           (LED_G2__POUT |= LED_G2__PIN)

//! \brief Turn on the swipe right LED
//!
#define LED_G2__ON            (LED_G2__POUT &= ~LED_G2__PIN)

//! \brief Toggle the swipe right LED
//!
#define LED_G2__TOGGLE        (LED_G2__POUT ^= LED_G2__PIN)

//! \brief A macro to access the LTA of an element.
//!
#define CAPT_LTA(e) (e->LTA.ui16Natural)

//! \brief A macro to access the filtered count of an element.
//!
#define CAPT_COUNT(e) (e->filterCount.ui16Natural)

//! \brief A macro to compute the delta between the LTA and the filtered count
//! of an element.
//!
#define CAPT_DELTA(e) (CAPT_LTA(e) - CAPT_COUNT(e))

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief The event handler that is called when new data is available from
//!        the proximity sensor.
//! \param[in] s is a pointer to the proximity sensor itself
static void proximityEventHandler(tSensor *s);

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


	//
	// Turn off LEDs on the sensor board
	//
	LED_A1__OFF;
	LED_A2__OFF;
	LED_A3__OFF;
	LED_A4__OFF;
	LED_G1__OFF;
	LED_G2__OFF;

	//
	// Enable unused sensors as shields to lower parasitic capacitances
	//
	CAPT_enableShieldIO(0,3);
	CAPT_enableShieldIO(1,3);
	CAPT_enableShieldIO(3,3);
	
	//
	// Register the proximity event handler with the range sensor
	//
	MAP_CAPT_registerCallback(&rangeSensor, &proximityEventHandler);
	
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
		// End of background loop iteration
		// Go to sleep if there is nothing left to do
		//
		CAPT_appSleep();
	} // End background loop
} // End main()

void proximityEventHandler(tSensor *s)
{
	tElement *e;
	uint16_t delta;

	//
	// Get a pointer to the proximity element
	//
	e = s->pCycle[0]->pElements[0];

	//
	// If a negative touch was detected, re-calibrate.
	//
	if (e->bNegativeTouch == true)
	{
		CAPT_calibrateSensor(s);
	}
	
	//
	// Otherwise, if a proximity event was detected,
	// process it.
	//
	else if (e->bProx == true)
	{
		delta = CAPT_DELTA(e);

		LED_A1__OFF;
		LED_A2__OFF;
		LED_A3__OFF;
		LED_A4__OFF;
		LED_G1__OFF;
		LED_G2__OFF;

		if (delta > 200)
		{
            LED_A1__ON;
			LED_A2__ON;
			LED_A3__ON;
			LED_A4__ON;
			LED_G1__ON;
			LED_G2__ON;
		}
		else if (delta > 100)
		{
			LED_A1__ON;
			LED_A2__ON;
			LED_A3__ON;
			LED_A4__ON;
		}
		else if (delta > 50)
		{
			LED_A1__ON;
			LED_A2__ON;
			LED_A3__ON;
		}
		else if (delta > 25)
		{
			LED_A1__ON;
			LED_A2__ON;
		}
		else if (delta > 12)
		{
			LED_A1__ON;
		}
	}
	
	//
	// If no proximity event is occurring, turn off the LEDs.
	//
	else
	{
		LED_A1__OFF;
		LED_A2__OFF;
		LED_A3__OFF;
		LED_A4__OFF;
		LED_G1__OFF;
		LED_G2__OFF;
	}
}
