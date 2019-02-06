/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 * February 2015
 * E. Chen
 *
 ******************************************************************************/

#include <driverlib.h>
#include "TempSensorMode.h"
#include "hal_LCD.h"

volatile unsigned char tempUnit = 0;         // Temperature Unit
volatile int degC;                       // Celcius measurement
volatile int degF;                       // Fahrenheit measurement

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
    //Enter LPM3 mode with interrupts enabled
    while(mode == TEMPSENSOR_MODE)
    {
        __bis_SR_register(LPM3_bits | GIE);                       // LPM3 with interrupts enabled
        __no_operation();                                         // Only for debugger

        if (tempSensorRunning)
        {
            // Turn LED1 on when waking up to calculate temperature and update display
            P1OUT |= BIT0;

            // Calculate Temperature in degree C and F
            signed short temp = (ADC12MEM0 - CALADC_12V_30C);
            degC = ((long)temp * 10 * (85-30) * 10)/((CALADC_12V_85C-CALADC_12V_30C)*10) + 300;
            degF = (degC) * 9 / 5 + 320;

            // Update temperature on LCD
            displayTemp();

            P1OUT &= ~BIT0;
        }
    }
}

void tempSensorModeInit()
{
    tempSensorRunning = 1;

    displayScrollText("TEMPSENSOR MODE");

    RTC_C_holdClock(RTC_C_BASE);                           // Stop stopwatch
    RTC_C_holdCounterPrescale(RTC_C_BASE, RTC_C_PRESCALE_0);

    // Select internal ref = 1.2V
    Ref_A_setReferenceVoltage(REF_A_BASE,
                              REF_A_VREF1_2V);
    // Internal Reference ON
    Ref_A_enableReferenceVoltage(REF_A_BASE);

    // Enables the internal temperature sensor
    Ref_A_enableTempSensor(REF_A_BASE);

    while(!Ref_A_isVariableReferenceVoltageOutputReady(REF_A_BASE));

    // Initialize the ADC12B Module
    /*
     * Base address of ADC12B Module
     * Use internal ADC12B bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider/pre-divider of 1
     * Use Temperature Sensor internal channel
     */
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_4;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_TEMPSENSEMAP;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    // Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    /*
     * Base address of ADC12B Module
     * For memory buffers 0-7 sample/hold for 256 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_256_CYCLES,
                               ADC12_B_CYCLEHOLD_4_CYCLES,
                               ADC12_B_MULTIPLESAMPLESDISABLE);

    // Configure Memory Buffer
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A30 to memory buffer 0
     * Vref+ = VRef+
     * Vref- = Vref-
     * Memory buffer 0 is the end of a sequence
     */
    ADC12_B_configureMemoryParam configureMemoryParam = {0};
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_TCMAP;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_ENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    ADC12_B_clearInterrupt(ADC12_B_BASE,
                           0,
                           ADC12_B_IFG0
                           );

    // Enable memory buffer 0 interrupt
    ADC12_B_enableInterrupt(ADC12_B_BASE,
                            ADC12_B_IE0,
                            0,
                            0);

    // Start ADC conversion
    ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM0, ADC12_B_REPEATED_SINGLECHANNEL);

    // TimerA1.1 (125ms ON-period) - ADC conversion trigger signal
    Timer_A_initUpMode(TIMER_A1_BASE, &initUpParam_A1);

    // Initialize compare mode to generate PWM1
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

    // Start timer A1 in up mode
    Timer_A_startCounter(TIMER_A1_BASE,
        TIMER_A_UP_MODE
        );


    // Check if any button is pressed
    Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
    __bis_SR_register(LPM3_bits | GIE);         // enter LPM3
    __no_operation();
}

void displayTemp()
{
    clearLCD();

    // Pick C or F depending on tempUnit state
    int deg;
    if (tempUnit == 0)
    {
        showChar('C',pos6);
        deg = degC;
    }
    else
    {
        showChar('F',pos6);
        deg = degF;
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
