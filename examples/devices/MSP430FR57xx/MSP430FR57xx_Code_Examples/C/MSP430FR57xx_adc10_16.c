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
//  MSP430FR57xx Demo - ADC10, Sample A10 Temp and Convert to oC and oF
//
//  Description: A single sample is made on A10 (temperature) with reference to
//  internal 1.5V Vref. Software sets ADC10SC to start sample and conversion
//  - ADC10SC automatically cleared at EOC. ADC10 internal oscillator times
//  sample and conversion. In Mainloop MSP430 waits in LPM4 to save power until
//  ADC10 conversion complete, ADC12_ISR will force exit from any LPMx in
//  Mainloop on reti.
//  ACLK = n/a, MCLK = SMCLK = default DCO , ADC10CLK = ADC10OSC
//
//  Uncalibrated temperature measured from device to devive will vary due to
//  slope and offset variance from device to device - please see datasheet.
//
//  This code example show how to use the TLV calibrated data to measure temperature.
//
//                MSP430FR5739
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |A10          P1.0|-->LED
//
//   F. Chen
//   Texas Instruments Inc.
//   Dec. 2012
//   Built with IAR Embedded Workbench Version: 5.51.1 & Code Composer Studio V5.2.1
//******************************************************************************
#include <msp430.h>

#define CALADC10_15V_30C  *((unsigned int *)0x1A1A)   // Temperature Sensor Calibration-30 C
                                                      //See device datasheet for TLV table memory mapping
#define CALADC10_15V_85C  *((unsigned int *)0x1A1C)   // Temperature Sensor Calibration-85 C


volatile long temp;
volatile long IntDegF;
volatile long IntDegC;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Configure ADC10 - Pulse sample mode; ADC10SC trigger
  ADC10CTL0 = ADC10SHT_8 + ADC10ON;         // 16 ADC10CLKs; ADC ON,temperature sample period>30us
  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0;     // s/w trig, single ch/conv
  ADC10CTL2 = ADC10RES;                     // 10-bit conversion results
  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_10;  // ADC input ch A10 => temp sense  
  
  // Configure internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_0+REFON;               // Select internal ref = 1.5V 
                                            // Internal Reference ON   
  ADC10IE |=ADC10IE0;                       // enable the Interrupt request for a completed ADC10_B conversion
  
  __delay_cycles(400);                      // Delay for Ref to settle
  
  
  while(1)
  {
    ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
	
	__bis_SR_register(LPM4_bits + GIE);     // LPM4 with interrupts enabled
    __no_operation();
	
	// Temperature in Celsius
    // The temperature (Temp, °„C)=
    IntDegC = (temp - CALADC10_15V_30C) *  (85-30)/(CALADC10_15V_85C-CALADC10_15V_30C) +30;

    // Temperature in Fahrenheit
    // Tf = (9/5)*Tc + 32
    IntDegF = 9*IntDegC/5+32; 

    __no_operation();                       // SET BREAKPOINT HERE
  
  }
}

// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADC10IV,12))
  {
    case  0: break;                          // No interrupt
    case  2: break;                          // conversion result overflow
    case  4: break;                          // conversion time overflow
    case  6: break;                          // ADC10HI
    case  8: break;                          // ADC10LO
    case 10: break;                          // ADC10IN
    case 12: temp = ADC10MEM0;         
             __bic_SR_register_on_exit(LPM4_bits);                                              
             break;                          // Clear CPUOFF bit from 0(SR)                         
    default: break; 
  }  
}

