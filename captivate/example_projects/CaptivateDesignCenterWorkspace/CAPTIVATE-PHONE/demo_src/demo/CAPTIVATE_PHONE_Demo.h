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
#ifndef CAPTIVATE_PROXIMITY_DEMO_H_
#define CAPTIVATE_PROXIMITY_DEMO_H_
//#############################################################################
//
//! \file  CAPTIVATE_PHONE_Demo.h
//!
//! \brief This module contains the demonstration code for the
//!        CAPTIVATE-PHONE demonstration panel in the CAPTIVATE-FR2633 EVM kit.
//!        This module implements the application code that interprets touches
//!        and controls the panel LED's and the haptic feedback effects. 
//
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Initialize the CAPTIVATE-PHONE-Demo application.  This call will
//!        setup the I2C master driver, initialize the DRV2605L haptic effect
//!        driver, and associate handlers with sensors.
extern void Demo_init(void);

//! \brief Check the UI status to determine if a valid touch has occurred.
//!        If a valid touch is detected, LED1 will be illuminated.  If the
//!        guard channel is in detect, LED2 will be illuminated.
extern void Demo_checkForValidTouch(void);

#endif /* CAPTIVATE_PROXIMITY_USEREXPERIENCE_H_ */
