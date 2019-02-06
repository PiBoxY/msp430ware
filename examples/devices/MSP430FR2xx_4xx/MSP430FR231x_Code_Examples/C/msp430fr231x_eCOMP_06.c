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
// MSP430FR231x Demo - eCOMP Toggle from LPM3; eCOMP in ultra low power mode,
//                     compare two channels(P1.0, P1.1) input voltage.
//
// Description: Use eCOMP to compare two channels input voltage.
//    When P1.0 voltage exceeds P1.1 voltage, CPOUT goes high
//    When P1.1 voltage exceeds P1.0 voltage, CPOUT goes low.
//
//                MSP430FR2311
//             ------------------
//         /|\|                  |
//          | |           P1.0/C0|<-- V0
//          --|RST        P1.1/C1|<-- V1
//            |                  |
//            |         P2.0/COUT|----> 'high'(V0 > V1); 'low'(V1 > V0)
//            |                  |
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1  
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure Comparator input & output
  P1SEL0 |= BIT0 | BIT1;                    // Select eCOMP input function on P1.0/C0 and P1.1/C1
  P1SEL1 |= BIT0 | BIT1;
  P2DIR |= BIT0;
  P2SEL1 |= BIT0;                           // Select CPOUT function on P2.0/COUT

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
  
  // Setup eCOMP
  CPCTL0 = CPNSEL0;                         // Select P1.0/C0 and P1.1/C1 as eCOMP input
  CPCTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CPCTL1 |= CPEN | CPMSEL;                  // Turn on eCOMP, in low power mode

  
  __bis_SR_register(LPM3_bits);             // Enter LPM3
  __no_operation();                         // For debug
}
