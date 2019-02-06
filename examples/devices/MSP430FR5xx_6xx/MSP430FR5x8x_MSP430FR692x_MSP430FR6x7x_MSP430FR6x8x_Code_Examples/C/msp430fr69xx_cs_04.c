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
//   MSP430FR69xx Demo - ACLK = XT1 = 32768Hz, SMCLK= XT2 = 8MHz, MCLK = DCO
//
//   Description: Configure ACLK = LFXT, SMCLK = HFXT and MCLK = DCO
//   NOTE1: 32768Hz and 8MHz crystals required!
//
//           MSP430FR6989
//         ---------------
//     /|\|               |
//      | |               |-LFXIN
//      --|RST            |-LFXOUT
//        |               |
//        |               |-HFXIN
//        |               |-HFXOUT
//        |               |
//        |           P1.0|---> LED
//        |           P5.2|---> ACLK = 32768Hz
//        |           P7.4|---> SMCLK = 8MHz
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;

  // Configure GPIO
  P1OUT = 0;
  P1DIR = BIT0;                             // For LED

  P5DIR |= BIT2;
  P5SEL0 |= BIT2;                           // Output ACLK
  P5SEL1 |= BIT2;

  P7DIR |= BIT4;
  P7SEL1 |= BIT4;                           // Output SMCLK
  P7SEL0 |= BIT4;

  PJSEL0 |= BIT4 | BIT5 | BIT6 | BIT7;      // For XT1 and XT2

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
  CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
  CSCTL4 |= LFXTDRIVE_3 | HFXTDRIVE_3;
  CSCTL4 &= ~(LFXTOFF | HFXTOFF);
  do
  {
    CSCTL5 &= ~(LFXTOFFG | HFXTOFFG);       // Clear XT1 and XT2 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  while (1) {
    P1OUT ^= 0x01;                          // Toggle LED
    __delay_cycles(8000000);                // Wait 8,000,000 CPU Cycles
  }
}
