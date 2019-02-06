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
// MSP430FR231x Demo - eCOMP output connected to Timer_B capture,
//                     detect pulse width of eCOMP output.
//
// Description: Use eCOMP to compare 'Vcompare' and 1/2 VCC. When Vcompare
//    exceeds 1/2 VCC, CPOUT goes high. When Vcompare is less than 1/2 VCC
//    then CPOUT goes low. Use Timer_B to capture the CPOUT. Get CPOUT pulse
//    width from TB0CCR1 counter. Timer_B use ACLK as clock source.
//
//                MSP430FR2311
//             ------------------
//         /|\|                  |
//          | |                  |
//          --|RST        P1.1/C1|<--Vcompare
//            |         P2.0/COUT|--> 'high'(Vcompare>1V); 'low'(Vcompare<1V)
//            |                  |
//            |                  |
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1 
//******************************************************************************
#include <msp430.h>

unsigned int pulse_width = 0;

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
  
  // Configure reference
  PMMCTL0_H = PMMPW_H;                      // Unlock the PMM registers
  PMMCTL2 |= INTREFEN;                      // Enable internal reference
  __delay_cycles(400);                      // Delay for reference settling  
  
  // Setup eCOMP
  CPCTL0 = CPPSEL0;                         // Select C1 as input for V+ terminal
  CPCTL0 |= CPNSEL1 | CPNSEL2;              // Select DAC as input for V- terminal
  CPCTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CPDACCTL |= CPDACEN;                      // Select VCC as reference and enable DAC
  CPDACDATA |= 0x0020;                      // CPDACBUF1 = 1/2 VCC
  CPCTL1 |= CPEN;                           // Turn on eCOMP, in high speed mode

  // Timer0_B3 Setup
  TB0CCTL1 |= CM_3 | CCIS_1 | CCIE | CAP | SCS;
                                            // Capture both edges,
                                            // Use CCI0B = eCOMP output,
                                            // Synchronous capture,
                                            // Enable capture mode,
                                            // Enable capture interrupt

  TB0CTL |= TBSSEL_1 | MC_2 | TBCLR;        // Use ACLK as clock source, clear TB0R
  // Start timer in continuous mode

  
  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3
  __no_operation();                         // For debug
}

// Timer0_B3 CCR1, TB Interrupt Handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            pulse_width = TB0CCR1 - pulse_width; // Set a breakpoint here to watch pulse_width
            break;                               // Get the count between two edges
        case TB0IV_TBCCR2:
            break;                               // CCR2 not used
        case TB0IV_TBIFG:
            break;                               // overflow
        default:
            break;
    }
}


