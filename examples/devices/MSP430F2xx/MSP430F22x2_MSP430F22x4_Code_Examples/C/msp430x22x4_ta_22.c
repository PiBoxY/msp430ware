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
//   MSP430F22x4 Demo - Timer_A, Ultra-Low Pwr Pulse Accumulator
//
//   Description: Timer_A is used as ultra-low power pulse counter. In this
//   example TAR is offset 100 counts, which are acculmulated on INCLK P2.1,
//   with the system in LPM4 - all internal clocks off. After 100 counts, TAR
//   will overflow requesting an interrupt, and waking the system. Timer_A is
//   then reconfigured with SMCLK as clock source in up mode - TACCR1 will then
//   toggle P1.0 every 50000 SMCLK cycles.
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        --->|P2.1/TAINCLK P1.0|-->LED
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   October 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT = 0;                                // All P1.x reset
  P2DIR = 0xFD;                             // All but P2.1 outputs
  P2SEL = 0x02;                             // P2.1 TAINCLK option select
  P2OUT = 0;                                // All P2.x reset
  P3DIR = 0xFF;                             // All P3.x outputs
  P3OUT = 0;                                // All P3.x reset
  P4DIR = 0xFF;                             // All P4.x outputs
  P4OUT = 0;                                // All P4.x reset
  TACTL = TASSEL1 + TASSEL0 + TACLR + TAIE; // Ext. INCLK, interrupt
  TAR = 0xFFFF - 100;                       // Offset until TAR overflow
  TACTL |= MC1;                             // Start Timer_A continuous mode
  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/ interrupts

  while (1)
  {
    P1OUT ^= 0x01;                          // P1.0 = toggle
    __bis_SR_register(LPM0_bits);           // CPU is not required
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMERA1_VECTOR
__interrupt void TA1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) TA1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (TAIV)        // Efficient switch-implementation
  {
    case 2:
      TACCR1 += 50000;                      // Add Offset to TACCR1
      __bic_SR_register_on_exit(LPM0_bits); // CPU active on reti
      break;
    case 10:
      TACTL = TASSEL1 + TACLR;              // SMCLK, clear TAR
      TACCTL1 = CCIE;                       // TACCR1 interrupt enabled
      TACCR1 = 50000;
      TACTL |= MC1;                         // Start Timer_A in continuous
      __bic_SR_register_on_exit(LPM4_bits); // Exit LPM4 on reti
      break;
  }
}




