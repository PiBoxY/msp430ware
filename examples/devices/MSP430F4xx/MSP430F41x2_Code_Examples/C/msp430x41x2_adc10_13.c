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
//  MSP430F41x2 Demo - ADC10, DTC Sample A1 32x, AVcc, TA1 Trig, DCO
//
//  Description; A1 is sampled in 32x burst using DTC 16 times per second
//  (ACLK/2048) with reference to AVcc. Activity is interrupt driven.
//  Timer_A in upmode uses TA1 toggle to drive ADC10 conversion. Sample burst
//  is automatically triggered by TA1.OUT0 rising edge every 2048 ACLK cycles.
//  ADC10_ISR will exit from LPM3 mode and return CPU active. Internal ADC10OSC
//  times sample (16x) and conversion (13x). DTC transfers conversion code to
//  RAM 200h - 240h. In the Mainloop P5.1 is toggled. Normal Mode is LPM3.
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//               MSP430F41x2
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P7.5/A1     P5.1 |--> LED
//
//  P. Thanigai
//  Texas Instruments Inc.
//  January 2009
//  Built with CCE Version: 3.1 and IAR Embedded Workbench Version: 4.11
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL1 = INCH_1 + SHS_2 + CONSEQ_2;    // TA1 trigger
  ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
  ADC10DTC1 = 0x20;                         // 32 conversions
  P5DIR |= 0x02;                            // Set P5.1 output
  ADC10AE0 |= 0x02;                         // P7.5 ADC10 option select
  TA1CCR0 = 1024-1;                         // PWM Period
  TA1CCTL0 = OUTMOD_4;                      // TA1CCR0 toggle
  TA1CTL = TASSEL_1 + MC_1;                 // ACLK, up mode

  for (;;)
  {
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = 0x200;                        // Data buffer start
    ADC10CTL0 |= ENC;                       // Sampling and conversion ready
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts
    P5OUT ^= 0x02;                          // Toggle P1.0 using exclusive-OR
  }
}

// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}

