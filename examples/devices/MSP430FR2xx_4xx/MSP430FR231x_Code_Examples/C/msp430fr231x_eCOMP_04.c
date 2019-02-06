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
//  MSP430FR231x Demo - eCOMP Hysteresis, COUT Toggle in LPM4; low speed mode
//
//  Description: Use eCOMP and DAC with VCC as reference to determine if input
//  'Vcompare' is high or low. When Vcompare exceeds Vcc*3/4 COUT goes high
//  and when Vcompare is less than Vcc*1/4 then COUT goes low.
//
//                MSP430FR2311
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST       P1.1/C1 |<--Vcompare
//            |                  |
//            |        P2.0/COUT |----> 'high'(Vcompare>Vcc*3/4); 'low'(Vcompare<Vcc*1/4)
//            |                  |
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure Comparator input & output
  P1SEL0 |= BIT1;                           // Select eCOMP input function on P1.1/C1
  P1SEL1 |= BIT1;
  P2DIR |= BIT0;
  P2SEL1 |= BIT0;                           // Select CPOUT function on P2.0/COUT

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  // Setup eCOMP
  CPCTL0 = CPPSEL0;                         // Select C1 as input for V+ terminal
  CPCTL0 |= CPNSEL1 | CPNSEL2;              // Select DAC as input for V- terminal
  CPCTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CPDACCTL = CPDACEN;                       // eCOMP output is selected as the buffer control source
  CPDACDATA = 0x1030;                       // CPDACBUF1=VCC *3/4   CPDACBUF2=VCC *1/4
  CPCTL1 |= CPEN | CPMSEL;                  // Turn on eCOMP, in low power mode


  __bis_SR_register(LPM4_bits);             // Enter LPM4
  __no_operation();                         // For debug
}
