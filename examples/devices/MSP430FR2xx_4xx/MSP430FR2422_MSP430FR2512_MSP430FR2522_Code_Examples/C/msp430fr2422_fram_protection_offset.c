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
//  MSP430FR2522 MSP430FR2422 Memory Protection Offset Configuration Example
//
//  Description: Configure FRWPOAx bits in SYSCFG0 register to unprotect
//               n*1KB(n = 1,2,...7) program FRAM. The rest of program FRAM is
//               protected by PFWP bit in SYSCFG0 register.
//               In this example, n = 1; Unprotect FRAM from 0xE300 to 0xE6FF
//
//  Step by Step configuration in CCS:
//               1. Replace ¡°FRAM: origin = 0xE300, length = 0x1C80¡± with
//                  ¡°FRAM: origin = 0xE700, length = 0x1880¡± in
//                  lnk_msp430fr2x22.cmd file
//               2. Configure SYSCFG0 register in user code
//                  ¡°SYSCFG0 = FRWPPW + FRWPOA0 + DFWP + PFWP;¡±
//
//  Step by Step configuration in IAR:
//               1. Replace all ¡°0xE300¡± with ¡°0xE700¡± in lnk430fr2x22.xcl file
//               2. Configure SYSCFG0 register in user code
//                  ¡°SYSCFG0 = FRWPPW + FRWPOA0 + DFWP + PFWP;¡±
//
//   Ling Zhu
//   Texas Instruments Inc.
//   July 2017
//   Built with Code Composer Studio v7.2.0
//******************************************************************************
#include <msp430.h>

#define  start_address  0xE300

unsigned int temp[512] = {0};
unsigned int index;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                          // Stop watchdog timer

    P1DIR |= BIT0;                                     // set LED pin as output
    P1OUT = 0;

    PM5CTL0 &= ~LOCKLPM5;                              // Disable the GPIO power-on default high-impedance mode
                                                       // to activate previously configured port settings


    SYSCFG0 = FRWPPW + FRWPOA0 + DFWP + PFWP;          // Unprotect 1KB FRAM from start address of program FRAM (0xE300 ~ 0xE6FF)

    for(index=0;index<512;index++)                     // Test Write to unprotect FRAM
    {
        *(unsigned int *)(start_address + index*2) = index;
    }

    for(index=0;index<512;index++)                     // Test Read from unprotect FRAM
    {
        temp[index] = *(unsigned int *)(start_address + index*2);
    }

    while(1)
    {
        P1OUT ^= BIT0;                                 // Toggle P1.0 using exclusive-OR
        __delay_cycles(500000);                        // Delay for 500000*(1/MCLK)=0.5s
    }
}
