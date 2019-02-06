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
//   MSP430x471xx Demo - USCI_A0 IrDA External Loopback Test, 4MHz SMCLK
//
//  Description: This example transmits bytes through the USCI module
//  configured for IrDA mode, and receives them using an external loopback
//  connection. The transfered sequence is 00h, 01h, 02h, ..., ffh. The
//  received bytes are also stored in memory starting at address RxData.
//  In the case of an RX error the LED is lighted and program execution stops.
//  An external loopback connection has been used as it allows for the
//  connection of a scope to monitor the communication, which is not possible
//  when using the internal loopback.
//
//  MCLK = SMCLK = DCO = 4MHz, ACLK = 32kHz
//  //* An external 32kHz XTAL on XIN XOUT is required for ACLK *//
//
//               MSP430x471xx
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |          UCA0RXD|--+   external
//           |          UCA0TXD|--+   loopback connection
//           |                 |
//           |                 |
//           |             P5.1|--->  LED
//           |                 |
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

unsigned char RxByte;
volatile unsigned char RxData[256];
unsigned char TxByte;
volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P2SEL |= BIT4+BIT5;                       // Use P2.4/P2.5 for USCI_A0
  P5OUT &= ~BIT1;                           // Clear P5.1
  P5DIR |= BIT1;                            // P5.1 output
  SCFQCTL = 121;                            // Set FLL to 3.998MHz
  SCFI0 = FLLD0 + FN_2;                     // Adjust range select
  UCA0CTL1 |= UCSWRST;                      // Set USCI_A0 SW Reset
  UCA0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCA0BR0 = 26;                             // 4Mhz/26=153.8kHz
  UCA0BR1 = 0;
  UCA0MCTL = UCBRF_1 + UCOS16;              // Set 1st stage modulator to 1
                                            // 16-times oversampling mode
  UCA0IRTCTL = UCIRTXPL2 + UCIRTXPL0 + UCIRTXCLK + UCIREN;
                                            // Pulse length = 6 half clock cyc
                                            // Enable BITCLK16, IrDA enc/dec
  UCA0CTL1 &= ~UCSWRST;                     // Resume USCI_A0 operation
  IE2 |= UCA0RXIE;                          // Enable RX int

  TxByte = 0x00;                            // TX data and pointer, 8-bit

  while (1)
  {
    for (i = 1000; i; i--);                 // Small delay
    while (!(IFG2 & UCA0TXIFG));            // USCI_A0 TX buffer ready?
    UCA0TXBUF = TxByte;                     // TX character

    __disable_interrupt();
    IE2 |= UCA0RXIE;                        // Enable RX int
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

    RxData[TxByte] = RxByte;                // Store RXed character in RAM
    if (TxByte != RxByte)                   // RX OK?
    {
      P5OUT |= BIT1;                        // LED P5.1 on
      while (1);                            // Trap PC here
    }
    TxByte++;                               // Next character to TX
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  RxByte = UCA0RXBUF;                       // Get RXed character
  IE2 &= ~UCA0RXIE;                         // Disable RX int
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
}
