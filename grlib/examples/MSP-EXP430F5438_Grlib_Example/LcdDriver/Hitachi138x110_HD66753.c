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
// Hitachi138x110_HD66753.c - Display driver for the Hitachi EA Hitachi102W-6 Dot Matrix
//                                     display with a HD66753 controller.
//
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "grlib.h"
#include "driverlib.h"
#include "Hitachi138x110_HD66753.h"
#include "HAL_MSP_EXP430F5438_HITACHI138x110_HD66753.h"

uint16_t Hitachi138x110Memory[18 * 110] = {0};
//extern uint16_t FFT_Image[];

uint8_t ucBacklightLevel = 8;
uint8_t ucContrast = 0x66;

uint8_t LcdInitializationMacro[] = {
    0x74, 0x00, 0x00, 0x76, 0x00, 0x01,  // R00 start oscillation
    0x74, 0x00, 0x01, 0x76, 0x00, 0x0D,  // R01 driver output control
    0x74, 0x00, 0x02, 0x76, 0x00, 0x4C,  // R02 LCD - driving waveform control
    0x74, 0x00, 0x03, 0x76, 0x12, 0x14,  // R03 Power control
    0x74, 0x00, 0x04, 0x76, 0x04, 0x66,  // R04 Contrast control
    0x74, 0x00, 0x05, 0x76, 0x00, 0x10,  // R05 Entry mode
    0x74, 0x00, 0x06, 0x76, 0x00, 0x00,  // R06 RAM data write mask
    0x74, 0x00, 0x07, 0x76, 0x00, 0x15,  // R07 Display control
    0x74, 0x00, 0x08, 0x76, 0x00, 0x03,  // R08 Cursor Control
    0x74, 0x00, 0x09, 0x76, 0x00, 0x00,  // R09 RAM data write mask
    0x74, 0x00, 0x0A, 0x76, 0x00, 0x15,  // R0A
    0x74, 0x00, 0x0B, 0x76, 0x00, 0x03,  // R0B Horizontal Cursor Position
    0x74, 0x00, 0x0C, 0x76, 0x00, 0x03,  // R0C Vertical Cursor Position
    0x74, 0x00, 0x0D, 0x76, 0x00, 0x00,  // R0D
    0x74, 0x00, 0x0E, 0x76, 0x00, 0x15,  // R0E
    0x74, 0x00, 0x0F, 0x76, 0x00, 0x03,  // R0F
    0x74, 0x00, 0x10, 0x76, 0x00, 0x15,  // R0E
    0x74, 0x00, 0x11, 0x76, 0x00, 0x03,  // R0F
};

uint8_t Set_Address_Macro[] = {0x74, 0x00, 0x11, 0x76, 0x00, 0x00};
uint8_t Draw_Block_Macro[] = {0x74, 0x00, 0x12, 0x76, 0xFF, 0xFF};

void Hitachi138x110_HD66753_initDisplay(void)
{
    HAL_LCD_initLCD();

    // Wake-up the LCD as per datasheet specifications
    Hitachi138x110_HD66753_enableDisplay();

    // LCD Initialization Routine Using Predefined Macros
    HAL_LCD_writeCommand(&LcdInitializationMacro[1 * 6]);
    HAL_LCD_writeCommand(&LcdInitializationMacro[2 * 6]);
    HAL_LCD_writeCommand(&LcdInitializationMacro[4 * 6]);
    HAL_LCD_writeCommand(&LcdInitializationMacro[5 * 6]);
    HAL_LCD_writeCommand(&LcdInitializationMacro[6 * 6]);
    HAL_LCD_writeCommand(&LcdInitializationMacro[7 * 6]);

    // Clear Display and turn on backlight
    Hitachi138x110_HD66753_setBacklight(ucBacklightLevel);
    Hitachi138x110_HD66753_setContrast(ucContrast);
    Hitachi138x110_HD66753_invertDisplay();
}

