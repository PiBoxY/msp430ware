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
//  MSP-FET430P140 Demo - USART1, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
//
//  Description: Echo a received character, RX ISR used. In the Mainloop UART1
//  is made ready to receive one character with interrupt active. The Mainloop
//  waits in LPM3. The UART1 ISR forces the Mainloop to exit LPM3 after
//  receiving one character which echo's back the received character.
//  ACLK = UCLK1 = LFXT1 = 32768, MCLK = SMCLK = DCO~ 800k
//  Baud rate divider with 32768hz XTAL @2400 = 32768Hz/2400 = 13.65 (000Dh)
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//                MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.6|----------->
//           |                 | 2400 - 8N1
//           |             P3.7|<-----------
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0xC0;                            // P3.6,7 = USART1 option select
  ME2 |= UTXE1 + URXE1;                     // Enable USART1 TXD/RXD
  UCTL1 |= CHAR;                            // 8-bit character
  UTCTL1 |= SSEL0;                          // UCLK = ACLK
  UBR01 = 0x0D;                             // 32k/2400 - 13.65
  UBR11 = 0x00;
  UMCTL1 = 0x6B;                            // Modulation
  UCTL1 &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt

// Mainloop
  for (;;)
  {
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/interrupt
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  TXBUF1 = RXBUF1;                          // RXBUF1 to TXBUF1
  }
}

// UART1 RX ISR will for exit from LPM3 in Mainloop
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1RX_VECTOR))) usart1_rx (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}
