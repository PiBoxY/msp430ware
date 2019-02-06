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
//   MSP430xG461x Demo - DAC12_0, Output Voltage Ramp on DAC0
//
//   Description: Using DAC12_0 and 2.5V ADC12REF reference with a gain of 1,
//   output positive ramp on P6.6. Normal mode is LPM0 with CPU off.  WDT used
//   to provide ~0.5ms interrupt used to wake up the CPU and update the DAC
//   with software. Use internal 2.5V Vref.
//   ACLK = 32kHz, SMCLK = MCLK = WDTCLK = default DCO 1048576Hz
//
//
//                MSP430xG461x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |        DAC0/P6.6|--> Ramp_positive
//            |                 |
//
//   A. Dannenberg/ M. Mitchell
//   Texas Instruments Inc.
//   October 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDT_MDLY_0_064;                  // WDT ~0.064ms interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  ADC12CTL0 = REF2_5V + REFON;              // Internal 2.5V ref on
  TACCR0 = 13600;                           // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TACLR + MC_1 + TASSEL_2;          // up mode, SMCLK
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  TACCTL0 &= ~CCIE;                         // Disable timer interrupt
  __disable_interrupt();                    // Disable Interrupts
  DAC12_0CTL = DAC12IR + DAC12AMP_5 + DAC12ENC; // Int ref gain 1

  while (1)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, interrupts enabled
    DAC12_0DAT++;                           // Positive ramp
    DAC12_0DAT &= 0x0FFF;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  TACTL = 0;                                // Clear Timer_A control registers
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPMx, interrupts enabled
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM0_bits);      // TOS = clear LPM0
}
