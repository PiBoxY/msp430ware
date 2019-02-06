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
//! \brief  This application demonstrates how to use the MSP-EXP430FR6989
//!         LaunchPad evaluation tool with the BOOSTXL-CAPKEYPAD touch keypad.
//!         The example records keystrokes from the touch keypad, and draws them
//!         on the LCD display.
//
//  Group:          MSP
//  Target Device:  MSP430FR6989
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

#include "board.h"
#include "display.h"
#include "keypad.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Keypad control word I2C register address
//!
#define DEMO_TIMEOUT (300)

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Demo_inputString keeps a history of keystrokes on the keypad
//!
char Demo_inputString[12];

//! \brief Demo_currkeypadStatus stores the current status of the keypad.
//!
uint16_t Demo_currkeypadStatus;

//! \brief Demo_currkeypadStatus stores the previous status of the keypad.
//!
uint16_t Demo_prevkeypadStatus;

//! \brief Demo_timeoutCounter tests for inactivity on the panel.
//!
uint16_t Demo_timeoutCounter;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Add a new keystroke to the input string
//! \param keypadStatus is the current keypad status.
int_fast8_t Demo_appendNewKeystrokes(uint16_t keypadStatus);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void main(void)
{
    // Set up system, IO, and clocks
    Board_initSys();
    Board_initIO();
    Board_initClocks();

    // Enable interrupts
    __enable_interrupt();

    // Start the LCD display
    Display_start();

    // Set up communication with the keypad over the I2C interface
    Keypad_start();
    Demo_timeoutCounter = 0;

    // Reset the input string memory to null.
    memset(&Demo_inputString[0], 0, sizeof(Demo_inputString));

    // This is the application loop.
    while(1)
    {
        // If an IRQ is set, update the keypad status and LCD accordingly
        if (Keypad_getIRQStatus() == true)
        {
            // Read the current keypad status
            Keypad_readStatus(&Demo_currkeypadStatus, &Demo_prevkeypadStatus);

            if (Demo_currkeypadStatus & KEYPAD__STAT_PROX)
            {
                Demo_timeoutCounter = DEMO_TIMEOUT;
            }
            else if (Demo_timeoutCounter > 0)
            {
                Demo_timeoutCounter--;
            }
            else
            {
                memset(&Demo_inputString[0], 0, sizeof(Demo_inputString));
                Display_clear();
            }

            if (Demo_currkeypadStatus & KEYPAD__STAT_GUARD)
            {
                GPIO_setOutputHighOnPin(LED2_PORT, LED2_PIN);
            }
            else
            {
                GPIO_setOutputLowOnPin(LED2_PORT, LED2_PIN);
            }

            if (Demo_currkeypadStatus & KEYPAD__STAT_TOUCH)
            {
                GPIO_setOutputHighOnPin(LED1_PORT, LED1_PIN);
                if (!(Demo_prevkeypadStatus & KEYPAD__STAT_TOUCH))
                {
                    if (Demo_currkeypadStatus & KEYPAD__STAT_KEYCANCEL)
                    {
                        memset(&Demo_inputString[0], 0, sizeof(Demo_inputString));
                        Display_showChar('C', pos1);
                        Display_showChar('A', pos2);
                        Display_showChar('N', pos3);
                        Display_showChar('C', pos4);
                        Display_showChar('E', pos5);
                        Display_showChar('L', pos6);
                    }
                    else if (Demo_currkeypadStatus & KEYPAD__STAT_KEYENTER)
                    {
                        memset(&Demo_inputString[0], 0, sizeof(Demo_inputString));
                        Display_showChar('E', pos1);
                        Display_showChar('N', pos2);
                        Display_showChar('T', pos3);
                        Display_showChar('E', pos4);
                        Display_showChar('R', pos5);
                        Display_showChar(' ', pos6);
                    }
                    else
                    {
                        if (Demo_appendNewKeystrokes(Demo_currkeypadStatus) > 0)
                        {
                            Display_showChar(Demo_inputString[0], pos1);
                            Display_showChar(Demo_inputString[1], pos2);
                            Display_showChar(Demo_inputString[2], pos3);
                            Display_showChar(Demo_inputString[3], pos4);
                            Display_showChar(Demo_inputString[4], pos5);
                            Display_showChar(Demo_inputString[5], pos6);
                        }
                    }
                }
            }
            else
            {
                GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
            }
        }

        __bis_SR_register(LPM4_bits);
    }
}

int_fast8_t Demo_appendNewKeystrokes(uint16_t keypadStatus)
{
    uint16_t testBit;
    int_fast8_t added;
    uint8_t i;
    char newKeystroke[2];

    testBit = KEYPAD__STAT_KEY1;
    newKeystroke[0] = '1';
    newKeystroke[1] = '\0';
    added = 0;

    if (strlen(Demo_inputString) >= (sizeof(Demo_inputString)-1))
    {
        return -1;
    }

    for (i=0; i<9; i++)
    {
        // New touch, register the appropriate key
        if (keypadStatus & testBit)
        {
            strncat(Demo_inputString, newKeystroke, sizeof(Demo_inputString) - strlen(Demo_inputString) - 1);
            added++;
        }
        newKeystroke[0]++;
        testBit <<= 1;
    }

    if (keypadStatus & KEYPAD__STAT_KEY0)
    {
        newKeystroke[0] = '0';
        strncat(Demo_inputString, newKeystroke, sizeof(Demo_inputString) - strlen(Demo_inputString) - 1);
        added++;
    }

    return added;
}
