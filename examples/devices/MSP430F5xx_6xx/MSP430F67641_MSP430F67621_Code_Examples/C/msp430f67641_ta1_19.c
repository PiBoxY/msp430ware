/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//******************************************************************************
//  MSP430F67641 Demo - Timer1_A1, PWM TA1.1, Up/Down Mode, DCO SMCLK
//
//  Description: This program generates a PWM output on P8.5 using
//  Timer1_A configured for up/down mode. The value in CCR0, 128, defines the
//  PWM period/2 and the value in CCR1 the PWM duty cycle. Using
//  ~1.045MHz SMCLK as TACLK, the timer period is ~233us with a 75% duty cycle
//  on P8.5.
//  SMCLK = MCLK = TACLK = default DCO ~1.045MHz.
//
//              MSP430F67641
//            ---------------
//        /|\|               |
//         | |               |
//         --|RST            |
//           |               |
//           |     P8.5/TA1.1|--> CCR1 - 75% PWM
//
//  E. Chen
//  Texas Instruments Inc.
//  January 2014
//  Built with CCS Version: 5.5.0 and IAR Embedded Workbench Version: 5.52
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P8.5 TA1.1
    P8DIR |= BIT5;                          // P8.5/TA1.1 output
    P8SEL |= BIT5;                          // Output TA1

    // Setup TA1
    TA1CCR0 = 128;                          // PWM Period/2
    TA1CCTL1 = OUTMOD_6;                    // CCR1 toggle/set
    TA1CCR1 = 32;                           // CCR1 PWM duty cycle
    TA1CTL = TASSEL_2 | MC_3 | TACLR;       // SMCLK, up-down mode, clear TAR

    __bis_SR_register(LPM0_bits);           // Enter LPM0
    __no_operation();                       // For debugger
}

