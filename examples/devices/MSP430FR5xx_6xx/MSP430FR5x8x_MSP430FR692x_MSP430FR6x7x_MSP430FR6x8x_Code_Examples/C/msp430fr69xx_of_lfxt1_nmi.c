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
//  MSP430FR59xx Demo - LFXT Oscillator Fault Detection
//
//  Description: System runs normally in LPM3 with with basic timer clocked by
//  32kHz ACLK with a 1 second interrupt. P1.0 is normally toggled every
//  1 second inside basic timer interrupt. If an LFXT oscillator fault occurs,
//  NMI is requested forcing exit from LPM3. P1.0 is toggled rapidly by
//  software as long as LFXT oscillator fault is present. Assuumed only
//  LFXT as NMI source - code does not check for other NMI sources.
//  ACLK = LFXT = 32768Hz, MCLK = SMCLK = default DCO = 1MHz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//
//
//           MSP430FR59x
//         -----------------
//    /|\ |              XIN|-
//     |  |                 | 32kHz
//     ---|RST          XOUT|-
//        |                 |
//        |             P1.0|-->LED
//
//   W. Goh
//   Texas Instruments Inc.
//   March 2014
//   Built with Code Composer Studio v5.5 and Embedded Workbench v5.60
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // GPIO Setup
  P1OUT = 0;
  P1DIR = BIT0;                             // For LED

  // Set up XT1
  PJSEL0 = BIT4 | BIT5;                     // For XT1

  // Clock System Setup
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
  CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL4 &= ~LFXTOFF;

  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag

  RTCCTL0_H = RTCKEY_H;                     // Unlock RTC key protected registers
  RTCCTL0_L = RTCRDYIE;					    // Real-time clock ready interrupt enable
  RTCCTL1 = RTCMODE;                        // to trigger interrupt every second
  RTCCTL1 &= ~RTCHOLD;                      // Start RTC calendar mode
  RTCCTL0_H = 0;                            // Lock RTC key protected registers

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  SFRIE1 = OFIE;                            // Enable osc fault interrupt

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
  __no_operation();                         // For debugger
}


// RTC Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(RTCIV, RTCIV_RT1PSIFG))
  {
    case RTCIV_NONE:      break;            // Vector  0:  No pending interrupt
    case RTCIV_RTCOFIFG:  break;			// Vector  2:  Oscillator fault
    case RTCIV_RTCRDYIFG:                   // Vector  4:  RTC Ready
      P1OUT ^= BIT0;                        // Toggle P1.0
      break;
    case RTCIV_RTCTEVIFG: break;            // Vector  6:  RTC interval timer
    case RTCIV_RTCAIFG:   break;            // Vector  8:  RTC user alarm
    case RTCIV_RT0PSIFG:  break;            // Vector  10:  RTC prescaler 0
    case RTCIV_RT1PSIFG:  break;            // Vector  12:  RTC prescaler 1
    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
  do
  {
    CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;
    __delay_cycles(60000);                  // Time for flag to set
    P1OUT ^= BIT0;                          // Toggle P1.0 using exclusive-OR
  } while (SFRIFG1 & OFIFG);                // Test oscillator fault flag
}
