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
//  C430F614x Demo - ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO
//
//  Description: Sample A11 (AVCC/2) 32x, Int reference, and transfer
//  code via DMA to Flash. Software writes to ADC10SC to trigger sample burst.
//  Each ADC10 sample & convert = (32+13)*2/SMCLK = 90/SMCLK = 75us
//  Enough time is provided between ADC10 conversions for each word moved by
//  the DMA to Flash to program. DMA transfers ADC10 code to Flash 1800h-183Fh.
//  In the Mainloop, the MSP430 waits in LPM0 during conversion and programming,
//  DMA_ISR will force exit from LPM0 in Mainloop.
//
//               C430F6147
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |A10              |
//
//  G. Larmore
//  Texas Instruments Inc.
//  June 2012
//  Built with CCS v5.2 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Configure ADC10; 
  ADC10CTL0 = ADC10SHT_3 + ADC10MSC + ADC10ON;// 32ADCclks, ADC on
  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_2 + ADC10SSEL_3 + ADC10DIV_1; // SMCLK/2    
                                            // Sampling timer, rpt single ch
  ADC10CTL2 = ADC10RES;                     // 10-bit resolution
  ADC10MCTL0 = ADC10INCH_11 + ADC10SREF_1;  // Vref+, A10
  
 // Configure internal reference
  while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
  REFCTL0 |= REFVSEL_0+REFON;               // Select internal ref = 1.5V 
                                            // Internal Reference ON   
  __delay_cycles(75);                       // Delay (~75us) for Ref to settle

  // Configure DMA (ADC10IFG trigger)
  DMACTL0 = DMA0TSEL_24;                    // ADC10IFG trigger
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) &ADC10MEM0);
                                            // Source single address  
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) 0x1800);
                                            // Destination array address  
  DMA0SZ = 0x20;                            // 32 conversions 
  DMA0CTL = DMADT_4 + DMADSTINCR_3 + DMAEN + DMAIE; // Rpt, inc dest, word access, 
                                            // enable int after 32 conversions   

  while(1)
  {
    FCTL3 = FWKEY;                          // Lock = 0
    FCTL1 = FWKEY + ERASE;                  // Erase bit = 1
    *(unsigned int *)0x1800 = 0;            // Dummy write to erase Flash seg
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    FCTL1 = FWKEY + WRT;                    // Write bit = 1
    ADC10CTL0 |= ADC10ENC + ADC10SC;        // Start sampling
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    FCTL3 = FWKEY + LOCK;                   // Lock = 1
    __no_operation();                       // << SET BREAKPOINT HERE
    __delay_cycles(5000);                   // Delay between conversions
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
      // 32 conversions complete
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

