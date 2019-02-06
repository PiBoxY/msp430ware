/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//                 MSP430G2XX                    LCD Sharp96x96
//                -----------------             -----------------
//               |     UCB0CLK/P1.5|---------> |SCLK     EXTCOMIN|-- 0
//            /|\|                 |           |            	 |
//             | |    UCB0SIMO/P1.7|---------> |SI        EXTMODE|-- 0
//             --|RST              |           |              	 |
//               |     UCB0STE/P1.4|---------> |SCS              |
//               |             	   |		   |         	     |
//               |             P1.3|---------> |DISP             |
//               |                 |		   |                 |
//               |                 | 	       |                 |
//               |                 |            -----------------
//
//*****************************************************************************
#include <stdint.h>

#ifndef __HAL_MSP-EXP430G2_SHARPLCD_H__
#define __HAL_MSP-EXP430G2_SHARPLCD_H__

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
#define LCD_SCS_PORT	                    P1OUT
#define LCD_SCS_DIR                         P1DIR

// Pins from MSP430 connected to LCD
#define LCD_MOSI_PIN                        BIT7
#define LCD_MOSI_PORT_SEL1					P1SEL
#define LCD_MOSI_PORT_SEL2                  P1SEL2
#define LCD_SCLK_PIN                        BIT5
#define LCD_SCLK_PORT_SEL1					P1SEL
#define LCD_SCLK_PORT_SEL2					P1SEL2
#define LCD_POWER_PIN                       BIT0
#define LCD_DISP_PIN                        BIT3
#define LCD_SCS_PIN                         BIT4

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   96
#define LCD_HORIZONTAL_MAX                 96

// Non-volatile Memory used to store DisplayBuffer
#define NON_VOLATILE_MEMORY_BUFFER
#ifdef NON_VOLATILE_MEMORY_BUFFER
#define USE_FLASH_BUFFER
#define NON_VOLATILE_MEMORY_ADDRESS			0xf400
#endif //NON_VOLATILE_MEMORY_BUFFER

//*****************************************************************************
//
// Clears CS line
//
// This macro allows to clear the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************

#define ClearCS()                    	\
       LCD_SCS_PORT &= ~LCD_SCS_PIN;

//*****************************************************************************
//
// Set CS line
//
// This macro allows to set the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************

#define SetCS()                    	\
       LCD_SCS_PORT |= LCD_SCS_PIN;

//*****************************************************************************
//
// Waits until the SPI communication with the LCD is finished a command to
// the LCD Driver
//
// \param None
//
// \return None
//*****************************************************************************

#define WaitUntilLcdWriteFinished()                \
        while  (UCB0STAT & UCBUSY)


//*****************************************************************************
//
// Writes command or data to the LCD Driver
//
// \param ucCmdData is the 8 or 16 bit command to send to the LCD driver
// Uses the SET_LCD_DATA macro
//
// \return None
//
//*****************************************************************************

#define WriteCmdData(ucCmdData)                    	\
        do                                         	\
        {                                           \
            while (!(IFG2 & UCB0TXIFG));      		\
            UCB0TXBUF = ucCmdData;                  \
        }                                           \
        while(0)

//*****************************************************************************
//
// Unlocks FLASH controller
//
// This macro unlocks flash memory controller.
//
// \return None
//
//*****************************************************************************

#define UnlockFlashMemory()                    	\
       FCTL3 = FWKEY


//*****************************************************************************
//
// Sets access right on FLASH controller
//
// \param uiAccesType defines access right. Valid parameters:
//		WRT.- Set Write access on flash controller
//		ERASE.- Sets Erase
//
// This macro sets the proper write or erase access on FLASH controller.
// \return None
//
//*****************************************************************************

#define SetFlashAccessRight(uiAccessType)                    	\
       FCTL1 = FWKEY + uiAccessType

//*****************************************************************************
//
// Removes access right on FLASH controller
//
// This macro removes access rights on FLASH controller.
// \return None
//
//*****************************************************************************

#define RemoveFlashAccessRight()                    	\
       FCTL1 = FWKEY

//*****************************************************************************
//
// Locks FLASH controller
//
// This macro locks flash memory controller.
//
// \return None
//
//*****************************************************************************

#define LockFlashMemory()                    	\
       FCTL3 = FWKEY + LOCK

//*****************************************************************************
//
// Prepare to write memory
//
// This macro unlocks flash memory controller and
// sets access right on flash controller
//
// \return None
//
//*****************************************************************************
#define PrepareMemoryWrite()					\
	UnlockFlashMemory();						\
	SetFlashAccessRight(WRT)

//*****************************************************************************
//
// Finish memory writing
//
// This macro removes access rights on flash controller and
// locks flash memory controller.
//
// \return None
//
//*****************************************************************************
#define FinishMemoryWrite()						\
	RemoveFlashAccessRight();					\
	LockFlashMemory()

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void LCDInit(void);
extern void InitializeDisplayBuffer(void *pvDisplayData, uint8_t ucValue);
#endif // __HAL_MSP-EXP430G2_SHARPLCD_H__
