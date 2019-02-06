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
//  MSP-FET430P140 Demo - DMA1/0, Blk Mode UART1 9600 Auto RX/TX String, ACLK
//
//  Description: DMA1 is used to capture a 16 byte string from USART1. The
//  DMA1 interrupt is used to trigger DMA0 to echo back the received string.
//  USART1 is configured at 96008N1.
//  Level senstive trigger used for UTXIFG1 to prevent loss of initial edge
//  sensitive triggers - UTXIFG1 which is set at POR.
//  ACLK = 32768Hz = UCLK1, MCLK = SMCLK = default DCO ~ 800k
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//  //* Final Production MSP430F16x(x) Device Required *//
//
//               MSP430F169
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32768Hz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|------------>
//           |                 | 9600 - 8N1
//           |             P3.7|<------------
//
//  M. Buccini / A. Dannenberg
//  Texas Instruments Inc.
//  November 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P3SEL = 0xC0;                             // P3.6,7 = USART1 TXD/RXD
  ME2 |= URXE1 + UTXE1;                     // Enabled UART1 RXD/TXD
  UCTL1 |= CHAR;                            // 8-bit char, SWRST=1
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x03;                             // 9600 from 32768Hz
  UBR11 = 0x00;
  UMCTL1 = 0x4A;                            // Modulation
  UCTL1 &= ~SWRST;                          // Enable USART

  DMACTL0 = DMA1TSEL_9 + DMA0TSEL_10;       // DMA1=URXIFG1, DMA0=UTXIFG1

// Playback
  DMA0SA = 0x220;                           // Src = RAM memory
  DMA0DA = (unsigned int)&U1TXBUF;          // Dst = UART TX Buffer
  DMA0SZ = 0x10;                            // Block size
  DMA0CTL = DMASRCINCR_3 + DMASBDB + DMALEVEL; // inc src, enable

// Record
  DMA1SA = (unsigned int)&U1RXBUF;          // Src address = UART RX Buffer
  DMA1DA = 0x220;                           // Dst address = RAM memory
  DMA1SZ = 0x10;                            // Size in bytes
  DMA1CTL = DMADSTINCR_3 + DMASBDB + DMAIE + DMAEN; // Sng, config

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}

// DMA interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DACDMA_VECTOR
__interrupt void DACDMA_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DACDMA_VECTOR))) DACDMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
  DMA1CTL &= ~DMAIFG;                       // Clear DMA1 interrupt flag
  DMA0CTL |= DMAEN;                         // Enable
  DMA1CTL |= DMAEN;                         // (re)Enable
}
