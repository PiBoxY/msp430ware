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
//  Description: Toggle P1.0 using software and Timer0_A1 ISR. Toggles every
//  1000 SMCLK cycles. SMCLK provides clock source for TA0CLK.
//  During the Timer0_A1 ISR, P1.0 is toggled and 1000 clock cycles are added to
//  TA0CCR0. Timer0_A1 ISR is triggered every 1000 cycles. CPU is normally off and
//  used only during Timer0_A1 ISR. Proper use of the TA0IV interrupt vector generator
//  is demonstrated.
/****************************Timer_A0*********************************************/
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*                               +----L092---+                                   */
/*                               |*1      14 |                                   */
/*                               | 2      13 |                                   */
/*                               | 3      12 |                                   */
/*                               | 4      11 |                                   */
/*                               | 5      10 |                                   */
/*     P1.0 -Toggle Output  <-   | 6       9 |                                   */
/*     P1.1 SMCLK~ 1Mhz     <-   | 7       8 |                                   */
/*                               +-----------+                                   */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>  

int main(void)
{
  	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  	P1DIR |= BIT0 + BIT1;                     // P1.0 output direction       
  	P1SEL0 |=  BIT1 ;                
  	P1SEL1 |=  BIT1 ;    
    
/***************************/
/* Setup CCS               */
/* SMCLK = HFCLK           */
/***************************/
    	CCSCTL0 = CCSKEY;                       	// Unlock CCS
    while (SFRIFG1 & OFIFG)                 	// Oscillator Flag(s)?
  {
    	CCSCTL7 = 0;                            	// Clear HF & LF OSC fault flags
    	SFRIFG1 = 0;                            	// Clear OFIFG  
  }  
    	CCSCTL4 = SELS_0;                      	// Select HFCLK/DCO as the source for SMCLK   
    	CCSCTL5 = DIVS_0;                      	// Set the Dividers for SMCLK to 1
    	CCSCTL0_H |= 0xFF;                     	// Lock CCS
/* Lock by writing to upper byte */
  
  
  	TA0CCTL1 = CCIE;                          // TA0CCR1 interrupt enabled
  	TA0CCR1 = 1000;					// Set value for TA0CCR1
  	TA0CTL = TASSEL_2 + MC_2;                 // SMCLK, Contmode

  __bis_SR_register(LPM0_bits + GIE);       	// Enter LPM0 w/ interrupt
}

// Timer0_A1 Interrupt Vector (TA0IV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt  void Timer0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) Timer0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(TA0IV, 14))       	// Efficient switch-implementation
  {
    case  2:                                	// TA0CCR1
      P1OUT ^= BIT0;                        	// Toggle P1.0
      TA0CCR1 += 1000;                        	// Add Offset to TA0CCR1
      break;
    case  4: break;                         	// TACCR2 not used
    case  6: break;                         	// Reserved
    case  8: break;                         	// Reserved
    case 10: break;                         	// Reserved   
    case 12: break;                         	// Reserved
    case 14: break;                         	// Overflow not used
  }
}
