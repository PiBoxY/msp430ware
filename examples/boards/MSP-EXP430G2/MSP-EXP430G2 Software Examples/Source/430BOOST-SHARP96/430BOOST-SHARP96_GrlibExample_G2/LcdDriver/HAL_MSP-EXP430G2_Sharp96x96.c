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
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// IMPORTANT NOTE: The following driver file has been modified to store
// the LCD DisplayBuffer on FLASH memory. This is due to the limited amount
// of RAM memory on the MSP430G2553 device.
//
//*****************************************************************************
//*****************************************************************************
//
// HAL_MSP-EXP430G2_Sharp96x96.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include <msp430g2553.h>
#include <stdint.h>
#include "grlib.h"
#include "HAL_MSP-EXP430G2_Sharp96x96.h"


//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the Sharp96x96 display. This function
//! configures the GPIO pins used to control the LCD display when the basic
//! GPIO interface is in use. On exit, the LCD has been reset and is ready to
//! receive command and data writes.
//!
//! \return None.
//
//*****************************************************************************
void LCDInit(void)
{
	// Configure:
	// LCD_MOSI_PIN for SPI_MOSI mode

	LCD_MOSI_PORT_SEL1 |= LCD_MOSI_PIN;
	LCD_MOSI_PORT_SEL2 |= LCD_MOSI_PIN;

	// Configure:
	// LCD_SCLK_PIN for SPI_CLK mode
	LCD_SCLK_PORT_SEL1 |= LCD_SCLK_PIN;
	LCD_SCLK_PORT_SEL2 |= LCD_SCLK_PIN;

	// Configure LCD_POWER_PIN as output to supply the LCD and LCD_DISP_PIN
	// as output for DISP
	LCD_POWER_DISP_DIR |= LCD_POWER_PIN + LCD_DISP_PIN;

	// Provide power to LCD and turn on DISP pins
	LCD_POWER_DISP_PORT |= LCD_POWER_PIN + LCD_DISP_PIN;

	// Configure LCD_SCS_PIN as output and clear pin
	LCD_SCS_DIR |= LCD_SCLK_PIN;
	ClearCS();


	// Configure USCIB0 for SPI mode:
	// SPI master,
	// 3-pin SPI,
	// 8-bit data,
	// Data is captured on the first UCLK edge and changed on the following edge
	// SMCLK sources USCI clock
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	UCB0CTL1 |= UCSSEL_3;
	UCB0BR0 |= 0x08;
	UCB0BR1 = 0;

	// Initialize USCI state machine
	UCB0CTL1 &= ~UCSWRST;

}

//*****************************************************************************
//
//! Initialize DisplayBuffer.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//!	\param ucValue is the foreground color of the buffered data.
//!
//! This function initializes the display buffer and discards any cached data.
//!
//! \return None.
//
//*****************************************************************************

void InitializeDisplayBuffer(void *pvDisplayData, uint8_t ucValue)
{
	uint16_t i=0,j=0;
	uint8_t *pucData = pvDisplayData;

	uint16_t ucSegment = 0;

	UnlockFlashMemory();

	for(i =0; i< LCD_VERTICAL_MAX; i++){
		for(j =0; j< (LCD_HORIZONTAL_MAX>>3); j++){
			if(ucSegment==0 && ClrBlack != ucValue){
				SetFlashAccessRight(ERASE);
				//Dummy write to erase Flash segment
				*pucData=0;

				SetFlashAccessRight(WRT);
			}
			while(FCTL3&BUSY);
			*pucData++ = ucValue;
			ucSegment++;
			if(ucSegment == 512)
				ucSegment = 0;
		}
	}

	FinishMemoryWrite();
}

                                      
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
