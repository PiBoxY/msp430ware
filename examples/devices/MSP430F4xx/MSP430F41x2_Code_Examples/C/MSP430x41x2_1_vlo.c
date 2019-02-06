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
//  MSP430x41x2 Demo - Software Toggle P1.0, MCLK = VLO/8
//
//  Description; Pulse P5.1 with a 1/100 active duty cycle using software.
//  Ultra-low frequency ~ 10kHz, ultra-low power active mode demonstrated. All 
//  the 7 I/O ports are configured as low outputs to eliminate floating inputs.
//  ACLK = VL0, MCLK = VLO ~ 10kHz, SMCLK = n/a
//
//               MSP430x41x2
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P5.1|-->LED
//            |        P1.1/MCLK|-->MCLK = VLO
//            |        P1.6/ACLK|-->ACLK = VLO
//
//  P. Thanigai
//  Texas Instruments Inc.
//  January 2009
//  Built with CCE Version: 3.1 and IAR Embedded Workbench Version: 4.11
//******************************************************************************

#include <msp430.h>


volatile unsigned int i;                    // volatile to prevent optimization

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL1 |= SELM0 + SELM1;                // MCLK = VLO
  FLL_CTL2 |= LF1XT1S_2;                    // LFXT1 = VLOCLK

  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  
  __bis_SR_register(SCG1 + SCG0);           // Stop DCO
 
  
  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT = 0;                                // All P1.x reset
  P2SEL = 0;                                // All P2.x GPIO function
  P2DIR = 0xFF;                             // All P2.x outputs
  P2OUT = 0;                                // All P2.x reset
  P3DIR = 0xFF;                             // All P3.x outputs
  P3OUT = 0;                                // All P3.x reset
  P4DIR = 0xFF;                             // All P4.x outputs
  P4OUT = 0;                                // All P4.x reset
  P5DIR = 0xFF;                             // All P5.x outputs
  P5OUT = 0;                                // All P5.x reset
  P6DIR = 0xFF;                             // All P6.x outputs
  P6OUT = 0;                                // All P6.x reset
  P7DIR = 0xFF;                             // All P7.x outputs
  P7OUT = 0;                                // All P7.x reset  
  
  P1SEL = BIT1+BIT6;                        // P1.1 & P1.6 to output MCLK & ACLK
  
  for (;;)
  {
    P5OUT |= 0x02;
    for (i = 10; i > 0; i--);               // Delay 1x
    P5OUT &= ~0x02;
    for (i = 1000; i > 0; i--);             // Delay 100x
  }
}
