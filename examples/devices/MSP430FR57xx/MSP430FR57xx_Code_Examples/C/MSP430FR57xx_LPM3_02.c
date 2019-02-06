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
//   MSP430FR57xx Demo - Enters LPM3 with ACLK = VLO
//
//   Description: Configure ACLK = VLO and enters LPM3. 
//   Note: On the FET board P1.0 drives an LED that can show high power numbers 
//   when turned ON. Measure current with LED jumper JP3 disconnected.
//   LED jumper disconnected.
//   ACLK = VLO, MCLK = SMCLK = 4MHz
// 
//           MSP430FR57x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |  
//        |          P1.0 |---> Disconnect JP3 for power meas.
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTTMSEL + WDTSSEL_1 + WDTIS_5; // ACLK, ~1s interrupts
  SFRIE1 |= WDTIE;                          // Enable WDT interrupt
 
  P1DIR = 0; 
  P1OUT = 0; 
  P1REN = 0xFF;
  
  P2DIR = 0; 
  P2OUT = 0; 
  P2REN = 0xFF;
  
  P3DIR = 0; 
  P3OUT = 0; 
  P3REN = 0xFF;
  
  P4DIR = 0; 
  P4OUT = 0; 
  P4REN = 0xFF;
  
  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
  CSCTL2 = SELA_1 + SELS_3 + SELM_3;        // set ACLK = VLO; MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_1 + DIVM_1;        // set all dividers 
  
  CSCTL4 = XT1OFF + XT2OFF;
  CSCTL5 &= ~(XT1OFFG + XT2OFFG);
  
  PJDIR = 0;
  PJOUT = 0;
  PJREN = 0xFF;
  
  REFCTL0 |= REFTCOFF; 
  REFCTL0 &= ~REFON;

  // Turn on LED 
  P1DIR |= BIT0;
  __bis_SR_register(LPM3_bits+GIE);
  
}


// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= 0x01;                            // Toggle P1.0 (LED)
}
