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
//  MSP-FET430P140 Demo - ADC12, Using 10 External Channels for Conversion
//
//  Description: This example shows how to perform A/D conversion on up to 10
//  external channels by showing that channels A8 and A9 can be used for
//  conversion of external signals when not using these channels as external 
//  reference inputs. A single sequence of conversions is performed
//  - one conversion on A8 and then one conversion on A9. Each conversion uses AVcc
//  and AVss for the references. The conversion results are stored in ADC12MEM0
//  and ADC12MEM1 respectively and are moved to 'results[]' upon completion of
//  the sequence. Test by applying voltages to pins VeREF+ for A8 and
//  VREF-/VeREF- for A9, then setting and running to a break point at the
//  "_BIC..." instruction in the ISR. To view the conversion results, open a
//  watch window in debugger and view 'results' or view ADC12MEM0 and ADC12MEM1 
//  in an ADC12 SFR window.
//  This can run even in LPM4 mode as ADC has its own clock
//  *NOTE*  When using channels A8 and A9 for external signals, internal
//  references must be used for the conversions. Refer to figure 17-1 in the
//  MSP430x1xx Family User's Guide.
//
//
//                 MSP430F149
//            -----------------
//           |                 |
//   Vin1 -->|VeREF+           |
//   Vin2 -->|VREF-/VeREF-     |
//
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

static volatile unsigned int results[2];    // Needs to be global in this example
                                            // Otherwise, the compiler removes it
                                            // because it is not used for anything.

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  ADC12CTL0 = ADC12ON+MSC+SHT0_15;          // Turn on ADC12, set sampling time
  ADC12CTL1 = SHP+CONSEQ_1;                 // Use sampling timer, single sequence
  ADC12MCTL0 = INCH_8;                      // ref+=AVcc, channel = A8
  ADC12MCTL1 = INCH_9+EOS;                  // ref+=AVcc, channel = A9, end seq.
  ADC12IE = 0x02;                           // Enable ADC12IFG.1
  ADC12CTL0 |= ENC;                         // Enable conversions

  while(1)
  {
  ADC12CTL0 |= ADC12SC;                     // Start conversion
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, Enable interrupts
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
  __bic_SR_register_on_exit(LPM0_bits);     // Clear LPM0, SET BREAKPOINT HERE
}
