/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
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
//******************************************************************************
//
//  functiongenerator.c
//
//  Uses SAC3's DAC to generate a sinusoidal, square, or triangle waveform with 
//  variable frequency and amplitude.
//  
//  SAC1 is configured as a Programmable Gain Amplifier and can be used to manipulate
//  the generated waveform or external signals.
//
//  The final output signal of SAC1 is measured by the internal ADC and transmitted
//  to through the backchannel UART to the computer.
//
//  E. Chen
//  Texas Instruments Inc.
//  May 2018
//******************************************************************************

#include <stdio.h>
#include <math.h>

#include "driverlib.h"
#include "functiongenerator.h"

/* DAC Signal input array */
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(signal)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int signal[MAX_SAMPLE_LEN] = {0};

/* Current index for DAC */
unsigned int index = 1;

/* Function Generator parameters */
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(inputSignalSource)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
char inputSignalSource = 0;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(signalType)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int signalType = 0;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(dacFreq)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int dacFreq = 8192;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(sampleLen)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int sampleLen = MAX_SAMPLE_LEN;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(signalFreq)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int signalFreq = 1;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(signalAmp)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int signalAmp = 1024;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(opAmpMode)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int opAmpMode = 0;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(opAmpGain)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int opAmpGain = 1;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(adcFreq)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
int adcFreq = 512;

/* FRAM Variable that stores function generator ADC results*/
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(functiongenerator_ADC_Result)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
unsigned int functiongenerator_ADC_Result = 0;

void functiongenerator(void) {
    functiongenerator_init_GPIO();

    sprintf(txString, "{\"inputSignalSource\":%d}\n", inputSignalSource);
    transmitString(txString);
    sprintf(txString, "{\"signalType\":%d}\n", signalType);
    transmitString(txString);
    sprintf(txString, "{\"signalFreq\":%d}\n", signalFreq);
    transmitString(txString);
    sprintf(txString, "{\"signalAmp\":%d}\n", signalAmp);
    transmitString(txString);
    sprintf(txString, "{\"dacFreq\":%d}\n", dacFreq);
    transmitString(txString);
    sprintf(txString, "{\"opAmpMode\":%d}\n", opAmpMode);
    transmitString(txString);
    sprintf(txString, "{\"opAmpGain\":%d}\n", opAmpGain);
    transmitString(txString);
    sprintf(txString, "{\"adcFreq\":%d}\n", adcFreq);
    transmitString(txString);

    functiongenerator_enable();

    while(mode == FUNCTIONGENERATOR_MODE) {
        __bis_SR_register(LPM0_bits + GIE);        // Enter LPM0, Enable Interrupt
        sprintf(txString, "{\"a\":%d}\n", functiongenerator_ADC_Result);
        transmitString(txString);
    }
}

void functiongenerator_init_GPIO(void) {
    // Set output on P1.0 LED1 and P6.6 LED2 to LOW
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);

    // Select P1.5 OA1O, P1.6 OA1-, & P1.7 OA1+ functions
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7, GPIO_TERNARY_MODULE_FUNCTION);

    // Select P3.5 OA3O function
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN5, GPIO_TERNARY_MODULE_FUNCTION);
}

void functiongenerator_enable() {
    transmitString("{\"clear\":1}\n");
    if (inputSignalSource == 0) {
        init_DAC_Signal();
        init_DAC();
    }
    if (inputSignalSource != 3)
        init_PGA();
    init_ADC();
}

void functiongenerator_disable() {
    // Disable DAC
    SAC_DAC_disable(SAC3_BASE);
    SAC_OA_disable(SAC3_BASE);
    SAC_disable(SAC3_BASE);

    Timer_B_stop(TB2_BASE);

    SAC_OA_disable(SAC1_BASE);
    SAC_disable(SAC1_BASE);

    // Disable ADC
    Timer_B_stop(TIMER_B1_BASE);
    Timer_B_clear(TIMER_B1_BASE);

    ADC_disable(ADC_BASE);
}

void init_DAC_Signal(){
    unsigned int i;
    float x;

    int dcBias = (opAmpMode == 0) ? (int)2047.5 : signalAmp;
    sampleLen = dacFreq / signalFreq;

    switch (signalType) {
        case SINE_WAVE:
            x = 2.0f * PI * signalFreq / dacFreq;
            for (i = 0; i < sampleLen; i++) {
                signal[i] = (int) (signalAmp * sinf(i * x) + dcBias);
            }
            break;
        case SQUARE_WAVE:
            for (i = 0; i < sampleLen/2; i++) {
                signal[i]= (int) (dcBias + signalAmp);
            }
            for (i = sampleLen/2; i < sampleLen; i++) {
                signal[i]= (int) (dcBias - signalAmp);
            }
            break;
        case SAWTOOTH_WAVE:
            x = 1.0f * signalAmp * 2 / sampleLen;
            for (i = 0; i < sampleLen; i++) {
                signal[i]= (int) (i * x + dcBias - signalAmp);
            }
            break;
        default:
            break;
    }
    index = 1;
}

