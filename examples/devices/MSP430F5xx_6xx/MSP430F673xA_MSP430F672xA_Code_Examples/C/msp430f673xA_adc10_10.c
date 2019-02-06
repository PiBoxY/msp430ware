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
//  MSP430F673xA Demo - ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO
//
//  Description: Sample A2/A1/A0 as single sequence with reference to AVcc.
//  Software sets ADC10SC to trigger sample sequence. In main loop MSP430 waits
//  in LPM0 to save power until ADC10 conversion complete, DMA_ISR will
//  force exit from LPM0. ADC10 internal oscillator times sample period
//  (16x) and conversion (13x). DMA transfers conversion results to ADC_Result
//  variable.
//
//                MSP430F673xA
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.0/A2          |
//        >---|P1.1/A1          |
//        >---|P1.2/A0          |
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

unsigned char ADC_Result[3];                      // 8-bit ADC conversion result array

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Setup P1.2 A0, 1.1 A1, 1.0 A2
    P1SEL |= BIT0 | BIT1 | BIT2;                  // Set P1.0,.1,.2 to non-IO
    __disable_interrupt();                        // Disable interrupts; Port Map cnfg
    PMAPKEYID = PMAPKEY;                          // Enable access Port Mapping regs
    P1MAP2 = PM_ANALOG;                           // Enable A0
    P1MAP1 = PM_ANALOG;                           // Enable A1
    P1MAP0 = PM_ANALOG;                           // Enable A2
    PMAPKEYID = 0;                                // Disable access Port Mapping regs
    __enable_interrupt();                         // Re-enable all interrupts

    // Setup ADC10
    ADC10CTL0 = ADC10SHT_2 | ADC10MSC | ADC10ON;  // 16ADCclks, MSC, ADC ON
    ADC10CTL1 = ADC10SHP | ADC10CONSEQ_1;         // pulse sample mode, sngle sequence
    ADC10CTL2 &= ~ADC10RES;                       // 8-bit resolution
    ADC10MCTL0 = ADC10INCH_2;                     // A0,A1,A2(EoS), AVCC reference

    // Setup DMA0 (ADC10IFG trigger)
    DMACTL0 = DMA0TSEL_24;                        // ADC10IFG trigger
    DMA0SZ = 0x03;                                // 3 conversions
    __data20_write_long((uintptr_t) &DMA0SA, (uintptr_t) &ADC10MEM0);
    // Source single address
    __data20_write_long((uintptr_t) &DMA0DA, (uintptr_t) &ADC_Result[0]);
    // Destination array address
    DMA0CTL = DMADT_4 | DMADSTINCR_3 | DMASRCBYTE | DMADSTBYTE | DMAEN | DMAIE;
    // Repeated single transfer
    // Increment destination
    // Byte access
    // Enable int after seq of convs
    while (1)
    {
        while (ADC10CTL1 & ADC10BUSY) ;           // Wait if ADC10 core is active
        ADC10CTL0 |= ADC10ENC | ADC10SC;          // Sampling and conversion start

        __bis_SR_register(LPM0_bits | GIE);       // Enter LMP0 w/ interrupt
        __delay_cycles(5000);                     // Delay between sequence convs
        __no_operation();                         // BREAKPOINT; View ADC_Result
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
        case DMAIV_NONE: break;                   // No interrupts
        case DMAIV_DMA0IFG:                       // DMA0IFG = DMA Channel 0
            // sequence of conversions complete
            __bic_SR_register_on_exit(LPM0_bits); // exit LPM0 on return
            break;
        case DMAIV_DMA1IFG: break;                // DMA1IFG = DMA Channel 1
        case DMAIV_DMA2IFG: break;                // DMA2IFG = DMA Channel 2
        case  8: break;                           // Reserved
        case 10: break;                           // Reserved
        case 12: break;                           // Reserved
        case 14: break;                           // Reserved
        case 16: break;                           // Reserved
        default: break;
    }
}

