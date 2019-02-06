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
//  MSP430F673x Demo - ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V
//
//  Description: A0 is sampled 16/second (ACLK/1024) with reference to 1.5V.
//  Timer_A is run in upmode and TA0.1 is used to automatically trigger
//  ADC10 conversion. Internal oscillator times sample (16x) and conversion
//  (13x). Inside ADC10_ISR if A0 > 0.5V, P1.0 is set, else reset. Normal mode
//  is LPM3.
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//                MSP430F673x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.2/A0      P1.0|--> LED
//            |             P1.1|--> TA0.1 (ADC trigger signal)
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                // Stop WDT

    // Setup P1.2 A0, P1.1 TA0.TA1, P1.0 output
    P1SEL |= BIT2;                           // Set P1.2 to non-IO
    __disable_interrupt();                   // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                     // Enable access Port Mapping regs
    P1MAP2 = PM_ANALOG;                      // Enable A0
    PMAPKEYID = 0;                           // Disable access Port Mapping regs
    __enable_interrupt();                    // Re-enable all interrupts
    P1DIR |= BIT1;                           // P1.1/TA0.1 output
    P1SEL |= BIT1;                           // TA0.1 function select
    P1DIR |= BIT0;                           // Set P1.0 to output direction
    P1OUT &= ~BIT0;                          // Clear P1.0

    // Setup XT1
    UCSCTL6 &= ~(XT1OFF);                    // XT1 On
    UCSCTL6 |= XCAP_3;                       // Internal load cap
    UCSCTL3 = 0;                             // FLL Reference Clock = XT1
    // Loop until XT1, XT2 & DCO stabilizes
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                   // Clear fault flags
    } while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    UCSCTL6 &= ~(XT1DRIVE_3);                // XT1 stable, reduce drive strength
    UCSCTL4 |= SELA_0;                       // ACLK = LFTX1 (by default)

    // Setup ADC10
    ADC10CTL0 |= ADC10ON | ADC10MSC;         // ADC10ON
    ADC10CTL1 |= ADC10SHS_1 | ADC10CONSEQ_2; // TA0.1 trig smpl strt; rpt sngl ch
    ADC10CTL2 |= ADC10RES;                   // 10-bit conversion results
    ADC10MCTL0 |= ADC10INCH_0 | ADC10SREF_1; // A0 ADC input select; Vref=1.5V

    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while (REFCTL0 & REFGENBUSY) ;           // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 | REFON;            // Select internal ref = 1.5V
                                             // Internal Reference ON
    // Reference settling time ~ 75us
    TA0CCR0 = 80;                            // Delay to allow Ref to settle
    TA0CCTL0 |= CCIE;                        // Compare-mode interrupt
    TA0CTL = TASSEL_2 | MC_1;                // TACLK = SMCLK, Up mode
    __bis_SR_register(CPUOFF | GIE);         // LPM0, TA0_ISR will force exit
    TA0CCTL0 &= ~CCIE;                       // Disable timer Interrupt

    ADC10CTL0 |= ADC10ENC;                   // ADC10 Enable
    ADC10IE |= ADC10IE0;                     // Enable ADC10 interrupt

    // Setup ADC conversion trigger signal - TimerA0.0 (32ms ON-period)
    TA0CCR0 = 1024 - 1;                      // PWM Period
    TA0CCR1 = 512 - 1;                       // TA0.1 ADC trigger
    TA0CCTL1 = OUTMOD_4;                     // TA0CCR0 toggle
    TA0CTL = TASSEL_1 | MC_1 | TACLR;        // ACLK, up mode, clear counter

    __bis_SR_register(LPM3_bits | GIE);      // Enter LPM3 w/ interrupts
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
        case  ADC10IV_NONE: break;           // No interrupt
        case  ADC10IV_ADC10OVIFG: break;     // conversion result overflow
        case  ADC10IV_ADC10TOVIFG: break;    // conversion time overflow
        case  ADC10IV_ADC10HIIFG: break;     // ADC10HI
        case  ADC10IV_ADC10LOIFG: break;     // ADC10LO
        case  ADC10IV_ADC10INIFG: break;     // ADC10IN
        case  ADC10IV_ADC10IFG:              // ADC10
            if (ADC10MEM0 < 0x155)           // ADC10MEM = A0 < 0.5V?
            {
                P1OUT &= ~BIT0;              // Clear P1.0 LED off
            }
            else
            {
                P1OUT |= BIT0;               // Set P1.0 LED on
            }
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
    // Internal Reference settling time delay elapsed
    TA0CTL = 0;                              // Clear Timer_A control registers
    __bic_SR_register_on_exit(LPM0_bits);    // Exit LPM0 on return
}

