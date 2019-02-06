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
//  MSP430F525x Demo - ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C
//
//  Description: Set ADC10 and the integrated temperature sensor to detect
//  temperature gradients. The temperature sensor output voltage is sampled
//  ~ every 62ms and then compared with the defined delta values using an ISR.
//  (ADC10OSC)/32 determines sample time which needs to be greater than
//  ~1us for temperature sensor.
//  ADC10 is operated in repeat-single channel mode with the sample and
//  convert trigger sourced from Timer_A CCR1. The ADC10IFG at the end
//  of each converstion will trigger an ISR.
//  ACLK = REFO, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC
//
//                MSP430F525x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |A10 (Temp)   P1.0|-->LED
//            |             P1.2|-->TA0.1 (ADC trigger signal)
//
//
//   E. Chen
//   Texas Instruments Inc.
//   August 2013
//   Built with IAR Embedded Workbench v5.52 & Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

static unsigned int FirstADCVal;            // holds 1st ADC result

#define ADCDeltaOn       6                  // ~ 2 Deg C delta for LED on

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog

  // Configure ADC10 - Pulse Sample Mode, TimerA0.0 Trigger
  ADC10CTL0 = ADC10SHT_3 + ADC10ON;         // 32 ADC10CLKs; ADC ON
  ADC10CTL1 = ADC10SHP + ADC10SHS_1 + ADC10CONSEQ_2; // TA0.1 trig., rpt single ch 
  ADC10CTL2 |= ADC10RES;                    // 10-bit conversion results
  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_10 ; // A10, internal Vref+
  
  // Configure internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT                                          
  REFCTL0 |= REFVSEL_2+REFON;               // Select internal ref = 2.5V 
                                            // Internal Reference ON   
  
  // Configure TA0 to provide delay for reference settling ~75us
  TA0CCR0 = 80;                             // Delay to allow Ref to settle
  TA0CCTL0 |= CCIE;                         // Compare-mode interrupt.
  TA0CTL = TASSEL_2 + MC_1;                 // TACLK = SMCLK, Up mode.
  __bis_SR_register(CPUOFF + GIE);          // LPM0, TA0_ISR will force exit
  TA0CCTL0 &= ~CCIE;                        // Disable timer Interrupt
  
  ADC10IE |= ADC10IE0;                      // Enable ADC conv complete interrupt  
  ADC10CTL0 |= ADC10ENC;
  
  // Configure TimerA0.0 as ADC conversion tirgger (period ~ 62ms)
  TA0CCR0 = 1024-1;                          // PWM Period
  TA0CCR1 = 512-1;                           // TA0.1 PWM
  TA0CCTL1 = OUTMOD_4;                       // TA0CCR0 toggle
  TA0CTL = TASSEL_1 + MC_1 + TACLR;          // ACLK, up mode
  
  P1DIR |= BIT2;                             // P1.2/TA0.1 output
  P1SEL |= BIT2;                             // Output TA0.1 ADC trigger signal
  
  while (ADC10CTL1 & ADC10BUSY);             // First conversion?
  FirstADCVal = ADC10MEM0;                   // Read out 1st ADC value
  P1OUT = 0x00;                              // Clear P1
  P1DIR = BIT0;                              // P1.0 as output
  __bis_SR_register(LPM3_bits + GIE);        // Enter LPM3 w/ interrupt
  __no_operation();                          // For debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
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
    case 12: if (ADC10MEM0 >= FirstADCVal + ADCDeltaOn)          
                P1OUT |= 0x01;               // Set P1.0 LED on
             else
                P1OUT &= ~0x01;              // Clear P1.0 LED off                                             
             break;                          // Clear CPUOFF bit from 0(SR)                         
    default: break; 
  }   
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  TA0CTL = 0;
  LPM0_EXIT;                                // Exit LPM0 on return
}

