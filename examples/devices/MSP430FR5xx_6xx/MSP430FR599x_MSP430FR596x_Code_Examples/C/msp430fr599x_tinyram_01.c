/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//  MSP430FR5x9x Demo - Store global variable in Tiny RAM and toggle LED
//
//  Description: Example demonstrates how to allocate a global counter variable
//  into Tiny RAM. The variable gets incremented in an endless loop each time
//  the LED on P1.0 gets blinked.
//  Background: MSP430FR599x devices have a very small additional section of
//  RAM called Tiny RAM that can be used by the user as extra storage - see
//  device datasheet for details. The RAM is available in LPM3/LPM4 even when
//  the main RAM is powered down. Note that for most use cases/applications,
//  there is no need to use Tiny RAM. Let the toolchain take care of allocating
//  data in the regular FRAM/RAM regions.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//           MSP430FR5994
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(globalCounterVariable, ".tinyram")
volatile unsigned long globalCounterVariable;
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma location=0x000006
__no_init volatile unsigned long globalCounterVariable;
#elif defined(__GNUC__)
volatile unsigned long globalCounterVariable __attribute__ ((section (".tinyram")));
#else
#error Compiler not supported!
#endif

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    // Initialize global counter variable located in Tiny RAM
    globalCounterVariable = 0;

    while(1)
    {
        P1OUT |= BIT0;                      // Turn on LED on P1.0
        __delay_cycles(100000);
        P1OUT &= ~BIT0;                     // Turn off LED on P1.0
        __delay_cycles(100000);

        globalCounterVariable++;            // Increment global counter located in Tiny TAM
    }
}
