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
//  MSP430F21x2 Demo - Timer1_A2, PWM TA1_1, Up Mode, DCO SMCLK
//
//  Description: This program generates one PWM outputs on P3.7 using
//  Timer1_A2 configured for up mode. The value in TA1CCR0, 512-1, defines the PWM
//  period and the value in TA1CCR1 defines the PWM duty cycle.
//  Using ~1.2MHz SMCLK as TACLK, the timer period is ~425us with
//  a 75% duty cycle on P3.7
//  ACLK = n/a, SMCLK = MCLK = TA1CLK = default DCO ~1.2MHz
//
//               MSP430F21x2
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |       P3.7/TA1_1|--> TA1CCR1 - 75% PWM
//           |                 |
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  January 2008
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.10A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3DIR |= 0x80;                            // 3.7 output
  P3SEL |= 0x80;                            // 3.7 TA1_1 otion
  TA1CCR0 = 512 - 1;                        // PWM Period
  TA1CCTL1 = OUTMOD_7;                      // TA1CCR1 reset/set
  TA1CCR1 = 384;                            // TA1CCR1 PWM duty cycle
  TA1CTL = TASSEL_2 + MC_1;                 // SMCLK, up mode

  __bis_SR_register(CPUOFF);                // Enter LPM0
}

