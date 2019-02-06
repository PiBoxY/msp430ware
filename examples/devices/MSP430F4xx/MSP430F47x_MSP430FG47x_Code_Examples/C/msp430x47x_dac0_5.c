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
//  MSP430x47x Demo - DAC12_0, Output Voltage Ramp on DAC0, DAC1 Constant, DAC ISR
//
//  Description: Using DAC12.0 outputs a 16 Step Ramp with 1 sample/sec
//  using Timer_B2 trigger. Ramp is output on P1.6. Normal mode is LPM3.
//  DAC1 outputs a constant ~1.2V on P1.4.
//  Use internal 2.5V Vref.   
//
//               MSP430x47x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |        DAC0/P1.4|--> Ramp_positive
//           |        DAC1/P1.6|--> 1.2V
//
//  M.Seamen/ P. Thanigai
//  Texas Instruments Inc.
//  September 2008
//  Built with IAR Embedded Workbench V4.11A and CCE V3.2
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  TBCCTL2 = OUTMOD_7;                       // Reset/set
  TBCCR2 = 16384;                           // PWM Duty Cycle
  TBCCR0 = 32767;                           // 1Hz Clock period
  TBCTL = TBSSEL_1 + MC_1;                  // ACLK, up mode
  SD16CTL |= SD16REFON;			            // Internal 1.2V ref on
  DAC12_1CTL = DAC12IR + DAC12SREF_3 + DAC12AMP_5  // SD16 Ref, 1.2V
  			   + DAC12OPS + DAC12ENC + DAC12CALON; 
  DAC12_1DAT = 0x0FFF;                      // ~1.2V
  DAC12_0CTL = DAC12IR + DAC12SREF_3 + DAC12AMP_5 
  				+ DAC12ENC + DAC12LSEL_3 + DAC12IE + DAC12OPS; // SD16 Ref, 1.2V                                           
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