//*****************************************************************************
//
//! Enables the display driver.
//!
//! This function enables the HD66753 display controller on the panel,
//! and also clears display data.
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_enableDisplay(void)
{
    // R00 start oscillation
    HAL_LCD_writeCommand(LcdInitializationMacro);

    // Wait a minimum of 25ms after issuing "start oscillation"
    __delay_cycles(SYSTEM_CLOCK_SPEED * .025);

    LcdInitializationMacro[3 * 6 + 5] |= BIT3;
    LcdInitializationMacro[3 * 6 + 5] &= ~BIT0;
    // R03 Power control
    HAL_LCD_writeCommand(&LcdInitializationMacro[3 * 6]);
}

//*****************************************************************************
//
//! Disables the display driver.
//!
//! This function disables the HD66753 display controller on the panel,
//! and also clears display data.
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_disableDisplay(void)
{
    // Disable backlight
    Hitachi138x110_HD66753_turnOffBacklight();

    ucBacklightLevel = 0;

    // LCD screen in standby
    LcdInitializationMacro[3 * 6 + 5] &= (~BIT3) & (~BIT2);
    LcdInitializationMacro[3 * 6 + 5] |= BIT0;
    HAL_LCD_writeCommand(&LcdInitializationMacro[3 * 6]);
}

//*****************************************************************************
//
//! Inverts the display color.
//!
//! This function inverts the colors displayed without changing
//! any data in the local buffer
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_invertDisplay(void)
{
    LcdInitializationMacro[7 * 6 + 5] ^= BIT1;
    HAL_LCD_writeCommand(&LcdInitializationMacro[7 * 6]);
}

//*****************************************************************************
//
//! Sets the backlight.
//!
//! This function sets the backlight level of the display,
//! from a level of 0-11 where 11 is the brightest setting.
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_setBacklight(uint8_t ucBrightness)
{
    uint16_t dutyCycle = 0, i, dummy;

    if(ucBrightness > 0)
    {
        HAL_LCD_initTimer(0);

        dummy = HAL_LCD_getTimerCaptureCompare();

        for(i = 0; i < ucBrightness; i++)
        {
            dutyCycle += dummy;
        }

        HAL_LCD_setTimerDutyCycle(dutyCycle);

        // If the backlight was previously turned off, turn it on.
        if(!ucBacklightLevel)
        {
            HAL_LCD_startTimerCounter();
        }
    }
    else
    {
        Hitachi138x110_HD66753_turnOffBacklight();
    }
    ucBacklightLevel = ucBrightness;
}

//*****************************************************************************
//
//! Shuts off the backlight.
//!
//! This function shuts off the backlight of the display
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_turnOffBacklight(void)
{
    HAL_LCD_turnOffBackLight();

    ucBacklightLevel = 0;
}

//*****************************************************************************
//
//! Sets the contrast.
//!
//! This function sets the contrast of the HD66753 display controller,
//! from a level of 70-127 where 127 is the darkest setting.
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753_setContrast(uint8_t ucNewContrast)
{
    if(ucNewContrast > 127)
    {
        ucNewContrast = 127;
    }
    if(ucNewContrast < 70)
    {
        ucNewContrast = 70;
    }
    LcdInitializationMacro[0x04 * 6 + 5] = ucNewContrast;
    HAL_LCD_writeCommand(&LcdInitializationMacro[0x04 * 6]);
    ucContrast = ucNewContrast;
}

