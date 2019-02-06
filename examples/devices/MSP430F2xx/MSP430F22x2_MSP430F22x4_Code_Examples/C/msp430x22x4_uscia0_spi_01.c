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
//   MSP430F22x4 Demo - USCI_A0, SPI Interface to HC164 Shift Register
//
//   Description: This program demonstrates USCI_A0 in SPI mode, interfaced to
//   a 'HC164 shift register, transferring the value contained in Data.
//   The value is incremented in the mainloop, effectively incrementing
//   HC164 QA - QH.
//   ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz, BRCLK = SMCLK/2
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 |        ^      HC164
//          --|RST          XOUT|-       |  -------------
//            |                 |        |-|/CLR,B       |  8
//            |    UCA0SIMO/P3.4|--------->|A          Qx|--\->
//            |     UCA0CLK/P3.0|--------->|CLK          |
//            |                 |          |             |
//
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   April 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

unsigned char Data;
volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P3SEL |= 0x11;                            // P3.0,4 USCI_A0 option select
  UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x02;
  UCA0BR1 = 0;
  UCA0MCTL = 0;
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  Data = 0x0FF;                             // Load inital data

  while(1)
  {
    Data++;                                 // Increment Data value
    while (!(IFG2 & UCA0TXIFG));            // USCI_A0 TX buffer ready?
    UCA0TXBUF = Data;                       // Byte to SPI TXBUF
    for(i = 0xFFFF; i > 0; i--);            // Delay
  }
}
