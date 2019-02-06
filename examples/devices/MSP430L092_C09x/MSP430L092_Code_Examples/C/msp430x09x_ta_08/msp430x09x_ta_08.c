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
//  Description: Use Timer CCRx units and overflow to generate
//  independent timing intervals. For demonstration, TA0CCR1 and TA0CCR2
//  output units are optionally selected with port pins P1.5 and P1.6
//  in toggle mode. As such, these pins will toggle when respective TA0CCRx
//  registers match the TA0R counter. Interrupts are also enabled with all
//  TA0CCRx units, software loads offset to next interval only - as long as
//  the interval offset is added to TACCRx, toggle rate is generated in
//  hardware. Timer0_A1 overflow ISR is used to toggle P1.0 with software.
//  Proper use of the TA0IV interrupt vector generator is demonstrated.
/*  An external digital clock running at 1Mhz is required for this example.     */
/****************************Timer_A0*********************************************/
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*                        +----L092---+                                          */
/*                        |*1      14 |     -> P1.6 TA0.1 output                 */
/*                        | 2      13 |     -> P1.5 TA0.2 output                 */
/*                        | 3      12 |                                          */
/*                        | 4      11 |                                          */
/*                        | 5      10 |                                          */
/*    1Mhz  CLKIN  ->     | 6       9 |                                          */
/*                        | 7       8 |                                          */
/*                        +-----------+                                          */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/
#include <msp430.h>  

int main(void)
{
  	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
   // Setup Port 1 to output for Timer A0
   // P1.5 = TA0.2 -> P1DIR.5 = 1; P1SEL0.5 = 1; P1SEL1.5 = 1;
   // P1.6 = TA0.1 -> P1DIR.6 = 1; P1SEL0.6 = 1; P1SEL1.6 = 0;
  
  	P1SEL0 |= BIT5 + BIT6;
  	P1SEL1 |= BIT5;
  	P1DIR  |= BIT0 + BIT5 + BIT6;
  
/************************/
/* Setup CCS            */
/* MCLK = SMCLK = HFCLK */
/* ACLK = HFCLK/32      */
/************************/
    
    	CCSCTL0 = CCSKEY;                       	// Unlock CCS  
    
    while (SFRIFG1 & OFIFG)                 	// Oscillator Flag(s)?
  {
    	CCSCTL7 = 0;                            	// Clear HF & LF OSC fault flags
    	SFRIFG1 = 0;                            	// Clear OFIFG  
  }
  
    	CCSCTL4 = SELS_2 ;    	// Select X-OSC as the source for SMCLK  
    	CCSCTL5 =  DIVS_0;     	// Set the Divider for SMCLK to 1
    	CCSCTL0_H |= 0xFF;                      	// Lock CCS
  
  	TA0CCTL0 = OUTMOD_4 + CCIE;               // TA0CCR0 toggle, interrupt enabled
  	TA0CCTL1 = OUTMOD_4 + CCIE;               // TACCR1 toggle, interrupt enabled
  	TA0CCTL2 = OUTMOD_4 + CCIE;               // TACCR2 toggle, interrupt enabled
  	TA0CTL = TASSEL_2 + MC_2 + TAIE;          // SMCLK, contmode, interrupt enabled

  __bis_SR_register(LPM0_bits + GIE);       	// Enter LPM3 w/interrupt
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A0 (void)
#else
#error Compiler not supported!
#endif
{
  TA0CCR0 += 4;                              	// Add Offset to TA0CCR0
}

// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer0_A1 (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(TA0IV, 14))        	// Efficient switch-implementation
  {
    case  2: TA0CCR1 += 16;                  	// Add Offset to TA0CCR1
             break;
    case  4: TA0CCR2 += 100;                 	// Add Offset to TA0CCR2
             break;
    case  6: break;                         	// Reserved
    case  8: break;                         	// Reserved
    case 10: break;                         	// Reserved   
    case 12: break;                         	// Reserved
    case 14: P1OUT ^= BIT0;                	// Timer0_A3 overflow
             break;
  }
}