//*****************************************************************************
//
// Sets the pixel address of the HD66753.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
static void Hitachi138x110_HD66753_setAddress(int16_t lX,
                                              int16_t lY)
{
    uint16_t ulAddress;
    ulAddress = (lY << 5) + (lX >> 3);            //Narrow down to 8 possible pixels
    Set_Address_Macro[4] = ulAddress >> 8;
    Set_Address_Macro[5] = ulAddress & 0xFF;
    HAL_LCD_writeCommand(Set_Address_Macro);
}

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753PixelDraw(void *pvDisplayData,
                                int16_t lX,
                                int16_t lY,
                                uint16_t ulValue)
{
    uint16_t ulBufferLocation, ulBufferValue, ulPixelOffset;

    //
    // Pixel location in the global LCD buffer
    //
    ulBufferLocation = lY * 18 + (lX >> 3);

    //
    // Pixel offset into the global buffer word containing 8 2-bit pixels
    // Multiply the X%8 by 2 because each pixel requires 2 bits
    //
    ulPixelOffset = (lX & 0x07) << 1;

    //
    // Extract the current value in the buffer so that the new
    // pixel won't affect the other 7 pixels present
    //
    ulBufferValue = Hitachi138x110Memory[ulBufferLocation];

    //
    // Clear the current pixel value. 0x03 = 0b11 to clear
    // 2 bits that represent the single pixel
    //
    ulBufferValue &= ~(0x03 << ulPixelOffset);

    //
    // Add in the new pixel value
    //
    ulBufferValue |= ulValue << ulPixelOffset;

    //
    // Rewrite the new buffer element to the buffer
    //
    Hitachi138x110Memory[ulBufferLocation] = ulBufferValue;

    //
    // Set the address for the pixel write
    //
    Hitachi138x110_HD66753_setAddress(lX, lY);

    //
    // Update Macro for write to LCD
    //
    Draw_Block_Macro[4] = ulBufferValue >> 8;
    Draw_Block_Macro[5] = ulBufferValue & 0xFF;

    //
    // Write new data to LCD
    //
    HAL_LCD_writeCommand(Draw_Block_Macro);
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753PixelDrawMultiple(void *pvDisplayData,
                                        int16_t lX,
                                        int16_t lY,
                                        int16_t lX0,
                                        int16_t lCount,
                                        int16_t lBPP,
                                        const uint8_t *pucData,
                                        const uint32_t *pucPalette)
{
    uint16_t ulBufferLocation, ulPixelOffset;
    uint16_t ulByte, ulValue;

    Hitachi138x110_HD66753_setAddress(lX, lY);

    ulBufferLocation = lY * 18 + (lX >> 3);
    ulPixelOffset = (lX & 0x07) << 1;

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel. Only 1BPP and 2BPP is supported on this display
    //
    if(lBPP == 1)
    {
        // The pixel data is in 1 bit per pixel format

        // Loop while there are more pixels to draw
        while(lCount > 0)
        {
            // Get the next byte of image data
            ulByte = *pucData++;

            // Loop through the pixels in this byte of image data
            for(; (lX0 < 8) && lCount; lX0++, lCount--)
            {
                ulValue = ((uint32_t *)pucPalette)[(ulByte >> (7 - lX0)) & 1];

                if(ulValue)
                {
                    // Set bits for White Pixel
                    Hitachi138x110Memory[ulBufferLocation] |=
                        (0x03 << ulPixelOffset);
                }
                else
                {
                    // Clear bits for Black Pixel
                    Hitachi138x110Memory[ulBufferLocation] &=
                        ~(0x03 << ulPixelOffset);
                }
                // End of integer, write to LCD
                // Update pixel offset and buffer location
                if(ulPixelOffset == 0x0E)
                {
                    if(lCount != 0)
                    {
                        // Update data in macro
                        Draw_Block_Macro[4] =
                            Hitachi138x110Memory[ulBufferLocation] >> 8;
                        Draw_Block_Macro[5] =
                            Hitachi138x110Memory[ulBufferLocation] & 0xFF;
                        // Write pixel data to LCD
                        HAL_LCD_writeCommand(Draw_Block_Macro);
                        ulBufferLocation++;
                        ulPixelOffset = 0;
                    }
                }
                else
                {
                    ulPixelOffset += 2;
                }
            }

            // Start at the beginning of the next byte of image data
            lX0 = 0;
        }
        // Update data in macro for final write
        Draw_Block_Macro[4] = Hitachi138x110Memory[ulBufferLocation] >> 8;
        Draw_Block_Macro[5] = Hitachi138x110Memory[ulBufferLocation] & 0xFF;
        // Write pixel data to LCD
        HAL_LCD_writeCommand(Draw_Block_Macro);
        // The image data has been drawn
    }
    else if(lBPP == 2)
    {
        // Loop while there are more pixels to draw
        while(lCount > 0)
        {
            // Get the next byte of image data
            ulByte = *pucData++;

            // Loop through the pixels in this byte of image data
            for(; (lX0 < 4) && lCount; lX0++, lCount--)
            {
                ulValue =
                    ((uint32_t *)pucPalette)[(ulByte >> (6 - (lX0 << 1))) & 3];

                // Clear pixel bits
                Hitachi138x110Memory[ulBufferLocation] &=
                    ~(0x03 << ulPixelOffset);
                // Add in new pixel bits
                Hitachi138x110Memory[ulBufferLocation] |= ulValue <<
                ulPixelOffset;
                // Update pixel offset and buffer location
                if(ulPixelOffset == 0x0E)
                {
                    if(lCount != 0)
                    {
                        // Update Macro for write to LCD
                        Draw_Block_Macro[4] =
                            Hitachi138x110Memory[ulBufferLocation] >> 8;
                        Draw_Block_Macro[5] =
                            Hitachi138x110Memory[ulBufferLocation] & 0xFF;
                        // Write pixel data to LCD
                        HAL_LCD_writeCommand(Draw_Block_Macro);
                        ulBufferLocation++;
                        ulPixelOffset = 0;
                    }
                }
                else
                {
                    ulPixelOffset += 2;
                }
            }
            // Start at the beginning of the next byte of image data
            lX0 = 0;
        }
        // Update data in macro for final write
        Draw_Block_Macro[4] = Hitachi138x110Memory[ulBufferLocation] >> 8;
        Draw_Block_Macro[5] = Hitachi138x110Memory[ulBufferLocation] & 0xFF;
        // Write pixel data to LCD
        HAL_LCD_writeCommand(Draw_Block_Macro);
        // The image data has been drawn
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753LineDrawH(void *pvDisplayData,
                                int16_t lX1,
                                int16_t lX2,
                                int16_t lY,
                                uint16_t ulValue)
{
    uint16_t ulBufferLocation, ulPixelMask1, ulPixelMask2, ulBufferValue,
             ulByte;
    uint8_t ucWordAddress1, ucWordAddress2;

    Hitachi138x110_HD66753_setAddress(lX1, lY);

    // Create pixel masks for the start/end of the line
    ulPixelMask1 = 0xFFFF << ((lX1 & 0x07) << 1);
    ulPixelMask2 = 0xFFFF >> (0x0E - ((lX2 & 0x07) << 1));

    // Find buffer locations of the start/end of the line
    ucWordAddress1 = lX1 >> 3;  //lX1/16 * 2BPP;
    ucWordAddress2 = lX2 >> 3;  //lX2/16 * 2BPP;

    // Get current buffer value
    ulBufferLocation = lY * 18 + ucWordAddress1;
    ulBufferValue = Hitachi138x110Memory[ulBufferLocation];

    // Horizontal Line spans more than 1 integer write to LCD
    if(ucWordAddress1 != ucWordAddress2)
    {
        switch(ulValue)
        {
        // Black
        case 0:
            ulBufferValue &= ~ulPixelMask1;
            break;
        // Dark Gray
        case 1:
            ulBufferValue &= ~ulPixelMask1;
            ulByte = 0x5555 & ulPixelMask1;
            ulBufferValue |= ulByte;
            break;
        // Light Gray
        case 2:
            ulBufferValue &= ~ulPixelMask1;
            ulByte = 0xAAAA & ulPixelMask1;
            ulBufferValue |= ulByte;
            break;
        // White
        case 3:
            ulBufferValue |= ulPixelMask1;
            break;
        default:
            ulBufferValue &= ~ulPixelMask1;
            break;
        }
        // Rewrite the new buffer element to the buffer
        Hitachi138x110Memory[ulBufferLocation++] = ulBufferValue;
        // Update Macro for write to LCD
        Draw_Block_Macro[4] = ulBufferValue >> 8;
        Draw_Block_Macro[5] = ulBufferValue & 0xFF;
        // Write new data to LCD
        HAL_LCD_writeCommand(Draw_Block_Macro);
        ucWordAddress1++;

        while(ucWordAddress1 < ucWordAddress2)
        {
            switch(ulValue)
            {
            // Black
            case 0:
                Hitachi138x110Memory[ulBufferLocation++] = 0x0000;
                Draw_Block_Macro[4] = 0x00;
                Draw_Block_Macro[5] = 0x00;
                HAL_LCD_writeCommand(Draw_Block_Macro);
                break;
            // Dark Gray
            case 1:
                Hitachi138x110Memory[ulBufferLocation++] = 0x5555;
                Draw_Block_Macro[4] = 0x55;
                Draw_Block_Macro[5] = 0x55;
                HAL_LCD_writeCommand(Draw_Block_Macro);
                break;
            // Light Gray
            case 2:
                Hitachi138x110Memory[ulBufferLocation++] = 0xAAAA;
                Draw_Block_Macro[4] = 0xAA;
                Draw_Block_Macro[5] = 0xAA;
                HAL_LCD_writeCommand(Draw_Block_Macro);
                break;
            // White
            case 3:
                Hitachi138x110Memory[ulBufferLocation++] = 0xFFFF;
                Draw_Block_Macro[4] = 0xFF;
                Draw_Block_Macro[5] = 0xFF;
                HAL_LCD_writeCommand(Draw_Block_Macro);
                break;
            default:
                Hitachi138x110Memory[ulBufferLocation++] = 0x0000;
                Draw_Block_Macro[4] = 0x00;
                Draw_Block_Macro[5] = 0x00;
                HAL_LCD_writeCommand(Draw_Block_Macro);
                break;
            }
            ucWordAddress1++;
        }

        // Get buffer value at end of the line
        ulBufferValue = Hitachi138x110Memory[ulBufferLocation];
        switch(ulValue)
        {
        // Black
        case 0:
            ulBufferValue &= ~ulPixelMask2;
            break;
        // Dark Gray
        case 1:
            ulBufferValue &= ~ulPixelMask2;
            ulByte = 0x5555 & ulPixelMask2;
            ulBufferValue |= ulByte;
            break;
        // Light Gray
        case 2:
            ulBufferValue &= ~ulPixelMask2;
            ulByte = 0xAAAA & ulPixelMask2;
            ulBufferValue |= ulByte;
            break;
        // White
        case 3:
            ulBufferValue |= ulPixelMask2;
            break;
        default:
            ulBufferValue &= ~ulPixelMask2;
            break;
        }
        // Rewrite the new buffer element to the buffer
        Hitachi138x110Memory[ulBufferLocation] = ulBufferValue;
        // Update Macro for write to LCD
        Draw_Block_Macro[4] = ulBufferValue >> 8;
        Draw_Block_Macro[5] = ulBufferValue & 0xFF;
        // Write new data to LCD
        HAL_LCD_writeCommand(Draw_Block_Macro);
    }
    // Horizontal line can be written with 1 integer write
    else
    {
        // Combine start/end pixel masks
        ulPixelMask1 &= ulPixelMask2;
        switch(ulValue)
        {
        // Black
        case 0:
            ulBufferValue &= ~ulPixelMask1;
            break;
        // Dark Gray
        case 1:
            ulBufferValue &= ~ulPixelMask1;
            ulByte = 0x5555 & ulPixelMask1;
            ulBufferValue |= ulByte;
            break;
        // Light Gray
        case 2:
            ulBufferValue &= ~ulPixelMask1;
            ulByte = 0xAAAA & ulPixelMask1;
            ulBufferValue |= ulByte;
            break;
        // White
        case 3:
            ulBufferValue |= ulPixelMask1;
            break;
        default:
            ulBufferValue &= ~ulPixelMask1;
            break;
        }
        // Rewrite the new buffer element to the buffer
        Hitachi138x110Memory[ulBufferLocation] = ulBufferValue;
        // Update Macro for write to LCD
        Draw_Block_Macro[4] = ulBufferValue >> 8;
        Draw_Block_Macro[5] = ulBufferValue & 0xFF;
        // Write new data to LCD
        HAL_LCD_writeCommand(Draw_Block_Macro);
    }
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753LineDrawV(void *pvDisplayData,
                                int16_t lX,
                                int16_t lY1,
                                int16_t lY2,
                                uint16_t ulValue)
{
    do
    {
        Hitachi138x110_HD66753PixelDraw(pvDisplayData, lX, lY1, ulValue);
    }
    while(lY1++ < lY2);
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753RectFill(void *pvDisplayData,
                               const Graphics_Rectangle *pRect,
                               uint16_t ulValue)
{
    int16_t x0 = pRect->sXMin;
    int16_t x1 = pRect->sXMax;
    int16_t y0 = pRect->sYMin;
    int16_t y1 = pRect->sYMax;

    do
    {
        Hitachi138x110_HD66753LineDrawH(pvDisplayData, x0, x1, y0, ulValue);
    }
    while(y0++ < y1);
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static uint32_t
Hitachi138x110_HD66753ColorTranslate(void *pvDisplayData,
                                     uint32_t ulValue)
{
    return(((((ulValue) & 0x00ff0000) >> 23) +
            (((ulValue) & 0x0000ff00) >> 15) +
            (((ulValue) & 0x000000ff))) >> 7);
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display. This function isn't
//! implemented for the main buffer because each driver operation updates the
//! buffer and the display. It is used however to draw the FFT Image. This image
//! is placed in a buffer called FFT_Image[] instead of Hitachi138x110Memory[].
//! This flush does not update Hitachi138x110Memory[].
//!
//! \return None.
//
//*****************************************************************************
static void
Hitachi138x110_HD66753Flush(void *pvDisplayData)
{
    // Draw 128x84 pixel image for FFT image
    // 128 pixels = 256 bits for 2BPP
    // 256 bits / 16 bits/write = 16 writes per line
    uint16_t * pulData = (uint16_t *)pvDisplayData;
    int16_t i, j;

    for(i = 0; i < 84; i++)
    {
        Hitachi138x110_HD66753_setAddress(1, 12 + i);
        for(j = 0; j < 16; j++)
        {
            Draw_Block_Macro[4] = *pulData >> 8;
            Draw_Block_Macro[5] = *pulData++ & 0xFF;
            HAL_LCD_writeCommand(Draw_Block_Macro);
        }
    }
}

//*****************************************************************************
//
//! Clear the screen
//!
//! This function "clears" by filling the entire screen with white pixels
//! and changes the corresponding pixels in the buffer as well
//!
//! \return None.
//
//*****************************************************************************
void Hitachi138x110_HD66753ClearDisplay(void *pvDisplayData,
                                        uint16_t ulValue)
{
    uint16_t i, j;

    //
    // Update Macro for write to LCD
    //
    Draw_Block_Macro[4] = 0xFF;
    Draw_Block_Macro[5] = 0xFF;

    //Row Address
    for(i = 0; i < 110; i++)
    {
        Hitachi138x110_HD66753_setAddress(0,i);

        //Column Address
        for(j = 0; j < 18; j++)
        {
            //
            // Update Buffer
            //
            Hitachi138x110Memory[i * 18 + j] = 0xFFFF;

            //
            // Write new data to LCD
            //
            HAL_LCD_writeCommand(Draw_Block_Macro);
        }
    }
}

//*****************************************************************************
//
//! The display structure that describes the driver for the Hitachi
//! 138x110 LCD with an HD66753 controller.
//
//*****************************************************************************
const Graphics_Display g_sHitachi138x110_HD66753 =
{
    sizeof(tDisplay),
    0,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
#else
    LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX,
#endif
    Hitachi138x110_HD66753PixelDraw,
    Hitachi138x110_HD66753PixelDrawMultiple,
    Hitachi138x110_HD66753LineDrawH,
    Hitachi138x110_HD66753LineDrawV,
    Hitachi138x110_HD66753RectFill,
    Hitachi138x110_HD66753ColorTranslate,
    Hitachi138x110_HD66753Flush,
    Hitachi138x110_HD66753ClearDisplay
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
