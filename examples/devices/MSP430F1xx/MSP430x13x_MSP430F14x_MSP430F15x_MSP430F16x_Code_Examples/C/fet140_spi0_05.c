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
//   MSP-FET430P140 Demo - USART0, SPI 3-Wire Master
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x00. Received data is expected to
//   decrement starting at 0xFF. USART0 RX ISR is used to handle communication
//   with the CPU normally in LPM0. The slave is initialized by the master
//   followed by a slave setup delay time in order to assure proper SPI
//   synchronization. P1.0 indicates proper data reception.
//   ACLK = n/a, MCLK = SMCLK = DCO ~ 800kHz
//
//                MSP430F149
//              -----------------
//          /|\|              XIN|-
//           | |                 |
//           --|RST          XOUT|-
//             |                 |
//       LED <-|P1.0         P3.1|-> Data Out (SIMO0)
//             |                 |
//     Scope <-|P3.4         P3.2|<- Data In (SOMI0)
//             |                 |
// Slave RST <-|P3.5         P3.3|-> Serial Clock Out (UCLK)
//
//  Z. Albus
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

char MST_Data = 0x00, SLV_Data = 0xFF;

int main(void)
{
  unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1OUT = 0x000;                            // Setup P1.0 for LED output
  P1DIR |= 0x001;
  P3SEL = 0x00E;                            // Setup P3 for SPI mode
  P3OUT = 0x020;                            // Setup P3.4 for Scope trigger and
  P3DIR |= 0x030;                           // P3.5 for slave initialization
  U0CTL = CHAR + SYNC + MM + SWRST;         // 8-bit, SPI, Master
  U0TCTL = CKPL + SSEL1 + STC;              // Polarity, SMCLK, 3-wire
  U0BR0 = 0x002;                            // SPICLK = SMCLK/2
  U0BR1 = 0x000;
  U0MCTL = 0x000;
  ME1 = USPIE0;                             // Module enable
  U0CTL &= ~SWRST;                          // SPI enable
  IE1 |= URXIE0;                            // Recieve interrupt enable
  __enable_interrupt();                     // Enable interrupts

  P3OUT &= ~0x020;                          // Toggle P3.5: slave reset
  P3OUT |= 0x020;
  i = 50000;                                // Delay
  do (i--);
  while (i != 0);

  while (1)
  {
    TXBUF0 = MST_Data;                      // Transmit first character
    LPM0;                                   // CPU off
  }
} // End Main

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void SPI0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) SPI0_rx (void)
#else
#error Compiler not supported!
#endif
{
  P3OUT ^= 0x010;                           // XOR P3.4 for scope trigger
  while ((IFG1 & UTXIFG0) == 0);            // USART0 TX buffer ready?
  if (U0RXBUF == SLV_Data)                  // Test for correct character RX'd
  {
    SLV_Data = SLV_Data -1;                 // Decrement incoming data mask
    MST_Data = MST_Data +1;                 // Increment out-going data
    TXBUF0 = MST_Data;
    P1OUT |= 0x001;                         // Pulse P1.0 indicating valid data
    P1OUT &= ~0x001;
  }
  else
  {
    TXBUF0 = MST_Data;
    P1OUT |= 0x001;                         // Set P1.0 indicating data error
  }
}
