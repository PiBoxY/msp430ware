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
//  MSP430F673x Demo - ADC10, Internal Reference toggle
//
//  Description: Sample and convert analog voltage at ADC input channel A1 in
//  single channel single conversion mode. Internal reference is toggled between
//  1.5V and 2.5V and the ADC measurements are made. With 1V input at A1, Vref+
//  of 1.5V and 2.5V gives ADC conversion result of ~682 and ~409 respectively.
//  LED at P1.0 is turned on when ADC conversion result >500 (that is with
//  Vref=1.5V) and turned off otherwise (when Vref=2.5V)
//  ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC
//
//                MSP430F673x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//            |          P1.1/A1|<-- 1V
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P1.1 A1, P1.0 output
    P1SEL |= BIT1;                          // Set P1.1 to non-IO
    __disable_interrupt();                  // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                    // Enable access Port Mapping regs
    P1MAP1 = PM_ANALOG;                     // Enable A0
    PMAPKEYID = 0;                          // Disable access Port Mapping regs
    __enable_interrupt();                   // Re-enable all interrupts
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Clear P1.0

    // Setup ADC10 - Pulse sample mode; ADC10SC trigger
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;       // 16 ADC10CLKs; Enable ADC10
    ADC10CTL1 = ADC10SHP | ADC10CONSEQ_0;   // s/w trig, single ch/conv
    ADC10CTL2 = ADC10RES;                   // 10-bit conversion results
    ADC10MCTL0 = ADC10SREF_1 | ADC10INCH_1; // A1, Vref+

    // Setup internal reference
    while (REFCTL0 & REFGENBUSY) ;          // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 | REFON;           // Select internal ref = 1.5V
                                            // Internal Reference ON
    __delay_cycles(75);                     // Delay (~75us) for Ref to settle

    ADC10CTL0 &= ~ADC10ENC;                 // ADC10 disable

    while (1)
    {
        ADC10CTL0 |= ADC10ENC | ADC10SC;    // Sample and conversion start

        // Toggle internal reference voltage
        while (REFCTL0 & REFGENBUSY) ;      // If ref generator busy, WAIT
        REFCTL0 ^= REFVSEL1;                // Toggle Int. Ref between 1.5V/2.5V
        while (!(ADC10IFG & ADC10IFG0)) ;   // Conversion complete?
        if (ADC10MEM0 > 500)                // ADC10MEM0 = A1 > 500
        {
            P1OUT |= BIT0;                  // Set P1.0; Vref=1.5V used
        }
        else
        {
            P1OUT &= ~BIT0;                 // Clear P1.0; Vref=2.5V used
        }

        __delay_cycles(0x8000);             // Delay between conversions
    }
}

