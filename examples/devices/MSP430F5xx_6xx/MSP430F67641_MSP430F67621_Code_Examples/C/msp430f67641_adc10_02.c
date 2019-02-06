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
//  MSP430F67641 Demo - ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V
//
//  Description: A single sample is made on A0 with reference to internal
//  1.5V Vref. Software sets ADC10SC to start sample and conversion - ADC10SC
//  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
//  and conversion. In main loop MSP430 waits in LPM0 to save power until ADC10
//  conversion complete, ADC10_ISR will force exit from LPM0 in main loop on
//  return from interrupt. If A0 > 0.5V, P1.0 set, else reset.
//
//                MSP430F67641
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.2/A0      P1.0|-->LED
//
//   E. Chen
//   Texas Instruments Inc.
//   January 2014
//   Built with CCS Version: 5.5.0 and IAR Embedded Workbench Version: 5.52
//******************************************************************************
#include <msp430.h>

unsigned int ADC_Result;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT


    // Setup P1.2 for A0
    P1SEL |= BIT2;                                // Set P1.2 to non-IO
    __disable_interrupt();                        // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                          // Enable access Port Mapping regs
    P1MAP2 = PM_ANALOG;                           // Enable A0
    PMAPKEYID = 0;                                // Disable access Port Mapping regs
    __enable_interrupt();                         // Re-enable all interrupts
    P1DIR |= BIT0;                                // Set P1.0 to output direction
    P1OUT &= ~BIT0;                               // Clear P1.0

    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while (REFCTL0 & REFGENBUSY) ;                // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 | REFON;                 // Select internal ref = 1.5V
                                                  // Internal Reference ON
    // Setup ADC10
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;             // S&H=16 ADC clks, Enable ADC10
    ADC10CTL1 |= ADC10SHP;                        // ADCCLK = MODOSC; sampling timer
    ADC10CTL2 |= ADC10RES;                        // 10-bit conversion results
    ADC10IE |= ADC10IE0;                          // Enable ADC conv cmplete interrupt
    ADC10MCTL0 |= ADC10INCH_0 | ADC10SREF_1;      // A0 ADC input select; Vref=1.5V

    // Configure TA0 to provide delay for reference settling ~75us
    TA0CCR0 = 80;                                 // Delay to allow Ref to settle
    TA0CCTL0 |= CCIE;                             // Compare-mode interrupt.
    TA0CTL = TASSEL_2 | MC_1;                     // TACLK = SMCLK, Up mode.
    __bis_SR_register(LPM0_bits | GIE);           // Enter LPM0 w/ interrupt
    TA0CCTL0 &= ~CCIE;                            // Disable timer interrupt

    while (1)
    {
        __delay_cycles(5000);                     // Delay between conversions

        ADC10CTL0 |= ADC10ENC | ADC10SC;          // Sampling and conversion start

        __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0 w/ interrupt
        __no_operation();                         // For debug only

        if (ADC_Result < 0x155)                   // ADC10MEM = A0 > 0.5V?
        {
            P1OUT &= ~0x01;                       // Clear P1.0 LED off
        }
        else
        {
            P1OUT |= 0x01;                        // Set P1.0 LED on
        }
    }
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
        case  ADC10IV_NONE: break;                // No interrupt
        case  ADC10IV_ADC10OVIFG: break;          // conversion result overflow
        case  ADC10IV_ADC10TOVIFG: break;         // conversion time overflow
        case  ADC10IV_ADC10HIIFG: break;          // ADC10HI
        case  ADC10IV_ADC10LOIFG: break;          // ADC10LO
        case  ADC10IV_ADC10INIFG: break;          // ADC10IN
        case  ADC10IV_ADC10IFG:                   // ADC10
            ADC_Result = ADC10MEM0;               // Store ADC10 channel 0 result
            __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 on return
            break;
        default: break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    TA0CTL = 0;                                   // Reset TA0
    __bic_SR_register_on_exit(LPM0_bits);         // Exit LPM0 on return
}

