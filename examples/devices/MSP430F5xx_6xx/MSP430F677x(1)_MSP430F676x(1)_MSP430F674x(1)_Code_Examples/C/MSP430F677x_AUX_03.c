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
//  MSP430F67791 Demo - AUX, Hardware-Controlled Switching, AUXVCC1
//
//  Description: Maintain 8-MHz system frequency when switching the AUX1.
//  ACLK is brought out on pin P1.2, SMCLK is brought out on P1.5, and MCLK
//  is brought out on pin P1.4.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz
//
//                MSP430F67791
//             -----------------
//         /|\|                 |
//          | |             P1.2|-->ACLK
//          --|RST          P1.4|-->MCLK
//            |             P1.5|-->SMCLK
//            |                 |
//            |          AUXVCC1|- 3.3V
//            |                 |
//
//   C. Fu
//   Texas Instruments Inc.
//   October 2012
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.51
//******************************************************************************
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P1.5 SMCLK, P1.4 MCLK, P1.2 ACLK
    P1DIR |= BIT2 | BIT4 | BIT5;            // ACLK, MCLK, SMCLK set out to pins
    P1SEL0 |= BIT2 | BIT4 | BIT5;           // P1.2,4,5 for debugging purposes.

    // Setup UCS
    UCSCTL3 |= SELREF_2;                   // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                     // Set ACLK = REFO

    __bis_SR_register(SCG0);               // Disable the FLL control loop
    UCSCTL0 = 0x0000;                      // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                   // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_1 | 243;                // Set DCO Multiplier for 8MHz
                                           // (N + 1) * FLLRef = Fdco
                                           // (243 + 1) * 32768 = 8MHz
                                           // Set FLL Div = fDCOCLK/2
    __bic_SR_register(SCG0);               // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
    __delay_cycles(250000);

    // Loop until XT1, XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                 // Clear fault flags
    } while (SFRIFG1 & OFIFG);             // Test oscillator fault flag

    PMMCTL0_H = PMMPW_H;                                   // Unlock all PMM registers, (PMMCOREV = 0, SVSMHRRL = 0)
    AUXCTL0 = AUXKEY;                                      // Clear the LOCKAUX bit, enable the write access to the AUXCTLx registers
    AUXCTL2 = AUX0LVL_1;                                   // AUX0LVL = 1, AUX1LVL = 0, AUX2LVL = 0
    PMMCTL0_H = 0;                                         // Lock all PMM registers

    while (1);
}
