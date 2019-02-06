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
//  MSP430x21x1 Demo - Comp_A, Simple 2.2V Low Battery Detect using CASHORT
//
//  Description: Two comparator_A reference generators 0.25*Vcc and ~0.55V
//  are compared for a simple battery check of 2.2V. In the subroutine
//  Batt_Check, a small capacitor between P2.3 and Vss is first charged
//  to 0.25*Vcc using the CASHORT feature, and then compared to ~0.55V. If
//  0.25*Vcc, is above ~0.55V, P1.0 is toggled, else set. Batt_Check is
//  called constantly in an endless loop - in an actual application,
//  Batt_Check should be called infrequently to save power.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~1.16MHz
//
//  There is a tolerance of the Comp_A reference generator and in the
//  device specific datasheet. In most applications, the tolerence of the
//  reference generator is more than adequate to detect Low Battery.
//
//		           MSP430F21x1
//             -----------------
//       /|\  |              XIN|-
//        |   |                 |
//        ----|RST          XOUT|-
//            |                 |
//     +------|P2.3/CA0     P1.0|-->LED
//     |      |                 |
//    ===2.2uf|                 |
//     |      |                 |			
//     +------|Vss              |
//
//
//  H. Grewal / A. Dannenberg
//  Texas Instruments Inc.
//  July 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//*****************************************************************************

#include <msp430.h>

void Batt_Check(void);

int main (void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 output
  CAPD  |= 0x08;                            // Disconnect port pins
  CACTL2 = P2CA0;                           // P2.3 = CA0
  __enable_interrupt();                     // Enable interrupts
  while (1)                                 // Mainloop
  {
    Batt_Check();
  }
}

void Batt_Check(void)
{
  CACTL2 |= CASHORT;                        // Short Ref and P2.3
  CACTL1 = CAREF_1 + CAON + CARSEL;         // 0.25*Vcc on P2.3, Comp. on
  CCR1 = 3605;                              // CCR1 ~ 110ms (5tau)
  CCTL1 = CCIE;                             // Compare mode, interrupt
  TACTL = TASSEL0+TACLR+MC1;                // ACLK, TA clear, continuous mode
  LPM0;                                     // enter LPM0, wait for CCR1 interrupt
  CACTL2 &= ~CASHORT;                        // Disable the Short
  CACTL1 = CARSEL + CAREF_2 + CAREF_1 + CAON; // 0.55V on -, Comp. on
  if ((CACTL2 & CAOUT) > 0)
    P1OUT ^= 0x01;                          // P1.0 toggle
  else
    P1OUT |= 0x01;                          // P1.0 set
  CACTL1 = 0x00;                            // Disable Comp_A, save power
}

// Timer A1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) Timer_A1 (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;                                // Exit LPM0
  CCTL1 &= ~CCIFG;                          // Clear CCR1 interrupt flag
  TACTL = 0;                                // Stop Timer
 }
