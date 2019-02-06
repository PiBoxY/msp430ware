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
//   MSP430x26x Demo - DMA0/1, Rpt'd single transfer to DAC12_0/1, Sin/Cos, TACCR1, XT2
//
//   Description: DMA0 and DMA1 are used to transfer a sine and cos look-up
//   table word-by-word as a repeating block to DAC12_0 and DAC12_1. The effect
//   is sine and cos wave outputs. Timer_A operates in upmode with TACCR1
//   loading DAC12_0 amd DAC12_1 on rising edge and DAC12_OIFG triggering next
//   DMA transfers. DAC12_0 and DAC12_1 are grouped for jitter-free operation.
//   ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//   //* An external watch crystal between XIN & XOUT is required for ACLK *//
//
//                MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |        DAC0/P6.6|--> ~ 10kHz sine wave
//            |        DAC1/P6.7|--> ~ 10kHz cos wave
//
//   B. Nisarga
//   Texas Instruments Inc.
//   September 2007
//   Built with CCE Version: 3.2.0
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// 12-bit Sine Lookup table with 32 steps
//------------------------------------------------------------------------------
const int Sin_tab[32] = { 2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
                          4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
                          2048, 1648, 1264,  910,  600,  345,  156,   39,
                             0,   39,  156,  345,  600,  910, 1264, 1648
                        };
//------------------------------------------------------------------------------
// 12-bit Cosine Lookup table with 32 steps
//------------------------------------------------------------------------------
const int Cos_tab[32] = { 1648, 1264,  910,  600,  345,  156,   39,    0,
                            39,   56,  345,  600,  910, 1264, 1648, 2048,
                          2447, 2831, 3185, 3495, 3750, 3939, 4056, 4095,
                           4056, 3939, 3750, 3495, 3185, 2831, 2447, 2048
                        };

int main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  ADC12CTL0 = REF2_5V + REFON;              // Internal 2.5V ref
  TACCR0 = 13600;                           // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TACLR + MC_1 + TASSEL_2;          // Up mode, SMCLK
  
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  TACCTL0 &= ~CCIE;                         // Disable timer interrupt
  __disable_interrupt();                    // Disable Interrupts
  DMACTL0 = DMA0TSEL_5 + DMA1TSEL_5;        // DAC12IFG triggers
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) Sin_tab);
                                            // Source block address
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &DAC12_0DAT);
                                            // Destination single address
  DMA0SZ = 0x020;                           // Block size
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, word-word
  __data20_write_long((uintptr_t) &DMA1SA,(uintptr_t) Cos_tab);
                                            // Source block address
  __data20_write_long((uintptr_t) &DMA1DA,(uintptr_t) &DAC12_1DAT);
                                            // Destination single address
  DMA1SZ = 0x020;                           // Block size
  DMA1CTL = DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, word-word
  DAC12_0CTL = DAC12LSEL_2 + DAC12IR + DAC12AMP_5 + DAC12IFG + DAC12ENC + DAC12GRP;
  DAC12_1CTL = DAC12LSEL_2 + DAC12IR + DAC12AMP_5 + DAC12IFG + DAC12ENC;
  TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
  TACCR1 = 01;                              // TACCR1 PWM Duty Cycle
  TACCR0 = 025-1;                           // Clock period of TACCR0
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, contmode

  while(1)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0
  }
}

// Delay for Ref to settle
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) TA0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  TACTL = 0;                                // Clear Timer_A control registers
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPMx, interrupts enabled
}
