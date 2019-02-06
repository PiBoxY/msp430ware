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
//  MSP430FR57xx Demo - ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
//
//  Description: Sample A1 64 times with AVcc ref. Software writes to ADC10SC
//  to trigger sample burst. In Mainloop MSP430 waits in LPM0 to save power
//  until ADC10 conversion burst complete, DMA_IFG will force exit from LPM0
//  in Mainloop on reti. ADC10 internal oscillator times sample period (16x)
//  and conversion (13x). DMA transfers conversion result in ADC10MEM0 to array
//  ADC_Result. P1.0 set at start of conversion burst, reset on completion.
//
//                MSP430FR5739
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.1/A1          |
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

unsigned int ADC_Result[64];
unsigned char i;
unsigned int ADC_Result_sum;
unsigned int ADC_Result_Average;

int main(void)
{  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  // Configure ADC pins
  P1SEL1 |= BIT1; 
  P1SEL0 |= BIT1; 

  // Configure ADC10 - pulse sample mode; software trigger; 
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10MSC; // 16ADCclks, ADC on
  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_2;     // pulse sample mode, rpt single ch
  ADC10CTL2 = ADC10RES;                     // 10-bits of resolution
  ADC10MCTL0 = ADC10INCH_1;                 // AVCC ref, A1
  
  // Configure DMA (ADC10IFG trigger)
  DMACTL0 = DMA0TSEL__ADC10IFG;                     // ADC10IFG trigger
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) &ADC10MEM0);
                                            // Source single address  
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &ADC_Result[0]);
                                            // Destination array address  
  DMA0SZ = 64;                              // 64 conversions 
  // Rpt, inc dest, word access, 
  DMA0CTL = DMADT_4 + DMADSTINCR_3 + DMAEN + DMAIE + DMALEVEL; 
                                         // enable int after 64 conversions  
  while(1)
  {
    ADC10CTL0 |= ADC10ENC + ADC10SC;        // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    __no_operation();                       // For debug only

    ADC_Result_sum = 0x0;                   // clear accumulate register
    for(i=0;i<64;i++)                       // BREAKPOINT HERE; View ADC_Result 
      ADC_Result_sum += ADC_Result[i];    
    
    ADC_Result_Average = ADC_Result_sum>>6; // Average of 64 conversions results  
    __delay_cycles(50000);                   // delay before next 64 conversions
  }  
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(DMAIV,16))
  {
    case  0: break;                          // No interrupt
    case  2: 
      // 64 conversions complete
      ADC10CTL0 &= ~ADC10ENC;
      __bic_SR_register_on_exit(CPUOFF);     // exit LPM
      break;                                 // DMA0IFG
    case  4: break;                          // DMA1IFG
    case  6: break;                          // DMA2IFG
    case  8: break;                          // Reserved
    case 10: break;                          // Reserved
    case 12: break;                          // Reserved
    case 14: break;                          // Reserved
    case 16: break;                          // Reserved
    default: break; 
  }   
}
  
