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
//   MSP430x26x Demo - DMA0, Repeated single transfer to DAC0, Sine Output, TACCR1, DCO
//
//   Description: DMA0 is used to transfer a sine look-up table word-by-word
//   as a repeating block to DAC12_0. The effect is sine wave output. Timer_A
//   operates in upmode with TACCR1 loading DAC12_0 on rising edge and
//   DAC12_0IFG triggering next DMA1 transfer. DAC12_0 uses internal 1.5V
//   reference.
//   ACLK = 32kHz, MCLK = SMCLK = TACLK = default DCO 1.045MHz
//
//                MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |        DAC0/P6.6|--> ~ 1kHz sine wave
//            |                 |
//
//   B. Nisarga
//   Texas Instruments Inc.
//   September 2007
//   Built with CCE Version: 3.2.0
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

//-------------------------------------------------------------------------------
// 12-bit Sine Lookup table with 32 steps
//-------------------------------------------------------------------------------

const int Sin_tab[32] = { 2048, 2447, 2831, 3185, 3495, 3750, 3939, 4056,
                          4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
                          2048, 1648, 1264,  910,  600,  345,  156,   39,
                             0,   39,  156,  345,  600,  910, 1264, 1648
                        };

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  ADC12CTL0 = REFON;                        // Internal reference
  __data20_write_long((uintptr_t) &DMA0SA,(uintptr_t) Sin_tab);
                                            // Source block address
  __data20_write_long((uintptr_t) &DMA0DA,(uintptr_t) &DAC12_0DAT);
                                            // Destination single address
  DMA0SZ = 0x020;                           // Block size
  DMACTL0 = DMA0TSEL_5;                     // DAC12IFG trigger
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, word-word
  DAC12_0CTL = DAC12LSEL_2 + DAC12IR + DAC12AMP_5 + DAC12IFG + DAC12ENC;
  TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
  TACCR1 = 01;                              // TACCR1 PWM Duty Cycle
  TACCR0 = 032-1;                           // Clock period of TACCR0
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, contmode
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0
}



