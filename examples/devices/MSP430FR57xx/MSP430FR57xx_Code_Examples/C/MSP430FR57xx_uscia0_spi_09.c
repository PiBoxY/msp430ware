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
//   MSP430F54x Demo - USCI_A0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission TXData = RXData-1.  
//   USCI RX ISR is used to handle communication with the CPU, normally in LPM0. 
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~1MHz.  BRCLK = ACLK/2
//
//
//                   MSP430FR5739
//                 -----------------
//             /|\|                 |
//              | |                 |
//              --|RST              |
//                |                 |
//                |             P2.0|-> Data Out (UCA0SIMO)
//                |                 |
//                |             P2.1|<- Data In (UCA0SOMI)
//                |                 |
//                |             P1.5|-> Serial Clock Out (UCA0CLK)
//
//
//   P. Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with CCS V4 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>

unsigned char RXData =0;
unsigned char TXData;

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P1DIR |= BIT0;
  P1OUT &= ~BIT0;

  PJSEL0 |= BIT4+BIT5;
  
  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers 
  CSCTL4 |= XT1DRIVE_0; 
  CSCTL4 &= ~XT1OFF;
  
  do
  {
    CSCTL5 &= ~XT1OFFG;
                                            // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG; 
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  P1SEL1 |= BIT5; 
  P2SEL1 |= BIT0 + BIT1;
  UCA0CTLW0 |= UCSWRST;                     // **Put state machine in reset**
  UCA0CTLW0 |= UCMST+UCSYNC+UCCKPL+UCMSB;   // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
  UCA0CTLW0 |= UCSSEL_1;                    // ACLK
  UCA0BR0 = 0x02;                           // /2
  UCA0BR1 = 0;                              //
  UCA0MCTLW = 0;                            // No modulation
  UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
  TXData = 0x1;                             // Holds TX data

  while(1)
  {
     
    UCA0IE |= UCTXIE;
    __bis_SR_register(LPM0_bits + GIE);      // CPU off, enable interrupts
    __no_operation();                       // Remain in LPM0 
    __delay_cycles(2000);                   // Delay before next transmission
    TXData++;                               // Increment transmit data
  }

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
  volatile unsigned int i;

  switch(__even_in_range(UCA0IV,0x04))
  {
    case 0: break;                          // Vector 0 - no interrupt
    case 2: 
           RXData = UCA0RXBUF; 
           UCA0IFG &= ~UCRXIFG;
           __bic_SR_register_on_exit(CPUOFF);// Wake up to setup next TX
           break;
    case 4: 
          UCA0TXBUF = TXData;             // Transmit characters
          UCA0IE &= ~UCTXIE; 
          break;
    default: break;
  }
}
