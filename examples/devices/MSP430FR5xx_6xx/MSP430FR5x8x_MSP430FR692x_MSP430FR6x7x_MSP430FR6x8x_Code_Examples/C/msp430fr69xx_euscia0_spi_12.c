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
//   MSP430F59xx Demo - eUSCI_A0, SPI 4-Wire Slave Data Echo
//
//   Description: SPI slave talks to SPI master using 4-wire mode. Data received
//   from master is echoed back.
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO ~ 1MHz
//   Note: Ensure slave is powered up before master to prevent delays due to
//   slave init.
//
//
//                   MSP430FR6989
//                 -----------------
//            /|\ |              XIN|-
//             |  |                 | 32KHz Crystal
//             ---|RST          XOUT|-
//                |                 |
//                |             P2.0|-> Data Out (UCA0SOMI)
//                |                 |
//                |             P2.1|<- Data In (UCA0SIMO)
//                |                 |
//                |             P1.5|-> Serial Clock Out (UCA0CLK)
//                |             P1.4|<- Slave Select (UCA0STE)
//
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  // Configure GPIO
  P1SEL1 |= BIT5 | BIT4;                    // Configure slave select and clk
  P2SEL0 |= BIT0 | BIT1;                    // Configure SOMI and MISO
  PJSEL0 |= BIT4 | BIT5;                    // For XT1

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // XT1 Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL1 &= ~DCORSEL;
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure USCI_A0 for SPI operation
  UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**
                                            // 4-pin, 8-bit SPI slave
  UCA0CTLW0 |= UCSYNC | UCCKPL | UCMSB | UCMODE_1 | UCSTEM;
                                            // Clock polarity high, MSB
  UCA0CTLW0 |= UCSSEL__SMCLK;               // ACLK
  UCA0BR0 = 0x02;                           // /2
  UCA0BR1 = 0;                              //
  UCA0MCTLW = 0;                            // No modulation
  UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0, enable interrupts
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(UCA0IFG&UCTXIFG)) { };           // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF;                    // Echo received data
}
