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
//******************************************************************************
//!  TI-Design Signal Processing with LEA on MSP430FR5994
//!
//!  William Goh
//!  Texas Instruments Inc.
//!  April 2016
//******************************************************************************

#include <driverlib.h>
#include "benchmark.h"

uint32_t cycleCounts;

void resetBenchmark(void)
{
    // Initialize the cycle counts to 0
    cycleCounts = 0;

    // SMCLK with TAIFG enabled
    TA2CTL = TACLR | TASSEL__SMCLK | TAIE;
}

void startBenchmark(void)
{
    // SMCLK with TAIFG enabled
    TA2CTL |= MC__CONTINUOUS;
}

void stopBenchmark(void)
{
    TA2CTL &= ~(MC__CONTINUOUS);
    cycleCounts = cycleCounts + TA2R;
    TA2CTL |= TACLR;
}

uint32_t getBenchmark(void)
{
    return(cycleCounts);
}

#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
{
    switch(__even_in_range(TA2IV, TA2IV_TAIFG))
    {
    case TA2IV_NONE:   break;               // No interrupt
    case TA2IV_TACCR1: break;               // CCR1 not used
    case TA2IV_3:      break;               // reserved
    case TA2IV_4:      break;               // reserved
    case TA2IV_5:      break;               // reserved
    case TA2IV_6:      break;               // reserved
    case TA2IV_TAIFG:                       // overflow
        cycleCounts = cycleCounts + 0xFFFF;
        break;
    default: break;
    }
}
