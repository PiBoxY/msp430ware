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
//! \brief  This application demonstrates how to use the MSP-EXP430F5529LP
//!         LaunchPad evaluation tool with the BOOSTXL-CAPKEYPAD touch keypad
//!         as a USB HID keyboard device.
//
//  Group:          MSP
//  Target Device:  MSP430F5529
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <msp430.h>
#include <driverlib.h>

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_HID_API/UsbHid.h"
#include "USB_app/keyboard.h"

#include "board.h"
#include "keypad.h"

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Key HID frame send completion status
//!
volatile uint8_t Demo_keySendComplete = TRUE;

//! \brief BOOSTXL-CAPKEYPAD to keyboard keystroke lookup table
//!
uint8_t Demo_keyLookupTable[12] =
{
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    KEY_RETURN,
    '0',
    KEY_BACKSPACE
};

//! \brief Demo_currkeypadStatus stores the current status of the keypad.
//!
uint16_t Demo_currkeypadStatus;

//! \brief Demo_currkeypadStatus stores the previous status of the keypad.
//!
uint16_t Demo_prevkeypadStatus;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Update the HID keyboard report based on the keypad status vars.
//! \param keypadStatus is the current keypad status.
//! \parma prevKeypadStatus is the previous keypad status.
void Demo_updateHIDReport(uint16_t keypadStatus,
                                 uint16_t prevKeypadStatus);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main (void)
{
    // Set up system, IO, and clocks
    Board_initSys();
    Board_initIO();
    Board_initClocks();

    // Start the USB HID keyboard interface
    Keyboard_init();
    USB_setup(TRUE, TRUE);

    // Enable interrupts
    __enable_interrupt();

    // Set up communication with the keypad over the I2C interface
    Keypad_start();

    while (1)
    {
        switch(USB_getConnectionState())
        {
            // This case is executed while your device is enumerated on the
            // USB host
            case ST_ENUM_ACTIVE:

                // Sleep until a key is pressed
                __bis_SR_register(CPUOFF);

                // If an IRQ is set, update the keypad status and LCD accordingly
                if (Keypad_getIRQStatus() == true)
                {
                    // Read the current keypad status
                    Keypad_readStatus(&Demo_currkeypadStatus, &Demo_prevkeypadStatus);

                    if (Demo_currkeypadStatus & KEYPAD__STAT_GUARD)
                    {
                        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
                    }
                    else
                    {
                        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
                    }

                    if (Demo_currkeypadStatus & KEYPAD__STAT_TOUCH)
                    {
                        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    }
                    else
                    {
                        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                    }

                    // Update the HID keyboard report
                    Demo_updateHIDReport(Demo_currkeypadStatus, Demo_prevkeypadStatus);
                }
                break;

            // These cases are executed while your device is disconnected from
            // the host (meaning, not enumerated); enumerated but suspended
            // by the host, or connected to a powered hub without a USB host
            // present.
            case ST_PHYS_DISCONNECTED:
            case ST_ENUM_SUSPENDED:
            case ST_PHYS_CONNECTED_NOENUM_SUSP:
                __bis_SR_register(LPM3_bits + GIE);
                _NOP();
                break;

            // The default is executed for the momentary state
            // ST_ENUM_IN_PROGRESS.  Usually, this state only last a few
            // seconds.  Be sure not to enter LPM3 in this state; USB
            // communication is taking place here, and therefore the mode must
            // be LPM0 or active-CPU.
            case ST_ENUM_IN_PROGRESS:
            default:;
        }
    }  //while(1)
} //main()

void Demo_updateHIDReport(uint16_t keypadStatus, uint16_t prevKeypadStatus)
{
    uint16_t testBit;
    uint8_t i;

    testBit = KEYPAD__STAT_KEY1;

    for (i=0; i<12; i++)
    {
        // Test for a state change to know if a report needs to be updated
        if ((keypadStatus & testBit) != (prevKeypadStatus & testBit))
        {
            // If the new status is PRESSED, update accordingly
            if (keypadStatus & testBit)
            {
                Demo_keySendComplete = FALSE;
                Keyboard_press(Demo_keyLookupTable[i]);
                while(!Demo_keySendComplete);
            }

            // Else if the new status is RELEASED, update accordingly
            else
            {
                Demo_keySendComplete = FALSE;
                Keyboard_release(Demo_keyLookupTable[i]);
                while(!Demo_keySendComplete);
            }
        }
        testBit <<= 1;
    }
}

#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
             UCS_clearFaultFlag(UCS_XT2OFFG);
            UCS_clearFaultFlag(UCS_DCOFFG);
            SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:
            // If the CPU accesses USB memory while the USB module is
            // suspended, a "bus error" can occur.  This generates an NMI.  If
            // USB is automatically disconnecting in your software, set a
            // breakpoint here and see if execution hits it.  See the
            // Programmer's Guide for more information.
            SYSBERRIV = 0; //clear bus error flag
            USB_disable(); //Disable
    }
}
