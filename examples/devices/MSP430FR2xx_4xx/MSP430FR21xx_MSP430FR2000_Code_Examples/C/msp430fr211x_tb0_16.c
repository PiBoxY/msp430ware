/* --COPYRIGHT--,BSD_EX
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
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//*******************************************************************************
//  MSP430FR211x Demo - Timer0_B3, PWM TB0.1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P1.6,P1.7 using
//  Timer0_B configured for up mode. The value in CCR0, 1000-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1MHz
//  SMCLK as TBCLK, the timer period is ~1ms with a 75% duty cycle on P1.6
//  and 25% on P1.7.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz
//
//
//           MSP430FR2111
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |     P1.6/TB0.1|--> CCR1 - 75% PWM
//        |     P1.7/TB0.2|--> CCR2 - 25% PWM
//
//
//   Xiaodong Li
//   Texas Instruments Inc.
//   Dec. 2015
//   Built with IAR Embedded Workbench v6.40 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P1DIR |= BIT6 | BIT7;                     // P1.6 and P1.7 output
    P1SEL1 |= BIT6 | BIT7;                    // P1.6 and P1.7 options select
    
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB0CCR0 = 1000-1;                         // PWM Period
    TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB0CCR1 = 750;                            // CCR1 PWM duty cycle
    TB0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TB0CCR2 = 250;                            // CCR2 PWM duty cycle
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

    __bis_SR_register(LPM0_bits);             // Enter LPM0
    __no_operation();                         // For debugger
}
