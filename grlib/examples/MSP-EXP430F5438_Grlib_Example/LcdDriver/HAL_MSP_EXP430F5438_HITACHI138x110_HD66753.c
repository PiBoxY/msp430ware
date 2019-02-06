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
#include "HAL_MSP_EXP430F5438_HITACHI138x110_HD66753.h"
#include "Hitachi138x110_HD66753.h"
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
    GPIO_setOutputHighOnPin(LCD_CS_PORT, LCD_CS_PIN);
    GPIO_setAsOutputPin(LCD_CS_PORT, LCD_CS_PIN);

    GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);
    GPIO_setAsOutputPin(LCD_RST_PORT, LCD_RST_PIN);

    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_BKLT_EN_PORT,
                                                LCD_BKLT_EN_PIN);

    // Clearing RESET pin for 1 msec to meet LCD specification.
    GPIO_setOutputLowOnPin(LCD_RST_PORT, LCD_RST_PIN);
    __delay_cycles(SYSTEM_CLOCK_SPEED * .001);
    GPIO_setOutputHighOnPin(LCD_RST_PORT, LCD_RST_PIN);

    // UCLK,MOSI setup, SOMI cleared
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SIMO_PORT,
                                                LCD_SIMO_PIN);
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_CLK_PORT,
                                                LCD_CLK_PIN);

    USCI_B_SPI_initMasterParam spiMasterConfig =
    {
        USCI_B_SPI_CLOCKSOURCE_SMCLK,                      // SMCLK Clock Source
        UCS_getSMCLK(),                                      // Get SMCLK frequency
        4000000,                                            // SPICLK = 15 MHz
        USCI_B_SPI_MSB_FIRST,                             // MSB First
        USCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,         // Phase
        USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,         // Low polarity
    };

    // Initialize the USCI_B2 module for SPI operation
    USCI_B_SPI_initMaster(LCD_USCI_BASE,
                          &spiMasterConfig);

    USCI_B_SPI_enable(LCD_USCI_BASE);

    USCI_B_SPI_clearInterrupt(LCD_USCI_BASE, USCI_B_SPI_RECEIVE_INTERRUPT);

    // Turn on Backlight
    // Uses PWM to control brightness
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_BKLT_EN_PORT,
                                                LCD_BKLT_EN_PIN);

    Timer_A_initCompareModeParam timerAInitParam =
    {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_RESET_SET,
        Timer_A_getCaptureCompareCount(LCD_TIMER_BASE_BKLT,
                                       TIMER_A_CAPTURECOMPARE_REGISTER_0) >> 1
    };

    Timer_A_initUpModeParam timerAUpModeParams =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        400,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_SKIP_CLEAR
    };

    Timer_A_initCompareMode(LCD_TIMER_BASE_BKLT,
                            &timerAInitParam);

    Timer_A_initUpMode(LCD_TIMER_BASE_BKLT,
                       &timerAUpModeParams);

    Timer_A_startCounter(LCD_TIMER_BASE_BKLT, TIMER_A_UP_MODE);
}

//*****************************************************************************
//
// Writes a command to the HD66753.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t *command)
{
    uint8_t i;
    // Store current GIE state
    uint16_t gie = __get_SR_register() & GIE;

    // Make this operation atomic
    __disable_interrupt();

    HAL_LCD_clearCS();                 //CS = 0 --> Start Transfer
    for(i = 0; i < 6; i++)
    {
        // Wait for TXIFG
        while(!(USCI_B_SPI_getInterruptStatus(LCD_USCI_BASE,
                                              USCI_B_SPI_TRANSMIT_INTERRUPT)))
        {
            ;
        }
        // Send Data
        USCI_B_SPI_transmitData(LCD_USCI_BASE,
                                *command++);

        if(i == 2)                                 //Pull CS up after 3 bytes
        {
            while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
            {
                ;
            }
            HAL_LCD_setCS();          //CS = 1 --> Stop Transfer
            HAL_LCD_clearCS();         //CS = 0 --> Start Transfer
        }
    }
    while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }
    HAL_LCD_setCS();                  //CS = 1 --> Stop Transfer

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
    Timer_A_initCompareModeParam timerAInitParam =
    {
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_RESET_SET,
        captureCompareVal
    };
    Timer_A_initCompareMode(LCD_TIMER_BASE_BKLT,
                            &timerAInitParam);
}

uint16_t HAL_LCD_getTimerCaptureCompare(){
    return(Timer_A_getCaptureCompareCount(LCD_TIMER_BASE_BKLT,
                                          TIMER_A_CAPTURECOMPARE_REGISTER_0) >>
           4);
}

void HAL_LCD_setTimerDutyCycle(uint16_t dutyCycle){
    Timer_A_setCompareValue(LCD_TIMER_BASE_BKLT,
                            TIMER_A_CAPTURECOMPARE_REGISTER_3,
                            dutyCycle);
}

void HAL_LCD_startTimerCounter(void){
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_BKLT_EN_PORT,
                                                LCD_BKLT_EN_PIN);
    Timer_A_startCounter(LCD_TIMER_BASE_BKLT, TIMER_A_UP_MODE);
}

void HAL_LCD_stopTimerCounter(void){
    Timer_A_stop(LCD_TIMER_BASE_BKLT);
}

void HAL_LCD_turnOffBackLight(void){
    GPIO_setAsOutputPin(LCD_BKLT_EN_PORT, LCD_BKLT_EN_PIN);
    GPIO_setOutputLowOnPin(LCD_BKLT_EN_PORT, LCD_BKLT_EN_PIN);

    Timer_A_stop(LCD_TIMER_BASE_BKLT);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