void init_DAC(void) {
    // Initialize Timer TB2.1 as DAC hardware trigger
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_B_outputPWMParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 32768/dacFreq - 1;
    param.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_RESET;
    param.dutyCycle = param.timerPeriod/2;
    Timer_B_outputPWM(TB2_BASE, &param);

    // Enable SAC3's DAC for function generator
    SAC_DAC_selectRefVoltage(SAC3_BASE, SAC_DAC_PRIMARY_REFERENCE);
    SAC_DAC_selectLoad(SAC3_BASE, SAC_DAC_LOAD_DEVICE_SPECIFIC_0);
    SAC_DAC_interruptEnable(SAC3_BASE);
    SAC_DAC_enable(SAC3_BASE);

    SAC_OA_init(SAC3_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_DAC, SAC_OA_NEGATIVE_INPUT_SOURCE_PGA);
    SAC_PGA_setMode(SAC3_BASE, SAC_PGA_MODE_BUFFER);
    SAC_OA_enable(SAC3_BASE);
    SAC_enable(SAC3_BASE);
}

void init_PGA(void) {
    // Inverting PGA Mode
    if (opAmpMode == 0) {
        SAC_DAC_selectRefVoltage(SAC1_BASE, SAC_DAC_PRIMARY_REFERENCE); // Select 1.5V int Vref as DAC reference
        SAC_DAC_setData(SAC1_BASE, 0x07FF);                             // Set SAC DAC to generate DC bias @ 3.3V / 0.5 = 1.65V
        SAC_DAC_enable(SAC1_BASE);                                      // Enable DAC

        //Select positive and negative pin input
        SAC_OA_init(SAC1_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_DAC, SAC_OA_NEGATIVE_INPUT_SOURCE_PGA);

        if (inputSignalSource == 0)
            // Set inverting PGA mode input to Paired OA Output
            SAC_PGA_setMode(SAC1_BASE, SAC_PGA_MODE_CASCADE_OA_INVERTING);
        else if (inputSignalSource == 1)
            // Set inverting PGA mode input to External Pin OA1-
            SAC_PGA_setMode(SAC1_BASE, SAC_PGA_MODE_INVERTING);

        // Set PGA Gain
        SAC_PGA_setGain(SAC1_BASE, opAmpGain << 4);
    }
    // Noninverting PGA Mode
    else if (opAmpMode == 1) {
        // Disable SAC1 DAC
        SAC_DAC_disable(SAC1_BASE);

        if (inputSignalSource == 0)
            //Select positive and negative pin input
            SAC_OA_init(SAC1_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_PAIR_OA, SAC_OA_NEGATIVE_INPUT_SOURCE_PGA);
        else if (inputSignalSource == 2)
            //Select positive and negative pin input
            SAC_OA_init(SAC1_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_EXTERNAL, SAC_OA_NEGATIVE_INPUT_SOURCE_PGA);

        // Set non-inverting PGA mode
        SAC_PGA_setMode(SAC1_BASE, SAC_PGA_MODE_NONINVERTING);

        // Set PGA Gain
        SAC_PGA_setGain(SAC1_BASE, opAmpGain << 4);
    }

    // Enable SAC and OA
    SAC_OA_enable(SAC1_BASE);
    SAC_enable(SAC1_BASE);
}

void init_ADC(void) {
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use TB1.1B as sample/hold signal to trigger conversion
     * USE SMCLK 24MHZ Digital Oscillator as clock source
     * Use clock divider of 24
     */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_2,
             ADC_CLOCKSOURCE_SMCLK,
             ADC_CLOCKDIVIDER_24);

    ADC_enable(ADC_BASE);

    /*
     * Base Address for the ADC Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_96_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    /*
     * Base Address for the ADC Module
     * Useing 12bit resolution
     */
    ADC_setResolution(ADC_BASE,
                      ADC_RESOLUTION_12BIT);

    //Configure the Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A1
     * Use positive reference of AVcc
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_A5,
                        ADC_VREFPOS_AVCC,
                        ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE,
                       ADC_COMPLETED_INTERRUPT);

    //Enable the Memory Buffer Interrupt
    ADC_enableInterrupt(ADC_BASE,
                        ADC_COMPLETED_INTERRUPT);

    Timer_B_outputPWMParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 32768/adcFreq - 1;
    param.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_RESET;
    param.dutyCycle = param.timerPeriod/2;
    Timer_B_outputPWM(TIMER_B1_BASE, &param);

    //Enable and Start the conversion
    //in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE, ADC_REPEATED_SINGLECHANNEL);
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SAC1_SAC3_VECTOR
__interrupt void SAC3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SAC1_SAC3_VECTOR))) SAC3_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(SAC3IV,SACIV_4))
  {
    case SACIV_0: break;
    case SACIV_2: break;
    case SACIV_4:
        // Update DAC data
        SAC_DAC_setData(SAC3_BASE, signal[index++]);
        if (index >= sampleLen)
            index = 0;
        break;
    default: break;
  }
}
