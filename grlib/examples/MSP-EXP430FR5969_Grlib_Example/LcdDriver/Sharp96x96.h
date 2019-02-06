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
// Sharp96x96.h - Prototypes for the Sharp96x96 LCD display driver
//
//*****************************************************************************

#ifndef __SHARPLCD_H__
#define __SHARPLCD_H__

#include "grlib.h"

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// SYSTEM_CLOCK_SPEED (in Hz) allows to properly closeout SPI communication
#define SYSTEM_CLOCK_SPEED      12000000

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   96
#define LCD_HORIZONTAL_MAX                 96

// Define LCD Screen Orientation Here
#define LANDSCAPE

//Maximum Colors in an image color palette
#define MAX_PALETTE_COLORS  2

//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************
#define SHARP_BLACK                                                     0x00
#define SHARP_WHITE                                                     0xFF
#define SHARP_SEND_TOGGLE_VCOM_COMMAND          0x01
#define SHARP_SKIP_TOGGLE_VCOM_COMMAND          0x00
#define SHARP_LCD_TRAILER_BYTE                          0x00
#define SHARP_VCOM_TOGGLE_BIT                           0x40
#define SHARP_LCD_CMD_CHANGE_VCOM                       0x00
#define SHARP_LCD_CMD_CLEAR_SCREEN                      0x20
#define SHARP_LCD_CMD_WRITE_LINE                        0x80

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern const Graphics_Display g_sharp96x96LCD;

extern void Sharp96x96_SendToggleVCOMCommand(void);
extern void Sharp96x96_initDisplay(void);
extern void Sharp96x96_disable(void);
extern void Sharp96x96_enable(void);
#endif // __SHARPLCD_H__
