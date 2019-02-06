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
//  MSP-FET430P440 Demo - ADC12, Sequence of Conversions (non-repeated)
//
//  This example shows how to perform A/D conversions on a sequence of channels.
//  A single sequence of conversions is performed - one conversion each on
//  channels A0, A1, A2, and A3. Each conversion uses AVcc and AVss for the
//  references. The conversion results are stored in ADC12MEM0, ADC12MEM1,
//  ADC12MEM2, and ADC12MEM3 respectively and are moved to 'results[]' upon
//  completion of the sequence. Test by applying voltages to pins A0, A1, A2,
//  and A3, then setting and running to a break point at the "_NOP()"
//  instruction in the ISR. To view the conversion results, open a watch window
//  with the debugger and view 'results' or view ADC12MEM0, ADC12MEM1,
//   ADC12MEM2, and ADC12MEM3 in a register window.
//  ACLK = n/a, MCLK = SMCLK = default DCO, ADC12CLK = ADC12OSC
//
//  Note that a sequence has no restrictions on which channels are converted.
//  For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
//  See the MSP430x4xx User's Guide for instructions on using the ADC12.
//
//
//                MSP430F449
//            -----------------
//           |                 |
//   Vin0 -->|P6.0/A0          |
//   Vin1 -->|P6.1/A1          |
//   Vin2 -->|P6.2/A2          |
//   Vin3 -->|P6.3/A3          |
//           |                 |
//
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************
#include <msp430.h>

// Needs to be global in this example. Otherwise, the
// compiler removes it because it is not used
static volatile unsigned int results[4];

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL = 0x0F;                             // Enable A/D channel inputs
  ADC12CTL0 = ADC12ON+MSC+SHT0_2;           // Turn on ADC12, set sampling time
  ADC12CTL1 = SHP+CONSEQ_1;                 // Use sampling timer, single sequ
  ADC12MCTL0 = INCH_0;                      // ref+=AVcc, channel = A0
  ADC12MCTL1 = INCH_1;                      // ref+=AVcc, channel = A1
  ADC12MCTL2 = INCH_2;                      // ref+=AVcc, channel = A2
  ADC12MCTL3 = INCH_3+EOS;                  // ref+=AVcc, channel = A3, end seq
  ADC12IE = 0x08;                           // Enable ADC12IFG.3
  ADC12CTL0 |= ENC;                         // Enable conversions
  __enable_interrupt();                     // Enable interrupts

  while(1)
  {
    ADC12CTL0 |= ADC12SC;                   // Start conversion
    __bis_SR_register(LPM0_bits);           // Enter LPM0
  }
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
  results[0] = ADC12MEM0;                   // Move results, IFG is cleared
  results[1] = ADC12MEM1;                   // Move results, IFG is cleared
  results[2] = ADC12MEM2;                   // Move results, IFG is cleared
  results[3] = ADC12MEM3;                   // Move results, IFG is cleared
  __bic_SR_register_on_exit(LPM0_bits);     // Clear LPM0
  __no_operation();                         // SET BREAKPOINT HERE
}
