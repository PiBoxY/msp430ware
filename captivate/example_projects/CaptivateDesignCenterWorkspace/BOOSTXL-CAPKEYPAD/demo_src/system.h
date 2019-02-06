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
//! \file   system.h
//!
//! \brief  This is the system information file for the BOOSTXL-CAPKEYPAD.
//! It defines the control and status structure, and makes it available to
//! other modules in the application.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef SYSTEM_H_
#define SYSTEM_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <stdint.h>
#include <msp430.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief The following definitions define the system control word.
//! The system control word is a 16-bit software register that controls
//! the operational state of the BOOSTXL-CAPKEYPAD.
//! This register is intended to be controlled by the host LaunchPad over the
//! host I2C interface.
//!
#define SYSTEM__CTRL_SHUTDOWN            (BIT0)
#define SYSTEM__CTRL_WOP_ENABLE          (BIT1)
#define SYSTEM__CTRL_UART_ENABLE         (BIT2)

//! \brief The following definitions define the system status word.
//! The system status word is a 16-bit software register that reports
//! the operational state of the BOOSTXL-CAPKEYPAD.
//! This register is intended to be read by the host LaunchPad over the
//! host I2C interface to determine the touch and proximity status of the system.
//! It is also used by internal software to control the LED backlighting.
//!
#define SYSTEM__STAT_ACTIVE       (BIT0)
#define SYSTEM__STAT_PROX         (BIT1)
#define SYSTEM__STAT_GUARD        (BIT2)
#define SYSTEM__STAT_TOUCH        (BIT3)
#define SYSTEM__STAT_KEY1         (BIT4)
#define SYSTEM__STAT_KEY2         (BIT5)
#define SYSTEM__STAT_KEY3         (BIT6)
#define SYSTEM__STAT_KEY4         (BIT7)
#define SYSTEM__STAT_KEY5         (BIT8)
#define SYSTEM__STAT_KEY6         (BIT9)
#define SYSTEM__STAT_KEY7         (BITA)
#define SYSTEM__STAT_KEY8         (BITB)
#define SYSTEM__STAT_KEY9         (BITC)
#define SYSTEM__STAT_KEYENTER     (BITD)
#define SYSTEM__STAT_KEY0         (BITE)
#define SYSTEM__STAT_KEYCANCEL    (BITF)
#define SYSTEM__STAT_ALLKEYS      (0xFFF0)

//! \brief  System_controlAndStatus is the control and status
//! structure definition.  The structure provides a 16-bit control word,
//! a 16-bit current status word, and a 16-bit last (previous) status word
//! for looking at history in the system.  The last status word reflects
//! the status word settings from the previous sample.
//!
typedef union
{
    struct
    {
        uint16_t controlWord;
        uint16_t statusWord;
        uint16_t lastStatusWord;
    };
    uint8_t regFile[6];
} System_controlAndStatus;


//! \brief SYSTEM_BYTE_REGISTER_COUNT defines the read register size
//!
#define SYSTEM_BYTE_REGISTER_COUNT (sizeof(System_controlAndStatus))

//! \brief SYSTEM_CTRL_BYTE_REGISTER_COUNT defines the read/write byte count
//! in the System_controlAndStatus structure.
//!
#define SYSTEM_CTRL_BYTE_REGISTER_COUNT (sizeof(uint16_t))


//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief System_topLevel is the single instantiation of the
//! System_controlAndStatus structure.
extern volatile System_controlAndStatus System_topLevel;

#endif /* SYSTEM_H_ */
