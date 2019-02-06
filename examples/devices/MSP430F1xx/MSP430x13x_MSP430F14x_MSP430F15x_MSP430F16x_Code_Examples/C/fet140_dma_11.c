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
//*******************************************************************************
//  MSP-FET430P140 Demo - DMA0/1, ADC12 A10 Block Xfer to MPY/RAM, TBCCR1, DCO
//
//  Description: A 0x20 word block of data is sampled and recorded into RAM
//  from the ADC12 channel 10 the integrated temperature sensor using the
//  Record() function. Timer_B CCR1 begins the ADC12 sample period, CCR0 the hold
//  and conversion start. Timer_B operates in the up mode with CCR0 defining the
//  sample period.
//  DMA0 will automatically transfer each ADC12 conversion code to the HW MPY
//  which will preform MPY x 0x1000 - this will rotate the operand 12-bits to
//  the left. DMA1 uses the multiplier ready trigger to move the lower byte of
//  the upper operand of the multiplier result to a RAM block. At the end of the
//  transfer block, DMA1 issues an interrupt.
//  The purpose of this example is to show how multiple DMA triggers can be
//  combined.
//  P1.0 is toggled during DMA transfer only for demonstration purposes.
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k
//  //* Final Production MSP430F16x(x) Device Required *//
//
//               MSP430F169
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |A10          P1.0|-->LED
//
//  M. Buccini / A. Dannenberg
//  Texas Instruments Inc.
//  November 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//******************************************************************************

#include <msp430.h>

void Record(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  while (1)                                 // Repeat forever
  {
    Record();
    __no_operation();                       // << SET BREAKPOINT HERE
  }
}

// Record ADC12 A10 channel data and store in RAM using DMA0
void Record(void)
{
  // setup modules
  volatile unsigned int i;
  ADC12MCTL0 = 0x01A;
  ADC12IFG = 0x00;
  ADC12CTL1 = SHS_3 + CONSEQ_2;             // S&H TB.OUT1, rep. single chan
  ADC12CTL0 = REF2_5V + REFON + ADC12ON + ENC;      // VRef ADC12 on, enabled
  for (i = 0xFFF; i > 0; i--);              // Time VRef to settle

  TBCCR0 = 100;                             // Init TBCCR0 w/ sample prd
  TBCCR1 = 100 - 30;                        // Trigger for ADC12 SC
  TBCCTL1 = OUTMOD_7;                       // Reset OUT1 on EQU1, set on EQU0
 
  DMACTL0 = DMA1TSEL_11 + DMA0TSEL_6;       // DMA1=MPY Ready, DMA0=ADC12IFGx

  DMA0SA = (unsigned int)&ADC12MEM0;        // Src address = ADC12 module
  DMA0DA = (unsigned int)&OP2;              // Dst address = multiplier
  DMA0SZ = 1;                               // Size in words
  DMA0CTL = DMADT_4 + DMAEN;                // Sng rpt, config

  DMA1SA = (unsigned int)&RESHI;            // Src address = multiplier
  DMA1DA = 0x220;                           // Dst address = RAM memory
  DMA1SZ = 0x20;                            // Size in bytes
  DMA1CTL = DMADSTINCR_3 + DMASBDB + DMAIE + DMAEN;   // Sng, config

  MPY = 0x1000;                             // MPY first operand

  // start recording and enter LPM0
  P1OUT |= 0x01;                            // Set P1.0 (LED On)
  TBCTL = TBSSEL_2 + MC_1 + TBCLR;          // SMCLK, clear TBR, up mode
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrups

  // power-down MSP430 modules
  ADC12CTL1 &= ~CONSEQ_2;                   // Stop conversion immediately
  ADC12CTL0 &= ~ENC;                        // Disable ADC12 conversion
  ADC12CTL0 = 0;                            // Switch off ADC12 & ref voltage
  TBCTL = 0;                                // Disable Timer_B
  P1OUT &= ~0x01;                           // Clear P1.0 (LED Off)
}

// DMA interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DACDMA_VECTOR
__interrupt void DACDMA_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DACDMA_VECTOR))) DACDMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
  DMA1CTL &= ~DMAIFG;                       // Clear DMA1 interrupt flag
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0 on reti
}
