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
//   MSP-FET430P140 Demo - USART0, SPI 3-Wire Slave
//
//   Description: SPI slave talks to SPI master using 3-wire mode. Data is sent
//   to the master starting at 0xFF and decrements. Received data from the
//   master is expected to start at 0x00 and increments with each transmission.
//   Prior to initial data exchange, master pulses RST for complete reset.
//   USART0 RX ISR is used to handle communication, CPU normally in LPM4.
//   ACLK = n/a, MCLK = SMCLK = DCO ~ 800kHz
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          | |             XOUT|-
// Master---+-|RST              |
//            |             P3.1|<- Data In (SIMO0)
//            |                 |
//            |             P3.2|-> Data Out (SOMI0)
//            |                 |
//            |             P3.3|<- Serial Clock In (UCLK)
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
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL = 0x0E;                             // Setup P3 for SPI mode
  U0CTL = CHAR + SYNC + SWRST;              // 8-bit, SPI, Slave
  U0TCTL = CKPL + STC;                      // Polarity, UCLK, 3-wire
  ME1 = USPIE0;                             // Module enable
  U0CTL &= ~SWRST;                          // SPI enable
  IE1 |= URXIE0;                            // Recieve interrupt enable
  __enable_interrupt();                     // Enable interrupts

  while (1)
  {
    TXBUF0 = SLV_Data;                      // Ready TXBUF0 w/ 1st character
    LPM4;                                   // Enter LPM4
  }
}// End Main

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void SPI0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) SPI0_rx (void)
#else
#error Compiler not supported!
#endif
{
  while ((IFG1 & UTXIFG0) == 0);            // USART0 TX buffer ready?
  if (U0RXBUF == MST_Data)                  // Test for correct character RX'd
  {
    SLV_Data = SLV_Data -1;                 // Decrement incoming data mask
    MST_Data = MST_Data +1;                 // Increment out-going data
    TXBUF0 = SLV_Data;
  }
  else
    TXBUF0 = SLV_Data;
}
