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
//  MSP-FET430P140 Demo - DMA0/1, Rpt'd Blk to DAC0/1, Sin/Cos, TACCR1, XT2
//
//  Description: DMA0 and DMA1 are used to transfer a sine and cos look-up table
//  word-by-word as a repeating block to DAC12_0 and DAC12_1. The effect is sine
//  and cos wave outputs. Timer_A operates in upmode with TACCR1 loading
//  DAC12_0 amd DAC12_1 on rising edge and DAC12_OIFG triggering next DMA0
//  DMA1 transfers. DAC12_0 and DAC12_1 are grouped for gitter-free operation.
//  ACLK= n/a, MCLK= SMCLK= TACLK= XT2= 8MHz
//  //* An external 8MHz XTAL on X2IN X2OUT is required for XT2CLK *//
//  //* Min Vcc required varies with MCLK frequency - refer to datasheet *//		
//  //* MSP430F169 Device Required *//
//
//               MSP430F169
//            -----------------
//        /|\|            XT2IN|-
//         | |                 | 8MHz
//         --|RST        XT2OUT|-
//           |                 |
//           |        DAC0/P6.6|--> ~ 10kHz sine wave
//           |        DAC1/P6.7|--> ~ 10kHz cos wave
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>


//------------------------------------------------------------------------------
// 12-bit Sine Lookup table with 32 steps
//------------------------------------------------------------------------------
static int Sin_tab[32] = {
        2048,
        2447,
        2831,
        3185,
        3495,
        3750,
        3939,
        4056,
        4095,
        4056,
        3939,
        3750,
        3495,
        3185,
        2831,
        2447,
        2048,
        1648,
        1264,
        910,
        600,
        345,
        156,
        39,
        0,
        39,
        156,
        345,
        600,
        910,
        1264,
        1648
};

static int Cos_tab[32] = {
        1648,
        1264,
        910,
        600,
        345,
        156,
        39,
        0,
        39,
        156,
        345,
        600,
        910,
        1264,
        1648,
        2048,
        2447,
        2831,
        3185,
        3495,
        3750,
        3939,
        4056,
        4095,
        4056,
        3939,
        3750,
        3495,
        3185,
        2831,
        2447,
        2048
};

int main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 &= ~XT2OFF;                       // XT2= HF XTAL

   do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0xFF; i > 0; i--);               // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  BCSCTL2 |= SELM_2 + SELS;                 // MCLK= SMCLK= XT2 (safe)
  ADC12CTL0 = REF2_5V + REFON;              // Internal 2.5V ref

  DMACTL0 = DMA0TSEL_5 + DMA1TSEL_5;        // DAC12IFG trigger
  DMA0SA = (int) Sin_tab;                   // Source block address
  DMA0DA = (unsigned int)&DAC12_0DAT;       // Destination single address
  DMA0SZ = 0x20;                            // Block size
  DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, word-word

  DMA1SA = (int) Cos_tab;                   // Source block address
  DMA1DA = (unsigned int)&DAC12_1DAT; 			// Destination single address
  DMA1SZ = 0x20;                            // Block size
  DMA1CTL = DMADT_4 + DMASRCINCR_3 + DMAEN; // Rpt, inc src, word-word

  DAC12_0CTL = DAC12LSEL_2 + DAC12IR + DAC12AMP_5 + DAC12IFG + DAC12ENC + DAC12GRP;
  DAC12_1CTL = DAC12LSEL_2 + DAC12IR + DAC12AMP_5 + DAC12IFG + DAC12ENC;

  CCTL1 = OUTMOD_3;                         // CCR1 set/reset
  CCR1 = 1;                                 // CCR1 PWM Duty Cycle	
  CCR0 = 25-1;                              // Clock period of CCR0
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode

  __bis_SR_register(CPUOFF);                // CPU off
}
