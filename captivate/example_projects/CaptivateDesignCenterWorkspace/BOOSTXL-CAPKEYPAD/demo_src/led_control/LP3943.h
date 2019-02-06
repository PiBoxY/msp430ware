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
//! \file   LP3943.h
//!
//! \brief  Provide control abstraction for the LP3932 LED driver,
//!         which allows for on/off/pwm control of up to 16 LEDs via
//!         the I2C bus.
//
//  Group:          MSP
//  Target Device:  MSP430
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef LP3943_H_
#define LP3943_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "msp430_swi2c_master.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief LP3943 register file definition
//!
#define LP3943_REG_INPUT1		0x00
#define LP3943_REG_INPUT2		0x01
#define LP3943_RW_REGISTERS		0x02
#define LP3943_REG_PSC0			0x02
#define LP3943_REG_PWM0			0x03
#define LP3943_REG_PSC1			0x04
#define LP3943_REG_PWM1			0x05
#define LP3943_REG_LS0			0x06
#define LP3943_REG_LS1			0x07
#define LP3943_REG_LS2			0x08
#define LP3943_REG_LS3			0x09

//! \brief LP3943 Auto increment register index bit
//! Set to write sequential registers in one burst
//!
#define LP3943_AUTO_INCREMENT_ENABLE	0x10

//! \brief LP3943 Prescalar options, 160 Hz to 32 Hz
//!
#define LP3943_PSC_160HZ		0
#define LP3943_PSC_80HZ			1
#define LP3943_PSC_60HZ			2
#define LP3943_PSC_40HZ			3
#define LP3943_PSC_32HZ			4

//! \brief LP3943 PWM duty cycle options, 0% to 100%
//!
#define LP3943_PWM_0			0x00
#define LP3943_PWM_5            0x0C
#define LP3943_PWM_10			0x19
#define LP3943_PWM_20			0x32
#define LP3943_PWM_25			0x40
#define LP3943_PWM_30			0x4C
#define LP3943_PWM_40			0x66
#define LP3943_PWM_50			0x80
#define LP3943_PWM_60			0x99
#define LP3943_PWM_70			0xB2
#define LP3943_PWM_75			0xC0
#define LP3943_PWM_80			0xCC
#define LP3943_PWM_90			0xE5
#define LP3943_PWM_100			0xFF

//! \brief LP3943 Register configuration
//!
#define LP3943_CONFIG_REG_CNT (6)
#define LP3943_LED_REG_CNT (4)

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief The LP3943 configuration registers.
//!
typedef union {
	struct
	{
		uint8_t inputReg1;
		uint8_t inputReg2;
		uint8_t	prescalar0;
		uint8_t pwm0;
		uint8_t prescalar1;
		uint8_t pwm1;
	};
	uint8_t configBuffer[LP3943_CONFIG_REG_CNT];
} LP3943_configRegisters;

//! \brief The available LED modes.
//!
enum
{
	LP3943_ledOff=0,
	LP3943_ledOn,
	LP3943_ledDimOption0,
	LP3943_ledDimOption1
};
typedef uint8_t LP3943_ledMode;

//! \brief The LED control registers (on/off/PWM0/PWM1)
//!
typedef union
{
	struct
	{
		LP3943_ledMode led0  : 2;
		LP3943_ledMode led1  : 2;
		LP3943_ledMode led2  : 2;
		LP3943_ledMode led3  : 2;

		LP3943_ledMode led4  : 2;
		LP3943_ledMode led5  : 2;
		LP3943_ledMode led6  : 2;
		LP3943_ledMode led7  : 2;

		LP3943_ledMode led8  : 2;
		LP3943_ledMode led9  : 2;
		LP3943_ledMode led10 : 2;
		LP3943_ledMode led11 : 2;

		LP3943_ledMode led12 : 2;
		LP3943_ledMode led13 : 2;
		LP3943_ledMode led14 : 2;
		LP3943_ledMode led15 : 2;
	};
	uint8_t ledBuffer[LP3943_LED_REG_CNT];
	uint32_t ledBitField;
} LP3943_ledRegisters;

//! \typedef The LP3943 complete device register file.
//!
typedef union
{
	struct
	{
		LP3943_configRegisters configReg;
		LP3943_ledRegisters ledReg;
		uint8_t address;
	};
	uint8_t regFile[LP3943_CONFIG_REG_CNT + LP3943_LED_REG_CNT];
} LP3943_device;

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Initialize the LP3943 device by writing its configuration registers
//! with the default values provided by the device structure that is passed in.
//! \param *device is the device to init
//! \return none
extern void LP3943_init(LP3943_device *device);

//! \brief Update LED PWM parameters from memory to the device
//! \param *device is the device to update
//! \return none
extern void LP3943_updateLEDPWMFromMemory(LP3943_device *device);

//! \brief Update the LED states from memory to the device itself
//! \param *device is the device config
//! \return none
extern void LP3943_updateLEDDriverFromMemory(LP3943_device *device);

//! \brief Set the state of an LED in local memory (not yet written to device)
//! \param *device is the device to update
//! \param led is the led status to update
//! \param mode is the new LED mode to set
//! \return none
extern void LP3943_setLEDStateInMemory(LP3943_device *device, uint8_t led,
		LP3943_ledMode mode);

//! \brief Set all LEDs on in local memory (not yet written to the device)
//! \param *device is the device to update
//! \return none
extern void LP3943_setAllLEDsOnInMemory(LP3943_device *device);

//! \brief Set all LEDs off in local memory (not yet written to the device)
//! \param *device is the device to update
//! \return none
extern void LP3943_setAllLEDsOffInMemory(LP3943_device *device);

//! \brief Set the state of an LED in local memory and on the device
//! immediately
//! \param *device is the device to update
//! \param led is the led status to update
//! \param mode is the new LED mode to set
//! \return none
extern void LP3943_setLEDStateImmediately(LP3943_device *device, uint8_t led,
		LP3943_ledMode mode);

#endif /* LP3943_H_ */
