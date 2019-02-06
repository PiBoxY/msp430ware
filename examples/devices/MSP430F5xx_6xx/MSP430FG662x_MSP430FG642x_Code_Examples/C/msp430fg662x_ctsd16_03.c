/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//  MSP430FG662x Demo - CTSD16, Continuous Rail-to-Rail Conversion, Single Channel
//
//  Description: This program uses the CTSD16 module to perform continuous
//  conversions on a single channel with rail-to-rail input. A CTSD16 interrupt
//  occurs when a conversion has completed.
//
//  Test by applying voltages to the input channel and setting a breakpoint
//  at the indicated line. Run program until it reaches the breakpoint, then use
//  the debugger's watch window to view the conversion results.
//
//  Results (upper 16 bits only) are stored in the array "results"
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz, SD_CLK = 1.024MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//  Notes: For minimum Vcc required for CTSD16 module - see datasheet
//         1nF cap btw Vref and AVss is recommended when using 1.2V ref
//
//               MSP430FG662x
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST             |
//            |                |
//   Vin1+ -->|A0.0+      VREF |---+
//   Vin1- -->|A0.0-           |   |
//            |                |  -+- 1nF
//            |                |  -+-
//            |                |   |
//            |           AVss |---+
//
//  M. Swanson
//  Texas Instruments, Inc
//  April 2014
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

/* Array to store CTSD16 conversion results */
unsigned int results[8];

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog

    P6SEL |= BIT4 | BIT5;                   // Select AD0+/- analog input pins

    CTSD16CTL = CTSD16REFS;                 // Internal ref
    CTSD16CCTL0  |= CTSD16DF;               // 2s complement
    CTSD16INCTL0 |= CTSD16RRI | CTSD16INCH_9; // Select AD0+ / AD0- for CH1
    CTSD16IFG &= ~(CTSD16IFG0 | CTSD16OVIFG0); // Clear CH0 interrrupt flags
    CTSD16IE |= CTSD16IE0 | CTSD16OVIE0;    // Enable CH0 overflow, result interrupts

    __delay_cycles(2000);                   // Delay ~120us for 1.2V ref to settle

    while(!(CTSD16CTL&CTSD16RRIRDY));       // Wait for rail-to-rail input ready

    CTSD16CCTL0 |= CTSD16SC;                // Set bit to start conversion
    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=CTSD16_VECTOR
__interrupt void CTSD16_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(CTSD16_VECTOR))) CTSD16_ISR (void)
#else
#error Compiler not supported!
#endif
{
    static unsigned char index = 0;
    switch (__even_in_range(CTSD16IV,CTSD16IV_CTSD16MEM0)) {
        case CTSD16IV_NONE: break;
        case CTSD16IV_CTSD16OVIFG:
            CTSD16IFG &= ~CTSD16OVIFG0;        // OPTIONAL BREAKPOINT HERE
             break;                            // Only triggers with below breakpoint
        case CTSD16IV_CTSD16MEM0:
                   results[index++] = CTSD16MEM0; // Save CH0 results (clears IFG)
                   if(index >= 8) {
                       index = 0;                 // SET BREAKPOINT HERE
                   }
                   break;
        default: break;
    }
}
