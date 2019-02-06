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
//  MSP430F461x Demo - USCI_A0 IrDA Monitor, 4MHz SMCLK
//
//  Description: This example receives bytes through the USCI module
//  configured for IrDA mode, and sends them out as ASCII strings using UART1
//  to a PC running a terminal software. The code can be used to monitor
//  and log an IrDA communication.
//
//  MCLK = SMCLK = DCO = 4MHz, ACLK = 32kHz
//  //* An external 32kHz XTAL on XIN XOUT is required for ACLK *//
//
//                                    MSP430F461x
//                               -----------------------
//                              |                       |
//                           /|\|                    XIN|-
//                            | |                       | 32kHz
//                            --|RST                XOUT|-
//                              |                       |
//    GP2W0116YPS   /|\         |                       |
//      -------      |          |                       |
//     |    Vcc|-----+  IrDA    |                       |
//     #    LED|-----+ 9600 8N1 |                       |
//     #    TxD|<---------------|P2.4/UCA0TXD           |
//     #    RxD|--------------->|P2.5/UCA0RXD      UTXD1|--> 115,200 8N1
//     #     SD|-----+          |                       |    Terminal SW
//     |    GND|-----+          |                       |
//      -------      |           -----------------------
//                  ---
//
//   Y. Yang
//   Texas Instruments Inc.
//   January 2013
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

unsigned char RxByte;

//  Table for nibble-to-ASCII conversion
unsigned const char Nibble2ASCII[] =
{
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'A',
  'B',
  'C',
  'D',
  'E',
  'F'
};

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  P2SEL |= 0x30;                            // Use P2.4/P2.5 for USCI_A0
  P4SEL |= 0x03;                            // P4.0,1 = USART1 TXD/RXD
  SCFQCTL = 121;                            // Set FLL to 3.998MHz
  SCFI0 = FLLD0|FN_2;                       // Adjust range select
  UCA0CTL1 |= UCSWRST;                      // Set USCI_A0 SW Reset
  UCA0CTL1 = UCSSEL_2|UCSWRST;              // Use SMCLK, keep SW reset
  UCA0BR0 = 26;                             // 4Mhz/26=153.8kHz
  UCA0BR1 = 0;
  UCA0MCTL = UCBRF_1|UCOS16;                // Set 1st stage modulator to 1
                                            // 16-times oversampling mode
  UCA0IRTCTL = UCIRTXPL2|UCIRTXPL0|UCIRTXCLK|UCIREN;
                                            // Pulse length = 6 half clock cyc
                                            // Enable BITCLK16, IrDA enc/dec
  UCA0IRRCTL = UCIRRXPL;                    // Light = low pulse
  UCA0CTL1 &= ~UCSWRST;                     // Resume USCI_A0 operation
  U1CTL |= SWRST;                           // Set USART1 SW Reset
  ME2 |= UTXE1|URXE1;                       // Enable USART1 TXD/RXD
  U1CTL = CHAR|SWRST;                       // 8-bit characters, keep SW reset
  U1TCTL = SSEL1;                           // BRCLK = SMCLK
  U1BR0 = 0x22;                             // 4MHz/115,200=34.72
  U1BR1 = 0x00;                             //
  U1MCTL = 0xdd;                            // Modulation
  U1CTL &= ~SWRST;                          // Release USART1 state machine

  while (1)
  {
    __disable_interrupt();
    IE2 |= UCA0RXIE;                        // Enable RX int
    __bis_SR_register(CPUOFF|GIE);          // Enter LPM0 w/ interrupts enabled

    while (!(IFG2 & UTXIFG1));              // Ensure TX buffer is ready
    U1TXBUF = Nibble2ASCII[(RxByte >> 4) & 0x0f];
                                            // TX upper nibble
    while (!(IFG2 & UTXIFG1));              // Ensure TX buffer is ready
    U1TXBUF = Nibble2ASCII[RxByte & 0x0f];  // TX lower nibble
    while (!(IFG2 & UTXIFG1));              // Ensure TX buffer is ready
    U1TXBUF = ' ';                          // TX space character
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
