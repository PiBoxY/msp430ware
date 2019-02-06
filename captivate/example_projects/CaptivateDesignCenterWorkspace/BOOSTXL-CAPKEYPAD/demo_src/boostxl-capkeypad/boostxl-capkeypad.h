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
//! \file   boostxl-capkeypad.h
//!
//! \brief  This is the BOOSTXL-CAPKEYPAD primary application module.
//! This module serves up the main routines that are called from main().
//! These routines are based on the CAPT_App routines, and have been
//! modified to suite the needs of the BOOSTXL-CAPKEYPAD application.
//!
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef BOOSTXL_CAPKEYPAD_H_
#define BOOSTXL_CAPKEYPAD_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>
#include "driverlib.h"
#include "captivate.h"
#include "led_control.h"

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Initialize the keypad.  This involves initializing the CapTIvate
//! peripheral and library, calibrating the keypad sensors, and starting the
//! CapTIvate interval timer based on the active mode settings.
//! \param none
//! \return none
extern void BXLCapKeypad_init(void);

//! \brief Stop the keypad. This shuts down CapTIvate and the LED driver.
//! \param none
//! \return none
extern void BXLCapKeypad_stop(void);

//! \brief Run the keypad update routine.  This routine will assess whether the
//! keypad is in active mode or wake-on-proximity sleep mode.  If in active mode,
//! the timer flag is tested to see if an update to the keypad is required.  In
//! wake-on-proximity mode, the conversion counter, detection, and error flags
//! are tested.  If they are set, they are handled accordingly and the required
//! operating mode is set.  If no flags are set, the routine will exit without
//! updating anything.
//! \param none
//! \return none
extern void BXLCapKeypad_run(void);

//! \brief Update the CapTIvate communications protocol.
//! This checks the incoming serial queue to see if the CapTIvate Design Center
//! has sent any messages.  If a message requires a re-calibration, it is
//! performed inside of this function.
//! \param none
//! \return none
extern void BXLCapKeypad_updateCapTIvateComm(void);

//! \brief Enter the application specified low power mode safely.
//! \param none
//! \return none
extern void BXLCapKeypad_sleep(void);

#endif /* BOOSTXL_CAPKEYPAD_H_ */
