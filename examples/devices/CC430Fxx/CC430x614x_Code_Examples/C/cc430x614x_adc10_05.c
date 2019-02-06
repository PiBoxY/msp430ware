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
//  C430F614x Demo - ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
//
//  Description: A single sample is made on A11 (AVcc/2) with reference to
//  internal 1.5V Vref. Software sets ADC10SC to start sample and conversion
//  - ADC10SC automatically cleared at EOC. ADC10 internal oscillator times
//  sample (16x) and conversion. ADC10BUSY flag is polled for EOC. If A11
//  (AVcc/2) < 0311h (1.15V) indicating AVcc is less 2.3V, P1.0 reset indicating
//  a lo_Batt condition, else set.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC
//
//                C430F6147
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |A11 (AVcc/2) P1.0|-->LED
//
//  G. Larmore
//  Texas Instruments Inc.
//  June 2012
//  Built with CCS v5.2 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
  ADC10CTL0 = ADC10SHT_2 + ADC10ON;         // 16 ADC10CLKs; ADC ON
  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_11;  // AVcc/2  
  
  // Configure internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_0+REFON;               // Select internal ref = 1.5V 
                                            // Internal Reference ON   
  __delay_cycles(75);                       // Delay (~75us) for Ref to settle
  
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;                           // Clear P1.0

  while(1)
  {
    ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
    while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
    __no_operation();                       // For debug only
    if (ADC10MEM0 < 0x311)                  // is ADC10MEM = A11 > 1.15V?
      P1OUT &= ~BIT0;                       // Clear P1.0 LED off
    else
      P1OUT |= BIT0;                         // Set P1.0 LED on
  }
}

