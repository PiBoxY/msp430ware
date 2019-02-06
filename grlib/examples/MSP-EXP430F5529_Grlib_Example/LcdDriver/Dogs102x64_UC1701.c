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
#include <msp430f5529.h>
#include "grlib.h"
#include "driverlib.h"
#include "Dogs102x64_UC1701.h"
#include "HAL_MSP_EXP430F5529_DOGS102X64_UC1701.h"

uint8_t DOGS102x64Memory[816] = {0};
uint8_t ucBacklightLevel = 11;
uint8_t ucContrast = 11;

//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the UC1701 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************

void
Dogs102x64_UC1701_initLCD(void)
{
    HAL_LCD_initLCD();

    HAL_LCD_writeCommand(SET_SCROLL_LINE + 0x00);
#if (defined LANDSCAPE)     // 6:00 viewing angle
    HAL_LCD_writeCommand(SET_SEG_DIRECTION + SEG_MIRROR);
    HAL_LCD_writeCommand(SET_COM_DIRECTION + COM_NORMAL);
#else                       // 12:00 viewing angle
    HAL_LCD_writeCommand(SET_SEG_DIRECTION + SEG_NORMAL);
    HAL_LCD_writeCommand(SET_COM_DIRECTION + COM_MIRROR);
#endif
    HAL_LCD_writeCommand(SET_ALL_PIXEL_ON + DISABLE);
    HAL_LCD_writeCommand(SET_INVERSE_DISPLAY + INVERSE);
    HAL_LCD_writeCommand(SET_LCD_BIAS_RATIO + NINTH);
    HAL_LCD_writeCommand(SET_POWER_CONTROL + BOOSTER + REGULATOR + FOLLOWER);
    HAL_LCD_writeCommand(SET_VLCD_RESISTOR_RATIO + INTERNAL_RESISTOR_RATIO);
    HAL_LCD_writeCommand(SET_ELECTRONIC_VOLUME_MSB);
    HAL_LCD_writeCommand(SET_ELECTRONIC_VOLUME_LSB + ELECTRONIC_VOLUME_PM);
    HAL_LCD_writeCommand(SET_ADV_PROGRAM_CONTROL0_MSB);
    HAL_LCD_writeCommand(SET_ADV_PROGRAM_CONTROL0_LSB + TEMP_COMP_11);
    HAL_LCD_writeCommand(SET_DISPLAY_ENABLE + ENABLE);

    Dogs102x64_UC1701_setBacklight(ucBacklightLevel);
    Dogs102x64_UC1701_setContrast(ucContrast);
    Dogs102x64_UC1701_fillScreen(0x01);
}

//*****************************************************************************
//
//! Disables the display driver.
//!
//! This function disables the UC1701 display controller on the panel,
//! and also clears display data.
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_disable(void)
{
    Dogs102x64_UC1701_fillScreen(0x01);
    Dogs102x64_UC1701_setBacklight(0);
    HAL_LCD_writeCommand(SET_DISPLAY_ENABLE + ENABLE);
}

//*****************************************************************************
//
//! Enables the display driver.
//!
//! This function enables the UC1701 display controller on the panel.
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_enable(void)
{
    HAL_LCD_writeCommand(SET_DISPLAY_ENABLE + DISABLE);
}

//*****************************************************************************
//
//! Inverts the display color.
//!
//! This function inverts the colors displayed without changing
//! any data in the buffer
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_inverseDisplay(void)
{
    HAL_LCD_writeCommand(SET_INVERSE_DISPLAY + REGULAR);
}

//*****************************************************************************
//
//! Sets display color back to the default.
//!
//! This function sets the display color back to normal
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_clearInverseDisplay(void)
{
    HAL_LCD_writeCommand(SET_INVERSE_DISPLAY + INVERSE);
}

