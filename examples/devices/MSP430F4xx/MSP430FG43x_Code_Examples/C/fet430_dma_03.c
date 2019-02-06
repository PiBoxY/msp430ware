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
//  MSP-FET430P430 Demo - DMA0, Repeat Single to UART 19200, TACCR2, DCO
//
//  Description: DMA0 is used to transfer the string "Hello World"
//  byte-by-byte to the USART0 TXBUF. Timer_A operates in continuous mode from
//  SMCLK/4 and TACCR2_CCIFG is used to trigger DMA0. After each complete
//  string transfer, source and DMA size are reset to inital setting
//  automatically using DMA transfer mode 4.
//  Baud rate divider with 1048576Hz = 1048576Hz/19200 = ~54.61 (036h|6Bh)
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//               MSP430FG439
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |       P2.4/UTXD0|------------> "Hello World"
//            |                 | 19200 - 8N1
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************
#include <msp430.h>

static char string1[] = { "Hello World\r\n" };

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  FLL_CTL0 |= XCAP18PF;                     // Set load capacitance for xtal
  P2SEL = 0x10;                             // P2.4 = USART0 TXD

  ME1 |= UTXE0;                             // Enabled USART0 TXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST=1
  UTCTL0 |= SSEL1;                          // UCLK = SMCLK
  UBR00 = 0x36;                             // 19200 from ~1Mhz
  UBR10 = 0x00;                             //
  UMCTL0 = 0x6B;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine

  DMA0SA = (int)string1;                    // Source string address
  DMA0DA = (unsigned int)&U0TXBUF;          // Destination single address
  DMA0SZ = sizeof string1 - 1;              // Block size (discard trailing 0)
  DMACTL0 = DMA0TSEL_1;                     // TACCR2 trigger
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASBDB + DMAEN; // Rpt, inc src
  TACTL = TASSEL_2 + ID_2 + MC_2;           // SMCLK/4, continuous mode

  __bis_SR_register(CPUOFF);                // Enter LPM0
}
