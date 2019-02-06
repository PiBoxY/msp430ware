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
#include "HAL_MSP_EXP430F5529LP_KITRONIX320X240_SSD2119_SPI.h"
#include "kitronix320x240x16_ssd2119_spi.h"
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
    // Configure the pins that connect to the LCD as GPIO outputs.
    //
    GPIO_setAsOutputPin(LCD_RESET_PORT,
                        LCD_RESET_PIN);

    GPIO_setOutputLowOnPin(LCD_RESET_PORT,
                           LCD_RESET_PIN);

    GPIO_setAsOutputPin(LCD_SDC_PORT,
                        LCD_SDC_PIN);

    GPIO_setOutputLowOnPin(LCD_SDC_PORT,
                           LCD_SDC_PIN);

    GPIO_setAsOutputPin(LCD_SCS_PORT,
                        LCD_SCS_PIN);

    GPIO_setOutputLowOnPin(LCD_SCS_PORT,
                           LCD_SCS_PIN);

    //
    // Configure SPI peripheral.
    //
    // Configure LCD_SIMO_PIN
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SDI_PORT,
                                                LCD_SDI_PIN);

    // Configure LCD_CLK_PIN  option select CLK
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SCL_PORT, LCD_SCL_PIN);

    USCI_B_SPI_initMasterParam spiMasterParams =
    {
        USCI_B_SPI_CLOCKSOURCE_SMCLK,
        UCS_getSMCLK(),
        15000000,
        USCI_B_SPI_MSB_FIRST,
        USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
        USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
    };

    // Initialize USCI_B1 for SPI Master operation
    USCI_B_SPI_initMaster(LCD_USCI_BASE,
                          &spiMasterParams
                          );

    USCI_B_SPI_clearInterrupt(LCD_USCI_BASE,
                              USCI_B_SPI_RECEIVE_INTERRUPT);

    USCI_B_SPI_enable(LCD_USCI_BASE);

    //
    // Set the LCD Backlight high to enable
    //
    GPIO_setAsOutputPin(LCD_PWM_PORT,
                        LCD_PWM_PIN);

    GPIO_setOutputHighOnPin(LCD_PWM_PORT,
                            LCD_PWM_PIN);

    //
    // Set the LCD control pins to their default values.
    //

    GPIO_setOutputHighOnPin(LCD_SDC_PORT,
                            LCD_SDC_PIN);

    GPIO_setOutputLowOnPin(LCD_SCS_PORT,
                           LCD_SCS_PIN);

    //
    // Delay for 1ms.
    //
    __delay_cycles(HAL_LCD_MCLK_FREQUENCY * 0.001);

    //
    // Deassert the LCD reset signal.
    //

    GPIO_setOutputHighOnPin(LCD_RESET_PORT,
                            LCD_RESET_PIN);

    //
    // Delay for 1ms while the LCD comes out of reset.
    //
    __delay_cycles(HAL_LCD_MCLK_FREQUENCY * 0.001);
}

//*****************************************************************************
//
// Writes a command to the UC1701.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t command)
{
    // Store current GIE state
    uint16_t gie = __get_SR_register() & GIE;

    // Make this operation atomic
    __disable_interrupt();

    //
    // Wait for any SPI transmission to complete before setting the LCD_SDC signal.
    //
    while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }

    //
    // Set the LCD_SDC signal low, indicating that following writes are commands.
    //
    GPIO_setOutputLowOnPin(LCD_SDC_PORT, LCD_SDC_PIN);

    //
    // Transmit the command.
    //
    USCI_B_SPI_transmitData(LCD_USCI_BASE, command);

    //
    // Wait for the SPI transmission to complete before setting the LCD_SDC signal.
    //
    while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }

    //
    // Set the LCD_SDC signal high, indicating that following writes are data.
    //
    GPIO_setOutputHighOnPin(LCD_SDC_PORT,LCD_SDC_PIN);

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
    uint8_t ui8Data;
    // Store current GIE state
    uint16_t gie = __get_SR_register() & GIE;

    // Make this operation atomic
    __disable_interrupt();

    //
    // Calculate the high byte to transmit.
    //
    ui8Data = (uint8_t)(data >> 8);

    //
    // Wait for the transmit buffer to become empty.
    //
    while(!USCI_B_SPI_getInterruptStatus(LCD_USCI_BASE,
                                         USCI_B_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }

    //
    // Transmit the high byte.
    //
    USCI_B_SPI_transmitData(LCD_USCI_BASE,ui8Data);

    //
    // Calculate the low byte to transmit.
    //
    ui8Data = (uint8_t)(data & 0xff);

    //
    // Wait for the transmit buffer to become empty.
    //
    while(!USCI_B_SPI_getInterruptStatus(LCD_USCI_BASE,
                                         USCI_B_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }

    //
    // Transmit the high byte.
    //
    USCI_B_SPI_transmitData(LCD_USCI_BASE,ui8Data);

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

void HAL_LCD_selectLCD(){
    //
    // Wait for any SPI transmission to complete before setting the LCD_SCS signal.
    //
    while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }

    GPIO_setOutputLowOnPin(LCD_SCS_PORT, LCD_SCS_PIN);
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

void HAL_LCD_deselectLCD(){
    //
    // Wait for any SPI transmission to complete before setting the LCD_SCS signal.
    //
    while(USCI_B_SPI_isBusy(LCD_USCI_BASE))
    {
        ;
    }

    GPIO_setOutputHighOnPin(LCD_SCS_PORT, LCD_SCS_PIN);
}

//*****************************************************************************
//
// Generates delay of
//
// \param cycles number of cycles to delay
//
// \return None
//
//*****************************************************************************
void HAL_LCD_delay30ms()
{
    __delay_cycles(HAL_LCD_MCLK_FREQUENCY * 0.030);
}

void HAL_LCD_initTimer(uint16_t captureCompareVal){
    Timer_B_initCompareModeParam timerBCompareParams =
    {
        TIMER_B_CAPTURECOMPARE_REGISTER_4,
        TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_B_OUTPUTMODE_RESET_SET,
        captureCompareVal
    };

    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_PWM_PORT,
                                                LCD_PWM_PIN);

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
    GPIO_setAsOutputPin(LCD_PWM_PORT,LCD_PWM_PIN);
    GPIO_setOutputLowOnPin(LCD_PWM_PORT,LCD_PWM_PIN);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
