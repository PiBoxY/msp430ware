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
//  MSP430F20x2/3 Demo - USI SPI Interface with HC165/164 Shift Registers
//
//  Description: Demonstrate USI in two-way SPI mode. Data are read from
//  an HC165, and same data written back to the HC164.
//  ACLK = n/a  MCLK = SMCLK = default DCO, USICLK = SMCLK/2
//  //*USART0 control bits are in different SFR's from other MSP430's//
//
//                         MSP430F20x2/3
//                       -----------------
//                   /|\|              XIN|-
//                    | |                 |     ^      HC164
//          HC165     --|RST          XOUT|-    |  -------------
//        ----------    |                 |     |-|/CLR,B       |  8
//    8  |      /LD|<---|P1.1   SIMO0/P1.6|------>|A          Qx|--\->
//   -\->|A-H   CLK|<---|P1.5/SCLK0 - P1.5|------>|CLK          |
//     |-|INH    QH|--->|P1.7/SOMI0       |       |             |
//     |-|SER      |    |                 |       |             |
//     - |         |    |                 |       |             |
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1OUT = 0;
  P1OUT |= 0x02;
  P1DIR |= 0x02;
  USICTL0 |= USIPE7 + USIPE6 + USIPE5 + USIMST + USIOE; // Port, SPI Master
  USICTL1 |= USICKPH + USIIE;               // Counter interrupt, flag remains set
  USICKCTL = USIDIV_1 + USISSEL_2;          // /2 SMCLK
  USICTL0 &= ~USISWRST;                     // USI released for operation
  USICNT = 8;                               // init-load counter

  while (1)                                 // Loop
  {
    unsigned i;
    for (i = 0xFFFF; i > 0; i--);           // Delay
    while (!(USIIFG & USICTL1));            // Counter clear?
    P1OUT &= ~0x02;                         // Latch data into 'HC165
    P1OUT |= 0x02;
//  ********************                    // Read data are ready to be written
    USICNT = 8;                             // re-load counter
  }
}
