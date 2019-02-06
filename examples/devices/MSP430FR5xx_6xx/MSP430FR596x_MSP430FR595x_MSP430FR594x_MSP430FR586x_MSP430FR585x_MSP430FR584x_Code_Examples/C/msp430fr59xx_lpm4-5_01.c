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
//   MSP430FR59xx Demo - Entering and waking up from LPM4.5 via P1.1 interrupt
//
//   Description: Download and run the program. LED1 (or P4.6) will remain ON if
//                LPM4.5 is correctly entered. Use a button S2 (or P1.1) on the
//                EXP board to wake the device up from LPM4.5. This will enable
//                the LFXT oscillator and blink the LED2 (on P1.0).
//
//                This demo was tested on MSP-EXP430FR5969 LaunchPad.
//
//           MSP430FR5969
//         ---------------
//     /|\|            XIN|-
//      | |               | 32KHz Crystal
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|---> LED2 (MSP-EXP430FR5969)
//        |           P4.6|---> LED1 (MSP-EXP430FR5969)
//        |               |
//        |           P1.1|<--- S2 push-button (MSP-EXP430FR5969)
//
//   William Goh / Andreas Dannenberg
//   Texas Instruments Inc.
//   August 2013
//   Built with IAR Embedded Workbench V5.52.1 & Code Composer Studio V5.5
//******************************************************************************

#include <msp430.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    // Configure GPIO
    P1OUT = 0;                              // Pull-up resistor on P1.1
    P1DIR = 0xFF;                           // Set all but P1.1 to output direction

    P2OUT = 0;
    P2DIR = 0xFF;

    P3OUT = 0;
    P3DIR = 0xFF;

    P4OUT = 0;
    P4DIR = 0xFF;

    PJOUT = BIT4;                           // Set PJ.4 / LFXTIN to high
    PJDIR = 0xFFFF;

    // Determine whether we are coming out of an LPMx.5 or a regular RESET.
    if (SYSRSTIV == SYSRSTIV_LPM5WU) {
        PJSEL0 = BIT4;                      // For XT1

        // Clock System Setup
        CSCTL0_H = CSKEY >> 8;              // Unlock CS registers
        CSCTL1 = DCOFSEL_0;                 // Set DCO to 1MHz
        CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
        CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers
        CSCTL4 &= ~LFXTOFF;

        // Configure LED pin for output
        P1DIR |= BIT0;

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings. The oscillator should now start...
        PM5CTL0 &= ~LOCKLPM5;

        do {
            CSCTL5 &= ~LFXTOFFG;            // Clear XT1 fault flag
            SFRIFG1 &= ~OFIFG;
        } while (SFRIFG1 & OFIFG);          // Test oscillator fault flag
    }
    else {
        // Configure P1.1 Interrupt
        P1OUT |= BIT1;                      // Pull-up resistor on P1.1
        P1REN |= BIT1;                      // Select pull-up mode for P1.1
        P1DIR = 0xFF ^ BIT1;                // Set all but P1.1 to output direction
        P1IES |= BIT1;                      // P1.1 Hi/Lo edge
        P1IFG = 0;                          // Clear all P1 interrupt flags
        P1IE |= BIT1;                       // P1.1 interrupt enabled

        P4OUT |= BIT6;                      // Turn on P4.6 (LED1) on EXP board
                                            // to indicate we are about to enter
                                            // LPM4.5

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        PMMCTL0_H = PMMPW_H;                // Open PMM Registers for write
        PMMCTL0_L |= PMMREGOFF;             // and set PMMREGOFF
        PMMCTL0_H = 0;                      // Lock PMM Registers

        // Enter LPM4 Note that this operation does not return. The LPM4.5
        // will exit through a RESET event, resulting in a re-start
        // of the code.
        __bis_SR_register(LPM4_bits);

        // Should never get here...
        while (1);
    }

    // Now blink the LED in an endless loop.
    while (1) {
        P1OUT ^= BIT0;                      // P1.0 = toggle
        __delay_cycles(100000);
    }
}
