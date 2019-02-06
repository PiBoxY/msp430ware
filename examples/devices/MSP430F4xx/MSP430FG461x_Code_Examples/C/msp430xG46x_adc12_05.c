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
//   MSP430xG461x Demo - ADC12, Using an External Reference
//
//   This example shows how to use an external positive reference for the ADC12.
//   The external reference is applied to the VeREF+ pin. Avss is used for the
//   negative reference. A single conversion is performed on channel A0. The
//   conversion results are stored in ADC12MEM0 after the conversion is 
//   complete. You can test by applying a voltage to channel A0, then
//   setting and running to a break point at "__no_operation()". To view the
//   conversion results, use the debugger to inspect 'ADCResult'.
//   ACLK = 32kHz, MCLK = SMCLK = default DCO 1048576Hz, ADC12CLK = ADC12OSC
//
//
//                  MSP430xG461x
//                ---------------
//               |            XIN|-
//        Vin -->|P6.0/A0        | 32kHz
//               |           XOUT|-
//        REF -->|VeREF+         |
//               |               |
//
//   Y. Yang
//   Texas Instruments Inc.
//   September 2012 
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

volatile unsigned int ADCResult;

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                   // Stop watchdog
  P6SEL |= 0x01;                            // ADC sampled on P6.0 
  ADC12CTL0 = ADC12ON|SHT0_2;               // turn on ADC12
                                            // Sample and hold for 16 ADC12CLK cycles
  ADC12CTL1 = SHP;                          // Use sampling timer
  ADC12MCTL0 = SREF_2;                      // Vr+=VeREF+ (external) & Vr-=Avss
  ADC12CTL0 |= ENC;                         // Enable conversions

  while (1)
  {
    ADC12CTL0 |= ADC12SC;                   // Start conversions
    while (!(ADC12IFG & 0x0001));           // Waits until conversion is done
    ADCResult = ADC12MEM0;                  // Move result to ADCResult
    __no_operation();                       // SET BREAKPOINT HERE
  }
}
