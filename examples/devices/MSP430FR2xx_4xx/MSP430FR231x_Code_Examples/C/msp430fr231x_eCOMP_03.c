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
// MSP430FR231x Demo - eCOMP output toggle, wake up from LPM3;
//                     Vcompare is compared against 1V.
//
// Description: Use eCOMP and internal VREF to determine if input 'Vcompare'
//    is high or low.  When Vcompare exceeds 1V(VREF*43/64), CPOUT goes high
//    and LED is turned on. When Vcompare is less than 1V(VREF*43/64) then
//    CPOUT goes low and LED is turned off. eCOMP interrupt enabled, wake up
//    from LPM3.
//
//                MSP430FR2311
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST        P1.1/C1|<--Vcompare
//            |         P2.0/COUT|--> 'high'(Vcompare>1V); 'low'(Vcompare<1V)
//            |              P1.0|--> LED
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

  // Configure GPIO
  P1OUT &= ~BIT0;                           // Clear P1.0 output latch for a defined power-on state
  P1DIR |= BIT0;                            // Set P1.0 to output direction

  // Configure Comparator input & output
  P1SEL0 |= BIT1;                           // Select eCOMP input function on P1.1/C1
  P1SEL1 |= BIT1;
  P2DIR |= BIT0;
  P2SEL1 |= BIT0;                           // Select CPOUT function on P2.0/COUT

  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
  
  // Configure reference
  PMMCTL0_H = PMMPW_H;                      // Unlock the PMM registers
  PMMCTL2 |= INTREFEN;                      // Enable internal reference
  while(!(PMMCTL2 & REFGENRDY));            // Poll till internal reference settles
  
  // Setup eCOMP
  CPCTL0 = CPPSEL0;                         // Select C1 as input for V+ terminal
  CPCTL0 |= CPNSEL1 | CPNSEL2;              // Select DAC as input for V- terminal
  CPCTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CPCTL1 |= CPIIE | CPIE;                   // Enable eCOMP dual edge interrupt
  CPDACCTL |= CPDACREFS | CPDACEN;          // Select on-chip VREF and enable DAC
  CPDACDATA |= 0x002B;                      // CPDACBUF1=On-chip VREF*43/64 = 1V
  CPCTL1 |= CPEN;                           // Turn on eCOMP, in high speed mode

  
  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3
  __no_operation();                         // For debug
}

// eCOMP interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ECOMP0_VECTOR
__interrupt void ECOMP0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ECOMP0_VECTOR))) ECOMP0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(CPIV, CPIV__CPIIFG))
    {
        case CPIV__NONE:
            break;
        case CPIV__CPIFG:
            P1OUT |= BIT0;
            break;
        case CPIV__CPIIFG:
            P1OUT &= ~BIT0;
            break;
        default:
            break;
    }
}

