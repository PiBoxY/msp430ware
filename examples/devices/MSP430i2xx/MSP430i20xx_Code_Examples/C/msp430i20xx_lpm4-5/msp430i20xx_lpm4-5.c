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
//  MSP430i20xx Demo - Entering and waking up from LPM3.5 via P1.1 interrupt
//
//  Description: Download and run the program. The LED will remain ON if LPM4.5
//  is correctly entered. Pull P2.1 high to wake the device up from LPM4.5.
//  This will blink the LED.
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO/16 = 1.024MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST         P1.4|--> LED
//            |            P2.1|--> Wakeup
//            |                |
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1OUT = 0x00;
    P1DIR = 0xFF;
    P2OUT = 0x00;
    P2DIR = 0xFF;

    // Determine whether we are coming out of an LPMx.5 or a regular RESET.
    if (LPM45CTL&LPM45IFG) {
        // Configure SMCLK = MCLK = ~1MHz
        CSCTL1 |=  DIVM__16 | DIVS__16;     // MCLK = DCO/16, SMCLK = DCO/16

        // Configure LED pin for output
        P1DIR |= BIT4;

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings.
        LPM45CTL &= ~LOCKLPM45;
    }
    else {
        // Configure P2.1 Interrupt
        P2OUT &= ~BIT1;                     // Pull-down P2.1
        P2DIR = 0xFF ^ BIT1;                // Set all but P2.1 to output direction
        P2IES &= ~BIT1;                     // P2.1 Lo/Hi edge
        P2IFG = 0;                          // Clear all P2 interrupt flags
        P2IE |= BIT1;                       // P2.1 interrupt enabled

        P1OUT |= BIT4;                      // Turn on LED to indicate we are about
                                            // to enter LPM4.5

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings and set REGOFF
        LPM45CTL &= ~LOCKLPM45;
        LPM45CTL |= PMMREGOFF;

        // Enter LPM4 Note that this operation does not return. The LPM4.5
        // will exit through a RESET event, resulting in a re-start
        // of the code.
        __bis_SR_register(LPM4_bits);

        // Should never get here...
        while (1);
    }

    // Now blink the LED in an endless loop.
    while (1) {
        P1OUT ^= BIT4;                      // LED = toggle
        __delay_cycles(100000);
    }
}
