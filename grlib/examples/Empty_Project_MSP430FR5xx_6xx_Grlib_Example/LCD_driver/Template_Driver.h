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
// Template_Driver.h - Display driver for any LCD Controller. This file serves as
//						a template for creating new LCD driver files
//
//*****************************************************************************

#ifndef __Template_Driver_H__
#define __Template_Driver_H__

//*****************************************************************************
//
// Include Files
//
//*****************************************************************************
#include "grlib.h"
/*Put any necessary include files here*/

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************
// Number of pixels on LCD X-axis
// TemplateDisplayFix
#define LCD_X_SIZE      1

// Number of pixels on LCD Y-axis
// TemplateDisplayFix
#define LCD_Y_SIZE      1

// Number of bits required to draw one pixel on the LCD screen
// TemplateDisplayFix
#define BPP     1

// Define LCD Screen Orientation Here
#define LANDSCAPE
//#define LANDSCAPE_FLIP
//#define PORTRAIT
//#define PORTRAIT_FLIP

//*****************************************************************************
//
// Defines for the pins that are used to communicate with the LCD Driver
//
//*****************************************************************************

/*Define Pin Names here i.e. #define LCD_RESET_OUT	P6OUT*/

//*****************************************************************************
//
// Defines for LCD driver configuration
//
//*****************************************************************************

/* Defines for pixels, colors, masks, etc. Anything Template_Driver.c needs*/

//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide. This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall. This is selected by defining
//               LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide. This is
//                   selected by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall. This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if landscape mode is
// 0 degrees of screen rotation, portrait flip is 90 degrees of clockwise
// rotation, landscape flip is 180 degrees of rotation, and portrait is
// 270 degress of clockwise rotation.
//
// If no screen orientation is selected, "landscape" mode will be used.
//
//*****************************************************************************
#if !defined(PORTRAIT) && !defined(PORTRAIT_FLIP) && \
    !defined(LANDSCAPE) && !defined(LANDSCAPE_FLIP)
#define LANDSCAPE
#endif

//*****************************************************************************
//
// Various definitions controlling coordinate space mapping and drawing
// direction in the four supported orientations.
//
//*****************************************************************************
#ifdef PORTRAIT
#define MAPPED_X(x, y) (LCD_X_SIZE - (y) - 1)
#define MAPPED_Y(x, y) (x)
#endif
#ifdef LANDSCAPE
#define MAPPED_X(x, y) (x)
#define MAPPED_Y(x, y) (y)
#endif
#ifdef PORTRAIT_FLIP
#define MAPPED_X(x, y)  (y)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (x) - 1)
#endif
#ifdef LANDSCAPE_FLIP
#define MAPPED_X(x, y)  (LCD_X_SIZE - (x) - 1)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (y) - 1)
#endif

//*****************************************************************************
//
// Various LCD Controller command name labels and associated control bits
//
//*****************************************************************************

//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************

/* All macros can go here. This is typically the color translation function (example below)
   and could also include Set_Address(), Write_Data(), etc. */

//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color. This particular example
// requires the 8-8-8 24 bit RGB color to convert into 5-6-5 16 bit RGB Color
// Your conversion should be made to fit your LCD settings.
//
// \return Returns the display-driver specific color
//
// TemplateDisplayFix

#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00f80000) >> 8) |               \
                                 (((c) & 0x0000fc00) >> 5) |               \
                                 (((c) & 0x000000f8) >> 3))

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Template_DriverInit(void);
extern const Graphics_Display g_sTemplate_Driver;
extern uint8_t Template_Memory[];

#endif // __Template_Driver_H__
