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
//  MSP430F673x Demo - UCS, Software Toggle P1.0 with 8MHz DCO
//
//  Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK is brought out on pin P11.0, SMCLK is brought out on P11.2, and MCLK
//  is brought out on pin P11.1.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = 8MHz
//
//                MSP430F673x
//             -----------------
//         /|\|                 |
//          | |             PJ.3|-->ACLK
//          --|RST          PJ.1|-->MCLK
//            |             PJ.0|-->SMCLK
//            |                 |
//            |             P1.0|-->LED
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;              // Stop WDT

    // Setup P1.0 output, J1.0 SMCLK, J1.1 MCLK, J1.3 ACLK
    P1DIR |= BIT0;                         // Set P1.0 to output direction
    P1OUT &= ~BIT0;                        // Clear P1.0
    PJDIR |= BIT0 | BIT1 | BIT3;           // ACLK, MCLK, SMCLK set out to pins
    PJSEL |= BIT0 | BIT1 | BIT3;           // PJ.0,1,3 for debugging purposes.


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

    while (1)
    {
        P1OUT ^= BIT0;                     // Toggle P1.0
        __delay_cycles(600000);            // Delay
    }
}

