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
//  MSP430F21x2 Demo - Timer1_A2, Toggle P3.6/TA0_1, Up Mode, 32kHz ACLK
//
//  Description: Toggle P3.6 using hardware TA0_1 output. Timer_A is configured
//  for up mode with TA1CCR0 defining period. In this
//  example, TA1CCR0 is loaded with 10-1 and TA0_1 will toggle P3.6 at TA1CLK/10.
//  Thus the output frequency on P3.6 will be the TA1CLK/20. No CPU or software
//  resources required. Normal operating mode is LPM3.
//  ACLK = TA1CLK = 32kHz, MCLK = default DCO ~1.2MHz
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//
//  As coded with TA1CLK = ACLK, P1.1 output frequency = 32768/20 = 1.6384kHz
//
//           MSP430F21x2
//         -----------------
//     /|\|              XIN|-
//      | |                 | 32kHz
//      --|RST          XOUT|-
//        |                 |
//        |       P3.6/TA0_1|--> ACLK/20
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
  P3DIR |= 0x40;                            // P1.1 output
  P3SEL |= 0x40;                            // P1.1 option select
  TA1CCTL0 = OUTMOD_4;                       // TACCR0 toggle mode
  TA1CCR0 = 10 - 1;
  TA1CTL = TASSEL_1 + MC_1;                  // ACLK, upmode

  __bis_SR_register(LPM3_bits);             // Enter LPM3
}
