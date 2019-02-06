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
//*****************************************************************************
//  MSP-FET430P140 Demo - ADC12, Single Channel Rpt Mode, TA1 as Sample Trigger
//
//  Description: Sample and convert A0 using Timer_A as sample trigger in
//  Pulse Sample mode. Put "Num_of_Results" ADC12MEM0 values in results[]
//  and Light LED (P1.0) when done.
//
//               MSP430F149
//             ---------------
//            |               |
//     Vin -->|P6.0/A0        |
//            |               |
//
//  H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//*****************************************************************************
#include <msp430.h>

#define   Num_of_Results   512
int results[Num_of_Results] = {0};

void ADC_Init(void);

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Disable the Watchdog.
  ADC_Init();                               // Initialize ADC12
  ADC12CTL0 |= ENC;                         // Start conversion
  __bis_SR_register(LPM0_bits);             // Enter LPM0
}


void ADC_Init(void)
{
  P1DIR = 0xff;                             // set port to outputs
  P1OUT = 0;                                // reset port outputs
  P6SEL |= 0x01;                            // select A0 input
  ADC12CTL0 = ADC12ON+SHT0_1+REF2_5V+REFON; // Setup ADC12
  ADC12CTL1 = SHP+CONSEQ_2+SHS_1;           // Timer triggers sampling
  ADC12MCTL0 = INCH_0 + SREF_1;
  ADC12IE = 0x0001;                         // Enable ADC12IFG.0


  TACCR0 = 1500;                            // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_1 | MC_1;                  // TACLK = ACLK, Up mode.
  __bis_SR_register(LPM3_bits + GIE);       // Wait for delay, Enable interrupts
  TACCTL0 &= ~CCIE;                         // Disable timer

  P2SEL |= BIT3;                            // Set for Timer A1
  P2DIR |= 0x08;
  TACCR0 = 7;                               // Init TACCR0 w/ sample prd=CCR0+1
  TACCR1 = 4;                               // Trig for ADC12 sample & convert
  TACCTL1 = OUTMOD_3;                       // Set/reset
  TACTL = TACLR | MC_1 | TASSEL_1;          // ACLK, clear TAR, up mode
}

// Timer_A0 Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void ta0_isr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) ta0_isr (void)
#else
#error Compiler not supported!
#endif
{
  TACTL = 0;
  LPM3_EXIT;                                // Exit LPM3 on return
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
  static unsigned int index = 0;

  results[index++] = ADC12MEM0;             // Move results

  if (index == 512)
  {
    ADC12CTL0 &= ~ENC;                      // Stop conversion
    index = 0;
    P1OUT |= 0x01;
    __bis_SR_register(LPM3_bits);           // Enter LPM3
  }
}

