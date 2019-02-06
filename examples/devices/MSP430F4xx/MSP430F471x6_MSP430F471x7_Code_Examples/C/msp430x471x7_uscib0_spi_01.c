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
//   MSP430x471xx Demo - USCI_B0, SPI Interface to TLC549 8-Bit ADC
//
//   Description: This program demonstrate USCI_A1 in SPI mode interfaced to a
//   TLC549 8-bit ADC. If AIN > 0.5(REF+ - REF-), P5.1 set, else reset.
//   R15 = 8-bit ADC code.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO ~1048k, BRCLK = SMCLK/2
//   //* VCC must be at least 3v for TLC549 *//
//
//                           MSP430x471xx
//                       -----------------
//                   /|\|              XIN|-
//        TLC549      | |                 |   32kHz
//    -------------   --|RST          XOUT|-
//   |           CS|<---|P3.0             |
//   |      DATAOUT|--->|P3.2/UCB0SOMI    |
// ~>| IN+  I/O CLK|<---|P3.3/UCB0CLK     |
//   |             |    |             P5.1|--> LED
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
  char data;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  FLL_CTL0 |= XCAP11PF;                     // Configure load caps

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  P5DIR |= BIT1;                            // P5.1 output
  P3SEL |= BIT3+BIT2;                       // P3.3,2 option select
  P3DIR |= BIT0;                            // P3.0 output direction
  UCB0CTL0 |= UCMST+UCSYNC+UCMSB;           // 3-pin, 8-bit SPI mstr, MSb 1st
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x02;
  UCB0BR1 = 0;
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  while(1)
  {
    P3OUT &= ~BIT0;                         // Enable TLC549, /CS reset
    UCB0TXBUF = 0x00;                       // Dummy write to start SPI
    while (!(IFG2 & UCB0RXIFG));            // USCI_B0 RX buffer ready?

    data = UCB0RXBUF;                       // data = 00|DATA

    P3OUT |= BIT0;                          // Disable TLC549, /CS set
    P5OUT &= ~BIT1;                         // P5.1 = 0

    if(data>=0x7F)                          // data = AIN > 0.5(REF+ - REF-)?
      P5OUT |= BIT1;                        // P5.1 = 1
  }
}
