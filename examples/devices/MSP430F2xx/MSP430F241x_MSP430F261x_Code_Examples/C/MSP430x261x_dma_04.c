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
//   MSP430x26x Demo - DMA0, single transfer Mode UART1 9600, ACLK
//
//   Description: DMA0 is used to transfer a string as a block to U1TXBUF.
//  UTXIFG1 WILL trigger DMA0. "Hello World" is TX'd via 9600 baud on UART1.
//   Watchdog in interval mode triggers block transfer every 1000ms.
//  Level senstive trigger used for UTXIFG1 to prevent loss of inital edge
//  sensitive triggers - UTXIFG1 which is set at POR.
//   ACLK = UCLK 32768Hz, MCLK = SMCLK = default DCO 1048576Hz
//   Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (0003h)
//
//                MSP430x2618
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32768Hz
//          --|RST          XOUT|-
//            |                 |
//            |     P3.6/UCA1TXD|------------> "Hello World"
//            |                 | 9600 - 8N1
//
//   B. Nisarga
//   Texas Instruments Inc.
//   September 2007
//   Built with CCE Version: 3.2.0
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

const char String1[13] = "\nHello World";

int main(void)
{
  WDTCTL = WDT_ADLY_1000;                   // WDT 1000ms, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P3SEL |= BIT6 + BIT7;                     // P3.6,7 = USART1 TXD/RXD
  //Configure USCIA1, UART
  UCA1CTL1 = UCSSEL_1;                      // ACLK
  /* baud rate = 9600 */
  UCA1BR0 = 03;                             // 32768Hz 9600 32k/9600=3.41
  UCA1BR1 = 0x0;
  UCA1MCTL = UCBRS_3;                       // Modulation UCBRSx = 3
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  
  // Configure DMA0
  DMACTL0 = DMA0TSEL_10;                    // UTXIFG1 trigger
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) String1);
                                            // Source block address
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &UCA1TXBUF);
                                            // Destination single address
  DMA0SZ = sizeof String1-1;                // Block size
  DMA0CTL = DMASRCINCR_3 + DMASBDB + DMALEVEL;  // Repeat, inc src, byte transfer
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupts
  __no_operation();                         // Reqd only for debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR                 // Trigger DMA block transfer
__interrupt void WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{
  DMA0CTL |= DMAEN;                         // Enable
}


