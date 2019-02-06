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
//  MSP430xG461x Demo - ADC12, Repeated Single Channel Conversions
//
//  This example shows how to perform repeated conversions on a single channel
//  using "repeat-single-channel" mode.  AVcc is used for the reference and
//  repeated conversions are performed on Channel A0. Each conversion result
//  is moved to an 8-element array called Results[].  Test by applying a
//  voltage to channel A0, then running. To view the conversion results, open a
//  watch window with the debugger and view Results[].
//  ACLK = 32kHz, MCLK = SMCLK = default DCO 1048576Hz, ADC12CLK = ADC12OSC
//
//                MSP430xG461x
//              ---------------
//             |            XIN|-
//      Vin -->|P6.0/A0        | 32kHz
//             |           XOUT|-
//
//   Y. Yang
//   Texas Instruments Inc.
//   September 2012 
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

volatile unsigned int Results[8];
unsigned int Index;

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog timer
  P6SEL |= 0x01;                            // ADC sampled on P6.0
  ADC12CTL0 = ADC12ON|SHT0_8|MSC;           // Turn on ADC12, Sample and hold 256 ADC12CLK cycles
  ADC12CTL1 = SHP|CONSEQ_2;                 // Use sampling timer, set mode
  ADC12IE = 0x01;                           // Enable ADC12IFG.0
  ADC12CTL0 |= ENC;                         // Enable conversions
  __enable_interrupt();                     // Enable interrupts
  ADC12CTL0 |= ADC12SC;                     // Start conversion
  __bis_SR_register(LPM0_bits);             // LPM0
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
  Results[Index++] = ADC12MEM0;             // Move result, IFG is cleared
  Index &= 0x07;                            // Modulo pointer, counts up to 7 and resets
  __no_operation();                         // SET BREAKPOINT HERE
}
