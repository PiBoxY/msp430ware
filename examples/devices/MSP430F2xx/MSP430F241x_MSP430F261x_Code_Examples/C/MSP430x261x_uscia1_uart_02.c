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
//   MSP430x26x Demo - USCI_A1, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM3,
//   USCI_A1 RX interrupt triggers TX Echo.
//   ACLK = BRCLK = LFXT1 = 32768Hz, MCLK = SMCLK = DCO ~1.045MHz
//   Baud rate divider with 32768Hz XTAL @2400 -- from User's Guide
//   //* An external watch crystal is required on XIN XOUT for ACLK *//
//
//                MSP430F261x/241x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |     P3.6/UCA1TXD|------------>
//            |                 | 2400 - 8N1
//            |     P3.7/UCA1RXD|<------------
//
//   B. Nisarga
//   Texas Instruments Inc.
//   September 2007
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL = 0xC0;                             // P3.6,7 = USCI_A1 TXD/RXD
  UCA1CTL1 |= UCSSEL_1;                     // CLK = ACLK
  UCA1BR0 = 0x0D;                           // 2400 (see User's Guide)
  UCA1BR1 = 0x00;                           //
  UCA1MCTL = UCBRS_6;                       // Modulation UCBRSx = 3
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UC1IE |= UCA1RXIE;                        // Enable USCI_A1 RX interrupt

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB1RX_VECTOR))) USCI1RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(UC1IFG&UCA1TXIFG));               // USCI_A1 TX buffer ready?
  UCA1TXBUF = UCA1RXBUF;                     // TX -> RXed character
}
