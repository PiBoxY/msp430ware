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
//  MSP430F21x2 Demo - Timer1_A2, Toggle P1.0 3.6,3.7, Cont. Mode ISR, 32kHz ACLK
//
//  Description: Use Timer1_A2 CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, TA1CCR0, TA1CCR1 and TA1CCR2
//  output units are optionally selected with port pins P3.6 and P3.7
//  in toggle mode. As such, these pins will toggle when respective TA1CCRx
//  registers match the T1AR counter. Interrupts are also enabled with all
//  TA1CCRx units, software loads offset to next interval only - as long as the
//  interval offset is added to TA1CCRx, toggle rate is generated in hardware.
//  Timer1_A1 overflow ISR is used to toggle P1.0 with software. Proper use
//  of the TA1IV interrupt vector generator is demonstrated.
//  ACLK = TA1CLK = 32kHz, MCLK = SMCLK = default DCO ~1.2MHz
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//  As coded and with TA1CLK = 32768Hz, toggle rates are:
//  P3.6 = TA1CCR0 = 32768/(2*4) = 4096Hz
//  P3.7 = TA1CCR1 = 32768/(2*16) = 1024Hz
//  P1.0 = overflow = 32768/(2*65536) = 0.25Hz
//
//              MSP430F21x2
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |       P3.6/TA0_1|--> TA1CCR0
//           |       P3.7/TA1_1|--> TA1CCR1
//           |                 |
//           |             P1.0|--> Overflow/software
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  January 2008
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.10A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0xC0;                            // P3.6 - P3.7 option select
  P3DIR |= 0xC0;                            // P3.6 - P3.7 output
  P1DIR |= 0x01;                            // P1.0 output
  TA1CCTL0 = OUTMOD_4 + CCIE;                // TACCR0 toggle, interrupt enabled
  TA1CCTL1 = OUTMOD_4 + CCIE;                // TACCR1 toggle, interrupt enabled
  TA1CTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, contmode, interrupt enabled

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/interrupt
}

// Timer1 A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0 (void)
#else
#error Compiler not supported!
#endif
{
  TA1CCR0 += 4;                              // Add Offset to TACCR0
}

// Timer1 A1 Interrupt Vector (TA1IV) handler

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1_A1 (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(TA1IV, 10))        // Efficient switch-implementation
  {
    case  2: TA1CCR1 += 16;                  // Add Offset to TACCR1
             break;

    case 10: P1OUT ^= 0x01;                 // Timer1_A3 overflow
             break;
  }
}

