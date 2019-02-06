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
//  MSP-FET430P140 Demo - DMA0, Repeated Block UART1 9600, TACCR2, ACLK
//
//  Description: DMA0 is used to transfer a string byte-by-byte as a repeating
//  block to U1TXBUF. Timer_A operates continuously at 32768Hz with CCR2IFG
//  triggering DMA0. "Hello World" is TX'd via 9600 baud UART1.
//  ACLK= TACLK 32768Hz, MCLK= SMCLK= default DCO ~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//  //* MSP430F169 Device Required *//
//
//               MSP430F169
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32768Hz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|------------> "Hello World"
//           |                 | 9600 - 8N1
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

// Definition necessary to compile with CCEv3.2
// Workaround for &U1TXBUF
#define _U1TXBUF_ 0x007F					// Physical address of U1TXBUF

static char string1[] = { "Hello World\r\n" };

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P3SEL = 0xC0;                             // P3.6,7 = USART1 TXD/RXD
  ME2 |= UTXE1;                             // Enabled UART1 TXD
  UCTL1 |= CHAR;                            // 8-bit char, SWRST=1
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x03;                             // 9600 from 32768Hz
  UBR11 = 0x00;
  UMCTL1 = 0x4A;                            // Modulation
  UCTL1 &= ~SWRST;                          // Enable USART

  DMACTL0 = DMA0TSEL_1;                     // 0-CCR2IFG
  DMA0SA = (unsigned int)string1;           // Source block address
  DMA0DA = (unsigned int)&U1TXBUF;                        // Dest single address
  DMA0SZ = sizeof string1-1;                // Block size
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASBDB + DMAEN;// Rpt, inc src, enable

  TACCR0 = 8192;                            // Char freq = TACLK/CCR0
  TACCR2 = 1;                               // For DMA0 trigger
  TACTL = TASSEL_1 + MC_1;                  // ACLK, up-mode

  __bis_SR_register(LPM3_bits);             // Enter LPM3
}

