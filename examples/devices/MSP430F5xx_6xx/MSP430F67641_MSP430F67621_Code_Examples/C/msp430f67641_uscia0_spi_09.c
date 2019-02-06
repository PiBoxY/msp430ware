/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//  MSP430F67641 Demo - USCI_A0, SPI 3-Wire Master Incremented Data
//
//  Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//  data is sent by the master starting at 0x01. Received data is expected to
//  be same as the previous transmission.  USCI RX ISR is used to handle
//  communication with the CPU, normally in LPM0. If high, P1.0 indicates
//  valid data reception.  Because all execution after LPM0 is in ISRs,
//  initialization waits for DCO to stabilize against ACLK.
//
//  ACLK = ~32.768kHz, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2
//
//  Use with SPI Slave Data Echo code example.
//
//
//                       MSP430F67641
//                    -----------------
//               /|\ |                 |
//                |  |                 |
//                +->|RST          P1.0|-> LED
//                   |                 |
//                   |             P1.3|-> Data Out (UCA0SIMO)
//                   |                 |
//                   |             P1.2|<- Data In (UCA0SOMI)
//                   |                 |
//                   |             P1.6|-> Serial Clock Out (UCA0CLK)
//
//  E. Chen
//  Texas Instruments Inc.
//  January 2014
//  Built with CCS Version: 5.5.0 and IAR Embedded Workbench Version: 5.52
//******************************************************************************
#include <msp430.h>

unsigned char MST_Data, SLV_Data;

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Setup P1.0 output, P1.2 UCA0SOMI, P1.3 UCA0SIMO, P1.6 UCA0CLK
    P1OUT &= ~BIT0;                               // Clear P1.0
    P1DIR |= BIT0;                                // Set P1.0 to output direction
    P1SEL |= BIT2 | BIT3 | BIT6;                  // Set P1.0,P1.2,P1.3,P1.6 to non-IO

    // Setup eUSCI_A0
    UCA0CTLW0 |= UCSWRST;                         // **Put state machine in reset**
    UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                                  // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL_2;                        // SMCLK
    UCA0BRW_L = 0x02;                             // /2
    UCA0BRW_H = 0;                                //
    UCA0MCTLW = 0;                                // No modulation
    UCA0CTLW0 &= ~UCSWRST;                        // **Initialize USCI state machine**
    UCA0IE = UCRXIE;                              // Enable USCI_A0 RX,TX interrupt

    __delay_cycles(100);                          // Wait for slave to initialize

    MST_Data = 0x01;                              // Initialize data values
    SLV_Data = 0x00;

    while (!(UCA0IFG & UCTXIFG)) ;                // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                         // Transmit first character

    __bis_SR_register(LPM0_bits | GIE);           // CPU off, enable interrupts
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
        case USCI_NONE: break;                    // No interrupt
        case USCI_SPI_UCRXIFG:                    // RXIFG
            while (!(UCA0IFG & UCTXIFG)) ;        // USCI_A0 TX buffer ready?

            if (UCA0RXBUF == SLV_Data)            // Test for correct character RX'd
                P1OUT |= 0x01;                    // If correct, light LED
            else
                P1OUT &= ~0x01;                   // If incorrect, clear LED

            MST_Data++;                           // Increment data
            SLV_Data++;
            UCA0TXBUF = MST_Data;                 // SET BREAKPOINT HERE!

            __delay_cycles(500);
            break;
        case USCI_SPI_UCTXIFG: break;             // TXIFG
        default: break;
    }
}

