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
//   MSP430x26x Demo - XT2 Oscillator Fault Detection
//
//   Description: System runs normally in LPM3 with Timer A clocked by
//   ACLK.  TimerA0 interrupt causes an exit from LPM3, and toggles P1.0.
//   If an XT2 oscillator fault occurs, NMI is requested, pausing execution and
//   flashing LED quickly until fault is resolved.  Assumed only XT2 as NMI
//   source - code does not check for other NMI sources. MCLK is buffered on 
//   P5.4. If fault occurs for crystal osc sourcing the MCLK, MCLK is 
//   automatically switched to DCO for its clock source
//   MCLK = XT2 = 8MHz normally, within ISRs MCLK = DCO freq ~1025kHz
//   //* An external 8MHx crystal is required between XT2 and XT2OUT , and
//   an external 32kHz crystal is required between XIN and XOUT.  *//	
//
//             MSP430F261x/241x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32KHz
//          --|RST          XOUT|-
//            |                 |
//            |            XT2IN|-
//            |                 | 8MHz
//            |           XT2OUT|-
//            |                 |
//            |             P1.0|-->LED
//            |        P5.4/MCLK|-->MCLK = XT2 HFXTAL
//
//  B. Nisarga
//  Texas Instruments Inc.
//  September 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  BCSCTL1 &= ~XT2OFF;                       // Activate XT2 high freq xtal
  BCSCTL3 |= XT2S_2;                        // 3 – 16MHz crystal or resonator

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
  
  BCSCTL2 |= SELM_2;                        // MCLK = XT2 HF XTAL (safe)

  P5DIR |= 0x10;                            // P5.4= output direction
  P5SEL |= 0x10;                            // P5.4= MCLK option select
  
  //Initialize Timer A to generate one-second interrupt
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 32767;                           // Set CCR0 to 32767(ACLK=32.768Hz)                                      
  TACTL = TASSEL_1 + MC_1;                  // ACLK, up mode

  P1DIR |= 0x01;                            // Set P1.0 to output direction

// An immediate Osc Fault will occur next
  IE1 |= OFIE;                              // Enable osc fault interrupt


  while(1)
  {
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts
    P1OUT ^= 0x01;                          // Toggle P1.0
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
    P1OUT ^= 0x01;                          // Toggle P1.0
  }while (IFG1 & OFIFG);                    // OSC Fault flag still set?
  IE1 |= OFIE;                              // Enable Osc Fault
}


// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR 
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits+GIE); // Exit LPM3 upon ISR exit
}
