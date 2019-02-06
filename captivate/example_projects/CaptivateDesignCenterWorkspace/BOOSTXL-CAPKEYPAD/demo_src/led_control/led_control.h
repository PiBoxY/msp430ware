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
//! \file   led_control.h
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

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include "LP3943.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief LED pin definitions
//! These definitions define the mapping between keys and LED pins on the LED
//! driver.
//!
#define LEDCONTROL__LED_KEY3          (0)
#define LEDCONTROL__LED_KEY6          (1)
#define LEDCONTROL__LED_KEY9          (2)
#define LEDCONTROL__LED_KEYCANCEL     (3)
#define LEDCONTROL__LED_KEY0          (4)
#define LEDCONTROL__LED_KEY8          (5)
#define LEDCONTROL__LED_KEY2          (6)
#define LEDCONTROL__LED_KEY5          (7)
#define LEDCONTROL__LED_KEYENTER      (8)
#define LEDCONTROL__LED_KEY7          (9)
#define LEDCONTROL__LED_KEY4          (10)
#define LEDCONTROL__LED_KEY1          (11)
#define LEDCONTROL__LED_DETECT        (12)
#define LEDCONTROL__LED_AWAKE         (13)
#define LEDCONTROL__LED_IDLE_PWM      (LP3943_PWM_20)

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Initialize the LED control module.
//! This includes opening the software I2C master driver, clearing the
//! RAM representation of the LED state, and writing the default configuration
//! to the LED driver via the softare I2C master interface.
//! \param none
//! \return none
extern void LEDControl_init(void);

//! \brief Set the duty cycle of a PWM on the LED driver.
//! \param pwm is the pwm to set
//! \param dutyCycle is the duty cycle to set to that PWM (0-255)
//! \return none
extern void LEDControl_setPWMDutyCycle(uint8_t pwm, uint8_t dutyCycle);

//! \brief Get the duty cycle of a PWM on the LED driver
//! \param pwm is the pwm to get the duty cycle from
//! \return the PWM duty cycle (0-255) for the requested PWM
extern uint8_t LEDControl_getPWMDutyCycle(uint8_t pwm);

//! \brief Update the LEDs on the BOOSTXL-CAPKEYPAD based on the system status.
//! \param none
//! \return none
extern void LEDControl_updateLEDs(void);

//! \brief Suspend LEDs on the BOOSTXL-CAPKEYPAD.
//! This sets all LEDs to the 'OFF' setting.
//! \param none
//! \return none
extern void LEDControl_suspendLEDs(void);

#endif /* LED_CONTROL_H_ */
