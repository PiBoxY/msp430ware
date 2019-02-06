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
// Dogs102x64_UC1701.h - Prototypes for the Dogs102x64 LCD
//                                     display driver with a UC1701
//                                     controller.
//
//*****************************************************************************

#ifndef __DOGS102X64_UC1701_H__
#define __DOGS102X64_UC1701_H__

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
#define SYSTEM_CLOCK_SPEED      25000000

#define LCD_HORIZONTAL_MAX        102
#define LCD_VERTICAL_MAX          64

// Define LCD Screen Orientation Here
#define LANDSCAPE
//#define LANDSCAPE_FLIP

//*****************************************************************************
//
// This driver operates in two different screen orientations.  They are:
//
// * Landscape - The screen is wider than it is tall, and there are more pins
//               and the white band on the top of the display. This is selected
//               by defining LANDSCAPE in the User Configuration section above.
//
// * Landscape flip - The screen is wider than it is tall, and there are more
//               pins and the white band on the bottom of the display. This is
//               selected by defining LANDSCAPE_FLIP in the User
//               Configuration section above.
//
// These can also be imagined in terms of screen rotation; if landscape mode is
// 0 degrees of rotation, landscape flip is 180 degrees of rotation
//
// If no screen orientation is selected, "landscape" mode will be used.
//
//*****************************************************************************
#if !defined(LANDSCAPE) && !defined(LANDSCAPE_FLIP)
#define LANDSCAPE
#endif

//*****************************************************************************
//
// Various UC1701 command name labels and associated control bits
//
//*****************************************************************************

//******************************************************************************
// Set SRAM column address. The column address (ca) must be set with 2 commands,
// the ca MSBs and LSBs.
// In both commands the MSBs or LSBs are set with the last 4 bits of the command
//******************************************************************************
#define SET_COLUMN_ADDRESS_MSB        0x10  // BIT0 - BIT4 = ca4 - ca7
#define SET_COLUMN_ADDRESS_LSB        0x00  // BIT0 - BIT4 = ca0 - ca3

//******************************************************************************
// Sets the power control options. The booster, regulator, and follower can all
// be independently controlled with the last 3 bits of this command
//******************************************************************************
#define SET_POWER_CONTROL             0x28
#define BOOSTER                       BIT0
#define REGULATOR                     BIT1
#define FOLLOWER                      BIT2

//
// Scroll image up by SL rows. SL = last 5 bits of this command. Range 0 - 63
//
#define SET_SCROLL_LINE               0x40

//******************************************************************************
// Set SRAM page address. The page address (pa) is the last 4 bits of this
// command. Range 0 -7
//******************************************************************************
#define SET_PAGE_ADDRESS              0xB0

//******************************************************************************
// Set internal resistor ratio Rb/Ra to adjust contrast
// Internal resistor ratio is controlled by the last 3 bits of this command
//******************************************************************************
#define SET_VLCD_RESISTOR_RATIO       0x20
#define INTERNAL_RESISTOR_RATIO       0x07  // Ratio = BIT0-BIT2

//******************************************************************************
// Set electronic volume (PM) to adjust contrast. The electronic volume must be
// set with 2 commands
// SET_ELECTRONIC_VOLUME_MSB command is static.
// PM = last 5 bits of SET_ELECTRONIC_VOLUME_LSB command
//******************************************************************************
#define SET_ELECTRONIC_VOLUME_MSB     0x81  //Static Command
#define SET_ELECTRONIC_VOLUME_LSB     0x00  //PM = BIT0-BIT4
#define ELECTRONIC_VOLUME_PM          0x0f

//******************************************************************************
// Set all pixels on. This command does not affect SRAM memory
// This functionality is enabled/disabled by BIT0 of this command
//******************************************************************************
#define SET_ALL_PIXEL_ON              0xA4
#define DISABLE                       0x00
#define ENABLE                        0x01

