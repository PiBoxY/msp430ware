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
//! \file   display.c
//
//  Group:          MSP
//  Target Device:  MSP430F5529
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "display.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief The font selection to use for numerals
#define NUMBER_FONT (g_sFontCmss48b)

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Graphic library context
//!
Graphics_Context g_sContext;

//! \brief The current line
//!
uint_fast16_t line;

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Display_start(void)
{
    Kitronix320x240x16_SSD2119Init();
    Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);
    Graphics_clearDisplay(&g_sContext);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
}

void Display_drawTitleBar(void)
{
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GOLD);
    Graphics_drawStringCentered(&g_sContext, "BOOSTXL-CAPKEYPAD DEMO", AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_drawLine(&g_sContext, 15, 30, 305, 30);
    Graphics_setFont(&g_sContext, &NUMBER_FONT);
}

void Display_writeTextBox(char* text)
{
    Graphics_drawStringCentered(&g_sContext, (int8_t*)text, AUTO_STRING_LENGTH, 159, line, OPAQUE_TEXT);
}

void Display_lineReturn(void)
{
    line += NUMBER_FONT.height + 5;
}

void Display_clearTextBox(void)
{
    Graphics_Rectangle textBox = { 0, 31, 319, 239};

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_fillRectangle(&g_sContext, &textBox);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    line = 60;
}

void Display_clearAll(void)
{
    Graphics_clearDisplay(&g_sContext);
}
