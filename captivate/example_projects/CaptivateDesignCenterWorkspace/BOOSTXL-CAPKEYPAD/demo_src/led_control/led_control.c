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
//! \file   led_control.c
//!
//! \brief  This file implements the LED lighting.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "system.h"
#include "led_control.h"

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief This structure represents the LP3943 LED driver configuration.
//!
LP3943_device LEDControl_ledDriver =
{
    .address = 0x60,
    .configReg.prescalar0 = LP3943_PSC_160HZ,
    .configReg.prescalar1 = LP3943_PSC_160HZ,
    .configReg.pwm0 = LP3943_PWM_0,
    .configReg.pwm1 = LP3943_PWM_0
};

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void LEDControl_init(void)
{
    SWI2C_initI2C();
    LP3943_setAllLEDsOffInMemory(&LEDControl_ledDriver);
    LP3943_init(&LEDControl_ledDriver);
}

void LEDControl_setPWMDutyCycle(uint8_t pwm, uint8_t dutyCycle)
{
    if (pwm == 0)
    {
        LEDControl_ledDriver.configReg.pwm0 = dutyCycle;
    }
    else if (pwm == 1)
    {
        LEDControl_ledDriver.configReg.pwm1 = dutyCycle;
    }
    LP3943_updateLEDPWMFromMemory(&LEDControl_ledDriver);
}

uint8_t LEDControl_getPWMDutyCycle(uint8_t pwm)
{
    if (pwm == 0)
    {
        return LEDControl_ledDriver.configReg.pwm0;
    }
    else if (pwm == 1)
    {
        return LEDControl_ledDriver.configReg.pwm1;
    }

    return 0;
}

void LEDControl_updateLEDs(void)
{
    LP3943_ledMode newMode;

    // Awake LED
    if (System_topLevel.statusWord & SYSTEM__STAT_ACTIVE)
    {
        newMode = LP3943_ledDimOption1;
    }
    else
    {
        newMode = LP3943_ledOff;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_AWAKE,
            newMode
        );

    // Detect LED
    if (System_topLevel.statusWord & SYSTEM__STAT_TOUCH)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledOff;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_DETECT,
            newMode
        );

    // Key 1 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY1)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY1,
            newMode
        );

    // Key 2 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY2)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY2,
            newMode
        );

    // Key 3 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY3)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY3,
            newMode
        );

    // Key 4 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY4)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY4,
            newMode
        );

    // Key 5 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY5)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY5,
            newMode
        );

    // Key 6 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY6)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY6,
            newMode
        );

    // Key 7 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY7)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY7,
            newMode
        );

    // Key 8 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY8)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY8,
            newMode
        );

    // Key 9 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY9)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY9,
            newMode
        );

    // Enter Key LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEYENTER)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEYENTER,
            newMode
        );

    // Key 0 LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEY0)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEY0,
            newMode
        );

    // Cancel Key LED
    if (System_topLevel.statusWord & SYSTEM__STAT_KEYCANCEL)
    {
        newMode = LP3943_ledOn;
    }
    else
    {
        newMode = LP3943_ledDimOption0;
    }
    LP3943_setLEDStateInMemory(
            &LEDControl_ledDriver,
            LEDCONTROL__LED_KEYCANCEL,
            newMode
        );

    LP3943_updateLEDDriverFromMemory(&LEDControl_ledDriver);
}

void LEDControl_suspendLEDs(void)
{
    LP3943_setAllLEDsOffInMemory(&LEDControl_ledDriver);
    LP3943_updateLEDDriverFromMemory(&LEDControl_ledDriver);
}
