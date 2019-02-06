/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//******************************************************************************
//!  TI-Design Signal Processing with LEA on MSP430FR5994
//!
//!  William Goh
//!  Texas Instruments Inc.
//!  April 2016
//******************************************************************************

#ifndef DAC8311_H_
#define DAC8311_H_

#include <stdint.h>

// MOSI and SCLK is located on P1.6, and P1.4
#define DAC8311_MOSI_PORT   GPIO_PORT_P5
#define DAC8311_MOSI_PIN    GPIO_PIN0
#define DAC8311_MOSI_PIN_FUNCTION            GPIO_PRIMARY_MODULE_FUNCTION
#define DAC8311_SCLK_PORT   GPIO_PORT_P5
#define DAC8311_SCLK_PIN    GPIO_PIN2
#define DAC8311_SCLK_PIN_FUNCTION            GPIO_PRIMARY_MODULE_FUNCTION

// DAC SYNC pin is located on P8.2
#define DAC8311_SYNC_PORT   GPIO_PORT_P8
#define DAC8311_SYNC_PIN    GPIO_PIN2

// Definition of USCI base address to be used for SPI communication
#define DAC8311_EUSCI_BASE          EUSCI_B1_BASE

// Defines to put DAC8311 in various modes
#define DAC8311_NORMAL_MODE         (0x0000)
#define DAC8311_OUTPUT_1K_TO_GND    (0x4000)
#define DAC8311_OUTPUT_100K_TO_GND  (0x8000)
#define DAC8311_OUTPUT_HIGHZ        (0xC000)

//******************************************************************************
//
//! \brief  Initializes the SPI module
//!
//! Current example uses USCI_B0
//!
//! \param  none
//!
//! \return none
//
//******************************************************************************
void DAC8311_init(void);

//******************************************************************************
//
//! \brief  Updates the DAC output and forces the device in Normal Mode
//!
//! This sets the DAC8311 dac output value. DAC8311 is a 14-bit DAC.
//!
//! \param  value   Accepts a 16-bit value and clears bit-15 and bit-16 as this
//!                 forces the DAC8311 into a Normal Mode
//!
//! \return none
//
//******************************************************************************
void DAC8311_updateDacOut(uint16_t value);

//******************************************************************************
//
//! \brief  Sets the DAC into a low power mode
//!
//! DAC8311 can be set into 3 different low power modes. See datasheet for
//! more information. By calling this function, it clears the DAC8311 register
//! values.
//!
//! \param  mode   Sets DAC into 3 differet low power modes
//!                Valid values are:
//!                    - \b DAC8311_OUTPUT_1K_TO_GND
//!                    - \b DAC8311_OUTPUT_100K_TO_GND
//!                    - \b DAC8311_OUTPUT_HIGHZ
//!
//! \return none
//
//******************************************************************************
void DAC8311_setLowPowerMode(uint16_t mode);

#endif /* DAC8311_H_ */
