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
//  MSP430x11x1 Demo - Software SPI Interface TLC549, Set P1.0 if > 0.5*Vcc
//
//  Description: Read a TLC549 ADC with reference to Vcc using software SPI and
//  store the 8-bit digital code in ADCData.  The TLC549 is sampled in a
//  continous loop. If ADCData > 0.5*Vcc, P1.0 set, else reset.
//  ACLK = n/a. MCLK = SMCLK = default DCO ~800k
//  //* VCC must be at least 3v for TLC549 *//
//
//                       MSP430F1121
//                    -----------------
//                /|\|              XIN|-
//                 | |                 |
//                 --|RST          XOUT|-
//      TLC549       |                 |
//    ----------     |                 |
//   |        CS|<---|P2.0         P1.0|-->LED
// ~>| AIN+  CLK|<---|P2.1             |
//   |        DO|--->|P2.3             |
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

#define CS              0x01                // P2.0 - Chip Select
#define CLK             0x02                // P2.1 - Clock
#define DO              0x08                // P2.3 - Data Out

int main (void)
{
  unsigned char ADCData;
  unsigned char Counter;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P2OUT = CS;                               // /CS set, - P2.x reset
  P2DIR |= CLK + CS;                        // /CS and CLK outputs
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  for (;;)                                  // Infinite loop
  {
  P2OUT &= ~CS;                             // /CS reset, enable ADC
  for (Counter = 8; Counter > 0;)
    {
    ADCData = ADCData  << 1;
      if ((P2IN & DO) == DO)
      {
      ADCData |= 0x01;
      }
    Counter --;
    P2OUT |= CLK;                           // Clock high
    P2OUT &= ~CLK;                          // Clock low
    }
  P2OUT |= CS;                              // /CS set, disable ADC
    if (ADCData < 0x7F)
    {
    P1OUT &= ~0x01;                         // Clear P1.0 LED off
    }
    else
    {
    P1OUT |= 0x01;                          // Set P1.0 LED on
    }
  }
}

