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
//***********************************************************************
//  MSP-FET430P140 Demo - Timer_A0, Capture of ACLK/8 using SMCLK
//
//  Description: Input capture of ACLK/8 on P1.1(TA0)
//  Run to breakpoint at the _NOP() instruction to see 16 capture
//  values and the differences.
//  ACLK = 32768Hz, MCLK = SMCLK = default ~800kHz
//  //* An external watch crystal on XIN XOUT is required for ACLK *//	
//
//                MSP430F149
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |        P2.0/ACLK|---+
//            |                 |   |
//            |         P1.1/TA0|<--+
//            |                 |
//            |             P1.0|--->LED

//
//  H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

unsigned int new_cap=0;
unsigned int old_cap=0;
unsigned int cap_diff=0;

unsigned int diff_array[16];                // RAM array for differences
unsigned int capture_array[16];             // RAM array for captures
unsigned char i=0;
unsigned char count = 0;

int main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  for (i=0; i<20000; i++)                   // Delay for crystal stabilization
  {
  }
  P1DIR = 0x01;                             // Set P1.0 out,1.1 input dir
  P1OUT &= ~0x01;                           // LED off
  P1SEL = 0x02;                             // Set P1.1 to TA0
  P2DIR = 0x01;                             // P2.0-ACLK
  P2SEL |= 0x01;
  BCSCTL1 |= DIVA_3;                        // ACLK/8

  CCTL0 = CM_1 + SCS + CCIS_0 + CAP + CCIE; // Rising edge + CCI0A (P1.1)
                                            // + Capture Mode + Interrupt

  TACTL = TASSEL_2 + MC_2;                  // SMCLK + Continuous Mode

  __bis_SR_register(LPM0_bits + GIE);       // LPM0 + Enable global ints
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) TimerA0 (void)
#else
#error Compiler not supported!
#endif
{
   new_cap = TACCR0;
   cap_diff = new_cap - old_cap;

   diff_array[i] = cap_diff;            // record difference to RAM array
   capture_array[i++] = new_cap;
   if (i == 16)
   {
     i = 0;
     P1OUT ^= 0x01;                         // Toggle P1.0 using exclusive-OR
   }
   old_cap = new_cap;                       // store this capture value
   count ++;
   if (count == 32)
   {
     count = 0;
     __no_operation();                      // SET BREAKPOINT HERE
   }

}
