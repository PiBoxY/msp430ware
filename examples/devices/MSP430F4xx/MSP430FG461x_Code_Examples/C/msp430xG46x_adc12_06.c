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
//  MSP430xG461x Demo - ADC12, Repeated Sequence of Conversions
//
//  This example shows how to perform a repeated sequence of conversions using
//  "repeat sequence-of-channels" mode.  AVcc is used for the reference and
//  repeated sequence of conversions is performed on Channels A0, A1, A2, and
//  A3. Each conversion result is stored in ADC12MEM0, ADC12MEM1, ADC12MEM2,
//  and ADC12MEM3 respectively. After each sequence, the 4 conversion results
//  are moved to A0results[], A1results[], A2results[], and A3results[]. Test
//  by applying voltages to channels A0 - A3. Open a watch window with the
//  debugger and view the results.
//
//  Note that a sequence has no restrictions on which channels are converted.
//  For example, a valid sequence could be A0, A3, A2, A4, A2, A1, A0, and A7.
//  See the MSP430x4xx User's Guide for instructions on using the ADC12.
//  ACLK = 32kHz, MCLK = SMCLK = default DCO 1048576Hz, ADC12CLK = ADC12OSC
//
//              MSP430xG461x
//            -----------------
//           |              XIN|-
//   Vin0 -->|P6.0/A0          | 32kHz
//   Vin1 -->|P6.1/A1      XOUT|-
//   Vin2 -->|P6.2/A2          |
//   Vin3 -->|P6.3/A3          |
//           |                 |
//
//  Y. Yang
//  Texas Instruments Inc.
//  September 2012
//  Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

volatile unsigned int A0results[8];
volatile unsigned int A1results[8];
volatile unsigned int A2results[8];
volatile unsigned int A3results[8];
unsigned int Index;

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                 // Stop watchdog
  P6SEL |= 0x0f;                          // Enable A/D inputs
  ADC12CTL0 = SHT0_8|MSC|ADC12ON;         // Turn on ADC12, use internal osc.
                                          // Sample and hold for 256 ADC12CLK cycles
                                          // New conversions triggered
                                          // automatically after each completed
  ADC12CTL1 = SHP|CONSEQ_3;               // Use sampling timer, Repeat-sequence-of-channels mode
  ADC12IE = 0x08;                         // Enable ADC12IFG.3 for ADC12MEM3
  ADC12MCTL0 = INCH_0;                    // A0 goes to MEM0, ref+=AVcc
  ADC12MCTL1 = INCH_1;                    // A1 goes to MEM1, ref+=AVcc
  ADC12MCTL2 = INCH_2;                    // A2 goes to MEM2, ref+=AVcc
  ADC12MCTL3 = EOS|INCH_3;                // A3 goes to MEM3, end of sequence, ref+=AVcc

  while (1)
  {
    ADC12CTL0 |= ENC;                     // Enable conversions
    ADC12CTL0 |= ADC12SC;                 // Start conversions
    __bis_SR_register(LPM0_bits|GIE);     // LPM0
  }
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
  A0results[Index] = ADC12MEM0;             // Move A0 results, IFG is cleared
  A1results[Index] = ADC12MEM1;             // Move A1 results, IFG is cleared
  A2results[Index] = ADC12MEM2;             // Move A2 results, IFG is cleared
  A3results[Index] = ADC12MEM3;             // Move A3 results, IFG is cleared
  Index = (Index + 1) & 0x7;                // Increment results index, modulo
  __no_operation();                         // SET BREAKPOINT HERE
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0
}
