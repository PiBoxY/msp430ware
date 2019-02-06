/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * LiveTempMode.c
 *
 * Continuously sample and convert internal ADC Temperature Sensor values and
 * transmit values through USB Backchannel UART.
 *
 * October 2017
 * E. Chen
 *
 ******************************************************************************/

#include "LiveTempMode.h"

_q8 threshold = _Q8(25.0);

bool thresholdChanged = false;

// LED PWM Paramerters
int period = 100;
int dutyCycle = 50;
int counter = 0;

void liveTemp()
{
    _q8 DegC;          // Q variables using global type
    RTC_stop(RTC_BASE);

    initAdc();
    initEusci();
    initTimerPWM();

    while (mode == 1)
    {
        char str[MAX_STRBUF_SIZE];

        //Enable and Start the conversion
        //in Single-Channel, Single Conversion Mode
        ADC_startConversion(ADC_BASE,
                            ADC_SINGLECHANNEL);

        __bis_SR_register(LPM3_bits | GIE);

        DegC = _Q8(((int16_t)(ADCMEM0 - CALADC_15V_30C)) * (85.0f-30.0f) / (CALADC_15V_85C - CALADC_15V_30C) + 30.0f);
        _Q8toa(str, "%2.2f", DegC);

        // Transmit JSON formatted string
        transmitString("{\"liveTempData\":");
        transmitString(str);
        if (thresholdChanged)
        {
            _Q8toa(str, "%2.2f", threshold);
            transmitString(",\"tempThreshold\":");
            transmitString(str);
            thresholdChanged = false;
        }
        transmitString("}\n");

        // Update LED PWM duty cycles depending on temperature relative to threshold
        float diff = _Q8toF(DegC) - _Q8toF(threshold);

        if (diff < 0)
        {
            diff *= -1;
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, diff*5);
            dutyCycle = 0;
        }
        else
        {
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, 0);
            dutyCycle = diff*5;
        }

        __delay_cycles(1600000);
    }
}

void initTimerPWM()
{
    //GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN1, GPIO_SECONDARY_MODULE_FUNCTION);
    P1SEL0 &= ~BIT1;
    P1SEL1 |= BIT1;

    //Generate PWM - Timer runs in Up mode
    Timer_A_outputPWMParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 100;
    param.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;
    param.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    param.dutyCycle = 50;
    Timer_A_outputPWM(TIMER_A0_BASE, &param);

    //Start timer in continuous mode sourced by ACLK
    Timer_A_initUpModeParam initUpParam = {0};
    initUpParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initUpParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initUpParam.timerPeriod = 1;
    initUpParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initUpParam.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initUpParam.timerClear = TIMER_A_DO_CLEAR;
    initUpParam.startTimer = false;
    Timer_A_initUpMode(TIMER_A1_BASE, &initUpParam);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}


//******************************************************************************
//
//This is the Timer B0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMERA1_ISR(void)
{
    counter++;
    if (counter > period)
    {
        counter = 0;
        if (dutyCycle > 0)
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

    if (counter > dutyCycle)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}
