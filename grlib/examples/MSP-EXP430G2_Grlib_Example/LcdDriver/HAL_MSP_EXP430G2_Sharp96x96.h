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
// HAL_MSP-EXP430G2_Sharp96x96.h - Prototypes for the Sharp96x96 LCD display driver.
//                                 There is no output from Sharp96x96 LCD.
//
//                 MSP430G2553                    LCD Sharp96x96
//                -----------------             -----------------
//               |     UCB0CLK/P1.5|---------> |SCLK     EXTCOMIN|-- 0
//            /|\|                 |           |                 |
//             | |    UCB0SIMO/P1.7|---------> |SI        EXTMODE|-- 0
//             --|RST              |           |                 |
//               |     UCB0STE/P1.4|---------> |SCS              |
//               |                 |		   |                 |
//               |             P1.3|---------> |DISP             |
//               |                 |		   |                 |
//               |                 |           |                 |
//               |                 |            -----------------
//*****************************************************************************

#ifndef __HAL_MSP_EXP430G2_SHARPLCD_H__
#define __HAL_MSP_EXP430G2_SHARPLCD_H__

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// Ports from MSP430 connected to LCD
#define LCD_MOSI_PORT                       P1OUT
#define LCD_SCLK_PORT                       P1OUT
#define LCD_POWER_DISP_PORT                 P1OUT
#define LCD_POWER_DISP_DIR                  P1DIR
#define LCD_SCS_PORT                        P1OUT
#define LCD_SCS_DIR                         P1DIR

// Pins from MSP430 connected to LCD
#define LCD_MOSI_PIN                        BIT7
#define LCD_MOSI_PORT_SEL1                                      P1SEL
#define LCD_MOSI_PORT_SEL2                  P1SEL2
#define LCD_SCLK_PIN                        BIT5
#define LCD_SCLK_PORT_SEL1                                      P1SEL
#define LCD_SCLK_PORT_SEL2                                      P1SEL2
#define LCD_POWER_PIN                       BIT0
#define LCD_DISP_PIN                        BIT3
#define LCD_SCS_PIN                         BIT4

// Non-volatile Memory used to store DisplayBuffer
#define NON_VOLATILE_MEMORY_BUFFER
#ifdef NON_VOLATILE_MEMORY_BUFFER
#define USE_FLASH_BUFFER
#define NON_VOLATILE_MEMORY_ADDRESS                     0xf100
#endif //NON_VOLATILE_MEMORY_BUFFER

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void HAL_LCD_initDisplay(void);
extern void HAL_LCD_writeCommandOrData(uint16_t command);
extern void HAL_LCD_clearCS(void);
extern void HAL_LCD_setCS(void);
extern void HAL_LCD_prepareMemoryWrite(void);
extern void HAL_LCD_finishMemoryWrite(void);
extern void HAL_LCD_waitUntilLcdWriteFinish(void);
extern void HAL_LCD_disableDisplay(void);
extern void HAL_LCD_enableDisplay(void);
extern void HAL_LCD_InitializeDisplayBuffer(void *pvDisplayData,
                                            uint8_t ucValue);
#endif // __HAL_MSP_EXP430G2_SHARPLCD_H__
