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
//  MSP430FG662x Demo - XT2 sources SMCLK
//
//  Description: This program demonstrates using XT2 to source SMCLK. XT1 is not
//  connected in this case.
//
//  By default, LFXT1 is requested by the following modules:
//     - FLL
//     - ACLK
//  If LFXT1 is NOT used and if the user does not change the source modules,
//  it causes the XT1xxOFIFG flag to be set because it is constantly looking
//  for LFXT1. OFIFG, global oscillator fault flag, will always be set if LFXT1
//  is set. Hence, it is important to ensure LFXT1 is no longer being sourced
//  if LFXT1 is NOT used.
//  MCLK = SMCLK = XT2
//
//               MSP430FG662x
//             -----------------
//        /|\ |                 |
//         |  |                 |
//         ---|RST              |
//            |            XT2IN|-
//            |                 | HF XTAL (4MHz - 24MHz)
//            |           XT2OUT|-
//            |                 |
//            |             P3.4|--> SMCLK = XT2
//
//   M. Swanson
//   Texas Instruments Inc.
//   April 2014
//   Built with Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

  P3DIR |= BIT4;                            // SMCLK set out to pins
  P3SEL |= BIT4;                            
  
  CTSD16CCTL0 |= CTSD16SC;                  // Workaround for CTSD16OFFG errata
  do
  {
	  CTSD16CTL &= ~CTSD16OFFG;
  }while (CTSD16CTL&CTSD16OFFG);            // End of CTSD16OFFG workaround

  while(BAKCTL & LOCKBAK)
	  BAKCTL &= ~(LOCKBAK);

  P7SEL |= BIT2;                            // Port select XT2
  UCSCTL6 &= ~XT2OFF;                       // Enable XT2 
  UCSCTL3 |= SELREF_2;                      // FLLref = REFO
                                            // Since LFXT1 is not used,
                                            // sourcing FLL with LFXT1 can cause
                                            // XT1OFFG flag to set
  UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

  // Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
  do
  {
    UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | /*XT1HFOFFG |*/ DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
                                            // expected frequency
  UCSCTL4 |= SELS_5 | SELM_5;               // SMCLK=MCLK=XT2

  while(1);                                 // Loop in place
}

