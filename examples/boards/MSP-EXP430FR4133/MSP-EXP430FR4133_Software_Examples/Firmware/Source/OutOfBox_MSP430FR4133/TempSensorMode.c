/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/*******************************************************************************
 *
 * TempSensorMode.c
 *
 * Simple thermometer application that uses the internal temperature sensor to
 * measure and display die temperature on the segmented LCD screen
 *
 * September 2014
 * E. Chen
 *
 ******************************************************************************/

#include "TempSensorMode.h"
#include "hal_LCD.h"
#include "main.h"

                                                        // See device datasheet for TLV table memory mapping
#define CALADC_15V_30C  *((unsigned int *)0x1A1A)       // Temperature Sensor Calibration-30 C
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)       // Temperature Sensor Calibration-85 C

volatile unsigned char * tempUnit = &BAKMEM4_H;         // Temperature Unit
volatile unsigned short *degC = (volatile unsigned short *) &BAKMEM5;                          // Celsius measurement
volatile unsigned short *degF = (volatile unsigned short *) &BAKMEM6;                          // Fahrenheit measurement

// TimerA UpMode Configuration Parameter
Timer_A_initUpModeParam initUpParam_A1 =
{
    TIMER_A_CLOCKSOURCE_ACLK,               // ACLK Clock Source
    TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1 = 32768Hz
    0x2000,                                 // Timer period
    TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE ,   // Disable CCR0 interrupt
    TIMER_A_DO_CLEAR                        // Clear value
};

Timer_A_initCompareModeParam initCompParam =
{
    TIMER_A_CAPTURECOMPARE_REGISTER_1,        // Compare register 1
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, // Disable Compare interrupt
    TIMER_A_OUTPUTMODE_RESET_SET,             // Timer output mode 7
    0x1000                                    // Compare value
};

void tempSensor()
{
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use Timer trigger 1 as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
        ADC_SAMPLEHOLDSOURCE_2,
        ADC_CLOCKSOURCE_ADCOSC,
        ADC_CLOCKDIVIDER_1);

    ADC_enable(ADC_BASE);

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A12 Temp Sensor
     * Use positive reference of Internally generated Vref
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
        ADC_INPUT_TEMPSENSOR,
        ADC_VREFPOS_INT,
        ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE,
            ADC_COMPLETED_INTERRUPT);

    // Enable the Memory Buffer Interrupt
    ADC_enableInterrupt(ADC_BASE,
            ADC_COMPLETED_INTERRUPT);

    ADC_startConversion(ADC_BASE,
                        ADC_REPEATED_SINGLECHANNEL);

    // Enable internal reference and temperature sensor
    PMM_enableInternalReference();
    PMM_enableTempSensor();

    // TimerA1.1 (125ms ON-period) - ADC conversion trigger signal
    Timer_A_initUpMode(TIMER_A1_BASE, &initUpParam_A1);

    //Initialize compare mode to generate PWM1
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

    // Start timer A1 in up mode
    Timer_A_startCounter(TIMER_A1_BASE,
        TIMER_A_UP_MODE
        );

    // Delay for reference settling
    __delay_cycles(300000);

    //Enter LPM3.5 mode with interrupts enabled
    while(*tempSensorRunning)
    {
        __bis_SR_register(LPM3_bits | GIE);                       // LPM3 with interrupts enabled
        __no_operation();                                         // Only for debugger

        if (*tempSensorRunning)
        {
        	// Turn LED1 on when waking up to calculate temperature and update display
            P1OUT |= BIT0;

            // Calculate Temperature in degree C and F
            signed short temp = (ADCMEM0 - CALADC_15V_30C);
            *degC = ((long)temp * 10 * (85-30) * 10)/((CALADC_15V_85C-CALADC_15V_30C)*10) + 300;
            *degF = (*degC) * 9 / 5 + 320;

            // Update temperature on LCD
            displayTemp();

            P1OUT &= ~BIT0;
        }
    }

    // Loop in LPM3 to while buttons are held down and debounce timer is running
    while(TA0CTL & MC__UP)
    {
        __bis_SR_register(LPM3_bits | GIE);         // Enter LPM3
        __no_operation();
    }

    if (*mode == TEMPSENSOR_MODE)
    {
        // Disable ADC, TimerA1, Internal Ref and Temp used by TempSensor Mode
        ADC_disableConversions(ADC_BASE,ADC_COMPLETECONVERSION);
        ADC_disable(ADC_BASE);

        Timer_A_stop(TIMER_A1_BASE);

        PMM_disableInternalReference();
        PMM_disableTempSensor();
        PMM_turnOffRegulator();

        __bis_SR_register(LPM4_bits | GIE);         // re-enter LPM3.5
        __no_operation();
    }
}

void tempSensorModeInit()
{
    *tempSensorRunning = 1;

    displayScrollText("TEMPSENSOR MODE");

    RTC_stop(RTC_BASE);                           // Stop stopwatch

    // Check if any button is pressed
    Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
}

void displayTemp()
{
    clearLCD();

    // Pick C or F depending on tempUnit state
    int deg;
    if (*tempUnit == 0)
    {
        showChar('C',pos6);
        deg = *degC;
    }
    else
    {
        showChar('F',pos6);
        deg = *degF;
    }

    // Handle negative values
    if (deg < 0)
    {
        deg *= -1;
        // Negative sign
        LCDMEM[pos1+1] |= 0x04;
    }

    // Handles displaying up to 999.9 degrees
    if (deg>=1000)
        showChar((deg/1000)%10 + '0',pos2);
    if (deg>=100)
        showChar((deg/100)%10 + '0',pos3);
    if (deg>=10)
        showChar((deg/10)%10 + '0',pos4);
    if (deg>=1)
        showChar((deg/1)%10 + '0',pos5);

    // Decimal point
    LCDMEM[pos4+1] |= 0x01;

    // Degree symbol
    LCDMEM[pos5+1] |= 0x04;
}
