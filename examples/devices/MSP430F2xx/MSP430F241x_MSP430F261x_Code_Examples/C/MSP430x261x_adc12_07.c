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
//  MSP430x26x Demo - ADC12, Repeated Single Channel Conversions
//
//  Description: This example shows how to perform repeated conversions on a
//  single channel using "repeat-single-channel" mode.  AVcc is used for the
//  reference and repeated conversions are performed on Channel A0. Each
//  conversion result is moved to an 8-element array called results[].  Test by
//  applying a voltage to channel A0, then running. Open a watch window in 
//  debugger and view the results. Set Breakpoint1 in the index increment line  
//  to see the array value change sequentially and Breakpoint2 to see the entire
//  array of conversion results in "results[]" for the specified Num_of_Results.
//  This can run even in LPM4 mode as ADC has its own clock
//
//           MSP430F261x/241x
//             ---------------
//            |               |
//     Vin -->|P6.0/A0        |
//            |               |
//
//
//  B. Nisarga
//  Texas Instruments Inc.
//  September 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************

#include <msp430.h>

#define   Num_of_Results   8

volatile unsigned int results[Num_of_Results];  // Needs to be global in this
                                              // example. Otherwise, the
                                              // compiler removes it because it
                                              // is not used for anything.

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL |= 0x01;                            // Enable A/D channel A0
  ADC12CTL0 = ADC12ON+SHT0_8+MSC;           // Turn on ADC12, set sampling time
  ADC12CTL1 = SHP+CONSEQ_2;                 // Use sampling timer, set mode
  ADC12IE = 0x01;                           // Enable ADC12IFG.0
  ADC12CTL0 |= ENC;                         // Enable conversions
  ADC12CTL0 |= ADC12SC;                     // Start conversion
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0,Enable interrupts
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
  static unsigned int index = 0;

  results[index] = ADC12MEM0;               // Move results
  index = (index+1)%Num_of_Results;         // Increment results index, modulo; Set Breakpoint1 here
  if (index == 0)
    __no_operation();                       // Set Breakpoint2 here
  
}
