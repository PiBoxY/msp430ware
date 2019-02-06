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
/*  Description: Toggle P1.0 using software and Timer0_A0 ISR. Timer0_A is       */
/*  configured for up mode, thus the timer overflows when TA0R counts to         */
/*  TA0CCR0. In this example, TA0CCR0 is loaded with 20000.                      */                   
/*                                                                               */
/****************************Timer_A0*********************************************/
/*                                                                               */
/* Requirements: Download to EEPROM                                              */
/*                                                                               */
/*                               +----L092---+                                   */
/*                               |*1      14 |                                   */
/*                               | 2      13 |                                   */
/*                               | 3      12 |                                   */
/*                               | 4      11 |                                   */
/*                               | 5      10 |                                   */
/*       P1.0 -Toggle Output  <- | 6       9 |                                   */
/*                               | 7       8 |                                   */
/*                               +-----------+                                   */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>  

int main(void)
{
  	WDTCTL = WDTPW + WDTHOLD;                // Stop WDT 

  	P1DIR |= BIT0 + BIT1;                    // P1.0 Output direction   
  	P1SEL0 |=  BIT1 ;                
  	P1SEL1 |=  BIT1 ;    
    
/************************/
/* Setup CCS            */
/* SMCLK = LFCLK        */
/************************/
    	CCSCTL0 = CCSKEY;                       // Unlock CCS
    	CCSCTL5 = 0;                            // Set the Dividers for SMCLK to 1
    	CCSCTL4 |= SELM_1;                      // Select LFCLK/VLO as the source for ACLK, MCLK, and SMCLK   
    	CCSCTL0_H |= 0xFF;                      // Lock CCS
/* Lock by writing to upper byte */
  
  	TA0CCTL0 = CCIE;                        // TA0CCR0 interrupt enabled
  	TA0CCR0 = 20000;				    // Set value for TA0CCR0
 	TA0CTL = TASSEL_2 + MC_1;               // SMCLK, upmode

  __bis_SR_register(LPM0_bits + GIE);         // Enter LPM0 w/ interrupt
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  	P1OUT ^= BIT0;                          // Toggle P1.0
}

