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
//  MSP430i20xx Demo - SD24, Single Conversion on a Single Channel, Poll IFG
//
//  Description: This program uses the SD24 module to perform a single
//  conversion on a single channel. The SD24 interrupt flag for CH0 is polled
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
//  Notes: For minimum Vcc required for SD24 module - see datasheet
//         100nF cap btw Vref and AVss is recommended when using 1.2V ref
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST             |
//            |                |
//   Vin1+ -->|A0.0+      VREF |---+
//   Vin1- -->|A0.0-           |   |
//            |                |  -+- 100nF
//            |                |  -+-
//            |                |   |
//            |           AVss |---+
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

unsigned int results;                       // SD24 Conversion Results

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    SD24CTL = SD24REFS;                     // Internal ref
    SD24CCTL0  |= SD24SNGL | SD24DF;        // Single sample, 2's complement

    __delay_cycles(3200);                   // Delay ~200us for 1.2V ref to settle

    while(1) {
        SD24CCTL0 |= SD24SC;                // Set bit to start conversion
        while(!(SD24CCTL0 & SD24IFG));      // Poll IFG until conversion completes
        results = SD24MEM0;                 // Save SD24 conversion results
        __no_operation();                   // SET BREAKPOINT HERE
    }
}
