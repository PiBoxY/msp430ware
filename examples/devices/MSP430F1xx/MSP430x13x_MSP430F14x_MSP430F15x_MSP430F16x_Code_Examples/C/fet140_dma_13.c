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
//  MSP-FET430P140 Demo - DMA0/1/2, USART0 UART 9600 Full-Duplex Transcvr, ACLK
//
//  Description: UART0 communicates continously as fast as possible full-duplex
//  with another device. With data transfer driven directly by DMA - no CPU
//  resources used.
//  DMA0 trigger from TX ready, which uses MPY to shift read P1 left
//  DMA1 trigger from MPY moves shifted P1 data to the USART0 TX buffer
//  DMA2 trigger from USART RX0 buffer moves received data to P1
//  The levels on P1.4/5 are TX'ed. RX'ed value is displayed on P1.0/1.
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//  //* Final Production MSP430F16x(x) Device Required *//
//
//                MSP430F169                   MSP430F169
//             -----------------            -----------------
//            |              XIN|-      /|\|              XIN|-
//            |                 | 32KHz  | |                 | 32KHz
//            |             XOUT|-       --|RST          XOUT|-
//            |                 | /|\      |                 |
//            |              RST|---       |                 |
//            |                 |          |                 |
//          ->|P1.4             |          |             P1.0|-> LED
//          ->|P1.5             |          |             P1.1|-> LED
//      LED <-|P1.0             |          |             P1.4|<-
//      LED <-|P1.1             |          |             P1.5|<-
//            |        UTXD/P3.4|--------->|P3.5/URXD        |
//            |                 | 9600 8N1 |                 |
//            |        URXD/P3.5|<---------|P3.4/UTXD        |
//
//
//  M. Buccini / A. Dannenberg
//  Texas Instruments Inc.
//  November 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1OUT = 0x00;                             // P1.0/1 setup for LED output
  P1DIR = 0x03;
  P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 32k/9600 - 3.41
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4a;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine

  MPY = 0x1000;                             // MPY first operand

  DMACTL0 = DMA2TSEL_3 + DMA1TSEL_11 + DMA0TSEL_4;  // URXIFG0, MPY, UTXIFG0

// TX load
  DMA1SA = (unsigned int)&RESHI;            // Src address = multiplier
  DMA1DA = (unsigned int)&U0TXBUF;          // Dst address = UART TX Buffer
  DMA1SZ = 1;                               // Size in bytes
  DMA1CTL = DMADT_4 + DMASBDB + DMAEN;      // Sng, config

// TX Init
  DMA0SA = (unsigned int)&P1IN;             // Src address
  DMA0DA = (unsigned int)&OP2;              // Dst address = multiplier
  DMA0SZ = 1;                               // Size in bytes
  DMA0CTL = DMADT_4 + DMASBDB + DMALEVEL + DMAEN;  // Sng rpt, config

// RX Store
  DMA2SA = (unsigned int)&U0RXBUF;          // Src address = UART RX Buffer
  DMA2DA = (unsigned int)&P1OUT;            // Dst address = P1
  DMA2SZ = 1;                               // Size in bytes
  DMA2CTL = DMADT_4 + DMASBDB + DMAEN;      // Sng, config

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}
