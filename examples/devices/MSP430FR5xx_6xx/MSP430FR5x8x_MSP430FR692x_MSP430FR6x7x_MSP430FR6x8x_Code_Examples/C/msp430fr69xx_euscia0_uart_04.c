/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//  MSP430FR69xx Demo - eUSCI_A0 External Loopback test
//
//  Description: This demo connects TX to RX of the MSP430 UART
//  The example code shows proper initialization of registers
//  and interrupts to receive and transmit data. If data is incorrect P1.0 LED is
//  turned ON.
//  ACLK = 32768Hz, MCLK = SMCLK = BRCLK = default DCO = ~1MHz
//
//
//               MSP430FR6989
//             -----------------
//            |              XIN|-
//            |                 |  32KHz Crystal
//            |             XOUT|-
//         /|\|                 |
//          | |                 |
//          --|RTS  P2.0/UCA0TXD|----|
//            |                 |    |
//            |                 |    |
//            |     P2.1/UCA0RXD|----|
//            |                 |
//            |             P1.0|---> LED
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

volatile unsigned char RXData = 0;
volatile unsigned char TXData = 1;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // stop watchdog

  // Configure GPIO
  P1OUT &= ~BIT0;                           // Clear P1.0 output latch
  P1DIR |= BIT0;                            // For LED on P1.0
  P2SEL0 |= BIT0 | BIT1;                    // Configure UART pins
  P2SEL1 &= ~(BIT0 | BIT1);
  PJSEL0 |= BIT4 | BIT5;                    // Configure XT1 pins

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // XT1 Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure USCI_A0 for UART mode
  UCA0CTL1 |= UCSWRST;
  UCA0CTL1 = UCSSEL__ACLK;                  // Set ACLK = 32768 as UCBRCLK
  UCA0BR0 = 3;                              // 9600 baud
  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
                                            // UCBRSx value = 0x53 (See UG)
  UCA0BR1 = 0;
  UCA0CTL1 &= ~UCSWRST;                     // release from reset
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  while (1)
  {
    while(!(UCA0IFG & UCTXIFG));
    UCA0TXBUF = TXData;                     // Load data onto buffer

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0, interrupts enabled
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_A0_VECTOR             // eUSCI ISR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  UCA0IFG &= ~UCRXIFG;                      // Clear interrupt
  RXData = UCA0RXBUF;                       // Clear buffer
  if(RXData != TXData)                      // Check value
  {
    P1OUT |= BIT0;                          // If incorrect turn on P1.0
      while(1);                             // Trap CPU
  }
  TXData++;                                 // Increment TX data
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0 on reti
}
