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
//   msp430fr413x Demo - Timer_A, Ultra-Low Pwr Pulse Accumulator
//
//   Description: Timer_A is used as ultra-low power pulse counter. In this
//   example TAR is offset 10 counts, which are acculmulated on INCLK P4.0,
//   with the system in LPM4 - all internal clocks off. After 20 counts, TAR
//   will overflow requesting an interrupt, and waking the system. Timer_A is
//   then reconfigured with SMCLK as clock source in up mode - TACCR2 will then
//   toggle P1.0 every 50000 SMCLK cycles.
//
//                MSP430FR4133
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        --->|P1.5/TA0CLK  P1.0|-->LED
//
//   Lixin Chen
//   Texas Instruments Inc.
//   March 2014
//   Built with IAR Embedded Workbench v6.10 & Code Composer Studio v6.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;
  P1OUT = 0;

  P1SEL0 |= BIT5;                           // P1.5 selected as TA0CLK

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure Timer_A
  TA0CTL = TASSEL_0 | MC_2 | TACLR | TAIE;  // ACLK, count mode, clear TAR, enable interrupt
  TA0R = 0xFFFF - 20;                       // Offset until TAR overflow
  __bis_SR_register(LPM4_bits | GIE);       // Enter LPM3, enable interrupts
  __no_operation();                         // For debugger

  while (1)
  {
    P1OUT ^= BIT0;                          // P1.0 = toggle
    __bis_SR_register(LPM0_bits);           // CPU is not required
  }
}

// Timer0_A3 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA0IV,TA0IV_TAIFG))
  {
      case TA0IV_NONE:
          break;                             // No interrupt
      case TA0IV_TACCR1:
          break;                             // CCR1 not used
      case TA0IV_TACCR2:
          TA0CCR2 += 50000;                  // Add Offset to TACCR1
          __bic_SR_register_on_exit(LPM0_bits);// CPU active on reti
          break;                             // CCR2 not used
      case TA0IV_3:
          break;                             // reserved
      case TA0IV_4:
          break;                             // reserved
      case TA0IV_5:
          break;                             // reserved
      case TA0IV_6:
          break;                             // reserved
      case TA0IV_TAIFG:                      // overflow
          TA0CTL = TASSEL_2 | TACLR;         // SMCLK, clear TAR
          TA0CCTL2 = CCIE;                   // TACCR1 interrupt enabled
          TA0CCR2 = 50000;
          TA0CTL |= MC_2;                    // Start Timer_A in continuous
          __bic_SR_register_on_exit(LPM4_bits);// Exit LPM4 on reti
          break;
      default:
          break;
  }
}



