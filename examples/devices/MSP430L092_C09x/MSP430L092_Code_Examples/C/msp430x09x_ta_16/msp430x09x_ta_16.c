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
//  Description: This program generates two PWM outputs on P1.5,6 using
//  Timer0_A3 configured for up mode. The value in TA0CCR0, 512-1, defines the PWM
//  period and the values in TA0CCR1 and TA0CCR2 the PWM duty cycles.
//  Using ~1MHz SMCLK as TACLK.
//
/****************************Timer_A0*********************************************/
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*              +----L092---+                                                    */
/*              |*1      14 |     -> P1.6 TA0.1 output                           */
/*              | 2      13 |     -> P1.5 TA0.2 output                           */
/*              | 3      12 |                                                    */
/*              | 4      11 |                                                    */
/*              | 5      10 |                                                    */
/*              | 6       9 |                                                    */
/*              | 7       8 |                                                    */
/*              +-----------+                                                    */
/*                                                                               */
/*  D.Dang/D.Archbold/D.Szmulewicz                                               */
/*  Texas Instruments Inc.                                                       */
/*  Built with IAR Version 5.10.4                                                */
/*********************************************************************************/

#include <msp430.h>  


int main(void)
{
  	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

 // P1.1 = SMCLK -> P1DIR.1 = 1; P1SEL0.1 = 1; P1SEL1.1 = 1;
 // P1.5 = TA0.2 -> P1DIR.5 = 1; P1SEL0.5 = 1; P1SEL1.5 = 1; 500Hz
 // P1.6 = TA0.1 -> P1DIR.6 = 1; P1SEL0.6 = 1; P1SEL1.6 = 0; 50Hz
  
  	P1SEL0 |= BIT1 + BIT5 + BIT6;
  	P1SEL1 |= BIT1 + BIT5;
  	P1DIR  |= BIT1 + BIT5 + BIT6;  
  
/***************************/
/* Setup CCS               */
/* SMCLK = HFCLK/1  ~1MHz  */
/***************************/

    	CCSCTL0 = CCSKEY;                         // Unlock CCS
    while (SFRIFG1 & OFIFG)                 	  // Oscillator Flag(s)?
  {
    	CCSCTL7 = 0;                              // Clear HF & LF OSC fault flags
    	SFRIFG1 = 0;                              // Clear OFIFG  
  }  
    	CCSCTL4 = SELS_0;                  	  // Select HFCLK/DCO as the source for SMCLK   
    	CCSCTL5 = DIVS_0;                	  // Set the Dividers for SMCLK to 1
    	CCSCTL0_H |= 0xFF;                        // Lock CCS
/* Lock by writing to upper byte */
  
  
  	TA0CCR0 = 512 - 1;                        // PWM Period
  	TA0CCTL1 = OUTMOD_7;                      // TA0CCR1 reset/set
  	TA0CCR1 = 384;                            // TA0CCR1 PWM duty cycle
  	TA0CCTL2 = OUTMOD_7;                      // TA0CCR2 reset/set
  	TA0CCR2 = 128;                            // TA0CCR2 PWM duty cycle
  	TA0CTL = TASSEL_2 + MC_1;                 // SMCLK, up mode

  __bis_SR_register(CPUOFF);                	// Enter LPM0
}

