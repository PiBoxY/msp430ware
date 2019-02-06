/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
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
// MSP430FR235x Demo - eCOMP Toggle from LPM3; eCOMP in ultra low power mode,
//                     Vcompare is compared against internal VREF.
//
// Description: Use eCOMP and internal VREF to determine if input 'Vcompare'
//    is high or low.  When Vcompare exceeds 1.47V(VREF*63/64), CPOUT goes high
//     and when Vcompare is less than 1.47V(VREF*63/64) then CPOUT goes low.
//
//                MSP430FR2355
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST        P1.1/C1|<--Vcompare
//            |                  |
//            |         P2.0/COUT|----> 'high'(Vcompare>1.47V); 'low'(Vcompare<1.47V)
//            |                  |
//
//   Cash Hao
//   Texas Instruments Inc.
//   November 2016
//   Built with IAR Embedded Workbench v6.50.0 & Code Composer Studio v6.2.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure Comparator input & output
  P1SEL0 |= BIT1;                           // Select eCOMP input function on P1.1/C1
  P1SEL1 |= BIT1;
  P2DIR |= BIT0;
  P2SEL1 |= BIT0;                           // Select CPOUT function on P2.0/COUT

  P1OUT |= BIT0 | BIT4; P3OUT |= BIT0 | BIT4;
  P1DIR |= BIT0 | BIT4; P3DIR |= BIT0 | BIT4;

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  // Configure reference
  PMMCTL0_H = PMMPW_H;                      // Unlock the PMM registers
  PMMCTL2 |= INTREFEN;                      // Enable internal reference
  while(!(PMMCTL2 & REFGENRDY));            // Poll till internal reference settles

  // Setup eCOMP
  CP0CTL0 = CPPSEL0;                         // Select C1 as input for V+ terminal
  CP0CTL0 |= CPNSEL1 | CPNSEL2;              // Select DAC as input for V- terminal
  CP0CTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CP0DACCTL |= CPDACREFS | CPDACEN;          // Select on-chip VREF and enable DAC
  CP0DACDATA |= 0x003f;                      // CPDACBUF1=On-chip VREF *63/64
  CP0CTL1 |= CPEN | CPMSEL;                  // Turn on eCOMP, in low power mode


  __bis_SR_register(LPM3_bits);             // Enter LPM3
  __no_operation();                         // For debug
}
