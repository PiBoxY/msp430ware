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
//  C430F614x Demo - ADC10, Sample A5, 1.5V Ref, TA0.1 Trig, Ultra-Low Pwr
//
//  Description: A5 is sampled 1024/second (32xACLK) with Vref = internal 1.5V.
//  All activity is interrupt driven with proper usage of MSP430 low-power
//  modes, ADC10 and Vref demonstrated. Timer_A0 used in upmode to drive ADC10 
//  conversion (continuous mode can also be used).
//  Inside of TA0_ISR software will enable ADC10 and internal reference and
//  allow > 75us delay for Vref to stabilize prior to sample start. Sample
//  start is automatically triggered by TA0 every 32 ACLK cycles. ADC10_ISR
//  will disable ADC10 and Vref and compare ADC10 conversion code. Internal
//  oscillator times sample (16x) and conversion (13x). If A5 > 0.2V, P1.0 is
//  set, else reset. Standy mode is LPM3.
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//                    +-----(0.9766us)---------\\------------------>+
//    TA0_ISR        TA0.1      ADC10_ISR             TA0_ISR        TA0.1
//  -----+------------+------------+-----------\\------+------------+----->
//   Enable ADC    Trigger ADC  Disable ADC, 
//   and Vref                   Ref; Compare
//       +-( ~75us--->+
//
//               C430F6147
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//       >---|P2.5/A5     P1.0 |--> LED
//           |             P3.2|-->TA0.1 (ADC trigger signal)

//
//  G. Larmore
//  Texas Instruments Inc.
//  June 2012
//  Built with CCS v5.2 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  // Port I/O config
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;
  P3DIR |= BIT2;                            // P3.2/TA0.1 output
  P3SEL |= BIT2;                            // TA0.1 options select
  
  // Configure XT1
  P5SEL |= BIT0 + BIT1;                     // Port select XT1
  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
  UCSCTL6 |= XCAP_3;                        // Internal load cap
  UCSCTL3 = 0;                              // FLL Reference Clock = XT1
  
  // Loop until XT2 & DCO stabilizes - In this case loop until XT1 and DCo settle
  do
  {
    UCSCTL7 &= ~(XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag  
  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive strength
  UCSCTL4 |= SELA_0;                        // ACLK = LFTX1 (by default)
  
  // Configure ADC
  ADC10CTL1 |= ADC10SHS_1;                  // TA0.1 trigger sample start
  ADC10CTL2 |= ADC10RES;                    // 10-bit conversion results
  ADC10MCTL0 |= ADC10INCH_5;                // A5 ADC input select;  
 
  // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT                                          
  REFCTL0 |= REFVSEL_0;                     // Select internal ref = 1.5V 

  // Configure Timer (TA0.1 - see device specific d/s) -> ADC conversion trigger
  TA0CCR0 = 32-1;                           // Timer period
  TA0CCTL0 = CCIE;
  TA0CCR1 = 32-1;                           // TA0.1 toggle period
  TA0CCTL1 = OUTMOD_4;               		// TA0CCR1 toggle
  TA0CTL = TASSEL_1 + MC_1 + TACLR;         // ACLK, up mode (reset on TA0.0)
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
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
  ADC10CTL0 &= ~ADC10ENC;                    // ADC10 ENC reset in order to 
                                             // modify ADC10CTL and MCTL registers
  switch(__even_in_range(ADC10IV,12))
  {
    case  0: break;                          // No interrupt
    case  2: break;                          // conversion result overflow
    case  4: break;                          // conversion time overflow
    case  6: break;                          // ADC10HI
    case  8: break;                          // ADC10LO
    case 10: break;                          // ADC10IN
    case 12: 
      ADC10CTL0 &= ~ADC10ON;                 // ADC10 disabled completely
      ADC10MCTL0 &= ~ADC10SREF_1;            // Clear internal Reference Vref+ select
      REFCTL0 &= ~REFON;                     // Turn off internal shared reference
      if (ADC10MEM0 < 0x88)                  // ADC10MEM0 = A5 < 0.2V?
         P1OUT &= ~0x01;                     // Clear P1.0 LED off
      else
         P1OUT |= 0x01;                      // Set P1.0 LED on                                         
      break;                                 // Clear CPUOFF bit from 0(SR)                         
    default: break; 
  }   
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  ADC10MCTL0 |= ADC10SREF_1;                // reference = Vref+
  ADC10CTL0 |= ADC10SHT_2 + ADC10ON;        // 16xADC clock cycles, ADC10ON 
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT                                          
  REFCTL0 |= REFON;                         // Internal Reference ON  
  __delay_cycles(80);                       // Reference settling time
  ADC10IE |= ADC10IE0;                      // Enable ADC conv complete interrupt
  ADC10CTL0 |= ADC10ENC;                    // ADC10 enable 
}
