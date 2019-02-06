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
//   MSP430FR69xx Demo - Enters LPM0 with ACLK = LFXT, MCLK = 1MHz
//
//   Description: Configure ACLK = LFXT and enters LPM0.
//
//   NOTE: On the FET board: MSP-TS430PZ100D drives an LED that can show high power numbers
//   when turned ON. Measure current with LED jumper JP9 disconnected.
//   LED jumper disconnected.
//   ACLK = LFXT, MCLK = SMCLK = 1MHz
//
//          MSP430FR6989
//         ---------------
//     /|\|            XIN|-
//      | |               |  32KHz Crystal
//      --|RST        XOUT|-
//        |               |
//        |               |
//        |          P1.0 |---> Disconnect JP9 for power meas.
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTTMSEL | WDTSSEL_1 | WDTIS_4; // ACLK, ~1s interrupts
  SFRIE1 |= WDTIE;                          // Enable WDT interrupt

  // Configure GPIO
  P1OUT = 0;
  P1DIR = 0xFF;

  P2OUT = 0;
  P2DIR = 0xFF;

  P3OUT = 0;
  P3DIR = 0xFF;

  P4OUT = 0;
  P4DIR = 0xFF;

  P5OUT = 0;
  P5DIR = 0xFF;

  P6OUT = 0;
  P6DIR = 0xFF;

  P7OUT = 0;
  P7DIR = 0xFF;

  P8OUT = 0;
  P8DIR = 0xFF;

  P9OUT = 0;
  P9DIR = 0xFF;

  P10OUT = 0;
  P10DIR = 0xFF;

  PJOUT = 0;
  PJSEL0 = BIT4 | BIT5;                     // For XT1
  PJDIR = 0xFFFF;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
  CSCTL4 &= ~LFXTOFF;                       // Enable LFXT1
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  P1DIR |= BIT0;                            // Turn on LED

  __bis_SR_register(LPM0_bits | GIE);
  __no_operation();                         // For debugger
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
