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
//   MSP430F461x Demo - Comparator A, Poll input CA0, result in P5.1
//
//   Description: The voltage at pin CA0 (Vcompare) is compared with reference
//   voltage of diode ref.  LED is toggled when Vcompare crosses the ref voltage.
//
//
//                 MSP430F461x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST           CA0|<--Vcompare
//            |                 |
//            |                 |
//            |             P5.1|-->LED
//
//  Y. Yang
//  Texas Instruments Inc.
//  September 2012
//  Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps

  CACTL1 = CAON|CAREF_3|CARSEL;             // Enable comp, Vref = Diode ref
  CACTL2 = P2CA0;                           // Connect pin 1.6 to CA0

  P5DIR |= 0x002;                           // P5.1 = output direction for LED
  P2DIR |= 0x040;							// P2.6 = output direction for CAOUT
  P2SEL |= 0x040;                           // P2.6 = CAOUT

  while(1)
    if (CACTL2&0x01)
      P5OUT |= 0x002;                       // Set LED if CA0 > Vref
    else
      P5OUT &= ~0x002;                      // Reset LED if CA0 < Vref
}
