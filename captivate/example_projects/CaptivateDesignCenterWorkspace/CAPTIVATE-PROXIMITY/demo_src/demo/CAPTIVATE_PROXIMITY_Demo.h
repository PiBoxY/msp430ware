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
//! \file   CAPTIVATE_PROXIMITY_Demo.h
//!
//! \brief  This is the CAPTIVATE_PROXIMITY_Demo module.  It contains the
//!         sensor handler that determines whether or not a swipe left/right
//!         has taken place.  It also sets the proximity status LED's.
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
// Global Variables
//*****************************************************************************

//! \brief Status variable that indicates the current swipe status.  This may
//!        be used by other parts of the application if desired.
//!
extern uint8_t Demo_swipeStatus;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Initialize the CAPTIVATE-PROXIMITY-Demo by associating the sensor
//!        callback and setting up the LEDs.
extern void Demo_init(void);

#endif /* CAPTIVATE_PROXIMITY_USEREXPERIENCE_H_ */
