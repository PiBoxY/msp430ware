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
//  MSP430F673x Demo - USCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
//
//  Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//  USCI_A0 RX interrupt triggers TX Echo.
//  If UCOS16=1, UCBRx=Fbrclk/(16*Baudrate)
//  Baud rate divider with UCBRx = 1MHz/(16*9600) = ~6.8
//  ACLK = REFO = ~32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  See User Guide for baud rate divider table
//
//
//                MSP430F673x
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |     P1.3/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.2/UCA0RXD|<------------
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P1.2 UCA0RXD, P1.3 UCA0TXD
    P1SEL |= BIT2 | BIT3;                   // Set P1.2, P1.3 to non-IO
    P1DIR |= BIT2 | BIT3;                   // Enable UCA0RXD, UCA0TXD

    // Setup LFXT1
    UCSCTL6 &= ~(XT1OFF);                   // XT1 On
    UCSCTL6 |= XCAP_3;                      // Internal load cap
    // Loop until XT1 fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                  // Clear fault flags
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

    // Setup eUSCI_A0
    UCA0CTLW0 |= UCSWRST;                   // **Put state machine in reset**
    UCA0CTLW0 |= UCSSEL_2;                  // SMCLK
    UCA0BRW_L = 6;                          // 1MHz 9600 (see User's Guide)
    UCA0BRW_H = 0;                          // 1MHz 9600
    UCA0MCTLW = UCBRF_13 | UCOS16;          // Modln UCBRSx=0, UCBRFx=0x13,
                                            // over sampling
    UCA0CTLW0 &= ~UCSWRST;                  // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                       // Enable USCI_A0 RX interrupt

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0, interrupts enabled
    __no_operation();                       // For debugger
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
        case USCI_UART_UCRXIFG:             // RXIFG
            while (!(UCA0IFG & UCTXIFG)) ;  // USCI_A0 TX buffer ready?
            UCA0TXBUF = UCA0RXBUF;          // TX -> RXed character
            break;
        case USCI_UART_UCTXIFG: break;      // TXIFG
        case USCI_UART_UCSTTIFG: break;     // TTIFG
        case USCI_UART_UCTXCPTIFG: break;   // TXCPTIFG
        default: break;
    }
}

