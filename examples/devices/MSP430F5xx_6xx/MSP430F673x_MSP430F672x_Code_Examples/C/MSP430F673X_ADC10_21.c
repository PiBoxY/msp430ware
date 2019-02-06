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
//  MSP430F673x Demo - ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
//
//  Description; A1 is sampled in every 0.5s using the TA0.0 trigger in single
//  channel/conversion mode. Window comparator is used to generate interrupts to
//  indicate when the input voltage goes above the High_Threshold or below the
//  Low_Threshold or is in-between the high and low thresholds. TimerA2 is used
//  as an interval timer used to control the LED at P1.0 to toggle slow/fast
//  or turn off according to the ADC10 Hi/Lo/IN interrupts.
//  ADC10 is configured in pulse sample mode and is triggered by the TA0.0
//
//               MSP430F673x
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.1/A1     P1.0 |--> LED
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

#define High_Threshold 818
#define Low_Threshold 409

unsigned int SlowToggle_Period = 20000 - 1;
unsigned int FastToggle_Period = 1000 - 1;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                          // Stop WDT

    // Setup P1.0 output, P1.1 A1
    P1SEL |= BIT1;                                     // Set P1.1 to non-IO
    __disable_interrupt();                             // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                               // Enable access Port Mapping regs
    P1MAP1 = PM_ANALOG;                                // Enable A1
    PMAPKEYID = 0;                                     // Disable access Port Mapping regs
    __enable_interrupt();                              // Re-enable all interrupts
    P1DIR |= BIT0;                                     // Set P1.0 to output direction
    P1OUT &= ~BIT0;                                    // Clear P1.0

    // Setup ADC10
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;                  // 16 ADCCLKs, enable ADC
    ADC10CTL1 = ADC10SHP | ADC10SHS_1 | ADC10CONSEQ_2; // Sampling timer
    // TA0.1 trigger; Repeat single ch
    ADC10CTL2 = ADC10RES;                              // 10-bit conversion results
    ADC10MCTL0 = ADC10INCH_1 | ADC10SREF_1;            // Vref+, A1
    ADC10HI = High_Threshold;                          // Window Comparator Hi-threshold
    ADC10LO = Low_Threshold;                           // Window Comparator Lo-threshold

    // Setup internal reference
    while (REFCTL0 & REFGENBUSY) ;                     // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_2 | REFON;                      // Select internal ref = 2.5V
                                                       // Internal Reference ON
    __delay_cycles(75);                                // Delay (~75us) for Ref to settle

    // Setup TA2 period-timer
    TA2CCTL0 = CCIE;                                   // CCR0 interrupt enabled
    TA2CTL = TASSEL_1 | TACLR;                         // ACLK, clear counter

    // Setup ADC10 timer trigger TA0.1
    TA0CCR0 = 16384 - 1;                               // PWM Period
    TA0CCR1 = 8192 - 1;                                // Dutycycle TA0.1
    TA0CCTL1 = OUTMOD_4;                               // TA0CCR1 toggle
    TA0CTL = TASSEL_1 | MC_1 | TACLR;                  // ACLK, up mode, clear counter

    ADC10IE |= ADC10HIIE | ADC10LOIE | ADC10INIE;      // Interrupt enable
    ADC10CTL0 |= ADC10ENC;                             // Enable conversion
    __bis_SR_register(LPM3_bits | GIE);                // Enter LPM3 w/ interrupts
    __no_operation();                                  // for debugger
}

// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(ADC10IV, 12))
    {
        case  ADC10IV_NONE: break;                     // No interrupt
        case  ADC10IV_ADC10OVIFG: break;               // conversion result overflow
        case  ADC10IV_ADC10TOVIFG: break;              // conversion time overflow
        case  ADC10IV_ADC10HIIFG:                      // ADC10HI
            ADC10IFG &= ~ADC10HIIFG;                   // Clear interrupt flag
            TA2CTL &= ~MC_1;                           // Turn off Timer
            TA2CCR0 = FastToggle_Period;               // Set Timer Period for fast toggle
            TA2CTL |= MC_1;
            break;
        case  ADC10IV_ADC10LOIFG:                      // ADC10LO
            ADC10IFG &= ~ADC10LOIFG;                   // Clear interrupt flag
            TA2CTL &= ~MC_1;                           // Turn off Timer
            P1OUT &= ~BIT0;
            break;
        case  ADC10IV_ADC10INIFG:                      // ADC10IN
            ADC10IFG &= ~ADC10INIFG;                   // Clear interrupt flag
            TA2CTL &= ~MC_1;                           // Turn off Timer
            TA2CCR0 = SlowToggle_Period;               // Set Timer Period for slow toggle
            TA2CTL |= MC_1;
            break;
        case  ADC10IV_ADC10IFG:                        // ADC10
        default: break;
    }
}

// Timer2 A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) TIMER2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT ^= BIT0;                                     // Toggle LED P1.0
}

