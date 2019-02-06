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
//  MSP430F51x2 Demo - Timer0_D3, Combining two CCRx register to control one 
//                     PWM channel; Normal Timer mode
//
//  Description: This code example shows how to combine two CCRx registers to 
//  control both the rising and falling edges of one PWM channel (TD0.2, TD1.2). 
//  In up-mode, TDxCCR1 controls the rising edge and TDxCCR2 controls the 
//  falling edge. In this example, since CCR1 registers of TD0/1 have the same 
//  count, the rising edges at TD0.2 and TD1.2 happen almost at the same 
//  time instance. With TD0CCR2=40 and TD1CCR2=96, the dutycycles of the TD0.2 
//  and TD1.2 are 30% and 43.75%. 
//  
//  ACLK = REF0; SMCLK = MCLK = default DCO ~1.045MHz.
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P2.0/TD0.2|--> CCR2 - 30% duty cycle; 
//            |                 |    ON period=(TD0CCR2-TD0CCR1)/32767 ~ 1.22ms
//            |       P2.3/TD1.2|--> CCR2 - 43.75% duty cycle; 
//            |                 |    ON period=(TD1CCR2-TD1CCR1)/32767 ~ 1.7ms 
//
//  B. Nisarga
//  Texas Instruments Inc.
//  Dec 2009
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Set port pins
  P2SEL |= BIT0+BIT3;                       // P2.0/TD0.2 options select
  P2DIR |= BIT0+BIT3;                       // P2.0 output  
  
  // Configure TimerD0 to combine CCR0/1 block to control TD0CCR2 PWM output
  TD0CTL1 |= TD2CMB + TDCLKM_0;             // Combine TD0CCR1 and TD0CCR2
  TD0CCTL2 |= OUTMOD_7;                     // TD0CCR2, Reset/Set
  TD0CCR0 = 128;
  TD0CCR1 = 40;  
  TD0CCR2 = 80;
  
  // Configure TimerD1 to combine CCR0/1 block to control TD1CCR2 PWM output
  TD1CTL1 |= TD2CMB + TDCLKM_0;             // Combine TD0CCR1 and TD0CCR2
  TD1CCTL2 |= OUTMOD_7;                     // TD1CCR2, Reset/Set
  TD1CCR0 = 128;
  TD1CCR1 = 40;  
  TD1CCR2 = 96;                             // duty cycle = (96-40)/32767 ~ 

  TD0CTL0 |= TDSSEL_1 + MC_1 + TDCLR;       // TD0, ACLK, up mode, clear timer
  TD1CTL0 |= TDSSEL_1 + MC_1 + TDCLR;       // TD1, ACLK, up mode, clear timer

  __bis_SR_register(LPM0_bits);             // Enter LPM0
  __no_operation();                         // For debugger    
  
}
