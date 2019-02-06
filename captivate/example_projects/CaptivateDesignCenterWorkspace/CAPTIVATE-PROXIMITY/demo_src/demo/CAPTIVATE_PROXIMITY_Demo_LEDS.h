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
#ifndef CAPTIVATE_PROXIMITY_DEMO_LEDS_H_
#define CAPTIVATE_PROXIMITY_DEMO_LEDS_H_
//#############################################################################
//
//! \file   CAPTIVATE_PROXIMITY_Demo_LEDS.h
//!
//! \brief  This module defines the LED pin-out and corresponding macros for
//!         LED on, off, and toggle for the CAPTIVATE-PROXIMITY-Demo.
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
// Includes
//*****************************************************************************

#include <msp430.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Defines the left LED POUT register
//!
#define LED_LEFT__POUT (P3OUT)

//! \brief Defines the left LED PDIR register
//!
#define LED_LEFT__PDIR (P3DIR)

//! \brief Defines the left LED pin
//!
#define LED_LEFT__PIN (BIT2)

//! \brief Turn off the left LED
//!
#define LED_LEFT__OFF (LED_LEFT__POUT |= LED_LEFT__PIN)

//! \brief Turn on the left LED
//!
#define LED_LEFT__ON  (LED_LEFT__POUT &= ~LED_LEFT__PIN)

//! \brief Toggle the left LED
//!
#define LED_LEFT__TOGGLE (LED_LEFT__POUT ^= LED_LEFT__PIN)

//! \brief Defines the center LED POUT register
//!
#define LED_CENTER__POUT (P2OUT)

//! \brief Defines the center LED PDIR register
//!
#define LED_CENTER__PDIR (P2DIR)

//! \brief Defines the center LED pin
//!
#define LED_CENTER__PIN (BIT7)

//! \brief Turn off the center LED
//!
#define LED_CENTER__OFF (LED_CENTER__POUT |= LED_CENTER__PIN)

//! \brief Turn on the center LED
//!
#define LED_CENTER__ON (LED_CENTER__POUT &= ~LED_CENTER__PIN)

//! \brief Toggle the center LED
//!
#define LED_CENTER__TOGGLE (LED_CENTER__POUT ^= LED_CENTER__PIN)

//! \brief Defines the right LED POUT register
//!
#define LED_RIGHT__POUT (P2OUT)

//! \brief Defines the right LED PDIR register
//!
#define LED_RIGHT__PDIR (P2DIR)

//! \brief Defines the right LED pin
//!
#define LED_RIGHT__PIN (BIT5)

//! \brief Turn off the right LED
//!
#define LED_RIGHT__OFF (LED_RIGHT__POUT |= LED_RIGHT__PIN)

//! \brief Turn on the right LED
//!
#define LED_RIGHT__ON (LED_RIGHT__POUT &= ~LED_RIGHT__PIN)

//! \brief Toggle the right LED
//!
#define LED_RIGHT__TOGGLE (LED_RIGHT__POUT ^= LED_RIGHT__PIN)

//! \brief Defines the ring LED POUT register
//!
#define LED_RING__POUT (P2OUT)

//! \brief Defines the ring LED PDIR register
//!
#define LED_RING__PDIR (P2DIR)

//! \brief Defines the ring LED pin
//!
#define LED_RING__PIN (BIT4)

//! \brief Turn off the ring LED
//!
#define LED_RING__OFF (LED_RING__POUT |= LED_RING__PIN)

//! \brief Turn on the ring LED
//!
#define LED_RING__ON (LED_RING__POUT &= ~LED_RING__PIN)

//! \brief Toggle the ring LED
//!
#define LED_RING__TOGGLE (LED_RING__POUT ^= LED_RING__PIN)

//! \brief Defines the swipe left LED POUT register
//!
#define LED_SWIPELEFT__POUT (P3OUT)

//! \brief Defines the swipe left LED PDIR register
//!
#define LED_SWIPELEFT__PDIR (P3DIR)

//! \brief Defines the swipe left LED pin
//!
#define LED_SWIPELEFT__PIN (BIT1)

//! \brief Turn off the swipe left LED
//!
#define LED_SWIPELEFT__OFF (LED_SWIPELEFT__POUT |= LED_SWIPELEFT__PIN)

//! \brief Turn on the swipe left LED
//!
#define LED_SWIPELEFT__ON (LED_SWIPELEFT__POUT &= ~LED_SWIPELEFT__PIN)

//! \brief Toggle the swipe left LED
//!
#define LED_SWIPELEFT__TOGGLE (LED_SWIPELEFT__POUT ^= LED_SWIPELEFT__PIN)

//! \brief Defines the swipe right LED POUT register
//!
#define LED_SWIPERIGHT__POUT          (P2OUT)

//! \brief Defines the swipe right LED PDIR register
//!
#define LED_SWIPERIGHT__PDIR          (P2DIR)

//! \brief Defines the swipe right LED pin
//!
#define LED_SWIPERIGHT__PIN           (BIT3)

//! \brief Turn off the swipe right LED
//!
#define LED_SWIPERIGHT__OFF           (LED_SWIPERIGHT__POUT |= LED_SWIPERIGHT__PIN)

//! \brief Turn on the swipe right LED
//!
#define LED_SWIPERIGHT__ON            (LED_SWIPERIGHT__POUT &= ~LED_SWIPERIGHT__PIN)

//! \brief Toggle the swipe right LED
//!
#define LED_SWIPERIGHT__TOGGLE        (LED_SWIPERIGHT__POUT ^= LED_SWIPERIGHT__PIN)

#endif /* CAPTIVATE_PROXIMITY_DEMO_LEDS_H_ */
