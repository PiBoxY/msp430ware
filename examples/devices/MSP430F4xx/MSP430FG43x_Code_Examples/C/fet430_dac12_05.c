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
//  MSP-FET430P430 Demo - DAC0 Output Voltage Ramp, DAC1 Constant, DAC ISR
//
//  Description: Using DAC12.0 outputs a 16 Step Ramp with 1 sample/sec
//  using Timer_B2 trigger. Ramp is output on VeRef+. Normal mode is LPM3.
//  DAC1 outputs a constant 1.5V on P6.7
//  Use internal 2.5V Vref.
//
//               MSP430FG439
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |      DAC0/VeRef+|--> Ramp_positive
//           |        DAC1/P6.7|--> 1.5V
//
//  H. Grewal / L. Westlund
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  TBCCTL2 = OUTMOD_7;                       // Reset/set
  TBCCR2 = 16384;                           // PWM Duty Cycle	
  TBCCR0 = 32767;                           // 1Hz Clock period
  TBCTL = TBSSEL_1 + MC_1;                  // ACLK, up mode
  ADC12CTL0 = REF2_5V + REFON;              // Internal 2.5V ref on
  DAC12_1CTL = DAC12IR + DAC12AMP_5 + DAC12ENC; // Output range, Amp setting
  DAC12_1DAT = 0x099A;                      // 1.5V
  DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC + DAC12LSEL_3 + DAC12IE + DAC12OPS;
                                            // Internal ref gain 1
  __enable_interrupt();                     // Enable interrupts
  DAC12_0DAT++;                             // Positive ramp

  LPM3;                                     // Enter LPM3
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DAC12_DMA_VECTOR
__interrupt void DACDMA_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DAC12_DMA_VECTOR))) DACDMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
  DAC12_0CTL &= ~DAC12IFG;                  // Clear DAC12 interrupt flag
  DAC12_0DAT = DAC12_0DAT+256;              // Positive ramp
  DAC12_0DAT &= 0xFFF;                      //
}
