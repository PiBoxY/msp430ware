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
//   MSP430FR231x Demo - Timer_B, Ultra-Low Power Pulse Accumulator
//
//   Description: Timer_B is used as ultra-low power pulse counter. In this
//   example TBR is offset 10 counts, which are accumulated on TB0CLK P2.7,
//   with the system in LPM4 - all internal clocks off. After 20 counts, TBR
//   will overflow requesting an interrupt, and wake up the system. Timer_B is
//   then reconfigured with SMCLK as clock source in up mode - TBCCR2 will then
//   toggle P1.0 every 50000 SMCLK cycles.
//
//                MSP430FR2311
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//        --->|P2.7/TB0CLK  P1.0|-->LED
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
  
  // Configure GPIO
  P1DIR |= BIT0;
  P1OUT = 0;
  
  P2SEL0 |= BIT7;                           // P2.7 selected as TB0CLK

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure Timer_B
  TB0CTL = TBSSEL_0 | MC_2 | TBCLR | TBIE;  // ACLK, count mode, clear TBR, enable interrupt
  TB0R = 0xFFFF - 20;                       // Offset until TBR overflow
  __bis_SR_register(LPM4_bits | GIE);       // Enter LPM4, enable interrupts
  __no_operation();                         // For debug

  while (1)
  {
    P1OUT ^= BIT0;                          // P1.0 = toggle
    __bis_SR_register(LPM0_bits);           // CPU is not required
  }
}

// Timer0_B3 Interrupt Vector (TBIV) handler
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
      case TB0IV_NONE:
          break;                             // No interrupt
      case TB0IV_TBCCR1:
          break;                             // CCR1 not used
      case TB0IV_TBCCR2:
          TB0CCR2 += 50000;                  // Add Offset to TBCCR1
          __bic_SR_register_on_exit(LPM0_bits);// CPU active on reti
          break;                             // CCR2 not used
      case TB0IV_TBIFG:                      // overflow
          TB0CTL = TBSSEL_2 | TBCLR;         // SMCLK, clear TBR
          TB0CCTL2 = CCIE;                   // TBCCR1 interrupt enabled
          TB0CCR2 = 50000;
          TB0CTL |= MC_2;                    // Start Timer_B in continuous
          __bic_SR_register_on_exit(LPM4_bits);// Exit LPM4 on reti
          break;
      default:
          break;
  }
}
