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
//  MSP430i20xx Demo - PMM Module, Compare VMONIN to Internal 1.16V, VMON ISR
//
//  Description: Setup VMON to compare input from VMONIN to internally generated
//  1.16V source. If VMONIN drifts below 1.16V, clear the LED. If VMONIN drifts
//  above 1.16V, turn the LED on.
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST         P2.3|<-- VMONIN
//            |            P1.4|--> LED
//            |                |
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop WDT

    // Setup P1.4 for LED
    P1DIR |=  BIT4;                     // LED Output
    P1OUT &= ~BIT4;                     // LED Off

    VCMONCTL |= VMONLVL_7;              // Compare VMONIN to 1.16V
    VCMONCTL |= VMONIE;                 // Enable VMON interrupt
    // When started up, VMON requires a settling time of 0.5us
    __delay_cycles(10); // MCLK = ~16MHz; 1 cycle = ~0.0625us

    // Setup P2.3 for VMONIN
    P2SEL0 |= BIT3;
    P2SEL1 |= BIT3;

    __bis_SR_register(GIE);             // Enable Interrupts

    while (1) {
        P1OUT |= BIT4;                  // VMONIN > 1.16V, LED On
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=VMON_VECTOR
__interrupt void VMON_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(VMON_VECTOR))) VMON_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT &= ~BIT4;                     // VMONIN < 1.16V, LED Off
}
