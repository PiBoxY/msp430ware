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
//  MSP430F673x Demo - DMA0, Single Transfer Using ADC10 Triggered by TimerA
//
//  Description: This software uses TA0CCR1 as a sample and convert input into
//  the A3 of ADC10. ADC10IFG is used to trigger a DMA transfer and DMA
//  interrupt triggers when DMA transfer is done. TA1 is set as an output and
//  P1.0 is toggled when DMA ISR is serviced.  Test by applying a voltage at the
//  A3 input channel and setting a breakpoint at the line indicated below.  Run
//  program until it reaches the breakpoint, then use the debugger's memory
//  view to observe &DMA_DST containing the ADC10 result.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = default DCO 1048576Hz
//
//                MSP430F673x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//            |             P1.1|-->TA1 output
//            |                 |
//            |             P9.3|<--A3
//
//   M. Swanson
//   Texas Instruments Inc.
//   December
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

// ADC conversion result
unsigned int DMA_DST;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Hold WDT

    P1OUT &= ~BIT0;                         // P1.0 clear
    P1DIR |= BIT0;                          // P1.0 output
    P1SEL |= BIT1;                          // P1.1 TA1 option select
    P1DIR |= BIT1;                          // Output direction
    P9SEL |= BIT3;                          // Enable A/D channel A3

    // Setup Timer A0
    TA0CCR0 = 0xFFFE;
    TA0CCR1 = 0x8000;
    TA0CCTL1 = OUTMOD_3;                    // CCR1 set/reset mode
    TA0CTL = MC_1 | TASSEL_2 | TACLR;       // SMCLK, Up-Mode, Clear counter

    // Setup ADC10
    ADC10CTL0 = ADC10SHT_15 | ADC10MSC | ADC10ON;
    // Sampling time = 1024 ADC10CLKs
    // Multiple sample and conversion
    // Enable ADC10
    ADC10CTL1 = ADC10SHS_1 | ADC10CONSEQ_2; // Use sampling timer; ADC10MEM0
                                            // Sample-and-hold source = CCI1A =
                                            // TA0CCR1 output
                                            // Repeated-single-channel
    ADC10MCTL0 = ADC10SREF_0 | ADC10INCH_3; // V+=AVcc V-=AVss, A3 channel
    ADC10CTL0 |= ADC10ENC;                  // Enable conversion

    // Setup DMA0
    DMACTL0 = DMA0TSEL_24;                  // ADC10IFGx triggered
    DMACTL4 = DMARMWDIS;                    // Read-modify-write disable
    __data20_write_long((uintptr_t) &DMA0SA, (uintptr_t) &ADC10MEM0);
    // Source block address
    __data20_write_long((uintptr_t) &DMA0DA, (uintptr_t) &DMA_DST);
    // Destination single address
    DMA0CTL &= ~DMAIFG;                     // Clear DMA interrupt flag
    DMA0SZ = 1;                             // DMA0 size = 1
    DMA0CTL = DMADT_4 | DMAIE | DMAEN;
    // Repeated single transfer
    // Enable DMA Interrupt, DMA

    __bis_SR_register(LPM0_bits | GIE);     // LPM0 w/ interrupts
}

//------------------------------------------------------------------------------
// DMA Interrupt Service Routine
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(DMAIV, 16))
    {
        case DMAIV_NONE: break;             // No interrupts
        case DMAIV_DMA0IFG:                 // DMA0IFG = DMA Channel 0
            P1OUT ^= BIT0;                  // Toggle P1.0 -
            __no_operation();               // PLACE BREAKPOINT HERE
            break;
        case DMAIV_DMA1IFG: break;          // DMA1IFG = DMA Channel 1
        case DMAIV_DMA2IFG: break;          // DMA2IFG = DMA Channel 2
        case 8: break;                      // Reserved
        case 10: break;                     // Reserved
        case 12: break;                     // Reserved
        case 14: break;                     // Reserved
        case 16: break;                     // Reserved
        default: break;
    }
}

