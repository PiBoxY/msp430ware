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
//  MSP430FG662x Demo - CTSD16, Single Conversion of 3 Input Channels
//
//  Description: This program uses the CTSD16 module to perform a single
//  conversion on 3 input channels (0, 1, and 2). A CTSD16 interrupt occurs when
//  the conversions have completed.
//
//  Test by applying voltages to the input channel and setting a breakpoint
//  at the indicated line. Run program until it reaches the breakpoint, then use
//  the debugger's watch window to view the conversion result.
//
//  Results (upper 16 bits only) are stored in the arrays "Chresults", with the
//  input channel number equal to the value's index (ex. input channel 0 stored
//  at Chresults[0]).
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
//   Vin1+ -->|P6.4/AD0+  VREF |---+
//   Vin1- -->|P6.5/AD0-       |   |
//   Vin2+ -->|P6.6/AD0+       |  -+- 1nF
//   Vin2- -->|P6.7/AD0-       |  -+-
//   Vin3+ -->|P7.4/AD0+       |   |
//   Vin3- -->|P7.5/AD0-  AVss |---+
//
//  M. Swanson
//  Texas Instruments, Inc
//  April 2014
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

#define Num_of_Channels    3

/* Arrays to store CTSD16 conversion results */
unsigned int Chresults[Num_of_Channels];
unsigned int i = 0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog

    P6SEL |= BIT4 | BIT5 | BIT6 | BIT7;     // Select AD0+ to AD2-...
    P7SEL |= BIT4 | BIT5;                   // as analog input

    CTSD16CTL = CTSD16REFS;                 // Internal ref
    CTSD16CCTL0  |= CTSD16SNGL | CTSD16DF;  // 2s complement, single conversion
    CTSD16INCTL0 |= CTSD16INCH_9;           // Select AD0+ / AD0- for CH0
    CTSD16IFG &= ~CTSD16IFG0;               // Clear CH0 result interrupt flag
    CTSD16IE |= CTSD16IE0;    // Enable CH0 result interrupts

    __delay_cycles(2000);                   // Delay ~120us for 1.2V ref to settle

    while(1) {
        CTSD16CCTL0 |= CTSD16SC;            // Set bit to start conversion
        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts
        __no_operation();                   // For debugger

        if (i >= Num_of_Channels) {
          i = 0;                        // SET BREAKPOINT HERE
          CTSD16INCTL0 = CTSD16INCH_9;      // Reset input to CH0
        } else {
          CTSD16INCTL0 += 2;                // Increment CH count by 1
        }
    }
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
    switch (__even_in_range(CTSD16IV,CTSD16IV_CTSD16MEM0)) {
        case CTSD16IV_NONE: break;
        case CTSD16IV_CTSD16OVIFG: break;
        case CTSD16IV_CTSD16MEM0:
                   Chresults[i++] = CTSD16MEM0; // Save CH0 result (clears IFG)
                   __bic_SR_register_on_exit(LPM0_bits); // Wake up
                   break;
        default: break;
    }
}
