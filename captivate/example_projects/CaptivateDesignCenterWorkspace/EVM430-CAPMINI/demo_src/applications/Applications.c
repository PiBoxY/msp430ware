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
//#############################################################################
//
//! \file   Applications.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2512
//
//  (C) Copyright 2018, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "msp430.h"
#include <captivate.h>           // CapTIvate Touch Software Library
#include "CAPT_UserConfig.h"     // CapTIvate Auto-Generated User Configuration
#include "Applications.h"

#ifdef __TI_COMPILER_VERSION__

#pragma PERSISTENT(CONV_COUNT)
unsigned int CONV_COUNT = 0;
#pragma PERSISTENT(CONV_GAIN)
unsigned int CONV_GAIN = 0;
#pragma PERSISTENT(PROX_THRESHOLD)
unsigned int PROX_THRESHOLD = 0;
#pragma PERSISTENT(TOUCH_THRESHOLD_KEY1)
unsigned char TOUCH_THRESHOLD_KEY1 = 0;
#pragma PERSISTENT(TOUCH_THRESHOLD_KEY2)
unsigned char TOUCH_THRESHOLD_KEY2 = 0;
#pragma PERSISTENT(TOUCH_THRESHOLD_KEY3)
unsigned char TOUCH_THRESHOLD_KEY3 = 0;
#pragma PERSISTENT(TOUCH_THRESHOLD_KEY4)
unsigned char TOUCH_THRESHOLD_KEY4 = 0;
#pragma PERSISTENT(PROX_DEBOUNCE_IN)
unsigned char PROX_DEBOUNCE_IN = 0;
#pragma PERSISTENT(PROX_DEBOUNCE_OUT)
unsigned char PROX_DEBOUNCE_OUT = 0;
#pragma PERSISTENT(TOUCH_DEBOUNCE_IN)
unsigned char TOUCH_DEBOUNCE_IN = 0;
#pragma PERSISTENT(TOUCH_DEBOUNCE_OUT)
unsigned char TOUCH_DEBOUNCE_OUT = 0;
#pragma PERSISTENT(LTA_FILTER_BETA)
unsigned char LTA_FILTER_BETA = 0;
#pragma PERSISTENT(COUNT_FILTER_BETA)
unsigned char COUNT_FILTER_BETA = 0;
#pragma PERSISTENT(SENSOR_TIMEOUT_THRESHOLD)
unsigned int SENSOR_TIMEOUT_THRESHOLD = 0;
#pragma PERSISTENT(ACTIVE_MODE_SCAN_PERIOD)
unsigned int ACTIVE_MODE_SCAN_PERIOD = 0;
#pragma PERSISTENT(WAKE_ON_PROX_SCAN_PERIOD)
unsigned int WAKE_ON_PROX_SCAN_PERIOD = 0;
#pragma PERSISTENT(INACTIVITY_TIMEOUT)
unsigned int INACTIVITY_TIMEOUT = 0;
#pragma PERSISTENT(WAKE_UP_INTERVAL)
unsigned char WAKE_UP_INTERVAL = 0;

#elif __IAR_SYSTEMS_ICC__
__persistent unsigned int CONV_COUNT = 0;
__persistent unsigned int CONV_GAIN = 0;
__persistent unsigned int PROX_THRESHOLD = 0;
__persistent unsigned char TOUCH_THRESHOLD_KEY1 = 0;
__persistent unsigned char TOUCH_THRESHOLD_KEY2 = 0;
__persistent unsigned char TOUCH_THRESHOLD_KEY3 = 0;
__persistent unsigned char TOUCH_THRESHOLD_KEY4 = 0;
__persistent unsigned char PROX_DEBOUNCE_IN = 0;
__persistent unsigned char PROX_DEBOUNCE_OUT = 0;
__persistent unsigned char TOUCH_DEBOUNCE_IN = 0;
__persistent unsigned char TOUCH_DEBOUNCE_OUT = 0;
__persistent unsigned char LTA_FILTER_BETA = 0;
__persistent unsigned char COUNT_FILTER_BETA = 0;
__persistent unsigned int SENSOR_TIMEOUT_THRESHOLD = 0;
__persistent unsigned int ACTIVE_MODE_SCAN_PERIOD = 0;
__persistent unsigned int WAKE_ON_PROX_SCAN_PERIOD = 0;
__persistent unsigned int INACTIVITY_TIMEOUT = 0;
__persistent unsigned char WAKE_UP_INTERVAL = 0;

