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
//  MSP430F51x2 Demo - TimerD0, Toggle P1.0, Overflow ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TimerD0 overflow ISR.
//  In this example an ISR triggers when TD overflows. Inside the TD
//  overflow ISR P1.0 is toggled. Toggle rate is approximatlely 16Hz.
//  Proper use of the TDIV interrupt vector generator is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TDCLK = default DCO ~1.045MHz
//
//           MSP430F51x2
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//  B. Nisarga
//  Texas Instruments Inc.
//  Dec 2009
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 output
  TD0CTL0 = TDSSEL_2 + MC_2 + TDCLR + TDIE; // SMCLK, contmode, clear TDR
                                            // enable interrupt

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

// Timer0_D1 Interrupt Vector (TDIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_D1_VECTOR
__interrupt void TIMER0_D1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_D1_VECTOR))) TIMER0_D1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TD0IV,30))
  {
    case  0: break;                          // No interrupt
    case  2: break;                          // CCR1 not used
    case  4: break;                          // CCR2 not used
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: break;
    case 16: P1OUT ^= 0x01;                  // overflow
             break;
    case 18: break;                          // Clock fail low
    case 20: break;                          // Clock fail high
    case 22: break;                          // Hi-res freq locked
    case 24: break;                          // Hi-res freq unlocked
    case 26: break;                          // reserved
    case 28: break;                          // reserved
    case 30: break;                          // reserved       
    default: break; 
  }
}

