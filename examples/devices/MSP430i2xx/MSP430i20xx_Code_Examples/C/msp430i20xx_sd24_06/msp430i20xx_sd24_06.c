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
//  MSP430i20xx Demo - SD24, Using the Integrated Temperature Sensor
//
//  Description: This program uses the SD24 module to perform a single
//  conversion on a single channel which is internally connected to the SD24's
//  temperature sensor. Once the conversion is completed, the result is stored in
//  a variable then converted into Celsius and Fahrenheit values.
//
//  Test by setting a breakpoint at the indicated line. Upon reaching the breakpoint
//  the conversion result will be stored in the results array. The result will
//  then be taken and converted into degrees K, C, and F and be saved in the same
//  array.
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz, SD_CLK = 1.024MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//  Notes: For minimum Vcc required for SD24 module - see datasheet
//         100nF cap btw Vref and AVss is recommended when using 1.2V ref
//
//  Sensor's temperature coefficient is 2.158mV/K (from datasheet)
//  Sensor's offset voltage ranges from -100mv to 100mV (assume 0)
//  Vsensor = 1.32mV * DegK + Vsensor_offset (assuming 0mv for offset)
//  Vsensor = (SD24MEM0)/32767 * Vref(mV)
//  DegK = (SD24MEM0 * 1200)/32767/2.158 = (SD24MEM0 * 1200)/70711
//  DegC =  DegK - 273
//  DegF = (DegC * 9/5) + 32
//
//
//               MSP430i20xx
//             -----------------
//         /|\|                |
//          | |                |
//          --|RST             |
//            |                |        (A0.6+/- connected internally)
//            |A0.6+      VREF |---+    (to SD24's temperature sensor)
//            |A0.6-           |   |
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

unsigned int  results[4];                       // SD24 Conversion and Temp Results
                                                // results[0] = raw SD24 results
                                                // results[1] = temp in K
                                                // results[2] = temp in C
                                                // results[3] = temp in F

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   // Stop WDT

    SD24CTL = SD24REFS;                         // Internal ref
    SD24CCTL0  |= SD24SNGL | SD24DF | SD24IE;   // Enable interrupt
    SD24INCTL0 |= SD24INCH_6;                   // Internal temp sensor

    __delay_cycles(3200);                       // Delay ~200us for 1.2V ref to settle

    while(1) {
        SD24CCTL0 |= SD24SC;                    // Set bit to start conversion
        __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
        __no_operation();                       // For debugger

        // Calculate Temperatures in different scales
        results[1] = ((unsigned long)results[0] * 1200)/70711;
        results[2] = results[1] - 273;
        results[3] = (results[2] * 9/5) + 32;

        __no_operation();                       // SET BREAKPOINT HERE
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24_VECTOR
__interrupt void SD24_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24_VECTOR))) SD24_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(SD24IV,SD24IV_SD24MEM3)) {
        case SD24IV_NONE: break;
        case SD24IV_SD24OVIFG: break;
        case SD24IV_SD24MEM0:
                   results[0] = SD24MEM0;       // Save CH0 results (clears IFG)
                   break;
        case SD24IV_SD24MEM1: break;
        case SD24IV_SD24MEM2: break;
        case SD24IV_SD24MEM3: break;
        default: break;
    }
    __bic_SR_register_on_exit(LPM0_bits);       // Wake up from LPM0
}
