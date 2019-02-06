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
//*****************************************************************************
//  MSP430F543xA Demo - FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
//
//  Description:  This program demonstrates using an external 32kHz crystal to
//  supply ACLK, and using a high speed crystal or resonator to supply SMCLK.
//  MLCK for the CPU is supplied by the internal DCO.  The 32kHz crystal
//  connects between pins Xin and Xout. The high frequency crystal or
//  resonator connects between pins XT2IN and XT2OUT.  The DCO clock is
//  generated internally and calibrated from the 32kHz crystal.  ACLK is
//  brought out on pin P11.0, SMCLK is brought out on P11.2, and MCLK is
//  brought out on pin P11.1.
//  ACLK = LFXT1 = 32768Hz, MCLK = default DCO = 32 x ACLK = 1048576Hz
//  SMCLK = HF XTAL
//  PMMCOREV = 2 to support up to 20MHz clock
//
//  NOTE: if the SMCLK/HF XTAL frequency exceeds 8MHz, VCore must be set
//  accordingly to support the system speed. Refer to MSP430x5xx Family User's Guide
//  Section 2.2 for more information.
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//  NOTE: External matching capacitors must be added for the high
//       speed crystal or resonator as required.
//
//                MSP430F5438A
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |                 |
//            |            XT2IN|-
//            |                 | HF XTAL or Resonator (add capacitors)
//            |           XT2OUT|-
//            |                 |
//            |            P11.0|--> ACLK = 32kHz Crystal Out
//            |                 |
//            |            P11.2|--> SMCLK = High Freq Xtal or Resonator Out
//            |                 |
//            |            P11.1|--> MCLK = Default DCO Frequency
//            |                 |
//            |                 |
//   Note: 
//   In order to run the system at up to 20MHz, VCore must be set at 1.8V 
//   or higher. This is done by invoking function SetVcoreUp().
// 
//   F. Chen
//   Texas Instruments Inc.
//   December 2012
//   Built with CCS Version: 5.2.1 and IAR Embedded Workbench Version: 5.51.1
//******************************************************************************

#include <msp430.h>


void SetVcoreUp (unsigned int level);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  SetVcoreUp(PMMCOREV_1);                     
  SetVcoreUp(PMMCOREV_2);                     // Set VCore to 1.8MHz for 20MHz
  
  P11DIR = BIT2 + BIT1 + BIT0;              // P11.2,1,0 to output direction
  P11SEL = BIT2 + BIT1 + BIT0;              // P11.2 to output SMCLK, P11.1
                                            // to output MCLK and P11.0 to
                                            // output ACLK
  P5SEL |= 0x0C;                            // Port select XT2
  P7SEL |= 0x03;                            // Port select XT1

  UCSCTL6 &= ~(XT1OFF + XT2OFF);            // Set XT1 & XT2 On
  UCSCTL6 |= XCAP_3;                        // Internal load cap

  // Loop until XT1,XT2 & DCO stabilizes
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

  UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
                                            // expected frequency
  UCSCTL4 |= SELA_0 + SELS_5;               // Select SMCLK, ACLK source and DCO source

  while(1);                                 // Loop in place
}

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;              
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
