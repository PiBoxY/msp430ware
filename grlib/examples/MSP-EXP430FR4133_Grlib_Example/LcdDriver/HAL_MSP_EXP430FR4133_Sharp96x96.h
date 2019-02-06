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
// HAL_MSP-EXP430FR4133_Sharp96x96.h - Prototypes for the Sharp96x96
//                  LCD display driver. There is no output from Sharp96x96 LCD
//
//                   MSP430FR4133                 LCD Sharp96x96
//                -----------------             -----------------
//               |     P5.1/UCB0CLK|---------> |SPI_CLK  EXT_MODE|--GND
//            /|\|                 |           |                 |
//             | |    P5.2/UCB0SIMO|---------> |SPI_SI   EXTCOMIN|--GND
//             --|RST              |           |                 |
//               |             P8.0|---------> |SPI_CS           |
//               |                 |		   |                 |
//               |             P2.7|---------> |DISP             |
//               |                 |		   |                 |
//               |             P8.1|-----*---> |VDD              |
//               |                 |      `--> |VDDA             |
//               |                 |            -----------------
//                -----------------
//*****************************************************************************

#ifndef __HAL_MSP_EXP430FR4133_SHARPLCD_H__
#define __HAL_MSP_EXP430FR4133_SHARPLCD_H__

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// Ports from MSP430 connected to LCD
#define LCD_SPI_SI_PORT                     GPIO_PORT_P5
#define LCD_SPI_CLK_PORT                    GPIO_PORT_P5
#define LCD_DISP_PORT                       GPIO_PORT_P2
#define LCD_POWER_PORT                      GPIO_PORT_P8
#define LCD_SPI_CS_PORT                     GPIO_PORT_P8

// Pins from MSP430 connected to LCD
#define LCD_SPI_SI_PIN                      GPIO_PIN2
#define LCD_SPI_SI_PIN_FUNCTION             GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_SPI_CLK_PIN                     GPIO_PIN1
#define LCD_SPI_CLK_PIN_FUNCTION            GPIO_PRIMARY_MODULE_FUNCTION
#define LCD_DISP_PIN                        GPIO_PIN7
#define LCD_POWER_PIN                       GPIO_PIN1
#define LCD_SPI_CS_PIN                      GPIO_PIN0

// Definition of USCI base address to be used for SPI communication
#define LCD_EUSCI_BASE                EUSCI_B0_BASE

// Non-volatile Memory used to store DisplayBuffer
//#define NON_VOLATILE_MEMORY_BUFFER
#ifdef NON_VOLATILE_MEMORY_BUFFER
#define NON_VOLATILE_MEMORY_ADDRESS                     0xc400
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

#endif // __HAL_MSP_EXP430FR4133_SHARPLCD_H__
