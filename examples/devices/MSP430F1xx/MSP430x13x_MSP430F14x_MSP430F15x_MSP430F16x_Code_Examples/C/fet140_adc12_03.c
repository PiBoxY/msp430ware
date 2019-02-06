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
//*****************************************************************************
//  MSP-FET430P140 Demo - ADC12, Sample A10 Temp, Set P1.0 if Temp ++ ~2C
//
//  Description: Use ADC12 and the integrated temperature sensor to detect
//  temperature gradients. The temperature sensor output voltage is sampled
//  ~ every 80ms and compared with the defined delta values using an ISR.
//  (ADC12OSC/256)/ determines sample time which needs to be greater than
//  30us for temperature sensor.
//  ADC12 is operated in repeat-single channel mode with the sample and
//  convert trigger sourced from Timer_A CCR1. The ADC12MEM0_IFG at the end
//  of each converstion will trigger an ISR.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k, ADC12CLK = ADC12OSC
//
//                MSP430Fx49
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |A10          P1.0|-->LED
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

#define ADCDeltaOn       12                 // ~2 Deg C delta

static unsigned int FirstADCVal;            // holds 1st ADC result

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P1OUT = 0x00;                             // Clear P1
  P1DIR = 0x01;                             // P1.0 as output
  ADC12CTL1 = SHS_1 + SHP + CONSEQ_2;       // TA trig., rpt conv.
  ADC12MCTL0 = SREF_1 + INCH_10;            // Channel A10, Vref+
  ADC12IE = 0x01;                           // Enable ADC12IFG.0
  ADC12CTL0 = SHT0_8 + REF2_5V + REFON + ADC12ON + ENC; // Config ADC12
  TACCTL1 = OUTMOD_4;                       // Toggle on EQU1 (TAR = 0)
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, cont-mode
  while (!(0x01 & ADC12IFG));               // First conversion?
  FirstADCVal = ADC12MEM0;                  // Read out 1st ADC value
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
  if (ADC12MEM0 <= FirstADCVal + ADCDeltaOn)
      P1OUT &= ~0x01;                       // LED off
  else P1OUT |= 0x01;                       // LED on
}