//*****************************************************************************
//
//! Fill screen with specified color
//!
//! This function fills the entire screen with white or black pixels
//! and changes the corresponding pixels in the buffer as well
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_fillScreen(uint32_t ulValue)
{
    uint16_t i, j;

    // White Pixels
    if(ulValue)
    {
        //Page Address
        for(i = 0; i < 8; i++)
        {
            Dogs102x64_UC1701_setAddress(0,i);
            //Column Address
            for(j = 0; j < 102; j++)
            {
                DOGS102x64Memory[i * 102 + j] = 0xFF;
                HAL_LCD_writeData(0xFF);
            }
        }
    }
    // Black Pixels
    else
    {
        //Page Address
        for(i = 0; i < 8; i++)
        {
            Dogs102x64_UC1701_setAddress(0,i);
            //Column Address
            for(j = 0; j < 102; j++)
            {
                DOGS102x64Memory[i * 102 + j] = 0x00;
                HAL_LCD_writeData(0x00);
            }
        }
    }
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
void Dogs102x64_UC1701_setBacklight(uint8_t brightness)
{
    uint16_t dutyCycle = 0, i, dummy;

    if(brightness > 0)
    {
        HAL_LCD_initTimer(0);

        dummy = HAL_LCD_getTimerCaptureCompare();

        dutyCycle = 12;

        for(i = 0; i < brightness; i++)
        {
            dutyCycle += dummy;
        }

        HAL_LCD_setTimerDutyCycle(dutyCycle);

        //If the backlight was previously turned off, turn it on.
        if(!ucBacklightLevel)
        {
            HAL_LCD_startTimerCounter();
        }
    }
    else
    {
        HAL_LCD_stopTimerCounter();
    }
    ucBacklightLevel = brightness;
}

//*****************************************************************************
//
//! Sets the contrast.
//!
//! This function sets the contrast of the UC1701 display controller,
//! from a level of 0-31 where 31 is the darkest setting.
//!
//! \return None.
//
//*****************************************************************************
void Dogs102x64_UC1701_setContrast(uint8_t newContrast)
{
    //check if parameter is in range
    if(newContrast > 31)
    {
        newContrast = 31;
    }

    HAL_LCD_writeCommand(SET_ELECTRONIC_VOLUME_MSB);
    HAL_LCD_writeCommand(SET_ELECTRONIC_VOLUME_LSB + newContrast);
    ucContrast = newContrast;
}

//******************************************************************************
// Sets the cursor to coordinates x, y.
//
// \param x and y are the LCD pixel coordinates to place the cursor
//
// This macro sets the cursor location. The LCD is broken down into
// column address and page address. The page address (0 - 7) splits the
// 64 pixel tall display into 8 pages. The column address
// requires a 2 byte write for the 4 LSBs and the 4 MSBs
// For 6:00 viewing the range for column address is (0 - 101)
// For 12:00 viewing all operations are the same except ca range is (30 - 131)
//
// \return None
//******************************************************************************
void Dogs102x64_UC1701_setAddress(uint16_t x,
                                  uint16_t y)
{
#if (defined LANDSCAPE)     // 6:00 viewing angle
    HAL_LCD_writeCommand(SET_PAGE_ADDRESS +
                         (uint16_t)(y));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_LSB +
                         (uint16_t)((x) & 0x0F));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_MSB +
                         (uint16_t)(((x) & 0xF0) >> 4));
#else // 12:00 viewing angle
    HAL_LCD_writeCommand(SET_PAGE_ADDRESS +
                         (uint16_t)(y));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_LSB +
                         (uint16_t)((x + 30) & 0x0F));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_MSB +
                         (uint16_t)(((x + 30) & 0xF0) >> 4));
#endif // LANDSCAPE
}

