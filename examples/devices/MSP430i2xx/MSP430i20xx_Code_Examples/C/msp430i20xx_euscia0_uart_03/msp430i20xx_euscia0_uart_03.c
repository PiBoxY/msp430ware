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
//  MSP430i20xx Demo - eUSCI_A, 115200 UART Echo ISR, DCO SMCLK
//
//  Description:  Echo a received character, RX ISR used. Normal mode is LPM0.
//  eUSCI_A RX interrupt triggers TX Echo.
//  Baud rate divider with 16.384Mhz = 16.384MHz/115200 = ~142 (0x8E)
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST             |
//            |    P1.3/UCA0TXD|--->
//            |    P1.2/UCA0RXD|<---
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    P1SEL0 |=   BIT2 | BIT3;            // P1.2/3 eUSCI_A Function
    P1SEL1 &= ~(BIT2 | BIT3);

    UCA0CTL1 |= UCSWRST;                // Hold eUSCI in reset
    UCA0CTL1 |= UCSSEL_2;               // SMCLK
    UCA0BR0   = 142;                    // 115200 baud
    UCA0BR1   = 0;
    UCA0MCTLW = 0x2200;                 // 16.384MHz/115200 = 142.22 (See UG)
    UCA0CTL1 &= ~UCSWRST;               // Release from reset
    UCA0IE   |= UCRXIE;                 // Enable RX interrupt

    __bis_SR_register(LPM0_bits | GIE); // Enter LPM0
    __no_operation();                   // For debugger
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG)) {
        case USCI_NONE: break;
        case USCI_UART_UCRXIFG:
                 while (!(UCA0IFG&UCTXIFG)); // USCI_A0 TX buffer ready?
                 UCA0TXBUF = UCA0RXBUF;      // TX -> RXed character
                 break;
        case USCI_UART_UCTXIFG: break;
        case USCI_UART_UCSTTIFG: break;
        case USCI_UART_UCTXCPTIFG: break;
        default: break;
    }
}
