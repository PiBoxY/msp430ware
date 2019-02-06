/* --COPYRIGHT--,BSD_EX
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
//  MSP430FR60xx Demo - Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK
//
//  Description: This program generates two PWM outputs on P1.2,P1.3 using
//  Timer1_A configured for up mode. The value in CCR0, 100-1, defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using 32768Hz
//  ACLK as TACLK, the timer period is ~3ms with a 75% duty cycle on P7.5
//  and 25% on P8.4.
//  ACLK = TACLK =32768Hz, SMCLK = MCLK = DCO / 2 = 8MHz / 2 = 4MHz
//
//          MSP430FR6047
//         ---------------
//     /|\|            XIN|-
//      | |               |  32KHz Crystal
//      --|RST        XOUT|-
//        |               |
//        |     P7.5/TA1.1|--> CCR1 - 75% PWM
//        |     P8.4/TA1.2|--> CCR2 - 25% PWM
//
//   Evan Wakefield
//   Texas Instruments Inc.
//   October 2016
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    // P7.5 and P8.4 for output
    P7DIR |= BIT5;
    P8DIR |= BIT4;

    // TA1.1 Select for P7.5
    P7SEL0 |= BIT5;
    P7SEL1 &= ~BIT5;

    // TA1.2 Select for P8.4
    P8SEL0 &= ~BIT4;
    P8SEL1 |= BIT4;

    PJSEL0 |= BIT4 | BIT5;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // XT1 Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; // Set ACLK = XT1; MCLK = DCO
    CSCTL3 = DIVA__1 | DIVS__2 | DIVM__2;   // Set all dividers
    CSCTL4 &= ~LFXTOFF;
    do
    {
        CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    TA1CCR0 = 100-1;                        // PWM Period
    TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA1CCR1 = 75;                           // CCR1 PWM duty cycle
    TA1CCTL2 = OUTMOD_7;                    // CCR2 reset/set
    TA1CCR2 = 25;                           // CCR2 PWM duty cycle
    TA1CTL = TASSEL__ACLK | MC__UP | TACLR; // ACLK, up mode, clear TAR

    __bis_SR_register(LPM3_bits);           // Enter LPM3
    __no_operation();                       // For debugger
}
