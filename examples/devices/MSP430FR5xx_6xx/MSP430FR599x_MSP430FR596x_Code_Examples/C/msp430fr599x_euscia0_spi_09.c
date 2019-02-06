/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//   MSP430FR5x9x Demo - eUSCI_B1, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission TXData = RXData-1.
//   USCI RX ISR is used to handle communication with the CPU, normally in LPM0.
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO ~1MHz.  BRCLK = ACLK/2
//
//
//                   MSP430FR5994
//                 -----------------
//            /|\ |              XIN|-
//             |  |                 |  32KHz Crystal
//             ---|RST          XOUT|-
//                |                 |
//                |             P5.0|-> Data Out (UCB1SIMO)
//                |                 |
//                |             P5.1|<- Data In (UCB1SOMI)
//                |                 |
//                |             P5.2|-> Serial Clock Out (UCB1CLK)
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

volatile unsigned char RXData = 0;
volatile unsigned char TXData;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    // Configure GPIO
    P5SEL1 &= ~(BIT0 | BIT1 | BIT2);        // USCI_B1 SCLK, MOSI, and MISO pin
    P5SEL0 |= (BIT0 | BIT1 | BIT2);
    PJSEL0 |= BIT4 | BIT5;                  // For XT1

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // XT1 Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set all dividers
    CSCTL4 &= ~LFXTOFF;
    do
    {
        CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Configure USCI_B1 for SPI operation
    UCB1CTLW0 = UCSWRST;                    // **Put state machine in reset**
    UCB1CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                            // Clock polarity high, MSB
    UCB1CTLW0 |= UCSSEL__ACLK;              // ACLK
    UCB1BRW = 0x02;                         // /2
    //UCB1MCTLW = 0;                          // No modulation
    UCB1CTLW0 &= ~UCSWRST;                  // **Initialize USCI state machine**
    UCB1IE |= UCRXIE;                       // Enable USCI_B1 RX interrupt
    TXData = 0x1;                           // Holds TX data

    while(1)
    {
        UCB1IE |= UCTXIE;
        __bis_SR_register(LPM0_bits | GIE); // CPU off, enable interrupts
        __delay_cycles(2000);               // Delay before next transmission
        TXData++;                           // Increment transmit data
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EUSCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_B1_VECTOR))) USCI_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
    {
        case USCI_NONE: break;
        case USCI_SPI_UCRXIFG:
            RXData = UCB1RXBUF;
            UCB1IFG &= ~UCRXIFG;
            __bic_SR_register_on_exit(LPM0_bits); // Wake up to setup next TX
            break;
        case USCI_SPI_UCTXIFG:
            UCB1TXBUF = TXData;                   // Transmit characters
            UCB1IE &= ~UCTXIE;
            break;
        default: break;
    }
}
