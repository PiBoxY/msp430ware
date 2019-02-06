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
//  MSP430G22x0 Demo - P1 Interrupt from LPM4 with Internal Pull-up
//
//  Description: A hi/low transition on P1.5 will trigger P1_ISR which,
//  toggles P1.2. Normal mode is LPM4 ~ 0.1uA.
//  Internal pullup enabled on P1.5.
//  ACLK = VLO, MCLK = SMCLK = default DCO
//
//               MSP430G22x0
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |      R          |
//      --o--| P1.5-o      P1.2|-->LED
//     \|/
//
//  B. Nisarga
//  Texas Instruments, Inc
//  December 2011
//  Built with CCE Version: 5.1 and IAR Embedded Workbench Version: 5.4
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  
  /* Initialization Code */
  P1REN = 0x1B;                             // Terminate unavailable Port1 pins (P1.0/1/3/4) properly
                                            // Config as Input with pull-down enabled
  BCSCTL3 |= LFXT1S_2;                      // Select VLO as low freq clock     
  /* End Initialization Code */ 
  
  P1DIR = BIT2;                             // P1.2 output, else input
  P1OUT =  BIT5;                            // P1.5 set, else reset
  P1REN |= BIT5;                            // P1.5 pullup
  P1IE |= BIT5;                             // P1.5 interrupt enabled
  P1IES |= BIT5;                            // P1.5 Hi/lo edge
  P1IFG &= ~BIT5;                           // P1.5 IFG cleared

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= BIT2;                            // P1.2 = toggle
  P1IFG &= ~BIT5;                           // P1.5 IFG cleared
}

