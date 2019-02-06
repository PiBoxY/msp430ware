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
//  MSP430x471xx Demo - FLL+, Runs Internal DCO at 16MHz
//
//  Description: This program demonstrates setting the internal DCO to run at
//  8MHz with auto-calibration by the FLL+.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO = (121+1) x 4 x ACLK = 16MHz
//  For correct CPU operation @ 16MHz, the supply voltage should be set accordingly
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//               MSP430x471xx
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.1|--> MCLK = 16MHz
//            |                 |
//            |             P1.5|--> ACLK = 32kHz
//            |                 |
//
//  K.Venkat
//  Texas Instruments Inc.
//  October 2008
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FLL_CTL0 |= DCOPLUS + XCAP11PF;           // DCO+ set, freq = xtal x D x N+1
  SCFI0 = FLLD_4+FN_4;                      // x4 DCO freq, 16MHz nominal DCO
  SCFQCTL = 121;                            // (121+1) x 32768 x 4 = 16 MHz
  P1DIR = BIT1+BIT5;                        // P1.1 & P1.5 to output direction
  P1SEL = BIT1+BIT5;                        // P1.1 & P1.5 to output MCLK & ACLK

  while(1);                                 // Loop in place
}
