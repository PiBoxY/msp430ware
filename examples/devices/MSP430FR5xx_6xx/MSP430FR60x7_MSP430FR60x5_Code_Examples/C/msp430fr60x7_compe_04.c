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
// MSP430FR60xx Demo - COMPE Toggle from LPM4; CompE in ultra low power mode
//                     Vcompare is compared against the Vcc*1/2
//
// Description: Use CompE and shared reference to determine if input 'Vcompare'
//    is high or low.  When Vcompare exceeds Vcc*1/2 COUT goes high and when
//    Vcompare is less than Vcc*1/2 then COUT goes low.
//
//                MSP430FR6047
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST        P1.1/C1|<--Vcompare
//            |                  |
//            |         P2.2/COUT|----> 'high'(Vcompare>Vcc*1/2); 'low'(Vcompare<Vcc*1/2)
//            |                  |
//
//   Cameron P LaFollette
//   Texas Instruments Inc.
//   November 2017
//   Built with IAR Embedded Workbench V7.10 & Code Composer Studio V7.2
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure all un-used GPIO to lowest power state
    PADIR = 0xFFFF;
    PAOUT = 0;

    PBDIR = 0xFFFF;
    PBOUT = 0;

    PCDIR = 0xFFFF;
    PCOUT = 0;

    PDDIR = 0xFFFF;
    PDOUT = 0;

    P9DIR = 0xFF;
    P9OUT = 0;

    PJDIR = 0xFF;
    PJOUT = 0;

    // Configure P1.1 as C1
    P1SEL0 |= BIT1;
    P1SEL1 |= BIT1;

    // Configure P2.2 Comp_E COUT
    P2SEL1 &= ~(BIT2);
    P2SEL0 |= BIT2;
    P2DIR |= BIT2;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Setup Comparator_E
    CECTL0 = CEIPEN | CEIPSEL_1;            // Enable V+, input channel CE01
    CECTL1 = CEMRVS | CEPWRMD_2;            // CMRVL selects the refV - VREF0
                                            // Ultra-low power comparator mode
    CECTL2 = CERS_1 | CERSEL | CEREF04;     // VREF is applied to -terminal
                                            // VCC applied to R-ladder; VREF0 is Vcc*1/2
    CECTL3 = BIT1;                          // Input Buffer Disable @P1.1/CE1
    CECTL1 |= CEON;                         // Turn On Comparator_E

    __delay_cycles(75);                     // delay for the reference to settle

    __bis_SR_register(LPM4_bits);           // Enter LPM4
    __no_operation();                       // For debug
}