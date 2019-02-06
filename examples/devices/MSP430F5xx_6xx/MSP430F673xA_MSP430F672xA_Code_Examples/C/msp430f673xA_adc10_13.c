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
//  MSP430F673xA Demo - ADC10, DMA Sample A1 32x, 2.5V Ref, TA3 Trig, DCO
//
//  Description; A1 is sampled 32x burst 16 times per second (ACLK/256) with
//  Vref = internal 2.5V. Activity is interrupt driven. Timer_A in upmode uses
//  TA3.0 toggle to drive ADC10 conversion. ADC conversions are automatically
//  triggered by TA0 rising edge every 2048 ACLK cycles. ADC10_ISR will exit
//  from LPM3 mode and return CPU active. Internal ADC10OSC times sample (16x)
//  and conversion (13x). DMA transfers conversion results to ADC_Result
//  variable.
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//               MSP430F673xA
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.1/A1          |
//           |             P3.0|--> TA3.0 (ADC trigger signal)
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

unsigned int ADC_Result[32];

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                          // Stop WDT

    // Setup P1.1 A1, P3.0 as TA3.0 output
    P1SEL |= BIT1;                                     // Set P1.1 to non-IO
    P3SEL |= BIT0;                                     // Set P3.0 to non-IO
    P3DIR |= BIT0;                                     // P3.0/TA3.0 output
    __disable_interrupt();                             // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                               // Enable access Port Mapping regs
    P1MAP1 = PM_ANALOG;                                // Enable A1
    P3MAP0 = PM_TA3_0;                                 // Enable TA3.0
    PMAPKEYID = 0;                                     // Disable access Port Mapping regs
    __enable_interrupt();                              // Re-enable all interrupts

    // Setup ADC10 - pulse sample mode; TB0 trigger; repeat single channel
    ADC10CTL0 = ADC10SHT_2 | ADC10ON;                  // 16 ADCCLKs, Enable ADC10
    ADC10CTL1 = ADC10SHP | ADC10SHS_2 | ADC10CONSEQ_2; // Sampling timer
    // TimerA3.0 trigger
    // Repeat single channel
    ADC10CTL2 = ADC10RES;                              // 10-bits resolution
    ADC10MCTL0 = ADC10SREF_1 | ADC10INCH_1;            // Vref+, A1

    // Setup internal reference
    while (REFCTL0 & REFGENBUSY) ;                     // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_2 | REFON;                      // Select internal ref = 2.5V
                                                       // Internal Reference ON
    __delay_cycles(75);                                // Delay (~75us) for Ref to settle

    // Setup DMA (ADC10IFG0 trigger)
    DMACTL0 = DMA0TSEL_24;                             // ADC10IFG trigger
    __data20_write_long((uintptr_t) &DMA0SA, (uintptr_t) &ADC10MEM0);
    // Source single address
    __data20_write_long((uintptr_t) &DMA0DA, (uintptr_t) &ADC_Result[0]);
    // Destination array address
    DMA0SZ = 32;                                       // 32 conversions
    DMA0CTL = DMADT_4 | DMADSTINCR_3 | DMAEN | DMAIE;  // Repeated single transfer
    // Increment destination
    // Enable interrupt after 32 convs

    // Setup Timer Trigger TA3.0
    TA3CCR0 = 256 - 1;                                 // TA3.0 Period
    TA3CCTL0 = OUTMOD_4;                               // TACCR0 toggle (ON period =~7.8ms)
    TA3CTL = TASSEL_1 | MC_1 | TACLR;                  // ACLK, up mode, clear counter

    while (1)
    {
        while (ADC10CTL1 & ADC10BUSY) ;                // Wait if ADC10 core is active
        ADC10CTL0 |= ADC10ENC;                         // Sampling and conversion ready
        __bis_SR_register(LPM3_bits | GIE);            // Enter LPM0, enable interrupts

        __delay_cycles(5000);                          // Delay between conversions
        __no_operation();                              // BREAKPOINT to check ADC_Result
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(DMAIV, 16))
    {
        case DMAIV_NONE: break;                        // No interrupts
        case DMAIV_DMA0IFG:                            // DMA0IFG = DMA Channel 0
            // 32 conversions complete
            __bic_SR_register_on_exit(LPM3_bits);      // Exit LPM0 on return
            break;
        case DMAIV_DMA1IFG: break;                     // DMA1IFG = DMA Channel 1
        case DMAIV_DMA2IFG: break;                     // DMA2IFG = DMA Channel 2
        case  8: break;                                // Reserved
        case 10: break;                                // Reserved
        case 12: break;                                // Reserved
        case 14: break;                                // Reserved
        case 16: break;                                // Reserved
        default: break;
    }
}

