/* --COPYRIGHT--,BSD_EX
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
//  MSP430F67791 Demo - Timer1_A1, PWM TA1.1, Up Mode, 32kHz ACLK
//
//  Description: This program generates a PWM output on P1.0 using Timer1_A
//  configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the value in CCR1 the PWM duty cycle. Using 32kHz
//  ACLK as TACLK, the timer period is ~ (512/32k) ~ 15.6ms with a 75% duty
//  cycle on P1.0. Normal operating mode is LPM3.
//  ACLK = TACLK = LFXT1 = 32768Hz, MCLK = default DCO ~1.045MHz.
//
//              MSP430F67791
//            ---------------
//        /|\|               |
//         | |               |
//         --|RST            |
//           |               |
//           |     P1.0/TA1.1|--> CCR1 - 75% PWM
//
//   C. Fu
//   Texas Instruments Inc.
//   October 2012
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.51
//******************************************************************************
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P1.0 TA1.1
    P1DIR |= BIT0;                          // P1.0/TA1.1 output
    P1SEL0 |= BIT0;                          // Output TA1

    // Setup LFXT1
    UCSCTL6 &= ~(XT1OFF);                   // XT1 On
    UCSCTL6 |= XCAP_3;                      // Internal load cap
    // Loop until XT1 fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                  // Clear fault flags
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

    // Setup TA1
    TA1CCR0 = 512 - 1;                      // PWM Period
    TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA1CCR1 = 384;                          // CCR1 PWM duty cycle
    TA1CTL = TASSEL_1 | MC_1 | TACLR;       // ACLK, up mode, clear TAR

    __bis_SR_register(LPM3_bits);           // Enter LPM3
    __no_operation();                       // For debugger
}

