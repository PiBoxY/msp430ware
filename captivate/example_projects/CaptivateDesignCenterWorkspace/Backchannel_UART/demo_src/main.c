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
// \file   main.c
//
// Description:  This program exports data from the CapTIvate BSWP demo board to the
//         terminal through the back channel UART at a baud rate of 9600 bits\sec.
//         It uses the SMCLK as the clock source and the device is put into LPM0.
//         When the touch-board detects a button press, the device transmit a character
//         over UART to indicate which button was pressed.
//         This demo uses the built-in functions for UART communication on the MSP430 device family
//        and is intended to give programmers a starting point for data
//         extraction when using the MSP430 series devices.
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2018, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
// Written by Nathan Block
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************


#include <msp430.h>                      // Generic MSP430 Device Include
#include "driverlib.h"                   // MSPWare Driver Library
#include "captivate.h"                   // CapTIvate Touch Software Library
#include "CAPT_App.h"                    // CapTIvate Application Code
#include "demo.h"

void main(void)
{
    //Set the watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Initialize the MCU
	// BSP_configureMCU() sets up the device IO and clocking
	// The global interrupt enable is set to allow peripherals
	// to wake the MCU.
	BSP_configureMCU();

    // Start the CapTIvate application
    CAPT_appStart();

    //Register the keypad sensor to the function handler
    MAP_CAPT_registerCallback(&keypadSensor, &keypad_callback);

    //open the port for communication
    UART_openPort(&UARTPort);

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
