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
//  MSP430x249 Demo - LFXT1 Oscillator Fault Detection
//
//  Description: System runs normally in LPM3 with Timer A clocked by 32kHz 
//  ACLK with a 1 second interrupt. P1.0 is normally toggled every 1 sec
//  inside Timer A0 interrupt. If an LFXT1 oscillator fault occurs, NMI is 
//  requested forcing exit from LPM3. And P1.0 is toggled rapidly by software as 
//  long as LFXT1 oscillator fault is present. Assuumed only LFXT1 as NMI source
//   - code does not check for other NMI sources. ACLK is buffered on P5.6.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//            MSP430x249
//         ---------------
//     /|\|            XIN|-
//      | |               | 32KHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//        |      P5.6/ACLK|-->ACLK = LFXT1
//
//  B. Nisarga
//  Texas Instruments Inc.
//  September 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>

volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |= 0x01;                            // P1.0 outputs - LED
  
  //Initialize Timer A to generate one-second interrupt
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 32767;                           // Set CCR0 to 32767(ACLK=32.768Hz)                                      
  TACTL = TASSEL_1 + MC_1;                  // ACLK, up mode                            

  P5DIR |= 0x40;                            // P5.6= output direction
  P5SEL |= 0x40;                            // P5.6= ACLK option select
  
  // An immedate Osc Fault will occur next
  IE1 |= OFIE;                              // Enable Osc Fault

  while(1) {
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3 w/ interrupts
    P1OUT ^= 0x01;                          // Toggle P1.0 using exclusive-OR
  }
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
  P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
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
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSC Fault flag
    for (i = 0x47FF; i > 0; i--);           // Time for flag to set
    P1OUT ^= 0x01;                          // Toggle P1.0
  }
  while (IFG1 & OFIFG);                     // OSC Fault flag still set?
  IE1 |= OFIE;                              // Enable Osc Fault
}



