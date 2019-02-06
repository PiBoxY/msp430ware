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
//   MSP430F22x4 Demo - USCI_A0, UART 19200 Echo ISR, HF XTAL SMCLK
//
//   Description: This program will echo a received character, RX interrupt
//   service used. XT1 oscillator used in this example.
//   ACLK = MCLK = SMCLK = BRCLK = LFXT1CLK = 8MHz
//   Baud rate divider with 8MHz XTAL @19200 = 8MHz/19200 = ~416.6
//   //* An external 8MHz XTAL on XIN XOUT is required for LFXT1CLK *//
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 | 8MHz
//          --|RST          XOUT|-
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 19200 - 8N1
//            |     P3.5/UCA0RXD|<------------
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   January 2007
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  BCSCTL1 |= XTS;                           // ACLK = LFXT1 = HF XTAL
  BCSCTL3 |= LFXT1S1;                       // 3 – 16MHz crystal or resonator
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while (IFG1 & OFIFG);                     // OSCFault flag still set?
  BCSCTL2 |= SELM_3 + SELS;                 // MCLK = SMCLK = LFXT1 (safe)

  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 160;                            // 8MHz/19200 = ~416.6
  UCA0BR1 = 1;                              //
  UCA0MCTL = UCBRS2 + UCBRS1;               // Modulation UCBRSx = 6
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
}

//  Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG2 & UCA0TXIFG));              // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}
