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
//   MSP430F461x Demo - USART1, 115200 UART Echo ISR, DCO SMCLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USART1 RX interrupt triggers TX Echo.
//   Baud rate divider with 1048576hz = 1048576/115200 = ~9.1 (009h|08h)
//   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//   //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//                MSP430F4619
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |       P4.0/UTXD1|------------>
//            |                 | 115200 - 8N1
//            |       P4.1/URXD1|<------------
//
//   Y. Yang
//   Texas Instruments Inc.
//   January 2013
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************

#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps

  P4SEL |= 0x03;                            // Set P4.1,0 as USART1 TXD/RXD

  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  U1CTL |= CHAR|SWRST;                      // 8-bit character
  ME2 |= UTXE1|URXE1;                       // Enable USART1 TXD/RXD
  U1TCTL |= SSEL1;                          // UCLK= SMCLK
  U1BR0 = 0x09;                             // 1MHz 115200
  U1BR1 = 0x00;                             // 1MHz 115200
  U1MCTL = 0x08;                            // 1MHz 115200 modulation
  U1CTL &= ~SWRST;                          // Initialize USART state machine
  IE2 |= URXIE1;                            // Enable USART1 RX interrupt

  __bis_SR_register(LPM0_bits|GIE);         // Enter LPM0 w/ interrupts enabled
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void USART1_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1RX_VECTOR))) USART1_rx (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  TXBUF1 = RXBUF1;                          // RXBUF1 to TXBUF1
}
