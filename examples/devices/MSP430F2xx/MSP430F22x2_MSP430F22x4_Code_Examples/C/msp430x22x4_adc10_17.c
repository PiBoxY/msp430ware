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
//   MSP430F22x4 Demo - ADC10, DTC Sample A0 -> TA1, AVcc, HF XTAL
//
//   Description: Use DTC to sample A0 with reference to AVcc continously and
//   directly transfer code to Timer_A CCR1 output unit. Timer_A has been
//   configured for 10-bit PWM mode. CCR1 duty cycle is automatically
//   proportional to ADC10 A0. No CPU resources are required and in this
//   example the CPU is turned off. ADC10 A0 sampling and transfer to TA1 done
//   continuously and automatically by the DTC.
//   MCLK = SMCLK = HF XTAL = 8MHz, ACLK = (HF XTAL)/8
//   As coded, ADC10CLK = ACLK/8 = 125kHz, and each A0 sample and transfer to
//   TA1 requires 77 ADC10CLK. With an 8MHz HF XTAL, loop transfer rate is
//   125k/77 = 1623/second.
//   //* An external HF XTAL on XIN XOUT is required for ACLK *//
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 | HF XTAL (3 – 16MHz crystal or resonator)
//          --|RST          XOUT|-
//            |                 |
//        >---|P2.0/A0      P1.2|--> TACCR1 - 0-1024 PWM
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   October 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 |= XTS + DIVA_3;                  // ACLK = (LFXT1 = HF XTAL)/8
  BCSCTL3 |= LFXT1S1;                       // LFXT1S1 = 3-16Mhz
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSC fault flag
    i = 0xFF;                               // i = Delay
    while (i--);                            // Additional delay to ensure start
  }
  while (OFIFG & IFG1);                     // OSC fault flag set?
  BCSCTL2 |= SELM_3 + SELS;                 // MCLK = SMCLK = LFXT1

  P1DIR |= 0x04;                            // P1.2 = output
  P1SEL |= 0x04;                            // P1.2 = TA1 outputs
  TACCR0 = 1024 - 1;                        // PWM Period
  TACCTL1 = OUTMOD_7;                       // TACCR1 reset/set
  TACCR1 = 512;                             // TACCR1 PWM Duty Cycle
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, upmode
  ADC10CTL1 = ADC10DIV_7 + ADC10SSEL_1 + CONSEQ_2; // ACLK
  ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON;   // 64x, multi conv.
  ADC10AE0 |= 0x01;                         // P2.0 ADC option select
  ADC10DTC0 |= ADC10CT;                     // Continous transfers
  ADC10DTC1 = 0x001;                        // 1 conversion location
  ADC10SA = (unsigned int)&TACCR1;          // Data trasfer location
  ADC10CTL0 |= ENC + ADC10SC;               // Start sampling
  __bis_SR_register(LPM0_bits);             // CPU not required
}
