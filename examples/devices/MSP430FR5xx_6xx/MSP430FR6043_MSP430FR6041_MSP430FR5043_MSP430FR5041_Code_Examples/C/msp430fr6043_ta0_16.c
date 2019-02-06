/* --COPYRIGHT--,BSD_EX
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
//  MSP430FR60xx Demo - Timer0_A3, PWM TA0.1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates two PWM outputs on P1.0,P1.1 using
//  Timer0_A configured for up mode. The value in CCR0, 1000-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using ~1MHz
//  SMCLK as TACLK, the timer period is ~1ms with a 75% duty cycle on P7.4
//  and 25% on P7.7.
//  ACLK = n/a, SMCLK = MCLK = TACLK = 1MHz
//
//
//           MSP430FR6043
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |     P6.7/TA0.1|--> CCR1 - 75% PWM
//        |     P2.5/TA0.2|--> CCR2 - 25% PWM
//
//   Nathan Siegel & Matt Calvo
//   Texas Instruments Inc.
//   November 2017
//   Built with IAR Embedded Workbench V7.10.4 & Code Composer Studio V7.1
//******************************************************************************

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P2DIR |= BIT5;                          // P2.5 and P6.7 output
    P6DIR |= BIT7;
    P2SEL0 |= BIT5;                         // P2.5 and P6.7 options select
    P6SEL0 |= BIT7;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_0;

    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO = 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK=VLO SMCLK=DCO
    CSCTL3 = DIVA__8 | DIVS__8 | DIVM__8;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure Timer0_A
    TA0CCR0 = 1000-1;                       // PWM Period
    TA0CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA0CCR1 = 750;                          // CCR1 PWM duty cycle
    TA0CCTL2 = OUTMOD_7;                    // CCR2 reset/set
    TA0CCR2 = 250;                          // CCR2 PWM duty cycle
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR

    __bis_SR_register(LPM0_bits);           // Enter LPM0
    __no_operation();                       // For debugger
}
