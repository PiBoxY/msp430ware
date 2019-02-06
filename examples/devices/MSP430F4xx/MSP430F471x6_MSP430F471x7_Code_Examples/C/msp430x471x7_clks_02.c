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
//*****************************************************************************
//  MSP430x471xx Demo - FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
//
//  Description:  This program demonstrates using an external 32kHz crystal to
//  supply ACLK, and using a high speed crystal or resonator to supply SMCLK.
//  MLCK for the CPU is supplied by the internal DCO.  The 32kHz crystal
//  connects between pins Xin and Xout. The high frequency crystal or
//  resonator connects between pins XT2IN and XT2OUT.  The DCO clock is
//  generated internally and calibrated from the 32kHz crystal.  ACLK is
//  brought out on pin P1.5, SMCLK is brought out on P1.4, and MCLK is
//  brought out on pin P1.1.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//  NOTE: External matching capacitors must be added for the high
//       speed crystal or resonator as required.
//
//                MSP430x471xx
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |                 |
//            |            XT2IN|-
//            |                 | HF XTAL or Resonator (add capacitors)
//            |           XT2OUT|-
//            |                 |
//            |             P1.5|--> ACLK = 32kHz Crystal Out
//            |                 |
//            |             P1.4|--> SMCLK = High Freq Xtal or Resonator Out
//            |                 |
//            |             P1.1|--> MCLK = Default DCO Frequency
//            |                 |
//            |                 |
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************

#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |= XCAP11PF;                     // Set load capacitance
  FLL_CTL1 &= ~XT2OFF;                      // Turn on XT2

  // Loop until 32kHz crystal stabilizes
  do
  {
    IFG1 &= ~OFIFG;                         // Clear oscillator fault flag
    for (i = 50000; i; i--);                // Delay
  }
  while (IFG1 & OFIFG);                     // Test osc fault flag

  FLL_CTL1 = SELS;                          // Select SMCLK source as XT2CLK
  FLL_CTL2 |= XT2S_2;
  P1DIR = BIT1+BIT4+BIT5;                   // Set P1.1,4,5 as outputs
  P1SEL = BIT1+BIT4+BIT5;                   // Select P1.1,4,5 as clocks

  while(1);                                 // Loop in place
}