//*****************************************************************************
// Sets the cursor to coordinates x, y.
//
// \param x and y are the LCD pixel coordinates to place the cursor
//
// This macro sets the cursor location.This sets the inverse y page address
// For 6:00 viewing the range for column address is (0 - 101)
// For 12:00 viewing all operations are the same except ca range is (30 - 131)
//
// \return None
//*****************************************************************************
void Dogs102x64_UC1701_setInverseAddress(uint16_t x,
                                         uint16_t y)
{
#if (defined LANDSCAPE)     // 6:00 viewing angle
    HAL_LCD_writeCommand(SET_PAGE_ADDRESS +
                         (uint16_t)(7 - (y)));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_LSB +
                         (uint16_t)((x) & 0x0F));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_MSB +
                         (uint16_t)(((x) & 0xF0) >> 4));
#else                       // 12:00 viewing angle
    HAL_LCD_writeCommand(SET_PAGE_ADDRESS +
                         (uint16_t)(7 - (y)));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_LSB +
                         (uint16_t)((x + 30) & 0x0F));
    HAL_LCD_writeCommand(SET_COLUMN_ADDRESS_MSB +
                         (uint16_t)(((x + 30) & 0xF0) >> 4));
#endif
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
Dogs102x64_UC1701_drawPixel(void *pvDisplayData,
                            int16_t lX,
                            int16_t lY,
                            uint16_t ulValue)
{
    uint8_t ulPageAddress, ulPixelHeight;
    uint16_t ulBufferLocation;

    ulPageAddress = lY / 8;
    ulPixelHeight = 0x01 << (lY & 0x07);
    ulBufferLocation = ulPageAddress * 102 + lX;

    // White Pixel
    if(ulValue)
    {
        DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight;
    }
    // Black Pixel
    else
    {
        DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight;
    }

    Dogs102x64_UC1701_setAddress(lX, ulPageAddress);
    HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation]);
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
Dogs102x64_UC1701_drawMultiplePixel(void *pvDisplayData,
                                    int16_t lX,
                                    int16_t lY,
                                    int16_t lX0,
                                    int16_t lCount,
                                    int16_t lBPP,
                                    const uint8_t *pucData,
                                    const uint32_t *pucPalette)
{
    uint8_t ulPageAddress, ulPixelHeight;
    uint16_t ulBufferLocation;
    uint16_t Byte, ulValue;

    ulPageAddress = lY / 8;
    ulPixelHeight = 0x01 << (lY & 0x07);
    ulBufferLocation = ulPageAddress * 102 + lX;

    //
    // Set the cursor increment to left to right, followed by top to bottom.
    //
    Dogs102x64_UC1701_setAddress(lX, ulPageAddress);

    //
    // Determine how to interpret the pixel data based on the number of bits
    // per pixel. Only 1BPP is supported on this display
    //
    if(lBPP == 1)
    {
        // The pixel data is in 1 bit per pixel format

        // Loop while there are more pixels to draw
        while(lCount > 0)
        {
            // Get the next byte of image data
            Byte = *pucData++;

            // Loop through the pixels in this byte of image data
            for(; (lX0 < 8) && lCount; lX0++, lCount--)
            {
                ulValue = ((uint32_t *)pucPalette)[(Byte >> (7 - lX0)) & 1];

                // White Pixel
                if(ulValue)
                {
                    DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight;
                }
                // Black Pixel
                else
                {
                    DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight;
                }
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }

            // Start at the beginning of the next byte of image data
            lX0 = 0;
        }
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
Dogs102x64_UC1701_drawHLine(void *pvDisplayData,
                            int16_t lX1,
                            int16_t lX2,
                            int16_t lY,
                            uint16_t ulValue)
{
    uint8_t ulPageAddress, ulPixelHeight;
    uint16_t ulBufferLocation;

    ulPageAddress = lY / 8;
    ulPixelHeight = 0x01 << (lY & 0x07);
    ulBufferLocation = ulPageAddress * 102 + lX1;

    // Only need to set this address once,
    // the cursor is auto-incremented horizontally
    Dogs102x64_UC1701_setAddress(lX1, ulPageAddress);

    while(lX1++ <= lX2)
    {
        // White Pixels
        if(ulValue)
        {
            DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight;
        }
        // Black Pixels
        else
        {
            DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight;
        }
        HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
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
Dogs102x64_UC1701_drawVLine(void *pvDisplayData,
                            int16_t iX,
                            int16_t iY1,
                            int16_t iY2,
                            uint16_t ulValue)
{
    uint8_t ulPageAddress1, ulPageAddress2, ulPixelHeight1, ulPixelHeight2;
    uint16_t ulBufferLocation;

    ulPageAddress1 = iY1 / 8;
    ulPageAddress2 = iY2 / 8;

    ulPixelHeight1 = 0xFF << (iY1 & 0x07);
    ulPixelHeight2 = 0xFF >> (7 - (iY2 & 0x07));
    ulBufferLocation = ulPageAddress1 * 102 + iX;

    //Vertical Line spans more than 1 page on the LCD
    if(ulPageAddress1 != ulPageAddress2)
    {
        //Write First Page of vertical Line
        Dogs102x64_UC1701_setAddress(iX, ulPageAddress1);

        // White Pixels
        if(ulValue)
        {
            DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight1;
        }
        // Black Pixels
        else
        {
            DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight1;
        }
        HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation]);

        // Skip to next page
        ulPageAddress1++;
        ulBufferLocation += 102;

        //Write Pages between First and Last Page
        while(ulPageAddress1 < ulPageAddress2)
        {
            Dogs102x64_UC1701_setAddress(iX, ulPageAddress1);

            // White Pixels
            if(ulValue)
            {
                DOGS102x64Memory[ulBufferLocation] |= 0xFF;
            }
            // Black Pixels
            else
            {
                DOGS102x64Memory[ulBufferLocation] &= ~0xFF;
            }
            HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation]);

            // Skip to next page
            ulPageAddress1++;
            ulBufferLocation += 102;
        }

        //Write Last Page of vertical Line
        Dogs102x64_UC1701_setAddress(iX, ulPageAddress2);

        // White Pixels
        if(ulValue)
        {
            DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight2;
        }
        // Black Pixels
        else
        {
            DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight2;
        }
        HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation]);
    }
    //Vertical line resides on one page on the LCD
    else
    {
        ulPixelHeight1 &= ulPixelHeight2;
        Dogs102x64_UC1701_setAddress(iX, ulPageAddress1);

        // White Pixels
        if(ulValue)
        {
            DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight1;
        }
        // Black Pixels
        else
        {
            DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight1;
        }
        HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation]);
    }
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
Dogs102x64_UC1701_fillRect(void *pvDisplayData,
                           const Graphics_Rectangle *pRect,
                           uint16_t ulValue)
{
    int16_t x0 = pRect->sXMin;
    int16_t x1 = pRect->sXMax;
    int16_t y0 = pRect->sYMin;
    int16_t y1 = pRect->sYMax;
    uint8_t ulPageAddress1, ulPageAddress2, ulPixelHeight1, ulPixelHeight2;
    uint16_t ulBufferLocation;

    ulPageAddress1 = y0 / 8;
    ulPageAddress2 = y1 / 8;

    ulPixelHeight1 = 0xFF << (y0 & 0x07);
    ulPixelHeight2 = 0xFF >> (7 - (y1 & 0x07));
    ulBufferLocation = ulPageAddress1 * 102 + x0;

    // Vertical Line spans more than 1 page on the LCD
    if(ulPageAddress1 != ulPageAddress2)
    {
        // Write First Page of vertical Line
        Dogs102x64_UC1701_setAddress(x0, ulPageAddress1);

        // White Pixels
        if(ulValue)
        {
            // Draw across page
            for(; x0 <= x1; x0++)
            {
                DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight1;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
        }
        // Black Pixels
        else
        {
            for(; x0 <= x1; x0++)
            {
                // Draw across page
                DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight1;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
        }

        // Skip to next page and reset variables
        ulPageAddress1++;
        x0 = pRect->sXMin;
        ulBufferLocation = ulPageAddress1 * 102 + x0;

        // Write Pages between First and Last Page
        while(ulPageAddress1 < ulPageAddress2)
        {
            Dogs102x64_UC1701_setAddress(x0, ulPageAddress1);

            // White Pixels
            if(ulValue)
            {
                // Draw across page
                for(; x0 <= x1; x0++)
                {
                    DOGS102x64Memory[ulBufferLocation] = 0xFF;
                    HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
                }
            }
            // Black Pixels
            else
            {
                for(; x0 <= x1; x0++)
                {
                    // Draw across page
                    DOGS102x64Memory[ulBufferLocation] = 0x00;
                    HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
                }
            }

            // Skip to next page and reset variables
            ulPageAddress1++;
            x0 = pRect->sXMin;
            ulBufferLocation = ulPageAddress1 * 102 + x0;
        }

        //Write Last Page of vertical Line
        Dogs102x64_UC1701_setAddress(x0, ulPageAddress2);

        // White Pixels
        if(ulValue)
        {
            // Draw across page
            for(; x0 <= x1; x0++)
            {
                DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight2;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
        }
        // Black Pixels
        else
        {
            for(; x0 <= x1; x0++)
            {
                // Draw across page
                DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight2;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
        }
    }
    //Vertical line resides on one page on the LCD
    else
    {
        ulPixelHeight1 &= ulPixelHeight2;
        Dogs102x64_UC1701_setAddress(x0, ulPageAddress1);

        // White Pixels
        if(ulValue)
        {
            // Draw across page
            for(; x0 <= x1; x0++)
            {
                DOGS102x64Memory[ulBufferLocation] |= ulPixelHeight1;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
        }
        // Black Pixels
        else
        {
            for(; x0 <= x1; x0++)
            {
                // Draw across page
                DOGS102x64Memory[ulBufferLocation] &= ~ulPixelHeight1;
                HAL_LCD_writeData(DOGS102x64Memory[ulBufferLocation++]);
            }
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
void Dogs102x64_UC1701_clearDisplay(void *pvDisplayData,
                                    uint16_t ulValue)
{
    int16_t i, j;

    //Page Address
    for(i = 0; i < 8; i++)
    {
        Dogs102x64_UC1701_setAddress(0,i);
        //Column Address
        for(j = 0; j < 102; j++)
        {
            DOGS102x64Memory[i * 102 + j] = 0xFF;
            HAL_LCD_writeData(0xFF);
        }
    }
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
Dogs102x64_UC1701_translateColor(void *pvDisplayData,
                                 uint32_t ulValue)
{
    //
    // Translate from a 24-bit RGB color to a Black or White 1BPP color.
    //
    return(((ulValue != 0) ? ulValue = 1 : ulValue));
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
//! implemented because each driver operation updates the buffer and the display
//!
//! \return None.
//
//*****************************************************************************
static void
Dogs102x64_UC1701_flush(void *pvDisplayData)
{
    // No operation
}

//*****************************************************************************
//
//! The display structure that describes the driver for the DOGS
//! 102x64 LCD with an UC1701 controller.
//
//*****************************************************************************
const Graphics_Display g_sDogs102x64_UC1701 =
{
    sizeof(Graphics_Display),
    DOGS102x64Memory,
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
    LCD_VERTICAL_MAX,
    LCD_HORIZONTAL_MAX,
#else
    LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX,
#endif
    Dogs102x64_UC1701_drawPixel,
    Dogs102x64_UC1701_drawMultiplePixel,
    Dogs102x64_UC1701_drawHLine,
    Dogs102x64_UC1701_drawVLine,
    Dogs102x64_UC1701_fillRect,
    Dogs102x64_UC1701_translateColor,
    Dogs102x64_UC1701_flush,
    Dogs102x64_UC1701_clearDisplay
};

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
