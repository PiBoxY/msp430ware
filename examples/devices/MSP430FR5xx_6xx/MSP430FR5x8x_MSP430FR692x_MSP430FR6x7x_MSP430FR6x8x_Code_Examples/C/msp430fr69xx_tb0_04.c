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
//  MSP430FR69xx Demo - TimerB, Toggle P1.0, Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software and the TimerB overflow ISR.
//  In this example an ISR triggers when TB overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TBIV interrupt
//  vector generator is demonstrated.
//
//  ACLK = TACLK = 32768Hz, MCLK = SMCLK = DCO / 2 = 8MHz / 2 = 4MHz
//
//          MSP430FR6989
//         ---------------
//     /|\|            XIN|-
//      | |               |  32KHz Crystal
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;
  P1OUT |= BIT0;
  PJSEL0 |= BIT4 | BIT5;

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Setup XT1
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK; // Set ACLK = XT1; MCLK = DCO
  CSCTL3 = DIVA__1 | DIVS__2 | DIVM__2;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  CSCTL0_H = 0;                             // Lock CS registers

  TB0CTL = TBSSEL__ACLK | MC__CONTINUOUS | TBCLR | TBIE; // ACLK, contmode, clear TBR
                                            // enable interrupt

  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3, enable interrupts
  __no_operation();                         // For debugger
}

// TimerB Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TB0IV,TB0IV_TBIFG))
  {
    case TB0IV_NONE:    break;              // No interrupt
    case TB0IV_TB0CCR1: break;              // TB0CCR1 interrupt
    case TB0IV_TB0CCR2: break;              // TB0CCR2 interrupt
    case TB0IV_TB0CCR3: break;              // TB0CCR3 interrupt
    case TB0IV_TB0CCR4: break;              // TB0CCR4 interrupt
    case TB0IV_TB0CCR5: break;              // TB0CCR5 interrupt
    case TB0IV_TB0CCR6: break;              // TB0CCR6 interrupt
    case TB0IV_TB0IFG:                      // overflow
      P1OUT ^= BIT0;
      break;
    default: break;
  }
}
