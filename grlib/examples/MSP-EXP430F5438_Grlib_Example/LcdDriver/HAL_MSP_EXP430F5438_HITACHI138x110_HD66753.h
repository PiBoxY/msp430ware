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
// HAL_MSP_EXP430F5438_HITACHI138x110_HD66753.h - Prototypes for the
//           Hitachi 138x110 HD66753 LCD display driver.
//									   There is no output from Sharp96x96 LCD.
//
//                 MSP430FR5969                  LCD HD66753
//                -----------------                  ------------
//               |     P9.1/UCB2SIMO|-*-LCD_DATA--> |SDA         |
//            /|\|     P9.2/UCB2SOMI|_|             |            |
//             | |      P9.3/UCB2CLK|---LCD_CLK---> |SCL         |
//             --|RST               |               |            |
//               |              P9.6|---LCD_CS----> |CS          |
//               |                  |               |            |
//               |              P9.7|---LCD_RST---> |RESET       |
//               |                  |		          ------------
//               |                  |
//               |                  |                  TPS61043
//               |                  |                 ----------
//               |                  |                |          |
//               |       P8.3/TA0.4 |----BKLT_EN --> |CTRL      |
//               |                  |                |          |
//               |                  |                 ----------
//                ------------------
//****************************************************************************

#ifndef __HAL_MSP_EXP430F5438_HITACHI138x110_HD66753__
#define __HAL_MSP_EXP430F5438_HITACHI138x110_HD66753__

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************
// Ports from MSP430 connected to LCD
#define LCD_BKLT_EN_PORT    GPIO_PORT_P8
#define LCD_CS_PORT         GPIO_PORT_P9
#define LCD_RST_PORT        GPIO_PORT_P9
#define LCD_SIMO_PORT       GPIO_PORT_P9
#define LCD_SOMI_PORT       GPIO_PORT_P9
#define LCD_CLK_PORT        GPIO_PORT_P9

// Pins from MSP430 connected to LCD
#define LCD_BKLT_EN_PIN     GPIO_PIN3
#define LCD_CS_PIN          GPIO_PIN6
#define LCD_RST_PIN         GPIO_PIN7
#define LCD_SIMO_PIN        GPIO_PIN1
#define LCD_SOMI_PIN        GPIO_PIN2
#define LCD_CLK_PIN         GPIO_PIN3

// Definition of USCI base address to be used for SPI communication
#define LCD_USCI_BASE                 USCI_B2_BASE

// Definition of TIMER_A base address to be used for backlight control
#define LCD_TIMER_BASE_BKLT       TIMER_A0_BASE

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
extern void HAL_LCD_writeCommand(uint8_t *command);
extern void HAL_LCD_clearCS(void);
extern void HAL_LCD_setCS(void);
extern void HAL_LCD_initTimer(uint16_t captureCompareVal);
extern uint16_t HAL_LCD_getTimerCaptureCompare();
extern void HAL_LCD_setTimerDutyCycle(uint16_t dutyCycle);
extern void HAL_LCD_startTimerCounter(void);
extern void HAL_LCD_stopTimerCounter(void);
extern void HAL_LCD_turnOffBackLight(void);

#endif // __HAL_MSP_EXP430F5438_HITACHI138x110_HD66753__
