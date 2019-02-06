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
//  MSP430FR231x Demo - eUSCI_B0, SPI 4-Wire Master Incremented Data
//                      eUSCI_B0 port remapped to P2.2-P2.5.
//
//  Description: SPI master talks to SPI slave using 4-wire mode. Incrementing
//  data is sent by the master starting at 0x01. Received data is expected to
//  be same as the previous transmission TXData = RXData-1.
//  The slave select signal is set to active high.
//  USCI RX ISR is used to handle communication with the CPU, normally in LPM0.
//  eUSCI_B0 remap functionality is controlled by the USCIBRMP bit of SYSCFG2
//  register. Set USCIBRMP bit to remap eUSCI_B0 port to P2.2-P2.5.
//  ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~1MHz.  BRCLK = SMCLK/2.
//
//
//                  MSP430FR2311
//                -----------------
//            /|\|                 |
//             | |                 |
//             --|RST              |
//               |                 |
//               |             P2.2|-> Slave Select (UCB0STE)
//               |             P2.3|-> Serial Clock Out (UCB0CLK)
//               |             P2.5|<- Data In (UCB0SOMI)
//               |             P2.4|-> Data Out (UCB0SIMO)
//
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

unsigned char RXData =0;
unsigned char TXData;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

    P2SEL0 |= BIT2 | BIT3 | BIT4 | BIT5;      // set 4-SPI pin as second function
    SYSCFG2 |= USCIBRMP;                      // eUSCI_B0 port remapped to P2.2-P2.5

    UCB0CTLW0 |= UCSWRST;                     // **Put state machine in reset**
                                              // 4-pin, 8-bit SPI master
    UCB0CTLW0 |= UCMST|UCSYNC|UCCKPL|UCMSB|UCMODE_1|UCSTEM;
                                              // Clock polarity high, MSB
    UCB0CTLW0 &= ~UCSSEL;
    UCB0CTLW0 |= UCSSEL__SMCLK;               // Select SMCLK
    UCB0BR0 = 0x02;                           // BRCLK = SMCLK/2
    UCB0BR1 = 0;                              //
    UCB0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
    UCB0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
    TXData = 0x01;                            // Holds TX data
  
    PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
    while(1)
    {
       UCB0IE |= UCTXIE;                      // Enable TX interrupt
    __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0,enable interrupts
    __no_operation();                         // For debug,Remain in LPM0
    __delay_cycles(2000);                     // Delay before next transmission
    TXData++;                                 // Increment transmit data
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV,USCI_SPI_UCTXIFG))
  {
    case USCI_NONE: break;                    // Vector 0 - no interrupt
    case USCI_SPI_UCRXIFG:
           RXData = UCB0RXBUF;
           UCB0IFG &= ~UCRXIFG;
           __bic_SR_register_on_exit(LPM0_bits);// Wake up to setup next TX
           break;
    case USCI_SPI_UCTXIFG:
          UCB0TXBUF = TXData;                // Transmit characters
          UCB0IE &= ~UCTXIE;
          break;
    default: break;
  }
}