#endif

uint16_t Now_Key_Value;

void APP_CheckForParamenterUpdate();

void
ButtonEventHandler(tSensor *pButton)
{
    if(pButton->bSensorTouch)  // Button is touched
    {
        Now_Key_Value = 0;
        if(pButton->pCycle[0]->pElements[0]->bTouch == 1)
            {
                Now_Key_Value |= Key1;
                LED1_ON; // LED1
            }
        else
            LED1_OFF;
        if(pButton->pCycle[1]->pElements[0]->bTouch == 1)
            {
                Now_Key_Value |= Key2;
                LED2_ON; // LED2
            }
        else
            LED2_OFF;
        if(pButton->pCycle[2]->pElements[0]->bTouch == 1)
            {
                Now_Key_Value |= Key3;
                LED3_ON; //LED3
            }
        else
            LED3_OFF;
        if(pButton->pCycle[3]->pElements[0]->bTouch == 1)
            {
                Now_Key_Value |= Key4;
                LED4_ON; // LED4
            }
        else
            LED4_OFF;

        switch (Now_Key_Value)
        {
        case Key1:
            TA1CCR0 = 3824;
            TA1CCR1 = 1912;
            break;
        case (Key1 + Key2):
            TA1CCR0 = 3407;
            TA1CCR1 = 1703;
        break;
        case Key2:
            TA1CCR0 = 3035;
            TA1CCR1 = 1517;
            break;
        case (Key2 + Key3):
            TA1CCR0 = 2865;
            TA1CCR1 = 1432;
        break;
        case Key3:
            TA1CCR0 = 2551;
            TA1CCR1 = 1275;
            break;
        case (Key3 + Key4):
            TA1CCR0 = 2273;
            TA1CCR1 = 1136;
        break;
        case Key4:
            TA1CCR0 = 2024;
            TA1CCR1 = 1012;
            break;
        case (Key4 + Key1):
            TA1CCR0 = 1912;
            TA1CCR1 = 956;
            break;
        default:
            break;
        }

        if(pButton->bSensorPrevTouch == 0)
        {
            TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR
            P2SEL0 |= BIT2;                           // P2.2 options select
        }
    }
    else // No button is touched. Clear Timer and turn off LEDs
    {
        TA1CTL = 0;
        P2SEL0 &= ~BIT2;
        P2OUT &= ~(BIT2 + BIT3 + BIT4 + BIT5 + BIT6);
    }

    // Save Parameter if updated
    APP_CheckForParamenterUpdate();
}

