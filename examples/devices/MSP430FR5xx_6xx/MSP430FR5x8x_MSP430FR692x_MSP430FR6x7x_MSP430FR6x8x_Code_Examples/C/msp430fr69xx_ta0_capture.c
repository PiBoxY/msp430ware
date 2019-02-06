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
//***************************************************************************************
//  MSP430FR69xx Demo - Timer0_A3 Capture of VLO Period using DCO SMCLK
//
//  Description; Capture a number of periods of the VLO clock and store them in an array.
//  When the set number of periods is captured the program is trapped and the LED on
//  P1.0 is toggled. At this point halt the program execution read out the values using
//  the debugger.
//  ACLK = VLOCLK = 9.4kHz (typ.), MCLK = SMCLK = default DCO / default divider = 1MHz
//
//                MSP430FR6989
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  P. Thanigai
//  Texas Instruments, Inc
//  April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//***************************************************************************************
#include <msp430.h>

#define NUMBER_TIMER_CAPTURES       20

volatile unsigned int timerAcaptureValues[NUMBER_TIMER_CAPTURES];
unsigned int timerAcapturePointer = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  // Configure GPIO
  P1OUT &= ~0x01;                           // Clear P1.0 output
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL2 &= ~SELA_7;
  CSCTL2 |= SELA__VLOCLK;                   // Select ACLK=VLOCLK
  CSCTL0_H = 0x00;                          // Lock CS module (use byte mode to upper byte)

  __delay_cycles(1000);                     // Allow clock system to settle

  // Timer0_A3 Setup
  TA0CCTL2 = CM_1 | CCIS_1 | SCS | CAP | CCIE;
                                            // Capture rising edge,
                                            // Use CCI2B=ACLK,
                                            // Synchronous capture,
                                            // Enable capture mode,
                                            // Enable capture interrupt

  TA0CTL = TASSEL__SMCLK | MC__CONTINUOUS;  // Use SMCLK as clock source,
                                            // Start timer in continuous mode

  __bis_SR_register(LPM0_bits | GIE);
  __no_operation();
}

// Timer0_A3 CC1-4, TA Interrupt Handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(TA0IV, TA0IV_TAIFG)) {
    case TA0IV_TA0CCR1:
      break;
    case TA0IV_TA0CCR2:
      timerAcaptureValues[timerAcapturePointer++] = TA0CCR2;

      if (timerAcapturePointer >= 20) {
        while (1) {
          P1OUT ^= 0x01;                    // Toggle P1.0 (LED)
          __delay_cycles(100000);
        }
      }
      break;
    case TA0IV_TA0IFG:
      break;
    default:
      break;
  }
}
