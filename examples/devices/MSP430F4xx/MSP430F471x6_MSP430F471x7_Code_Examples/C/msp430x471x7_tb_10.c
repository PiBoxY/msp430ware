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
//*******************************************************************************
//   MSP430x471xx Demo - Timer_B, PWM TB1-2 Up Mode, DCO SMCLK
//
//  Description: This program outputs two PWM signals on P3.6 and P3.7
//  using Timer_B configured for up mode. The value in CCR0 defines the PWM
//  period and the values in CCR1 and CCR2 the PWM duty cycles. Using SMCLK
//  as TBCLK, the timer period is 488us.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//                MSP430x471xx
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |         P3.6/TB1|--> CCR1 - 75% PWM
//           |         P3.7/TB2|--> CCR2 - 25% PWM
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |= XCAP11PF;                     // Set load capacitance for xtal  
  P3DIR |= BIT6+BIT7;                       // P3.6,7 as outputs
  P3SEL |= BIT6+BIT7;                       // P3.6,7 TBx options
  TBCCR0 = 512-1;                           // PWM Period
  TBCCTL1 = OUTMOD_7;                       // CCR1 reset/set
  TBCCR1 = 384;                             // CCR1 PWM duty cycle
  TBCCTL2 = OUTMOD_7;
  TBCCR2 = 128;

  TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, up mode

  __bis_SR_register(CPUOFF);                // Enter LPM0
}

