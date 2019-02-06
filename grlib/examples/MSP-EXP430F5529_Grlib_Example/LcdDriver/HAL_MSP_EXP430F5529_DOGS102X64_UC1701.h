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
//TODO
//*****************************************************************************
//
// HAL_MSP_EXP430F5529_DOGS102X64_UC1701.h - Prototypes for the
//           Dogs102x64_UC1701 LCD display driver.
//
//
//                 MSP430F5529                      LCD UC1701
//                -----------------                  ------------
//               |     P4.1/UCB1SIMO|---SIMO------> |SDA         |
//            /|\|                  |               |            |
//             | |      P4.3/UCB1CLK|---SCLK------> |SCK         |
//             --|RST               |               |            |
//               |              P7.4|---LCD_CS----> |CS0         |
//               |              P5.6|---LCD_D/C---> |CD          |
//               |              P5.7|---LCD_RST---> |RST         |
//               |                  |		          ------------
//               |                  |
//               |                  |                TPS75105DSKR
//               |                  |                 ----------
//               |                  |                |          |
//               |        P7.6/TB0.4|---LCD_BL_EN--> |CTRL      |
//               |                  |                |          |
//               |                  |                 ----------
//                ------------------
//****************************************************************************

#ifndef __HAL_MSP_EXP430F5529_DOGS102X64_UC1701__
#define __HAL_MSP_EXP430F5529_DOGS102X64_UC1701__

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************
// Ports from MSP430 connected to LCD
#define LCD_BL_EN_PORT      GPIO_PORT_P7
#define LCD_CS_PORT         GPIO_PORT_P7
#define LCD_RST_PORT        GPIO_PORT_P5
#define LCD_SIMO_PORT       GPIO_PORT_P4
#define SCLK_PORT           GPIO_PORT_P4
#define LCD_DC_PORT                     GPIO_PORT_P5

// Pins from MSP430 connected to LCD
#define LCD_BL_EN_PIN       GPIO_PIN6
#define LCD_CS_PIN          GPIO_PIN4
#define LCD_RST_PIN         GPIO_PIN7
#define LCD_SIMO_PIN        GPIO_PIN1
#define SCLK_PIN            GPIO_PIN3
#define LCD_DC_PIN          GPIO_PIN6

// Definition of USCI base address to be used for SPI communication
#define LCD_USCI_BASE                 USCI_B1_BASE

// Definition of TIMER_A base address to be used for backlight control
#define LCD_TIMER_BASE_BKLT       TIMER_B0_BASE

//*****************************************************************************
//
// Deprecated names.  These definitions ensure backwards compatibility
// but new code should avoid using deprecated struct names since these will
// be removed at some point in the future.
//
//*****************************************************************************

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void HAL_LCD_initLCD(void);
extern void HAL_LCD_writeCommand(uint8_t command);
extern void HAL_LCD_writeData(uint16_t data);
extern void HAL_LCD_clearCS(void);
extern void HAL_LCD_setCS(void);
extern void HAL_LCD_initTimer(uint16_t captureCompareVal);
extern uint16_t HAL_LCD_getTimerCaptureCompare();
extern void HAL_LCD_setTimerDutyCycle(uint16_t dutyCycle);
extern void HAL_LCD_startTimerCounter(void);
extern void HAL_LCD_stopTimerCounter(void);

#endif // __HAL_MSP_EXP430F5529_DOGS102X64_UC1701__
