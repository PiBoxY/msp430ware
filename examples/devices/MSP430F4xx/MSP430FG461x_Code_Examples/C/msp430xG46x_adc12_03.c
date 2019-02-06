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
//   MSP430F461x Demo - ADC12, Sample A10 Temp, Set P5.1 if Temp ++ ~2C
//
//   Description: Use ADC12 and the integrated temperature sensor to detect
//   temperature gradients. The temperature sensor output voltage is sampled
//   ~ every 80ms and compared with the defined delta values using an ISR.
//   (ADC12OSC/256)/ determines sample time which needs to be greater than
//   30us for temperature sensor.
//   ADC12 is operated in repeat-single channel mode with the sample and
//   convert trigger sourced from Timer_A CCR1. The ADC12MEM0_IFG at the end
//   of each conversion will trigger an ISR.
//   ACLK = 32kHz, MCLK = SMCLK = default DCO 1048576Hz, ADC12CLK = ADC12OSC
//
//               MSP430F461x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.1|-->LED
//
//
//   Y. Yang
//   Texas Instruments Inc.
//   August 2012
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

#define ADC_DELTA_ON  6                    // ~ 1 Deg C delta for LED on
unsigned int ADCResult;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog
  ADC12CTL1 = SHS_1|SHP|CONSEQ_2;           // TA trig., rpt conv
  ADC12MCTL0 = SREF_1 | INCH_10;            // Channel A10, Vref+
  ADC12IE |= 0x001;                         // Enable ADC12IFG.0
  ADC12CTL0 = SHT0_8|REF2_5V|REFON|ADC12ON; // Config ADC12
  TACCR0 = 13600;                           // Delay for reference start-up
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TACLR | MC_1 | TASSEL_2;          // up mode, SMCLK
  __bis_SR_register(LPM0_bits|GIE);         // Enter LPM0, Enable interrupts
  TACCTL0 &= ~CCIE;                         // Disable timer interrupt
  __disable_interrupt();                    // Disable Interrupts
  ADC12CTL0 |= ENC;                         // Config ADC12
  TACCTL1 = OUTMOD_4;                       // Toggle on EQU1 (TAR = 0)
  TACTL = TASSEL_2 | MC_2;                  // SMCLK, cont-mode
  
  while (!(ADC12IFG & 0x0001));             // First conversion?
  ADCResult = ADC12MEM0;                    // Read out 1st ADC value
  ADCResult += ADC_DELTA_ON;
  P5OUT = 0;                                // Clear P5
  P5DIR |= 0x02;                            // P5.1 as output

  __bis_SR_register(LPM0_bits | GIE);       // LPM0
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
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if (ADC12MEM0 >= ADCResult)               // ADC12MEM = A0 > ADCResult?
    P5OUT |= 0x02;                          // P5.1 = 1
  else
    P5OUT &= ~0x02;                         // P5.1 = 0
}
