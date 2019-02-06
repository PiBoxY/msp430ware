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
//  MSP-FET430x140 Demo - USART0, Ultra-Low Pwr UART 9600 RX/TX, 32kHz ACLK
//
//  Description: This program demonstrates a full-duplex 9600-baud UART using
//  USART0 and a 32kHz crystal.  The program will wait in LPM3, and receive
//  a string[8] and echo back the complete string.
//  ACLK = LFXT1 = UCLK0, MCLK = SMCLK = default DCO ~ 800KHz
//  Baud rate divider with 32768hz XTAL @9600 = 32768Hz/9600 = 3.41 (000Dh 4Ah )
//  //* An external watch crystal is required on XIN XOUT for ACLK *//	
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |             P3.4|----------->
//           |                 | 9600 - 8N1
//           |             P3.5|<-----------
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

static char string1[8];

char i;
char j = 0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P3SEL = 0x30;                             // P3.3,4 = USART0 TXD/RXD

  ME1 |= UTXE0 + URXE0;                     // Enabled USART0 TXD/RXD
  UCTL0 |= CHAR;                            // 8-bit character, SWRST=1
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x03;                             // 9600 from 1Mhz
  UBR10 = 0x00;                             //
  UMCTL0 = 0x4A;                            // Modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0 + UTXIE0;                   // Enable USART0 RX/TX interrupt
  IFG1 &= ~UTXIFG0;                         // Clear inital flag on POR

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}

// UART0 TX ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0TX_VECTOR
__interrupt void usart0_tx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0TX_VECTOR))) usart0_tx (void)
#else
#error Compiler not supported!
#endif
{
  if (i < sizeof string1)
    TXBUF0 = string1[i++];
}

// UART0 RX ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void usart0_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) usart0_rx (void)
#else
#error Compiler not supported!
#endif
{
  string1[j++] = RXBUF0;
  if (j > sizeof string1-1)
  {
    i = 0;
    j = 0;
    TXBUF0 = string1[i++];
  }
}
