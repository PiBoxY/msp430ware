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
//   MSP430x471xx Demo - XT2 Oscillator Fault Detection
//
//   Description: System runs normally in LPM3 with with basic timer clocked by
//   ACLK.  BT interrupt causes an exit from LPM3, and mainloop toggles P5.1.
//   If an XT2 oscillator fault occurs, NMI is requested, pausing execution and
//   flashing LED quickly until fault is resolved.  Assumed only XT2 as NMI
//   source - code does not check for other NMI sources.
//   MCLK = XT2 = 12MHz (upto 16 MHz), within ISRs MCLK = DCO freq~1048kHz
//   //* An external 12MHx crystal is required between XT2 and XT2OUT , and
//   an external 32kHz crystal is required between XIN and XOUT.  *//	
//
//                MSP430x471xx
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32KHz
//          --|RST          XOUT|-
//            |                 |
//            |            XT2IN|-
//            |                 | 12MHz
//            |           XT2OUT|-
//            |                 |
//            |             P5.1|--> LED
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  FLL_CTL0 |= XCAP11PF;                     // Configure load caps
  FLL_CTL1 &= ~XT2OFF;                      // Clear bit = HFXT2 on

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
  FLL_CTL1 |= SELM_XT2;                     // MCLK = XT2

  P1DIR |= BIT1;                           // P1.1 output direction
  P1SEL |= BIT1;                           // P1.1 option select

  BTCTL = BT_ADLY_1000;                     // One-second interrupt
  IE2 |= BTIE;                              // Enable Basic Timer interrupt

  P5DIR |= BIT1;                           // P5.1 = output direction

// An immediate Osc Fault will occur next
  IE1 |= OFIE;                              // Enable osc fault interrupt


  while(1)
  {
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts
    P5OUT ^= BIT1;                          // Toggle P5.1
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=NMI_VECTOR
__interrupt void nmi_ (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(NMI_VECTOR))) nmi_ (void)
#else
#error Compiler not supported!
#endif
{
  volatile unsigned int i;

  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSC Fault flag
    for (i = 0x47FF; i > 0; i--);           // Time for flag to set
    P5OUT ^= BIT1;                          // Toggle P5.2 using exclusive-OR
  }
  while (IFG1 & OFIFG);                     // OSC Fault flag still set?
  IE1 |= OFIE;                              // Enable Osc Fault
}


// Basic Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=BASICTIMER_VECTOR
__interrupt void basic_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(BASICTIMER_VECTOR))) basic_timer (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits+GIE); // Exit LPM3 upon ISR exit
}
