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
//  MSP430F22x4 Demo - DCOCLK Biased with External Resistor Rosc
//
//  Description: Use external resistor on P2.5 as DCO current source. DCOCLK
//  is used by default as the MCLK source. Mainloop drives a 10 cycle software
//  loop toggling P1.1. Measure DCOCLK/10 on P1.1. Example also disables XTAL
//  buffer. Use of external resistor reduces temperature sensitivity of DCOCLK.
//  NOTE: With no resistor conencted, the oscillator will stop. Also, in this
//  case, the ability of re-establishing JTAG connectivity may be affected.
//
//  DCOCLK Frequency using DCO = 3 and RSEL = 4 values @ 3V
//  R=100K -> ~2MHz (See device datasheet)
//
//               MSP430F22x4
//            -----------------
//   /|\  /|\|              XIN|-
//    |    | |                 |
//    R    --|RST          XOUT|-
//    |      |                 |
//    -------|P2.5/ROSC    P1.1|-->DCOCLK/10
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  October 2006
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x02;                            // P1.1 = output direction
  BCSCTL2 |= DCOR;                          // Rosc
  BCSCTL1 &= ~(RSEL1 + RSEL0);              // RSEL = 4
  __bis_SR_register(OSCOFF);                // XTAL not used

  while(1)
  {
    P1OUT |= 0x02;
    P1OUT &= ~0x02;
  }
}
