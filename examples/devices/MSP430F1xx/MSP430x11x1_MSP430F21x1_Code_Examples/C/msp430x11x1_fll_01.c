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
//  MSP430x11x1 Demo - Basic Clock, Implement Auto RSEL SW FLL
//
//  Description: Set DCO clock to (Delta)*(4096) using software FLL. DCO clock
//  is output on P1.4 as SMCLK. DCO clock, which is the selected SMCLK source
//  for Timer_A is integrated over LFXT1/8 (4096) until SMCLK is is equal
//  to Delta. CCR2 captures ACLK. To use, Set_DCO Timer_A must be
//  operating in continuous mode. Watch crystal for ACLK is required for
//  this example. Delta must be kept in a range that allows possible
//  DCO speeds. Minimum Delta must ensure that Set_DCO loop can complete
//  within capture interval. Maximum delta can be calculated be
//  f(DCOx7) / 4096.  f(DCOx7) can be found in device specific datasheet.
//  ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = target DCO
//  [] To measure SMCLK on P1.4 with the FET, use "Release JTAG on Go"
//  in the debugger to have access to the port. Then run the code.
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//           MSP430F1121
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |     P1.4/SMCLK|--> SMLCK = target DCO
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

void Set_DCO (void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x10;                            // P1.4 output
  P1SEL |= 0x10;                            // P1.4 SMCLK output
  Set_DCO();                                // Set DCO

  while (1);
}

//------------------------------------------------------------------------------
void Set_DCO (void)                         // Set DCO to selected frequency
//------------------------------------------------------------------------------
{
//#define DELTA 900                         // target DCO = DELTA*(4096) = 3686400
#define DELTA 256                           // target DCO = DELTA*(4096) = 1048576
//#define DELTA 70                          // target DCO = DELTA*(4096) = 286720
  unsigned int Compare, Oldcapture = 0;

  BCSCTL1 |= DIVA_3;                        // ACLK= LFXT1CLK/8
  CCTL2 = CM_1 + CCIS_1 + CAP;              // CAP, ACLK
  TACTL = TASSEL_2 + MC_2 + TACLR;          // SMCLK, cont-mode, clear

  while (1)
  {
    while (!(CCIFG & CCTL2));               // Wait until capture occured
    CCTL2 &= ~CCIFG;                        // Capture occured, clear flag
    Compare = CCR2;                         // Get current captured SMCLK
    Compare = Compare - Oldcapture;         // SMCLK difference
    Oldcapture = CCR2;                      // Save current captured SMCLK

    if (DELTA == Compare) break;            // If equal, leave "while(1)"
    else if (DELTA < Compare)               // DCO is too fast, slow it down
    {
      DCOCTL--;
      if (DCOCTL == 0xFF)
      {
        if (!(BCSCTL1 == (XT2OFF + DIVA_3)))
        BCSCTL1--;                          // Did DCO roll under?, Sel lower RSEL
      }
    }
    else
    {
      DCOCTL++;
      if (DCOCTL == 0x00)
        {
          if (!(BCSCTL1 == (XT2OFF + DIVA_3 + 0x07)))
          BCSCTL1++;                        // Did DCO roll over? Sel higher RSEL
        }
    }
  }
  CCTL2 = 0;                                // Stop CCR2
  TACTL = 0;                                // Stop Timer_A
}
