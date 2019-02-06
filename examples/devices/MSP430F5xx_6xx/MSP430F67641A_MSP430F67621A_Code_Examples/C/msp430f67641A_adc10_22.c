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
//  MSP430F67641A Demo - ADC10, Sample A3, AVcc Ref, SD24_B Trig, Set P1.0 if
//                     A0 > 0.5*AVcc
//
//  Description: A single sample is made on A3 with reference to AVcc.
//  SD24_B triggers ADC10SC to start sample and conversion - ADC10SC
//  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
//  and conversion. In main loop MSP430 waits in LPM0 to save power until SD24_B
//  conversion complete, SD24_B_ISR will force exit from LPM0 in main loop on
//  the return from the interrupt. If A0 > 0.5*AVcc, P1.0 set, else reset. Both
//  SD24_B and ADC10 results are stored in ADC_Result and SD24_B_Result
//  respectively.
//
//               MSP430F67641A
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//    Vin+>---|SD2P0            |
//    Vin->---|SD2N0            |
//     Vin>---|P9.3/A3      P1.0|-->LED
//
//   E. Chen
//   Texas Instruments Inc.
//   January 2015
//   Built with CCS Version: 5.5.0 and IAR Embedded Workbench Version: 5.52
//******************************************************************************
#include <msp430.h>

unsigned int ADC_Result;
unsigned int SD24_B_Result;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P9.3 for A3, P1.0 output
    P9SEL |= BIT3;                          // Enable A3
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Clear P1.0

    // Setup ADC10
    ADC10CTL0 |= ADC10SHT_2 | ADC10ON;      // S&H=16 ADC clks, Enable ADC10
    ADC10CTL1 |= ADC10SHS_3                 // SD24_B trigger pulse
        | ADC10SHP                          // Sampling timer
        | ADC10CONSEQ_2;                    // Repeated single channel conv.
    ADC10MCTL0 |= ADC10INCH_3;              // A3 ADC input select; Vref=AVCC
    ADC10IE |= ADC10IE0;                    // Enable ADC conv cmplete interrupt
    ADC10CTL0 |= ADC10ENC;                  // Enable ADC conversion

    // Setup
    SD24BCTL0 = SD24REFS                    // Select internal REF
        | SD24SSEL_0;                       // Select MCLK as SD24_B clock
    SD24BTRGCTL = SD24TRGIE | SD24SNGL      // Enable interrupt, single conv.
        | SD24SCS_4;                        // SD24GRP0SC start conversion
    SD24BCCTL2 |= SD24SNGL | SD24SCS_4;     // Single conversion off SD24GRP0SC

    SD24BINCTL2 |= SD24INTDLY0;             // Interrupt on 3rd sample
    SD24BIE |= SD24IE2;                     // Enable channel 2 interrupt

    __delay_cycles(0x3600);                 // Delay for 1.5V REF startup

    while (1)
    {
        __delay_cycles(5000);

        SD24BCTL1 |= SD24GRP0SC;            // Set bit to start conversion
        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts
        SD24BCTL1 &= ~SD24GRP0SC;           // Clear bit for next conversion
        __no_operation();                   // SET BREAKPOINT HERE

        if (ADC_Result < 0x1FF)
        {
            P1OUT &= ~BIT0;                 // Clear P1.0 LED off
        }
        else
        {
            P1OUT |= BIT0;                  // Set P1.0 LED on
        }
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24B_VECTOR
__interrupt void SD24BISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24B_VECTOR))) SD24BISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (SD24BIV)
    {
        case SD24BIV_SD24OVIFG:             // SD24MEM Overflow
            break;
        case SD24BIV_SD24TRGIFG:            // SD24 Trigger IFG
            SD24BTRGCTL &= ~SD24TRGIFG;     // Clear interrupt flag
            break;
        case SD24BIV_SD24IFG0:              // SD24MEM0 IFG
            break;
        case SD24BIV_SD24IFG1:              // SD24MEM1 IFG
            break;
        case SD24BIV_SD24IFG2:              // SD24MEM2 IFG
            SD24_B_Result = SD24BMEMH2;     // Save CH2 results (clears IFG)
            break;
    }

    __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
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
        case  ADC10IV_NONE: break;          // No interrupt
        case  ADC10IV_ADC10OVIFG: break;    // conversion result overflow
        case  ADC10IV_ADC10TOVIFG: break;   // conversion time overflow
        case  ADC10IV_ADC10HIIFG: break;    // ADC10HI
        case  ADC10IV_ADC10LOIFG: break;    // ADC10LO
        case  ADC10IV_ADC10INIFG: break;    // ADC10IN
        case  ADC10IV_ADC10IFG:             // ADC10
            ADC_Result = ADC10MEM0;         // Store ADC10 channel 0 result
            break;
        default: break;
    }
}

