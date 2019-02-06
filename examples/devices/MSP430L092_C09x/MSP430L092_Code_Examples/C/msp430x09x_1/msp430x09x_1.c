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
//  MSP430x09x Demo - Software Toggle P1.0
//
//  Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = MCLK = SMCLK = default HFDCO=1MHZ
//
//  Note:The output of P1.0 can be measured from oscilloscope 
// (in the socket board :MSP-TS430L092, there is no LED connect to P1.0).
//
//               MSP430x09x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |             P1.0|--> oscilloscope
//
//
//  F. Chen
//  Texas Instruments Inc.
//  Dec. 2012
//  Built with IAR Version 5.51.1 and CCS Version 5.2.1
//****************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  CCSCTL0 = CCSKEY;                         // open CCS
  CCSCTL4 = SELA__HFCLK+SELS__HFCLK;       //SMCLK and ACLK clock source selection
  CCSCTL5 = DIVA__1;                       //ACLK Source Divider f(ACLK)/4
  CCSCTL8 = 0;

  do
  {
    SFRIFG1 = 0;
    CCSCTL7 = 0;
  }
  while( SFRIFG1 & OFIFG );                // clear OSC fault flag

  P1DIR |= BIT0;                            // Set P1.0 to output direction

  for (;;)
  {
    volatile unsigned int i;

    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR

    i = 500;                           // Delay
    do (i--);
    while (i != 0);
  }
}
