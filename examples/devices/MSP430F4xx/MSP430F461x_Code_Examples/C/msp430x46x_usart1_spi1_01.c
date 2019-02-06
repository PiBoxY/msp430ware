/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//   MSP430F461x Demo - USART1, SPI Interface to TLC549 8-Bit ADC
//
//   Description:  This program demonstrates USART1 in SPI mode, interfaced to a
//   TLC549 8-bit ADC. If AIN > 0.5(REF+ - REF-), then P5.1 is set, otherwise it
//   is reset.  R15 = 8-bit ADC code.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO ~ 1048k, UCLK1 = SMCLK/2
//   //* VCC must be at least 3V for TLC549 *//
//
//                           MSP430F461x
//                       -----------------
//                   /|\|              XIN|-
//        TLC549      | |                 |
//    -------------   --|RST          XOUT|-
//   |           CS|<---|P4.6             |
//   |      DATAOUT|--->|P4.4/SOMI1       |
// ~>| IN+  I/O CLK|<---|P4.5/UCLK1       |
//
//   Y. Yang
//   Texas Instruments Inc.
//   January 2013
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;
  char data;

  WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog timer
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  P5DIR |= 0x02;                            // P5.1 as output
  P4SEL |= 0x30;                            // Set P4.4 and P4.5 as peripheral module functions 
  P4DIR |= 0x60;                            // Set P4.5 & P4.6 as outputs
  U1CTL = SWRST;                            // SW Reset
  ME2 |= USPIE1;                            // Enable USART1 SPI
  U1CTL |= CHAR|SYNC|MM|SWRST;              // 8-bit SPI master, keep sw reset
  U1TCTL |= CKPH|SSEL1|SSEL0|STC;           // SMCLK, 3-pin mode
  U1BR0 = 0x02;                             // SMCLK/2 for baud rate
  U1BR1 = 0;                                
  U1MCTL = 0;                               // Clear modulation
  U1CTL &= ~SWRST;                          // Initialize USART state machine

  while(1)
  {
    P4OUT &= ~0x40;                         // Enable TLC549 by driving /CS low
    U1TXBUF = 0x00;                         // Dummy write to start SPI
    while (!(IFG2 & URXIFG1));              // USART1 RX buffer ready?

    data = U1RXBUF;                         // data = 00|DATA

    P4OUT |= 0x40;                          // Disable TLC549 by driving /CS high
    P5OUT &= ~0x02;                         // P5.1 = 0

    if(data>=0x7F)                          // data = AIN > 0.5(REF+ - REF-)?
      P5OUT |= 0x02;                        // P5.2 = 1
  }
}
