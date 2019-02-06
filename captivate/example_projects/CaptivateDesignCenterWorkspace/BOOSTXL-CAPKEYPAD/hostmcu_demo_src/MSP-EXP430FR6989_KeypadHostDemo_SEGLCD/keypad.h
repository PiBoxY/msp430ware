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
//! \file   keypad.h
//
//! \brief  This module defines an interface to the BOOSTXL-CAPKEYPAD.
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

#ifndef KEYPAD_H_
#define KEYPAD_H_

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Keypad control word I2C register address
//!
#define KEYPAD_CTRL_WORD (0x00)

//! \brief The following definitions define the system control word.
//! The system control word is a 16-bit software register that controls
//! the operational state of the BOOSTXL-CAPKEYPAD.
//! This register is intended to be controlled by the host LaunchPad over the
//! host I2C interface.
//!
#define KEYPAD__CTRL_SHUTDOWN            (BIT0)
#define KEYPAD__CTRL_WOP_ENABLE          (BIT1)
#define KEYPAD__CTRL_UART_ENABLE         (BIT2)

//! \brief Keypad status word I2C register address
//!
#define KEYPAD_STAT_WORD (0x02)

//! \brief Keypad previous status word I2C register address
//!
#define KEYPAD_PREV_STAT_WORD (0x04)

//! \brief The following definitions define the system status word.
//! The system status word is a 16-bit software register that reports
//! the operational state of the BOOSTXL-CAPKEYPAD.
//! This register is intended to be read by the host LaunchPad over the
//! host I2C interface to determine the touch and proximity status of the system.
//! It is also used by internal software to control the LED backlighting.
//!
#define KEYPAD__STAT_ACTIVE       (BIT0)
#define KEYPAD__STAT_PROX         (BIT1)
#define KEYPAD__STAT_GUARD        (BIT2)
#define KEYPAD__STAT_TOUCH        (BIT3)
#define KEYPAD__STAT_KEY1         (BIT4)
#define KEYPAD__STAT_KEY2         (BIT5)
#define KEYPAD__STAT_KEY3         (BIT6)
#define KEYPAD__STAT_KEY4         (BIT7)
#define KEYPAD__STAT_KEY5         (BIT8)
#define KEYPAD__STAT_KEY6         (BIT9)
#define KEYPAD__STAT_KEY7         (BITA)
#define KEYPAD__STAT_KEY8         (BITB)
#define KEYPAD__STAT_KEY9         (BITC)
#define KEYPAD__STAT_KEYENTER     (BITD)
#define KEYPAD__STAT_KEY0         (BITE)
#define KEYPAD__STAT_KEYCANCEL    (BITF)
#define KEYPAD__STAT_ALLKEYS      (0xFFF0)

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Start the keypad communication channel
//! \param none
void Keypad_start(void);

//! \brief Get the status of the keypad IRQ signal
//! \param none
bool Keypad_getIRQStatus(void);

//! \brief Read the current status of the keypad
//! \param *currentStatus is a pointer to the status variable to update
//! \param *previousStatus is a pointer to a previous status holder to update
void Keypad_readStatus(uint16_t *currentStatus, uint16_t *previousStatus);

#endif /* KEYPAD_H_ */
