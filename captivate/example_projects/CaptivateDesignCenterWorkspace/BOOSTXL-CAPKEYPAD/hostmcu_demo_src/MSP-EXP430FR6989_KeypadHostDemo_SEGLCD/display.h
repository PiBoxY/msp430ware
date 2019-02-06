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
//! \file   display.h
//
//! \brief  This module defines a display abstraction layer for the
//!         segmented LCD.
//
//  Group:          MSP
//  Target Device:  MSP430FR6989
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef DISPLAY_H_
#define DISPLAY_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <driverlib.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//Change based on LCD Memory locations
#define pos1 9   /* Digit A1 begins at S18 */
#define pos2 5   /* Digit A2 begins at S10 */
#define pos3 3   /* Digit A3 begins at S6  */
#define pos4 18  /* Digit A4 begins at S36 */
#define pos5 14  /* Digit A5 begins at S28 */
#define pos6 7   /* Digit A6 begins at S14 */

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Start the display driver
//! \param none
void Display_start(void);

//! \brief Displays input character at given LCD digit/position
//! Only spaces, numeric digits, and uppercase letters are accepted characters
void Display_showChar(char c, uint8_t position);

//! \brief Scroll input string across LCD screen from left to right
//! \param msg is the string to scroll to the display
void Display_scrollText(char *msg);

//! \brief Clear the full display
//! \param none
void Display_clear(void);

#endif /* DISPLAY_H_ */
