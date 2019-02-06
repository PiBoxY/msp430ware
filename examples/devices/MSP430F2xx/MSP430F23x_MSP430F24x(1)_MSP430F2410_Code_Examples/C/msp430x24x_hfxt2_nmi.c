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
//   MSP430x24x Demo - Basic Clock, MCLK Sourced from HF XTAL XT2, NMI
//
//   Description: Proper selection of an HF XTAL XT2 for MCLK is shown with
//   enabled OSC fault interrupt - if HF XTAL fails, NMI interrupt is requested.
//   HF XTAL can only source MCLK when OSC fault is clear. For demonstration
//   purposes P1.0 is set during OSC fault. MCLK/10 is on P1.1.
//   ACLK = n/a; MCLK = HFXTAL XT2
//   //* HF XTAL XT2 NOT INSTALLED ON FET *//
//   //* Min Vcc required varies with MCLK frequency - refer to datasheet *//
//
//                MSP430xG249
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |            XT2IN|-
//            |                 | HF XTAL (400kHz - 16MHz)
//            |           XT2OUT|-
//            |                 |
//            |             P1.1|-->MCLK/10
//            |             P1.0|-->LED
//
//  B. Nisarga
//  Texas Instruments Inc.
//  September 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  P1DIR |= 0x03;                            // P1.0,1 = output direction
  P1OUT &= ~0x01;                           // P1.0 = reset
  
  BCSCTL1 &= ~XT2OFF;                       // Activate XT2 high freq xtal
  BCSCTL3 |= XT2S_2;                        // 3 – 16MHz crystal or resonator
  IE1 |= OFIE;                              // Enable osc fault interrupt

  while (1)
  {
    P1OUT |= 0x02;                          // P1.1 = 1
    P1OUT &= ~0x02;                         // P1.1 = 0
  }
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=NMI_VECTOR
__interrupt void NMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(NMI_VECTOR))) NMI_ISR (void)
#else
#error Compiler not supported!
#endif
{
  volatile unsigned int i;

  P1OUT |= 0x01;                            // P1.0 = set

  BCSCTL2 &= ~SELM_3;                       // Ensure MCLK runs from DCO
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSCFault flag
    for (i = 0xFF; i > 0; i--);             // Time for flag to set
  }
  while (IFG1 & OFIFG);                     // OSCFault flag still set?

  BCSCTL2 |= SELM_2;                        // MCLK = XT2 HF XTAL (safe)

  for (i=0; i>20000; i--);                  // Delay for LED on inside ISR
  P1OUT &= ~0x01;                           // P1.0 = reset
}
