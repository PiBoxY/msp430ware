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
//  MSP430FR57xx Demo - ADC10, Sample A1, 1.5V Ref, TA0.1 Trig, Set P1.0 if A1>0.5V
//
//  Description: A1 is sampled 16/second (ACLK/1024) with reference to 1.5V.
//  Timer_A is run in upmode and TA0.1 is used to automatically trigger
//  ADC10 conversion. Internal oscillator times sample (16x) and conversion 
//  (13x).Inside ADC10_ISR if A1 > 0.5V, P1.0 is set, else reset. Normal mode 
//  is LPM3.
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//               MSP430FR5739
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P1.1/A1     P1.0 |--> LED
//           |                 |
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;                           // Clear P1.0
  
  // XT1 Setup   
  PJSEL0 |= BIT4 + BIT5; 
 
  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers 
  CSCTL4 |= XT1DRIVE_0; 
  CSCTL4 &= ~XT1OFF;
  
  do
  {
    CSCTL5 &= ~XT1OFFG;
                                            // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG; 
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  // Configure ADC pins
  P1SEL1 |= BIT1; 
  P1SEL0 |= BIT1;   

  // Configure ADC10 
  ADC10CTL0 |= ADC10ON + ADC10MSC;          // ADC10ON
  ADC10CTL1 |= ADC10SHS_1 + ADC10CONSEQ_2;  // rpt single ch; TA0.1 trig sample start
  ADC10CTL2 |= ADC10RES;                    // 10-bit conversion results
  ADC10MCTL0 |= ADC10INCH_1 + ADC10SREF_1;  // A0 ADC input select; Vref=1.5V

  // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT                                          
  REFCTL0 |= REFVSEL_0+REFON;               // Select internal ref = 1.5V 
                                            // Internal Reference ON   
  __delay_cycles(400);                      // ref delay  
  ADC10CTL0 |= ADC10ENC;                     // ADC10 Enable
  ADC10IE |= ADC10IE0;                       // Enable ADC conv complete interrupt
  
  // ADC conversion trigger signal - TimerA0.0 (32ms ON-period)
  TA0CCR0 = 1024-1;                          // PWM Period
  TA0CCR1 = 512-1;                           // TA0.1 ADC trigger
  TA0CCTL1 = OUTMOD_4;                       // TA0CCR0 toggle
  TA0CTL = TASSEL_1 + MC_1 + TACLR;          // ACLK, up mode

  __bis_SR_register(LPM3_bits + GIE);        // Enter LPM3 w/ interrupts
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
    case 12: if (ADC10MEM0 < 0x155)          // ADC10MEM = A0 < 0.5V?
                P1OUT &= ~BIT0;              // Clear P1.0 LED off
             else
                P1OUT |= BIT0;               // Set P1.0 LED on                                           
             break;                          // Clear CPUOFF bit from 0(SR)                         
    default: break; 
  }   
}


