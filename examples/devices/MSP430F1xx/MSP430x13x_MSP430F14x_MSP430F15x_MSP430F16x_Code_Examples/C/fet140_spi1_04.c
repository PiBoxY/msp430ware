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
//   MSP-FET430P140 Demo - USART1, SPI Full-Duplex 3-Wire Master P1.x Exchange
//
//   Description: SPI1 Master communicates at fast as possible, full-duplex with
//   SPI Slave using 3-wire mode. The level on P1.4/5 is TX'ed and RX'ed to P1.0
//   and P1.1. Master will pulse slave Reset on init to insure synch start.
//   Master normal mode is LPM0.
//   ACLK = n/a, MCLK = SMCLK = UCLK = DCO ~ 800kHz
//
//             fet140_slav0               fet140_mstr1
//             MSP430F149 Slave           MSP430F149 Master
//             -----------------          -----------------
//            |              XIN|-    /|\|              XIN|-
//            |                 |      | |                 |
//            |             XOUT|-     --|RST          XOUT|-
//            |                 | /|\    |                 |
//            |              RST|--+<----|P5.0             |
//      LED <-|P1.0             |        |             P1.4|<-
//      LED <-|P1.1             |        |             P1.5|<-
//          ->|P1.4             |        |             P1.0|-> LED
//          ->|P1.5             |        |             P1.1|-> LED
//            |       SIMO0/P3.1|<-------|P5.1/SIMO1       |
//            |       SOMI0/P3.2|------->|P5.2/SOMI1       |
//            |        UCLK/P3.3|<-------|P5.3/UCLK        |
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P1OUT = 0x00;                             // P1.0 setup for LED output
  P1DIR |= 0x03;
  P5SEL |= 0x0E;                            // P5.1,2,3 SPI option select
  P5OUT &= ~0x01;
  P5DIR |= 0x01;                            //Reset Slave
  P5DIR &= ~0x01;
  U1CTL = CHAR + SYNC + MM + SWRST;         // 8-bit, SPI, Master
  U1TCTL = CKPL + SSEL1 + STC;              // Polarity, SMCLK, 3-wire
  U1BR0 = 0x02;                             // SPICLK = SMCLK/2
  U1BR1 = 0x00;
  U1MCTL = 0x00;
  ME2 |= USPIE1;                            // Module enable
  U1CTL &= ~SWRST;                          // SPI enable
  IE2 |= URXIE1 + UTXIE1;                   // RX and TX interrupt enable

  for (i = 0xFFF; i > 0; i--);              // Time for slave to ready
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void SPI1_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1RX_VECTOR))) SPI1_rx (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT = RXBUF1;                           // RXBUF1 to TXBUF1
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1TX_VECTOR
__interrupt void SPI1_tx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1TX_VECTOR))) SPI1_tx (void)
#else
#error Compiler not supported!
#endif
{
  unsigned int i;

  i = P1IN;
  i = i >> 4;
  TXBUF1 = i;                               // Transmit character
}