void APP_CheckForParamenterUpdate()
{
    SYSCFG0 = FRWPPW;                   // Program FRAM write enable

    if(CONV_COUNT != BTN00.ui16ConversionCount)
    {
        CONV_COUNT = BTN00.ui16ConversionCount;
    }
    if(CONV_GAIN != BTN00.ui16ConversionGain)
    {
        CONV_GAIN = BTN00.ui16ConversionGain;
    }
    if(PROX_THRESHOLD != BTN00.ui16ProxThreshold)
    {
        PROX_THRESHOLD = BTN00.ui16ProxThreshold;
    }
    if(TOUCH_THRESHOLD_KEY1 != BTN00.pCycle[0]->pElements[0]->ui8TouchThreshold)
    {
        TOUCH_THRESHOLD_KEY1 = BTN00.pCycle[0]->pElements[0]->ui8TouchThreshold;
    }
    if(TOUCH_THRESHOLD_KEY2 != BTN00.pCycle[0]->pElements[1]->ui8TouchThreshold)
    {
        TOUCH_THRESHOLD_KEY2 = BTN00.pCycle[0]->pElements[1]->ui8TouchThreshold;
    }
    if(TOUCH_THRESHOLD_KEY3 != BTN00.pCycle[0]->pElements[2]->ui8TouchThreshold)
    {
        TOUCH_THRESHOLD_KEY3 = BTN00.pCycle[0]->pElements[2]->ui8TouchThreshold;
    }
    if(TOUCH_THRESHOLD_KEY4 != BTN00.pCycle[0]->pElements[3]->ui8TouchThreshold)
    {
        TOUCH_THRESHOLD_KEY4 = BTN00.pCycle[0]->pElements[3]->ui8TouchThreshold;
    }
    if(PROX_DEBOUNCE_IN != BTN00.ProxDbThreshold.DbIn)
    {
        PROX_DEBOUNCE_IN = BTN00.ProxDbThreshold.DbIn;
    }
    if(PROX_DEBOUNCE_OUT != BTN00.ProxDbThreshold.DbOut)
    {
        PROX_DEBOUNCE_OUT = BTN00.ProxDbThreshold.DbOut;
    }
    if(TOUCH_DEBOUNCE_IN != BTN00.TouchDbThreshold.DbIn)
    {
        TOUCH_DEBOUNCE_IN = BTN00.TouchDbThreshold.DbIn;
    }
    if(TOUCH_DEBOUNCE_OUT != BTN00.TouchDbThreshold.DbOut)
    {
        TOUCH_DEBOUNCE_OUT = BTN00.TouchDbThreshold.DbOut;
    }
    if(LTA_FILTER_BETA != BTN00.ui8LTABeta)
    {
        LTA_FILTER_BETA = BTN00.ui8LTABeta;
    }
    if(COUNT_FILTER_BETA != BTN00.ui8CntBeta)
    {
        COUNT_FILTER_BETA = BTN00.ui8CntBeta;
    }
    if(SENSOR_TIMEOUT_THRESHOLD != BTN00.ui16TimeoutThreshold)
    {
        SENSOR_TIMEOUT_THRESHOLD = BTN00.ui16TimeoutThreshold;
    }
    if(ACTIVE_MODE_SCAN_PERIOD != g_uiApp.ui16ActiveModeScanPeriod)
    {
        ACTIVE_MODE_SCAN_PERIOD = g_uiApp.ui16ActiveModeScanPeriod;
    }
    if(WAKE_ON_PROX_SCAN_PERIOD != g_uiApp.ui16WakeOnProxModeScanPeriod)
    {
        WAKE_ON_PROX_SCAN_PERIOD = g_uiApp.ui16WakeOnProxModeScanPeriod;
    }
    if(INACTIVITY_TIMEOUT != g_uiApp.ui16InactivityTimeout)
    {
        INACTIVITY_TIMEOUT = g_uiApp.ui16InactivityTimeout;
    }
    if(WAKE_UP_INTERVAL != g_uiApp.ui8WakeupInterval)
    {
        WAKE_UP_INTERVAL = g_uiApp.ui8WakeupInterval;
    }

    SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
}


