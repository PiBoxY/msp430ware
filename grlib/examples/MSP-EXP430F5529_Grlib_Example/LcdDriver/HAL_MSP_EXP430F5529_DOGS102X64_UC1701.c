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
// TODO update this diagram
//*****************************************************************************
//
// HAL_MSP-EXP430FR5969_Sharp96x96.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "grlib.h"
#include "HAL_MSP_EXP430F5529_DOGS102X64_UC1701.h"
#include "Dogs102x64_UC1701.h"
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
void HAL_LCD_initLCD(void)
{
    //
    // Configure the pins that connect to the LCD as outputs.
    //

    // Configure LCD_RST_PIN as output pin
    GPIO_setAsOutputPin(LCD_RST_PORT,LCD_RST_PIN);
    // Reset is active low. Clear reset for 3usec to meet reset minimum
    // pulse width.
    GPIO_setOutputLowOnPin(LCD_RST_PORT,LCD_RST_PIN);
    __delay_cycles(SYSTEM_CLOCK_SPEED * .000003);
    GPIO_setOutputHighOnPin(LCD_RST_PORT,LCD_RST_PIN);

    // Configure LCD_CS_PIN as output. CS is active low
    GPIO_setOutputLowOnPin(LCD_CS_PORT,LCD_CS_PIN);
    GPIO_setAsOutputPin(LCD_CS_PORT,LCD_CS_PIN);

    // Configure LCD_DC_PIN as output and clear output to set it in command
    // mode
    GPIO_setAsOutputPin(LCD_DC_PORT, LCD_DC_PIN);
    GPIO_setOutputLowOnPin(LCD_DC_PORT, LCD_DC_PIN);

    // Configure LCD_SIMO_PIN
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SIMO_PORT,
                                                LCD_SIMO_PIN);

    // // Configure LCD_CLK_PIN  option select CLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(SCLK_PORT, SCLK_PIN);

    USCI_B_SPI_initMasterParam spiInitParam =
    {
        USCI_B_SPI_CLOCKSOURCE_SMCLK,
        UCS_getSMCLK(),
        12500000,
        USCI_B_SPI_MSB_FIRST,
        USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
        USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW
    };

    // Initialize USCI_B1 for SPI Master operation
    USCI_B_SPI_initMaster(LCD_USCI_BASE,
                          &spiInitParam
                          );

    USCI_B_SPI_enable(LCD_USCI_BASE);

    USCI_B_SPI_clearInterrupt(LCD_USCI_BASE,
                              USCI_B_SPI_RECEIVE_INTERRUPT);

    // Turn on Backlight
    // Uses PWM to control brightness
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_BL_EN_PORT,
                                                LCD_BL_EN_PIN);

    Timer_B_initCompareModeParam timerBCompareParams =
    {
        TIMER_B_CAPTURECOMPARE_REGISTER_4,
        TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_B_OUTPUTMODE_RESET_SET,
        (Timer_B_getCaptureCompareCount(LCD_TIMER_BASE_BKLT,
                                        TIMER_B_CAPTURECOMPARE_REGISTER_0) >> 1)
    };

    Timer_B_initUpModeParam timerBInitUpMode =
    {
        TIMER_B_CLOCKSOURCE_ACLK,
        TIMER_B_CLOCKSOURCE_DIVIDER_1,
        50,
        TIMER_B_TBIE_INTERRUPT_DISABLE,
        TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_B_SKIP_CLEAR
    };

    // start at full brightness (8)
    Timer_B_initCompareMode(LCD_TIMER_BASE_BKLT,
                            &timerBCompareParams);

    Timer_B_initUpMode(LCD_TIMER_BASE_BKLT,
                       &timerBInitUpMode);

    Timer_B_startCounter(LCD_TIMER_BASE_BKLT,
                         TIMER_B_UP_MODE
                         );
}

