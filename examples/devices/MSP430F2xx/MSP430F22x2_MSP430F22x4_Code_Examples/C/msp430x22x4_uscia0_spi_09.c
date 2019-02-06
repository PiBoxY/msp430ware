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
//   MSP430F22x4 Demo - USCI_A0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission.  USCI RX ISR is used to handle
//   communication with the CPU, normally in LPM0. If high, P1.0 indicates
//   valid data reception.
//   ACLK = n/a, MCLK = SMCLK = DCO ~1.2MHz, BRCLK = SMCLK/2
//
//   Use with SPI Slave Data Echo code example. If slave is in debug mode, P3.6
//   slave reset signal conflicts with slave's JTAG; to work around, use IAR's
//   "Release JTAG on Go" on slave device.  If breakpoints are set in
//   slave RX ISR, master must stopped also to avoid overrunning slave
//   RXBUF.
//
//                    MSP430F22x4
//                 -----------------
//             /|\|              XIN|-
//              | |                 |
//              --|RST          XOUT|-
//                |                 |
//                |             P3.4|-> Data Out (UCA0SIMO)
//                |                 |
//          LED <-|P1.0         P3.5|<- Data In (UCA0SOMI)
//                |                 |
//  Slave reset <-|P3.6         P3.0|-> Serial Clock Out (UCA0CLK)
//
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   April 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

unsigned char MST_Data, SLV_Data;

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1OUT = 0x00;                             // P1 setup for LED
  P1DIR |= 0x01;                            //
  P3OUT = 0x40;                             // Set slave reset
  P3DIR |= 0x40;                            //
  P3SEL |= 0x31;                            // P3.0,4,5 USCI_A0 option select
  UCA0CTL0 |= UCCKPL + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 |= 0x02;                          // /2
  UCA0BR1 = 0;                              //
  UCA0MCTL = 0;                             // No modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

  P3OUT &= ~0x40;                           // Now with SPI signals initialized,
  P3OUT |= 0x40;                            // reset slave

  for (i = 50; i > 0; i--);                 // Wait for slave to initialize

  MST_Data = 0x01;                          // Initialize data values
  SLV_Data = 0x00;

  UCA0TXBUF = MST_Data;                     // Transmit first character

  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}

// Test for valid RX and TX character
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  volatile unsigned int i;

  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?

  if (UCA0RXBUF == SLV_Data)                // Test for correct character RX'd
    P1OUT |= 0x01;                          // If correct, light LED
  else
    P1OUT &= ~0x01;                         // If incorrect, clear LED

  MST_Data++;                               // Increment master value
  SLV_Data++;                               // Increment expected slave value
  UCA0TXBUF = MST_Data;                     // Send next value

  for (i = 30; i; i--);                     // Add time between transmissions to
}                                           // make sure slave can keep up

