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
//  MSP430FG662x Demo - CTSD16, Single Conversion on a Single Channel, Poll IFG
//
//  Description: This program uses the CTSD16 module to perform a single
//  conversion on a single channel. The CTSD16 interrupt flag for CH0 is polled
//  to determine when a conversion has completed.
//
//  Test by applying a voltage to the input channel and setting a breakpoint
//  at the indicated line. Run program until it reaches the breakpoint, then use
//  the debugger's watch window to view the conversion results.
//
//  Results (upper 16 bits only) are stored in the variable "results"
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
//   Vin1+ -->|P6.4/A0.0+ VREF |---+
//   Vin1- -->|P6.5/A0.0-      |   |
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

unsigned int  results;                          // CTSD16 Conversion Results

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    P6SEL |= BIT4 | BIT5;                       // Select AD0+/- analog input pins

    CTSD16CTL = CTSD16REFS;                     // Internal ref
    CTSD16CCTL0  |= CTSD16SNGL | CTSD16DF;      // Single sample, 2's complement
    CTSD16INCTL0 |= CTSD16INCH_9;				// Select AD0+ / AD0- for CH1

    __delay_cycles(2000);                   // Delay ~120us for 1.2V ref to settle

    while(1) {
        CTSD16CCTL0 |= CTSD16SC;                // Set bit to start conversion
        while(!(CTSD16IFG & CTSD16IFG));      // Poll IFG until conversion completes
        results = CTSD16MEM0;                   // Save CTSD16 conversion results
        __no_operation();                       // SET BREAKPOINT HERE
    }
}