//******************************************************************************
// Inverse the pixels displayed. This command does not affect SRAM memory, only
// how it is read.
// Set to regular for each "1" in SRAM memory to represent a black pixel
// Set to inverse for each "1" in SRAM memory to represent a white pixel
// Pixel polarity is controlled by BIT0 of this command
//******************************************************************************
#define SET_INVERSE_DISPLAY           0xA6
#define REGULAR                       0x00
#define INVERSE                       0x01

//******************************************************************************
// Enable or disable the display. Enabling the display exits all sleep modes
// and restores power
// Enable/disable is controlled by BIT0 of this command
//******************************************************************************
#define SET_DISPLAY_ENABLE            0xAE
#define DISABLE                       0x00
#define ENABLE                        0x01

//******************************************************************************
// Set SEG direction (mirror X axis). Set SEG to mirror for 6:00 viewing because
// of the mirrored layout
// When SEG is set to mirror, column addresses range from 0 -101
// Set SEG to normal for 12:00 viewing
// When SEG is set to normal, column addresses range from 30-131
// SEG is controlled by BIT0 of this command
//******************************************************************************
#define SET_SEG_DIRECTION             0xA0
#define SEG_MIRROR                    0x01
#define SEG_NORMAL                    0x00

//
// Set COM direction (mirror Y axis). Set COM to normal for 6:00 viewing
// When COm is set to normal Y pixels progress 0 - 63
// Set COM to mirror for 12:00 viewing
// When COM is set to mirror Y pixels progress 63 - 0
// COM is controlled by BIT3 of this command
//
#define SET_COM_DIRECTION             0xC0
#define COM_MIRROR                    0x08
#define COM_NORMAL                    0x00

//******************************************************************************
// Reset the system. All control registers are reset, SRAM memory is not
// affected
//******************************************************************************
#define SYSTEM_RESET                  0xE2

//******************************************************************************
// No operation
//******************************************************************************
#define NOP                           0xE3

//******************************************************************************
// Set voltage bias ratio BR. BR is controlled by BIT0
// BR: 0 = 1/9; 1 = 1/7
//******************************************************************************

#define SET_LCD_BIAS_RATIO            0xA2
#define NINTH                         0x00
#define SEVENTH                       0x01

//******************************************************************************
// Advanced Program Controls. This 2 command sequence sets the temperature
// compensation and the cursor wrapping options. SET_ADV_PROGRAM_CONTROL0_MSB is
// a static value SET_ADV_PROGRAM_CONTROL0_LSB controls the temperature
// compensation with BIT7, the column wrap-around with BIT1 and the page
// wrap-around with BIT0
//******************************************************************************

//Set temp. compensation curve to -0.11%/C
#define SET_ADV_PROGRAM_CONTROL0_MSB  0xFA
#define SET_ADV_PROGRAM_CONTROL0_LSB  0x10
#define TEMP_COMP_5                   0x00
#define TEMP_COMP_11                  0x80
#define COLUMN_WRAP_AROUND            0x02
#define PAGE_WRAP_AROUND              0x01

//******************************************************************************
//
// Macros for the Display Driver
//
//******************************************************************************

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void Dogs102x64_UC1701_initLCD(void);
extern void Dogs102x64_UC1701_fillScreen(uint32_t ulValue);
extern void Dogs102x64_UC1701_setBacklight(uint8_t brightness);
extern void Dogs102x64_UC1701_setContrast(uint8_t newContrast);
extern void Dogs102x64_UC1701_backlightInit(void);
extern void Dogs102x64_UC1701_disable(void);
extern void Dogs102x64_UC1701_enable(void);
extern void Dogs102x64_UC1701_inverseDisplay(void);
extern void Dogs102x64_UC1701_clearInverseDisplay(void);
extern void Dogs102x64_UC1701_setInverseAddress(uint16_t x,
                                                uint16_t y);
extern void Dogs102x64_UC1701_setAddress(uint16_t x,
                                         uint16_t y);

extern const Graphics_Display g_sDogs102x64_UC1701;

#endif // __DOGS102X64_UC1701_H__
