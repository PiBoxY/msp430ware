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
// HAL_MSP-EXP430FR4133_Sharp96x96.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "grlib.h"
#include "HAL_MSP_EXP430FR4133_Sharp96x96.h"
#include "driverlib.h"

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
void HAL_LCD_initDisplay(void)
{
    // Configure LCD_SCLK_PIN for SPI_CLK mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SPI_CLK_PORT,
                                                LCD_SPI_CLK_PIN,
                                                LCD_SPI_CLK_PIN_FUNCTION);

    // Configure LCD_MOSI_PIN for SPI_MOSI mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SPI_SI_PORT,
                                                LCD_SPI_SI_PIN,
                                                LCD_SPI_SI_PIN_FUNCTION);

    // Provide power to LCD
    GPIO_setAsOutputPin(LCD_POWER_PORT,
                        LCD_POWER_PIN);

    GPIO_setOutputHighOnPin(LCD_POWER_PORT,
                            LCD_POWER_PIN);

    // Turn on DISP
    GPIO_setAsOutputPin(LCD_DISP_PORT,
                        LCD_DISP_PIN);

    GPIO_setOutputHighOnPin(LCD_DISP_PORT,
                            LCD_DISP_PIN);

    // Configure LCD_SPI_CS_PIN as output pin
    GPIO_setAsOutputPin(LCD_SPI_CS_PORT,
                        LCD_SPI_CS_PIN);

    HAL_LCD_clearCS();

    EUSCI_B_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = 1000000;
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT;
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW;
    param.spiMode = EUSCI_B_SPI_3PIN;

    EUSCI_B_SPI_initMaster(LCD_EUSCI_BASE,&param);

    EUSCI_B_SPI_enable(LCD_EUSCI_BASE);
}

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
void HAL_LCD_writeCommandOrData(uint16_t command)
{
    while(!EUSCI_B_SPI_getInterruptStatus(LCD_EUSCI_BASE,
                                          EUSCI_B_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }
    EUSCI_B_SPI_transmitData(LCD_EUSCI_BASE,command);
}

//*****************************************************************************
//
// Clears CS line
//
// This macro allows to clear the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
void HAL_LCD_clearCS(void){
    GPIO_setOutputLowOnPin(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN);
}

//*****************************************************************************
//
// Set CS line
//
// This macro allows to set the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
void HAL_LCD_setCS(void){
    GPIO_setOutputHighOnPin(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN);
}

//*****************************************************************************
//
// Waits until the SPI communication with the LCD is finished a command to
// the LCD Driver
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_waitUntilLcdWriteFinish(void)
{
    while(EUSCI_B_SPI_isBusy(LCD_EUSCI_BASE))
    {
        ;
    }
}

//*****************************************************************************
//
// Disables Shapr96x96 LCD
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_disableDisplay(void)
{
    GPIO_setOutputLowOnPin(LCD_DISP_PORT, LCD_DISP_PIN);
    GPIO_setOutputLowOnPin(LCD_POWER_PORT, LCD_POWER_PIN);
}

//*****************************************************************************
//
// Enables Shapr96x96 LCD
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_enableDisplay(void)
{
    GPIO_setOutputHighOnPin(LCD_POWER_PORT, LCD_POWER_PIN);
    GPIO_setOutputHighOnPin(LCD_DISP_PORT, LCD_DISP_PIN);
}

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
void HAL_LCD_prepareMemoryWrite()
{
    SysCtl_enableFRAMWrite(SYSCTL_FRAMWRITEPROTECTION_PROGRAM);
}

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
void HAL_LCD_finishMemoryWrite()
{
    SysCtl_protectFRAMWrite(SYSCTL_FRAMWRITEPROTECTION_PROGRAM);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
