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
//  MSP-FET430P140 Demo - ADC12, Sample A10 Temp and Convert to oC and oF
//
//  Description: A single sample is made on A10 with reference to internal
//  1.5V Vref. Software sets ADC12SC to start sample and conversion - ADC12SC
//  automatically cleared at EOC. ADC12 internal oscillator times sample
//  and conversion. In Mainloop MSP430 waits in LPM0 to save power until
//  ADC10 conversion complete, ADC12_ISR will force exit from any LPMx in
//  Mainloop on reti.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800kHz, ADC12CLK = ADC12OSC
// 
//  Uncalibrated temperature measured from device to devive will vary do to
//  slope and offset variance from device to device - please see datasheet.
//
//                MSP430Fx49
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |A10              |
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int long temp;
int long IntDegF;
int long IntDegC;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC12CTL0 = SHT0_8 + REFON + ADC12ON;
  ADC12CTL1 = SHP;                          // enable sample timer
  ADC12MCTL0 = SREF_1 + INCH_10;
  ADC12IE = 0x001;
  ADC12CTL0 |= ENC;

  while(1)
  {
    ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

//  oF = ((x/4096)*1500mV)-923mV)*1/1.97mV = x*761/4096 - 468
//  IntDegF = (ADC12MEM0 - 2519)* 761/4096
    IntDegF = (temp - 2519) * 761;
    IntDegF = IntDegF / 4096;

//  oC = ((x/4096)*1500mV)-986mV)*1/3.55mV = x*423/4096 - 278
//  IntDegC = (ADC12MEM0 - 2692)* 423/4096
    IntDegC = (temp - 2692) * 423;
    IntDegC = IntDegC / 4096;

    __no_operation();                       // << SET BREAKPOINT HERE
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
    temp = ADC12MEM0;                       // Move results, IFG is cleared
    __bic_SR_register_on_exit(CPUOFF);      // Clear CPUOFF bit from 0(SR)
}