//*****************************************************************************
//
// Writes a command to the UC1701.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t ucData)
{
    // Store current GIE state
    uint16_t gie = __get_SR_register() & GIE;

    // Make this operation atomic
    __disable_interrupt();

    // CS Low
    HAL_LCD_clearCS();

    // CD Low
    GPIO_setOutputLowOnPin(LCD_DC_PORT,LCD_DC_PIN);

    // USCI_B1 TX buffer ready?
    while(USCI_B_SPI_TRANSMIT_INTERRUPT != USCI_B_SPI_getInterruptStatus(
              LCD_USCI_BASE,
              USCI_B_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }
    // Transmit data
    USCI_B_SPI_transmitData(
        LCD_USCI_BASE,
        ucData
        );

    // Wait for all TX/RX to finish
    while(USCI_B_SPI_BUSY == USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }

    // Dummy read to empty RX buffer and clear any overrun conditions
    USCI_B_SPI_receiveData(LCD_USCI_BASE);

    // CS High
    HAL_LCD_setCS();

    // Restore original GIE state
    __bis_SR_register(gie);
}

//*****************************************************************************
//
// Writes a data word to the UC1701.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeData(uint16_t data)
{
    // Store current GIE state
    uint16_t gie = __get_SR_register() & GIE;

    // Make this operation atomic
    __disable_interrupt();

    HAL_LCD_clearCS();

    GPIO_setOutputHighOnPin(LCD_DC_PORT,LCD_DC_PIN);

    // USCI_B1 TX buffer ready?
    while(!USCI_B_SPI_getInterruptStatus(USCI_B1_BASE,
                                         USCI_B_SPI_TRANSMIT_INTERRUPT
                                         ))
    {
        ;
    }

    // Transmit data and increment pointer
    USCI_B_SPI_transmitData(USCI_B1_BASE,
                            data
                            );

    // Wait for all TX/RX to finish
    while(USCI_B_SPI_isBusy(USCI_B1_BASE))
    {
        ;
    }

    // Dummy read to empty RX buffer and clear any overrun conditions
    USCI_B_SPI_receiveData(USCI_B1_BASE);

    // CS High
    HAL_LCD_setCS();

    // Restore original GIE state
    __bis_SR_register(gie);
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

void HAL_LCD_clearCS(){
    GPIO_setOutputLowOnPin(LCD_CS_PORT, LCD_CS_PIN);
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

void HAL_LCD_setCS(){
    GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN);
}

void HAL_LCD_initTimer(uint16_t captureCompareVal){
    Timer_B_initCompareModeParam timerBCompareParams =
    {
        TIMER_B_CAPTURECOMPARE_REGISTER_4,
        TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_B_OUTPUTMODE_RESET_SET,
        captureCompareVal
    };

    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_BL_EN_PORT,
                                                LCD_BL_EN_PIN);

    Timer_B_initCompareMode(LCD_TIMER_BASE_BKLT,
                            &timerBCompareParams);
}

uint16_t HAL_LCD_getTimerCaptureCompare(){
    return(Timer_B_getCaptureCompareCount(LCD_TIMER_BASE_BKLT,
                                          TIMER_B_CAPTURECOMPARE_REGISTER_0) >>
           4);
}

void HAL_LCD_setTimerDutyCycle(uint16_t dutyCycle){
    Timer_B_setCompareValue(LCD_TIMER_BASE_BKLT,
                            TIMER_B_CAPTURECOMPARE_REGISTER_4,
                            dutyCycle);
}

void HAL_LCD_startTimerCounter(void){
    Timer_B_startCounter(LCD_TIMER_BASE_BKLT,
                         TIMER_B_UP_MODE);
}

void HAL_LCD_stopTimerCounter(void){
    Timer_B_stop(LCD_TIMER_BASE_BKLT);
    GPIO_setAsOutputPin(LCD_BL_EN_PORT,LCD_BL_EN_PIN);
    GPIO_setOutputLowOnPin(LCD_BL_EN_PORT,LCD_BL_EN_PIN);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
