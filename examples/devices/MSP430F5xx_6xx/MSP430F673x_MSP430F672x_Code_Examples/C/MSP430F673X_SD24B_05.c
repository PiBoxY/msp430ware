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
//*****************************************************************************
//  MSP430F673x Demo - SD24_B, Single Conversion on a Single Channel Using ISR
//
//  Description: This program uses the SD24_B module to perform a single
//  conversion on a single channel. An SD24_B interrupt occurs when a
//  conversion has completed. Test by applying a voltage to channel 2
//  (SD2P0, SD2N0) and setting a breakpoint at the line indicated below.
//  Run program until it reaches the breakpoint, then use the debugger's
//  watch window to view the conversion result. Results (upper 16 bits only)
//  for channel 2 are stored in variable "results".
//  ACLK = n/a, MCLK = SMCLK = DCO =  ~ 1.1MHz
//  //* For minimum Vcc required for SD24_B module - see datasheet          *//
//  //* 100nF cap between Vref and AVss is recommended when using 1.5V REF  *//
//
//                MSP430F673x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//    Vin+ -->|SD2P0            |
//    Vin- -->|SD2N0            |
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//  M. Swanson
//  Texas Instruments, Inc
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//*****************************************************************************
#include <msp430.h>

/* Unsigned integer to store SD24_B conversion result */
unsigned int results;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    SD24BCTL0 = SD24REFS | SD24SSEL_1;      // Select internal REF
                                            // Select SMCLK as SD24_B clock source

    SD24BCCTL2 |= SD24SNGL;                 // Single conversion

    SD24BINCTL2 |= SD24INTDLY0;             // Interrupt on 3rd sample
    SD24BIE |= SD24IE2;                     // Enable channel 2 interrupt

    __delay_cycles(0x3600);                 // Delay for 1.5V REF startup

    while (1)
    {
        SD24BCCTL2 |= SD24SC;               // Set bit to start conversion
        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts
        __no_operation();
        __no_operation();                   // SET BREAKPOINT HERE
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
            break;
        case SD24BIV_SD24IFG0:              // SD24MEM0 IFG
            break;
        case SD24BIV_SD24IFG1:              // SD24MEM1 IFG
            break;
        case SD24BIV_SD24IFG2:              // SD24MEM2 IFG
            results = SD24BMEMH2;           // Save CH2 results (clears IFG)
            break;
    }

    __bic_SR_register_on_exit(LPM0_bits);   // Exit LPM0
}

