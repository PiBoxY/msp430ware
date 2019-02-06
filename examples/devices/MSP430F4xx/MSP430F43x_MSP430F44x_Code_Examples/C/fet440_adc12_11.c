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
//  MSP-FET430P440 Demo - ADC12, Sample A10 Temp and Convert to oC, TA1 Trigger
//
//  Description: Use the ADC12's integrated temperature sensor to measure
//  temperature. Sample time is set for 125 ADC12CLK cycles to allow 30us for
//  the integrated temperature sensor (see datasheet). ADC12 is operated in
//  repeat-single-channel mode with the sample and convert trigger sourced
//  from Timer_A CCR1. Timer_A is configured for continuous mode and is clocked
//  by SMCLK. TA1 is set for toggle mode triggering the ADC12 every 125ms.
//  The ADC12MEM0_IFG bit set at the end of each conversion triggers an ISR.
//  Normal mode is LPM0.
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCO = 32xACLK = 1048576Hz,
//  ADC12CLK = ADC12OSC
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//  Uncalibrated temperature measured from device to devive will vary do to
//  slope and offset variance from device to device - please see datasheet.
//
//                MSP430F449
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32KHZ
//          --|RST          XOUT|-
//            |                 |
//            |A10 (Temp)   P5.1|-->LED
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//*****************************************************************************
#include <msp430.h>

int long temp;
int long IntDegC;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P5DIR = 0x02;                             // Set P5.2 as output
  ADC12CTL1 = SHS_1 + SHP + CONSEQ_2;       // TA1, samp timer, rpt
  ADC12MCTL0 = SREF_1 + INCH_10;            // VRef+, A10
  ADC12IE = 0x01;                           // Enable ADC12MEM0 interrupt
  ADC12CTL0 = SHT0_7 + REFON + ADC12ON + ENC; // sample for 192 ADC12CLK cycles
  TACCTL1 = OUTMOD_4;                       // Toggle
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, cont-mode

  while(1)
  {
    __bis_SR_register(LPM0_bits + GIE);
    //  oC = ((x/4096)*1500mV)-986mV)*1/3.55mV = x*423/4096 - 278
    IntDegC = (temp - 2692) * 423;
    IntDegC = IntDegC / 4096;
    __no_operation();                       // SET BREAKPOINT HERE
  }
 }

// ADC12 Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
  temp = ADC12MEM0;                         // IFG is cleared
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
  P5OUT ^= 0x02;                            // Toggle LED
}
