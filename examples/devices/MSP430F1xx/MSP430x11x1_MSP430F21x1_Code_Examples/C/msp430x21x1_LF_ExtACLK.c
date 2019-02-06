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
//  MSP430x21x1 Demo - LF Mode Ext Digital Clk source
//
//  Description: Buffer ACLK on P2.0, default SMCLK(DCO) on P1.4 and MCLK/10 on
//  P1.1. To measure SMCLK on P1.4 with the FET, use "Release JTAG on Go"
//  in the debugger to have access to the port. Then run the code.
//  ACLK = LFXT1 = Ext Clk, MCLK = SMCLK = default DCO ~1.16MHz	
//
//                MSP430F21x1
//             -----------------
//         /|\|              XIN|<--LF Ext ACLK (10-50 kHz)
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |       P1.4/SMCLK|-->SMCLK = ~1.16MHz
//            |             P1.1|-->MCLK/10 = DCO/10
//            |        P2.0/ACLK|-->ACLK = LF Ext ACLK
//
//  H. Grewal / A. Dannenberg
//  Texas Instruments Inc.
//  June 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop Watchdog Timer
  BCSCTL3 |= LFXT1S_3;                      // Ext Digital Clk
  P1DIR |= 0x12;                            // P1.1 and P1.4 outputs
  P1SEL |= 0x10;                            // P1.4 SMCLK output
  P2DIR |= 0x01;                            // P2.0 output
  P2SEL |= 0x01;                            // P2.0 ACLK output

  while(1)
  {
    P1OUT |= 0x02;    	                    // P1.1 = 1
    P1OUT &= ~0x02;                         // P1.1 = 0
  }
}

