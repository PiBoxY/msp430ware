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
//  MSP-FET430P140 Demo - DMA0/1/2, USART0 SPI 3-Wire SPI Master P1.x Exchange
//
//  Description: SPI Master communicates at fast as possible, full-duplex with
//  SPI Slave using 3-wire mode. The level on P1.4/5 is TX'ed and RX'ed to P1.0
//  and P1.1. Master will pulse slave Reset on init to insure synch start.
//  With data transfer driven directly by DMA - no CPU resources used.
//  DMA0 trigger from TX read, which uses MPY to shift read P1 left
//  DMA1 trigger from MPY moves shifted P1 data to the USART0 TX buffer
//  DMA2 trigger from USART0 RX buffer moves received data to P1
//  Master mode is LPM0.
//  ACLK = n/a, MCLK = SMCLK = DCO ~ 800kHz, ULCK = external
//  //* Final Production MSP430F16x(x) Device Required *//
//
//             fet140_dma_15              fet140_dma_14
//             MSP430F169 Slave           MSP430F169 Master
//             -----------------          -----------------
//            |              XIN|-    /|\|              XIN|-
//            |                 |      | |                 |
//            |             XOUT|-     --|RST          XOUT|-
//            |                 | /|\    |                 |
//            |              RST|--+<----|P3.0             |
//      LED <-|P1.0             |        |             P1.4|<-
//      LED <-|P1.1             |        |             P1.5|<-
//          ->|P1.4             |        |             P1.0|-> LED
//          ->|P1.5             |        |             P1.1|-> LED
//            |       SIMO0/P3.1|<-------|P3.1/SIMO0       |
//            |       SOMI0/P3.2|------->|P3.2/SOMI0       |
//            |        UCLK/P3.3|<-------|P3.3/UCLK        |
//
//  M. Buccini / A. Dannenberg
//  Texas Instruments Inc.
//  November 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>
// Definition necessary to compile with CCEv3.2
// Workaround for &P1OUT
#define _P1OUT_ 0x0021						// Physical address of P1OUT
int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P1OUT = 0x00;                             // P1.0 setup for LED output
  P1DIR |= 0x03;
  P3SEL |= 0x0E;                            // P3.1,2,3 SPI option select
  P3OUT &= ~0x01;
  P3DIR |= 0x01;                            // Reset Slave
  P3DIR &= ~0x01;
  U0CTL = CHAR + SYNC + MM + SWRST;         // 8-bit, SPI, Master
  U0TCTL = CKPL + SSEL1 + STC;              // Polarity, SMCLK, 3-wire
  U0BR0 = 0x02;                             // SPICLK = SMCLK/2
  U0BR1 = 0x00;
  U0MCTL = 0x00;
  ME1 |= USPIE0;                            // Module enable
  U0CTL &= ~SWRST;                          // SPI enable

  for (i = 0xFFF; i > 0; i--);              // Time for slave to ready

  MPY = 0x1000;                             // MPY first operand

// Setup triggers first
  DMACTL0 = DMA2TSEL_3 + DMA1TSEL_11 + DMA0TSEL_4;  // URXIFG0, MPY, UTXIFG0

// RX Store
  DMA2SA = (unsigned int)&U0RXBUF;          // Src address = UART RX Buffer
  DMA2DA = (unsigned int)&P1OUT;            // Dst address = P1
  DMA2SZ = 1;                               // Size in bytes
  DMA2CTL = DMADT_4 + DMASBDB + DMAEN;      // Sng, config

// TX load
  DMA1SA = (unsigned int)&RESHI;            // Src address = multiplier
  DMA1DA = (unsigned int)&U0TXBUF;          // Dst address
  DMA1SZ = 1;                               // Size in bytes
  DMA1CTL = DMADT_4 + DMASBDB + DMAEN;      // Sng, config

// TX Init
  DMA0SA = (unsigned int)&P1IN;             // Src address
  DMA0DA = (unsigned int)&OP2;              // Dst address = multiplier
  DMA0SZ = 1;                               // Size in words
  DMA0CTL = DMADT_4 + DMASBDB + DMALEVEL + DMAEN;  // Sng rpt, config

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
