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
//   MSP430x26x Demo - DMA2, Rpt'd single transfer to DAC1, 8-Bit Sine, TBCCR2, DCO
//
//   Description: DMA2 is used to transfer a sine look-up table byte-by-byte
//   as a repeating block to DAC12_1. The effect is a sine wave output. Timer_B
//   operates in upmode with TBCCR2 loading DAC12_1 on the rising edge, CCR2_CCIFG
//   triggering the next DMA2 transfer. Use external reference on VeRef+.
//   ACLK = 32kHz, MCLK = SMCLK = TBCLK = default DCO 1.045MHz
//
//                MSP430F261x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//   Ext Ref->|VeRef+  DAC1/P6.7|--> ~ 1kHz sine wave output
//
//
//   B. Nisarga
//   Texas Instruments Inc.
//   September 2007
//   Built with CCE Version: 3.2.0
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// 8-bit Sine Lookup table with 32 steps
//------------------------------------------------------------------------------
const char Sin_tab[32] = { 128, 152, 176, 198, 218, 234, 245, 253,
                           255, 253, 245, 234, 218, 198, 176, 152,
                           128, 103,  79,  57,  37,  21,  10,   2,
                             0,   2,  10,  21,  37,  57,  79, 103
                         };

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  __data20_write_long((uintptr_t) &DMA2SA,(uintptr_t) Sin_tab);
                                            // Source block address
  __data20_write_long((uintptr_t) &DMA2DA,(uintptr_t) &DAC12_1DAT);
                                            // Destination single address
  DMA2SZ = 0x020;                           // Block size
  DMACTL0 = DMA2TSEL_2;                     // TBCCR2 trigger
  DMA2CTL = DMASBDB + DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, byte-byte
  DAC12_1CTL = DAC12SREF_2 + DAC12RES + DAC12LSEL_3 + DAC12IR + DAC12AMP_5 + DAC12ENC;
  TBCCTL2 = OUTMOD_7;                       // Reset/set
  TBCCR2 = 01;                              // PWM Duty Cycle
  TBCCR0 = 032 - 1;                         // Clock period of TBCCR0
  TBCTL = TBSSEL_2 + MC_1;                  // SMCLK, up mode
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0
}
