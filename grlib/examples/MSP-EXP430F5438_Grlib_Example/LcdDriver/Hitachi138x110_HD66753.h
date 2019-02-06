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
//*****************************************************************************
//
// Hitachi138x110_HD66753.h - Prototypes for the Hitachi138x110 LCD
//                                     display driver with a HD66753
//                                     controller.
//*****************************************************************************

#ifndef __Hitachi138x110_HD66753_H__
#define __Hitachi138x110_HD66753_H__

//*****************************************************************************
//
// Include Files
//
//*****************************************************************************
#include "grlib.h"

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// SYSTEM_CLOCK_SPEED (in Hz)
#define SYSTEM_CLOCK_SPEED      16000000

#define LCD_HORIZONTAL_MAX        138
#define LCD_VERTICAL_MAX          110

// Define LCD Screen Orientation Here
#define LANDSCAPE
//#define LANDSCAPE_FLIP

// Grayscale level definitions
#define PIXEL_OFF               3
#define PIXEL_LIGHT             2
#define PIXEL_DARK              1
#define PIXEL_ON                0

#define INVERT_TEXT             BIT0
#define OVERWRITE_TEXT          BIT2
#define GRAYSCALE_TEXT          BIT1

//*****************************************************************************
//
// This driver operates in two different screen orientations.  They are:
//
// * Landscape - The screen is wider than it is tall, and there are more pins and the
//               white band on the top of the display. This is selected by defining
//               LANDSCAPE in the User Configuration section above.
//
// * Landscape flip - The screen is wider than it is tall, and there are more pins
//               and the white band on the bottom of the display. This is selected by
//               defining LANDSCAPE_FLIP in the User Configuration section above.
//
// These can also be imagined in terms of screen rotation; if landscape mode is 0 degrees
// of rotation, landscape flip is 180 degrees of rotation
//
// If no screen orientation is selected, "landscape" mode will be used.
//
//*****************************************************************************
#if !defined(LANDSCAPE) && !defined(LANDSCAPE_FLIP)
#define LANDSCAPE
#endif

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Hitachi138x110_HD66753_initDisplay(void);
extern void Hitachi138x110_HD66753_enableDisplay(void);
extern void Hitachi138x110_HD66753_disableDisplay(void);
extern void Hitachi138x110_HD66753_invertDisplay(void);
extern void Hitachi138x110_HD66753_setBacklight(uint8_t brightness);
extern void Hitachi138x110_HD66753_setContrast(uint8_t newContrast);
extern void Hitachi138x110_HD66753_turnOffBacklight(void);

extern const Graphics_Display g_sHitachi138x110_HD66753;

#endif // __Hitachi138x110_HD66753_H__
