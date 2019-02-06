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
//  MSP430F51x2 Demo - TimerD0/1-TEC, Synchronizing timers, Normal timer mode
//
//  Description: This code example shows how to configure the TEC module for
//  synchronizing two timer instances (TD0 and TD1). In this example code, TD0 is 
//  the master timer and TD1 is the slave instance. The clock configuration of 
//  the master is applied to the slave. The counter length, high res enable
//  and timer mode settings of the master are applied to the slave as well. 
//
//  Note: TDxCCR0 registers of both master and slave timer instance (TD0 and 
//      TD1 in this case) should be the same value
//
//  ACLK = REFO = 32kHz; SMCLK = MCLK = DCO Clock = 12MHz; TD0 = TD1 = 10kHz
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P1.6/TD0.0|--> CCR0 - 50% dutycycle 
//            |       P2.1/TD1.0|--> CCR0 - 50% dutycycle
//            |                 |
//            |       P1.7/TD0.1|--> CCR1 - 20% dutycycle
//            |       P2.2/TD1.1|--> CCR1 - 40% dutycycle
//            |                 |
//            |       P2.0/TD0.2|--> CCR2 - 60% dutycycle
//            |       P2.3/TD1.2|--> CCR2 - 80% dutycycle
//
//  B. Nisarga
//  Texas Instruments Inc.
//  Nov 2010
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************

#include <msp430.h>

void SetVcoreUp (unsigned int level);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Increase Vcore setting to level1 to support fsystem=12MHz
  // NOTE: Change core voltage one level at a time..
  SetVcoreUp (0x01);    
  
  // Configure DCO = 12Mhz
  UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                  // Disable the FLL control loop
  UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  // Worst-case settling time for the DCO when the DCO range bits have been
  // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  // UG for optimization.
  // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
  __delay_cycles(375000);
	
  // Loop until XT1 & DCO fault flag is cleared
  do
  {
    UCSCTL7 &= ~(XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                            // Clear XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  // Configure TD0, TD1 port pins
  P1SEL |= BIT6 + BIT7;                     // P1.6/TD0.0, P1.7,TD0.1, options select
  P1DIR |= BIT6 + BIT7;                     // Output direction
  P2SEL |= BIT0 + BIT1 + BIT2 + BIT3;       // P2.0/TD0.2, P2.1/TD1.0, P2.2/TD1.1, P2.3/TD1.2, options select
  P2DIR |= BIT0 + BIT1 + BIT2 + BIT3;       // Output direction
  
  // Configure Master Timer Instance - TimerD0
  // Freq = 100kHz, TD0.1/TD0.2 PWM Period = 10us
  TD0CCR0 = 120;                            // TD0CCR0 PWM period = 2*10us = 20us
  TD0CCTL0 |= OUTMOD_4;                     // TD0CCR0, Toggle, 50% dutycycle
  TD0CCR1 = 24;                             // 20% dutycycle
  TD0CCTL1 |= OUTMOD_7;                     // TD0CCR1, Reset/Set    
  TD0CCR2 = 48;                             // 40% dutycycle
  TD0CCTL2 |= OUTMOD_7;                     // TD0CCR2, Reset/Set
  TD0CTL0 = TDSSEL_2;                       // TD0, SMCLK
  TD0CTL1 = TDCLKM_0;                       // TD0 clock = external clock source = SMCLK  

  // Configure Slave Timer Instance - TimerD1 PWM outputs 
  TD1CCR0 = 120;                            // TD1CCR0 PWM period = 2*10us = 20us
  TD1CCTL0 |= OUTMOD_4;                     // TD1CCR0, Toggle, 50% dutycycle
  TD1CCR1 = 72;                             // 60% dutycycle
  TD1CCTL1 |= OUTMOD_7;                     // TD1CCR1, Reset/Set    
  TD1CCR2 = 96;                             // 80% dutycycle
  TD1CCTL2 |= OUTMOD_7;                     // TD1CCR2, Reset/Set
  TD1CTL1 = TDCLKM_2;                       // TD1 clock = Auxiliary clock source from master timer instance
  
  // Syncronizing master (TD0) and slave (TD1) timer instances
  TEC1XCTL2 |= TECAXCLREN;		    // Enable synchronized clear by enabling Aux clear of slave timer 
  TD0CTL0 |= MC_1 + TDCLR;                  // Clear timer counter, Up mode, Start timers  

  __bis_SR_register(LPM0_bits);             // Enter LPM0
  __no_operation();                         // For debugger  
  
}


void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;              
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
