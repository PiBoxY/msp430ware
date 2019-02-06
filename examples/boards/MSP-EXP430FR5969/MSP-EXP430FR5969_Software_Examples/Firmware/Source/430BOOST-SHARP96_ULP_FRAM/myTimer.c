/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
 * Timer.c
 * User Experience Code for the MSP-EXP430FR5969
 * Timer Functions
 *
 * Created: Version 1.0:
 *
 *************************************************************************//****
 * Rev 1.2 - driverlib integration
 ******************************************************************************/

#include "driverlib.h"
#include "Sharp96x96.h"

#include "myTimer.h"
#include "FR59xx_EXP.h"

/**********************************************************************//**
 * @brief  Sleep in LPM3 for input amount of ACLK/8 cycles
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void TA1_sleep(unsigned int time)
{
    Timer_A_initUpModeParam timerA_param = {0};
    timerA_param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    timerA_param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    timerA_param.timerPeriod = time;
    timerA_param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    timerA_param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    timerA_param.timerClear = TIMER_A_SKIP_CLEAR;
    timerA_param.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &timerA_param);

    __bis_SR_register(LPM3_bits + GIE);
    __no_operation();
}

/**********************************************************************//**
 * @brief  Turn on Timer A0 for VCOM toggling (for LCD)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void TA0_enableVCOMToggle(void)
{
    Timer_A_initUpModeParam timerA_param = {0};
    timerA_param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    timerA_param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_8;
    timerA_param.timerPeriod = 4095;
    timerA_param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    timerA_param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    timerA_param.timerClear = TIMER_A_SKIP_CLEAR;
    timerA_param.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &timerA_param);
}

/**********************************************************************//**
 * @brief  Turn off Timer A1
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void TA1_turnOff(void)
{
    Timer_A_disableCaptureCompareInterrupt(__MSP430_BASEADDRESS_T1A3__, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stop(__MSP430_BASEADDRESS_T1A3__);
}

/**********************************************************************//**
 * @brief  Turn off Timer A0
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void TA0_turnOff(void)
{
    Timer_A_disableCaptureCompareInterrupt(__MSP430_BASEADDRESS_T0A3__, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stop(__MSP430_BASEADDRESS_T0A3__);
}

/**********************************************************************//**
 * @brief  Interrupt Service Routine for Timer A1
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TA1_ISR(void)
#elif defined(__GNUC__)
void TA1_ISR(void) __attribute__((interrupt(TIMER1_A0_VECTOR)));
void TA1_ISR(void)
#else
#error Compiler not supported!
#endif
{
    Timer_A_disableCaptureCompareInterrupt(__MSP430_BASEADDRESS_T1A3__, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_stop(__MSP430_BASEADDRESS_T1A3__);

    if (mode == APP_ULP_ADC) {
        select = SEL_MENU_MODE;
    }

    __bic_SR_register_on_exit(LPM3_bits);
}

/**********************************************************************//**
 * @brief  Interrupt Service Routine for Timer A0 (for VCom on LCD)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void TA0_ISR(void) __attribute__((interrupt(TIMER0_A0_VECTOR)));
void TA0_ISR(void)
#else
#error Compiler not supported!
#endif
{
    Sharp96x96_SendToggleVCOMCommand();
}
