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
//   MSP430F22x4 Demo - Timer_B, PWM TB1-2, Up/Down Mode, HF XTAL ACLK
//
//   Description: This program generates two PWM outputs on P4.1,2 using
//   Timer_B configured for up/down mode. The value in TBCCR0, 128, defines
//   the PWM period/2 and the values in TBCCR1-2 the PWM duty cycles. Using
//   HF XTAL ACLK as TBCLK, the timer period is HF XTAL/256 with a 75%
//   duty cycle on P4.1 and 25% on P4.2.
//   ACLK = MCLK = TBCLK = HF XTAL
//   //* HF XTAL REQUIRED AND NOT INSTALLED ON FET *//
//   //* Min Vcc required varies with MCLK frequency - refer to datasheet *//
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 | HF XTAL (3 – 16MHz crystal or resonator)
//          --|RST          XOUT|-
//            |                 |
//            |         P4.1/TB1|--> TBCCR1 - 75% PWM
//            |         P4.2/TB2|--> TBCCR2 - 25% PWM
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   April 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P4DIR |= 0x06;                            // P4.1 and P4.2 output
  P4SEL |= 0x06;                            // P4.1 and P4.2 TB1/2 otions

  BCSCTL1 |= XTS;                           // LFXT1 = HF XTAL
  BCSCTL3 = LFXT1S1;                        // LFXT1S1 = 3-16Mhz
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSC fault flag
    i = 0xFF;                               // i = Delay
    while (i--);                            // Additional delay to ensure start
  }
  while (OFIFG & IFG1);                     // OSC fault flag set?
  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1

  TBCCR0 = 128;                             // PWM Period/2
  TBCCTL1 = OUTMOD_6;                       // TBCCR1 toggle/set
  TBCCR1 = 32;                              // TBCCR1 PWM Duty Cycle
  TBCCTL2 = OUTMOD_6;                       // TBCCR2 toggle/set
  TBCCR2 = 96;                              // TBCCR2 PWM duty cycle
  TBCTL = TBSSEL_1 + MC_3;                  // ACLK, updown mode

  __bis_SR_register(LPM0_bits);             // CPU off
}
