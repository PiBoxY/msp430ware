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
//  MSP-FET430P140 Demo - USART0, SPI Interface to HC165/164 Shift Registers
//
//  Description: Demonstrate USART0 in two-way SPI mode. Data are read from
//  an HC165, and same data written back to the HC164.
//  ACLK = n/a  MCLK = SMCLK = default DCO = UCLK0 = DCO/2
//  //* USART0 control bits are in different SFR's from other MSP430's *//
//
//                           MSP430F149
//                       -----------------
//                   /|\|              XIN|-
//                    | |                 |     ^      HC164
//          HC165     --|RST          XOUT|-    |  -------------
//        ----------    |                 |     |-|/CLR,B       |  8
//    8  |      /LD|<---|P3.0   SIMO0/P3.1|------>|A          Qx|--\->
//   -\->|A-H   CLK|<---|P3.3/UCLK0 - P3.3|------>|CLK          |
//     |-|INH    QH|--->|P3.2/SOMI0       |       |             |
//     |-|SER      |    |                 |       |             |
//     - |         |    |                 |       |             |
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ME1 |= USPIE0;                            // Enable USART0 SPI mode
  UCTL0 |= CHAR + SYNC + MM;                // 8-bit SPI Master **SWRST**
  UTCTL0 |= CKPH + SSEL1 + SSEL0 + STC;     // SMCLK, 3-pin mode
  UBR00 = 0x02;                             // UCLK/2
  UBR10 = 0x00;                             // 0
  UMCTL0 = 0x00;                            // no modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  P3SEL |= 0x0E;                            // P3.1-3 SPI option select
  P3DIR |= 0x01;                            // P3.0 output direction

  while (1)                                 // Loop
  {
    volatile unsigned int i;
    for (i = 0xFFFF; i > 0; i--);           // Delay
    while (!(IFG1 & UTXIFG0));              // USART0 TX buffer ready?
    P3OUT &= ~0x01;                         // Latch data into 'HC165
    P3OUT |= 0x01;
    TXBUF0 = RXBUF0;                        // RXBUF0 to TXBUF0
  }
}
