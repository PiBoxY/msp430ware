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
//   MSP430F67791 Demo - USCI_A0, SPI 3-Wire Slave Data Echo
//
//   Description: SPI slave talks to SPI master using 3-wire mode. Data received
//   from master is echoed back.  USCI RX ISR is used to handle communication,
//   CPU normally in LPM0.
//
//   ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz
//
//   Use with SPI Master Incremented Data code example.
//
//
//                       MSP430F67791
//                    -----------------
//               /|\ |                 |
//                |  |                 |
//                +->|RST              |
//                   |                 |
//                   |             P3.1|-> Data Out (UCA0SIMO)
//                   |                 |
//                   |             P3.0|<- Data In (UCA0SOMI)
//                   |                 |
//                   |             P3.2|-> Serial Clock Out (UCA0CLK)
//
//   C. Fu
//   Texas Instruments Inc.
//   October 2012
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.51
//******************************************************************************
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P3.0 UCA0SOMI, P3.1 UCA0SIMO, P3.2 UCA0CLK
    P3SEL0 |= BIT0 | BIT1 | BIT2;                  // Set P3.0,P3.1,P3.2 to non-IO

    // Setup eUSCI_A0
    UCA0CTLW0 |= UCSWRST;                   // **Put state machine in reset**
    UCA0CTLW0 |= UCSYNC | UCCKPL | UCMSB;   // 3-pin, 8-bit SPI slave,
                                            // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL_2;                  // SMCLK
    UCA0BRW_L = 0x02;                       // /2
    UCA0BRW_H = 0;                          //
    UCA0MCTLW = 0;                          // No modulation
    UCA0CTLW0 &= ~UCSWRST;                  // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                       // Enable RX interrupt

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0, enable interrupts
}

// USCI_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCA0IV, 4))
    {
        case USCI_NONE: break;              // No interrupt
        case USCI_SPI_UCRXIFG:              // RXIFG
            while (!(UCA0IFG & UCTXIFG)) ;  // USCI_A0 TX buffer ready?
            UCA0TXBUF = UCA0RXBUF;
            break;
        case USCI_SPI_UCTXIFG: break;       // TXIFG
        default: break;
    }
}

