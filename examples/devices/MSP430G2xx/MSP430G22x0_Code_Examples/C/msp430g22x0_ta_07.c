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
//  MSP430G22x0 Demo - Timer_A, Toggle P1.5-6, Cont. Mode ISR, DCO SMCLK
//
//  Description: Use Timer_A TA0CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, TA0CCR0 and TA0CCR1 output
//  units are optionally selected with port pins P1.5 and P1.6 in toggle
//  mode. As such, these pins will toggle when respective TA0CCRx registers match
//  the TAR counter. Interrupts are also enabled with all TA0CCRx units,
//  software loads offset to next interval only - as long as the interval offset
//  is aded to CCRx, toggle rate is generated in hardware. Timer_A overflow ISR
//  is used to toggle P1.2 with software. Proper use of the TAIV interrupt
//  vector generator is demonstrated.
//  ACLK = VLO, MCLK = SMCLK = TACLK = default DCO ~1MHz
//  As coded and assuming ~1MHz DCO, toggle rates are:
//  P1.5 = TA0CCR0 ~ 1MHz/(200) ~5000Hz
//  P1.6 = TA0CCR1 ~ 1MHz/(1000) ~1000Hz
//  P1.2 = overflow ~ 1MHz/(65536) ~15Hz
//
//               MSP430G22x0
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.5/TA0|--> TA0CCR0
//           |         P1.6/TA1|--> TA0CCR1
//           |             P1.2|--> Overflow/software
//
//  B. Nisarga
//  Texas Instruments, Inc
//  December 2011
//  Built with CCE Version: 5.1 and IAR Embedded Workbench Version: 5.4
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  
  /* Initialization Code */
  P1REN = 0x1B;                             // Terminate unavailable Port1 pins (P1.0/1/3/4) properly
                                            // Config as Input with pull-down enabled
  BCSCTL3 |= LFXT1S_2;                      // Select VLO as low freq clock     
  /* End Initialization Code */ 
  
  P1SEL |= BIT5 | BIT6;                     // P1.5 - P1.6 option select
  P1DIR |= BIT2 | BIT5 | BIT6;              // P1.2, P1.5, P1.6 outputs
  TA0CCTL0 = OUTMOD_4 | CCIE;               // TA0CCR0 toggle, interrupt enabled
  TA0CCTL1 = OUTMOD_4 | CCIE;               // TA0CCR1 toggle, interrupt enabled
  TA0CTL = TASSEL_2 | MC_2 | TAIE;          // SMCLK, Contmode, int enabled

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A0 (void)
#else
#error Compiler not supported!
#endif
{
  TA0CCR0 += 200;                              // Add Offset to TA0CCR0
}

// Timer_A2 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) Timer_A1 (void)
#else
#error Compiler not supported!
#endif
{
  switch( TAIV )
  {
  case  2: TA0CCR1 += 1000;                   // Add Offset to TA0CCR1
           break;
  case 10: P1OUT ^= BIT2;                     // Timer_A3 overflow
           break;
 }
}