void
Panel_Init()
{
    // Check default Conversion count and gain
    if(CONV_COUNT == 0)
    {   // Save Parameters to FRAM
        SYSCFG0 = FRWPPW;                   // Program FRAM write enable

        CONV_COUNT = BTN00.ui16ConversionCount;
        CONV_GAIN = BTN00.ui16ConversionGain;
        PROX_THRESHOLD = BTN00.ui16ProxThreshold;
        TOUCH_THRESHOLD_KEY1 = BTN00.pCycle[0]->pElements[0]->ui8TouchThreshold;
        TOUCH_THRESHOLD_KEY2 = BTN00.pCycle[0]->pElements[1]->ui8TouchThreshold;
        TOUCH_THRESHOLD_KEY3 = BTN00.pCycle[0]->pElements[2]->ui8TouchThreshold;
        TOUCH_THRESHOLD_KEY4 = BTN00.pCycle[0]->pElements[3]->ui8TouchThreshold;
        PROX_DEBOUNCE_IN = BTN00.ProxDbThreshold.DbIn;
        PROX_DEBOUNCE_OUT = BTN00.ProxDbThreshold.DbOut;
        TOUCH_DEBOUNCE_IN = BTN00.TouchDbThreshold.DbIn;
        TOUCH_DEBOUNCE_OUT = BTN00.TouchDbThreshold.DbOut;
        LTA_FILTER_BETA = BTN00.ui8LTABeta;
        COUNT_FILTER_BETA = BTN00.ui8CntBeta;
        SENSOR_TIMEOUT_THRESHOLD = BTN00.ui16TimeoutThreshold;
        ACTIVE_MODE_SCAN_PERIOD = g_uiApp.ui16ActiveModeScanPeriod;
        WAKE_ON_PROX_SCAN_PERIOD = g_uiApp.ui16WakeOnProxModeScanPeriod;
        INACTIVITY_TIMEOUT = g_uiApp.ui16InactivityTimeout;
        WAKE_UP_INTERVAL = g_uiApp.ui8WakeupInterval;

        SYSCFG0 = FRWPPW | PFWP;            // Program FRAM write protected (not writable)
    }
    else // Load Parameters from FRAM
    {
        BTN00.ui16ConversionCount = CONV_COUNT;
        BTN00.ui16ConversionGain = CONV_GAIN;
        BTN00.ui16ProxThreshold = PROX_THRESHOLD;
        BTN00.pCycle[0]->pElements[0]->ui8TouchThreshold = TOUCH_THRESHOLD_KEY1;
        BTN00.pCycle[0]->pElements[1]->ui8TouchThreshold = TOUCH_THRESHOLD_KEY2;
        BTN00.pCycle[0]->pElements[2]->ui8TouchThreshold = TOUCH_THRESHOLD_KEY3;
        BTN00.pCycle[0]->pElements[3]->ui8TouchThreshold = TOUCH_THRESHOLD_KEY4;
        BTN00.ProxDbThreshold.DbIn = PROX_DEBOUNCE_IN;
        BTN00.ProxDbThreshold.DbOut = PROX_DEBOUNCE_OUT;
        BTN00.TouchDbThreshold.DbIn = TOUCH_DEBOUNCE_IN;
        BTN00.TouchDbThreshold.DbOut = TOUCH_DEBOUNCE_OUT;
        BTN00.ui8LTABeta = LTA_FILTER_BETA;
        BTN00.ui8CntBeta = COUNT_FILTER_BETA;
        BTN00.ui16TimeoutThreshold = SENSOR_TIMEOUT_THRESHOLD;
        g_uiApp.ui16ActiveModeScanPeriod = ACTIVE_MODE_SCAN_PERIOD;
        g_uiApp.ui16WakeOnProxModeScanPeriod = WAKE_ON_PROX_SCAN_PERIOD;
        g_uiApp.ui16InactivityTimeout = INACTIVITY_TIMEOUT;
        g_uiApp.ui8WakeupInterval = WAKE_UP_INTERVAL;
    }

    BTN00.pvCallback = &ButtonEventHandler;

    // Configure LED
    P2DIR |= BIT3 + BIT4 + BIT5 + BIT6;
    P2OUT &= ~(BIT3 + BIT4 + BIT5 + BIT6);
    P2SEL1 = 0;

    // Configure PWM output
    TA1CCR0 = 0;                       // PWM Period
    TA1CCTL1 = OUTMOD_7;               // CCR1 reset/set
    TA1CCR1 = 0;                       // CCR1 PWM duty cycle
}
