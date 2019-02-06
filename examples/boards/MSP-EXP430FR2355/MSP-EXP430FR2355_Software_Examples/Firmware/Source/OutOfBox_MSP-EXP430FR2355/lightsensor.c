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
//  lightsensor.c
//
//  Uses SAC2 in generic Op-Amp mode along with onboard photodiode, capacitor, and 
//  resistor to implement a light sensor circuit. The resulting voltage is buffered
//  through SAC0, measured by the internal ADC, and used to control Timer PWMs
//  dutycycle to modulate the brightness of LED1 and LED2.
//
//  E. Chen
//  Texas Instruments Inc.
//  May 2018
//******************************************************************************

#include "driverlib.h"
#include "lightsensor.h"

int led1_dutycycle = 0;
int led2_dutycycle = 0;
int calibratedADC = 500;
int period = 100;
int counter = 0;
int deadzone = 5;
int runningAvg = 500;

/* FRAM Variable that stores lightsensor ADC results*/
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(lightsensor_ADC_Result)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
unsigned int lightsensor_ADC_Result = 0;          // ADC conversion result

void lightsensor() {
    /* Initialize peripherals */
    lightsensor_init_GPIO();
    lightsensor_init_LED_PWM();
    lightsensor_init_SACOA();
    lightsensor_init_ADC();

    while(mode == LIGHTSENSOR_MODE){
        __bis_SR_register(LPM0_bits + GIE);

        runningAvg = (( runningAvg * 9 ) + lightsensor_ADC_Result)/10;
        int diff = (runningAvg - calibratedADC)/4;

        if (diff < deadzone) {
            diff *= -1;
            led1_dutycycle = diff - deadzone;
            led2_dutycycle = 0;
        }
        else if (diff > deadzone) {
            led1_dutycycle = 0;
            led2_dutycycle = diff - deadzone;
        }
        else {
            led1_dutycycle = 0;
            led2_dutycycle = 0;
        }
    }
}

void lightsensor_init_GPIO(void) {
    //Set Px.x to output direction

    P1DIR |= 0xFF;
    P1OUT = 0x00;

    P3DIR |= 0xFF;
    P3OUT = 0x00;

    P6DIR |= 0xFF;
    P6OUT = 0x00;

    //Set P4.1 (S1) to input
    P4DIR &= (~BIT1);
    P4OUT |= BIT1;
    P4REN |= BIT1;
    P4IES |= BIT1;

    P4IFG &= ~BIT1;
    P4IE |= BIT1;
}

/* Configure TimerB0 to generate PWM on LEDs */
void lightsensor_init_LED_PWM(void) {
    //Start timer
    Timer_B_initUpModeParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 1;
    param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    param.startTimer = true;
    Timer_B_initUpMode(TIMER_B0_BASE, &param);
}

void lightsensor_disable_LED_PWM(void) {
    Timer_B_stop(TIMER_B0_BASE);
}

void lightsensor_init_SACOA(void) {
    //Configure Op-Amp functionality
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,
                                               GPIO_PIN1 | GPIO_PIN3 | GPIO_PIN2,
                                               GPIO_TERNARY_MODULE_FUNCTION);

    //Select external source for both positive and negative inputs
    SAC_OA_init(SAC2_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_EXTERNAL,
                SAC_OA_NEGATIVE_INPUT_SOURCE_EXTERNAL);

    //Select low speed and low power mode
    SAC_OA_selectPowerMode(SAC2_BASE, SAC_OA_POWER_MODE_LOW_SPEED_LOW_POWER);

    SAC_OA_enable(SAC2_BASE);                  // Enable SAC2 OA
    SAC_enable(SAC2_BASE);                     // Enable SAC2

    //Select external source for both positive and negative inputs
    SAC_OA_init(SAC0_BASE, SAC_OA_POSITIVE_INPUT_SOURCE_PAIR_OA,
                SAC_OA_NEGATIVE_INPUT_SOURCE_PGA);

    SAC_OA_enable(SAC0_BASE);                  // Enable SAC0 OA
    SAC_enable(SAC0_BASE);                     // Enable SAC0
}

void lightsensor_init_ADC(void) {
    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use TB1.1B as sample/hold signal to trigger conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_2,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);

    ADC_enable(ADC_BASE);

    /*
     * Base Address for the ADC Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_16_CYCLES,
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
                        ADC_INPUT_VEREF_P,
                        ADC_VREFPOS_AVCC,
                        ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE,
                       ADC_COMPLETED_INTERRUPT);

    //Enable the Memory Buffer Interrupt
    ADC_enableInterrupt(ADC_BASE,
                        ADC_COMPLETED_INTERRUPT);

    // Generate sample/hold signal to trigger ADC conversion
    Timer_B_outputPWMParam param2 = {0};
    param2.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param2.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param2.timerPeriod = TIMER_PERIOD;
    param2.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
    param2.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_RESET;
    param2.dutyCycle = DUTY_CYCLE;
    Timer_B_outputPWM(TIMER_B1_BASE, &param2);

    //Enable and Start the conversion
    //in Single-Channel, Single Conversion Mode
    ADC_startConversion(ADC_BASE, ADC_REPEATED_SINGLECHANNEL);
}

//******************************************************************************
//
//This is the Timer B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_B0_VECTOR)))
#endif
void TIMER0_B0_ISR(void)
{
    counter++;
    if (counter > period)
    {
        counter = 0;
        if (led1_dutycycle > 0) {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
        if (led2_dutycycle > 0) {
            GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);
        }
    }

    if (counter == led1_dutycycle)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

    if (counter == led2_dutycycle)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT4_VECTOR))) PORT_4_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    calibratedADC = lightsensor_ADC_Result;
}
