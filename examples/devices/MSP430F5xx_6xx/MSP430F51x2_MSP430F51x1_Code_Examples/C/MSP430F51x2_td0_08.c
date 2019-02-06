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
//  MSP430F51x2 Demo - TimerD0, Toggle P1.0;P2.3-5, Cont. Mode ISR, 32kHz ACLK
//
//  Description: Use TimerD0 CCRx units and overflow to generate four
//  independent timing intervals. For demonstration, CCR0, CCR1 and CCR2
//  output units are optionally selected with port pins P2.3, P2.4 and P2.5
//  in toggle mode. As such, these pins will toggle when respective CCRx
//  registers match the TDR counter. Interrupts are also enabled with all
//  CCRx units, software loads offset to next interval only - as long as
//  the interval offset is added to CCRx, toggle rate is generated in
//  hardware. TimerD0 overflow ISR is used to toggle P1.0 with software.
//  Proper use of the TDIV interrupt vector generator is demonstrated.
//  ACLK = TDCLK = 32kHz, MCLK = SMCLK = default DCO ~1.045MHz
//
//  As coded and with TDCLK = 32768Hz, toggle rates are:
//  P1.6= CCR0 = 32768/(2*4) = 4096Hz
//  P1.7= CCR1 = 32768/(2*16) = 1024Hz
//  P2.0= CCR2 = 32768/(2*100) = 163.84Hz
//  P1.0= overflow = 32768/(2*65536) = 0.25Hz
//
//               MSP430F51x2
//            -------------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//           |                   |
//           |         P1.6/TD0.0|--> CCR0
//           |         P1.7/TD0.1|--> CCR1
//           |         P2.0/TD0.2|--> CCR2
//           |               P1.0|--> Overflow/software (LED)
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
  // Configure ports
  P1SEL |= BIT6+BIT7;                       // P1.6,7 - option select
  P1DIR |= BIT6+BIT7;                       // P1.6,7 - outputs
  P2SEL |= BIT0;                            // P2.0 option select
  P2DIR |= BIT0;                            // P2.0 outputs
  P1DIR |= 0x01;                            // P1.0 - Outputs

  // XT1 configure
  PJSEL |= BIT4+BIT5;                       // Port select XT1
  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
  UCSCTL6 |= XCAP_3;                        // Internal load cap
  UCSCTL3 = 0;                              // FLL Reference Clock = XT1

  // Loop until XT1 & DCO stabilizes - In this case loop until XT1 and DCo settle
  do
  {
    UCSCTL7 &= ~(XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag  
  UCSCTL6 &= ~(XT1DRIVE_3);                 // Xtal is now stable, reduce drive strength
  UCSCTL4 |= SELA_0;                        // ACLK = LFTX1 (by default)
  
  // Configure TD0
  TD0CCTL0 = OUTMOD_4 + CCIE;               // CCR0 toggle, interrupt enabled
  TD0CCTL1 = OUTMOD_4 + CCIE;               // CCR1 toggle, interrupt enabled
  TD0CCTL2 = OUTMOD_4 + CCIE;               // CCR2 toggle, interrupt enabled
  TD0CTL0 = TDSSEL_1 + MC_2 + TDCLR + TDIE; // ACLK, contmode, clear TDR,
                                            // interrupt enabled

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}

// Timer0_D0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_D0_VECTOR
__interrupt void TIMER0_D0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_D0_VECTOR))) TIMER0_D0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  TD0CCR0 += 4;                             // Add Offset to CCR0
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
    case 0: break;                  
    case 2:  TD0CCR1 += 16;                  // Add Offset to CCR1
             break;
    case 4:  TD0CCR2 += 100;                 // Add Offset to CCR2
             break;
    case 6:  break;                          // CCR3 not used
    case 8:  break;                          // CCR4 not used
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

