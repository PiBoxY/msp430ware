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
//  MSP430TC0701 Demo - Software Port Interrupt Service on P1.4 from LPM4
//
//  Description: A Lo/Hi transition on P1.4 will trigger P1ISR the first time. 
//  On hitting the P1ISR, device exits LPM4 mode and executes section of code in
//  main() which includes toggling an LED and the P1.4IES bit which switches between 
//  Lo/Hi and Hi/Lo trigger transitions to alternatively enter the P1ISR. 
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430F552x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P1.4         P1.0|-->LED
//     \|/
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built IAR Embedded Workbench Version: 4.11A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;
  P1REN |= BIT4;                            // Enable P1.4 internal resistance
  P1OUT |= BIT4;                            // Set P1.4 as pull-Up resistance
  P1IES &= ~BIT4;                           // P1.4 Lo/Hi edge
  P1IFG &= ~BIT4;                           // P1.4 IFG cleared
  P1IE |= BIT4;                             // P1.4 interrupt enabled

  while(1)
  {
    __bis_SR_register(LPM4_bits + GIE);     // Enter LPM4 w/interrupt
    __no_operation();						// For debugger
    P1OUT ^= BIT0;                          // P1.0 = toggle
    P1IES ^= BIT4;                          // Toggle between H-L and L-H transition triggers
    P1IE |= BIT4;                           // Enable port interrupt
  }
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  P1IFG &= ~BIT4;                         // Clear P1.4 IFG
  P1IE &= ~ BIT4;                         // Clear P1.4 IE
  __bic_SR_register_on_exit(LPM4_bits);   // Exit LPM4 
}

