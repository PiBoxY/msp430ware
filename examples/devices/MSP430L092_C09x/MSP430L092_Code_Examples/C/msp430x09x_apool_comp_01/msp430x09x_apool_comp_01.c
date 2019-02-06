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
// Comparator in Compensated Mode - An input signal on A0 is compared with 
// an input signal on A2.  The signal present on A2 uses the input divider of 
// the module. The Results of the Comparator will be present on P1.3
// 
//                                     MSP430 L092
//                                   -----------------
//                               /|\|              XIN|-
//                                | |                 |
//                                --|RST          XOUT|-
//                                  |             P1.4|<--Comparator Input A0 
//           Comparator Input A2--> | P1.0        P1.3|-->Comparator Output
//
//    D.Dang/D.Archbold/D.Szmulewicz                                            
//    Texas Instruments Inc.                                                      
//    Built with IAR Version 5.10.4                                               
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;               // Stop watchdog timer to prevent time out reset
  SYSCNF |= 0x3000;                       // debug issue

  
  /**************************/
  /* Setup CCS              */
  /* MCLK = SMCLK = HFCLK/1 */
  /**************************/

  CCSCTL0 = CCSKEY;                       // Unlock CCS
  while (SFRIFG1 & OFIFG)                 // Oscillator Flag(s)?
  {
    CCSCTL7 = 0;                          // Clear HF & LF OSC fault flags
    SFRIFG1 = 0;                          // Clear OFIFG  
  }

  CCSCTL5 =  DIVM_0 + DIVS_0;             // Set the Dividers for  MCLK, and SMCLK to 1
  CCSCTL4 |= SELM_0 + SELS_0;             // Select HFCLK as the source for MCLK, and SMCLK                 
  CCSCTL0_H |= 0xFF;                      // Lock CCS
 
  /**************************/
  /* Setup Port 1           */
  /* Output P1.3            */
  /**************************/
                       
  P1DIR |= BIT3;
  P1SEL0 |= BIT3;
  P1SEL1 |= BIT3;
  
  /**************************/
  /* Configure APOOL        */
  /**************************/
  
  APIFG = 0;                              // Clear all APIFG 
  APCNF = CLKSEL_2;  
  APIFG = 0; 

  APCTL = APNSEL_0 + APPSEL_2;            // A2=P vs A0=N 
  APVDIV |= A2DIV_2;                      // A2 divide by 8
  APOMR = AZCMP;                          // Comparator in Compensated mode (CTEN = 0) 
  APCNF |= CMPON;                         // Enable comparator

__no_operation();                         // If A2/8 > A0, P1.3 = 1, else P1.3 = 0
while (1);  

}

