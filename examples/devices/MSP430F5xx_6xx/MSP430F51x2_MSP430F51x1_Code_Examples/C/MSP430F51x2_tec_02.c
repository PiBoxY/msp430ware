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
//  MSP430F51x2 Demo - TimerD0/1-TEC, External Clear, Level trigger
//
//  Description: This code example shows how to configure the TEC module for
//  controlling the timerD PWM dutycycle from external clear input. In this 
//  example code, TD0 is used to generate the external clear pulses. TD1 is 
//  configired to output PWM of 25%, 50% and 75%. With the EXTCLR clear event 
//  on TD1 input, the TD1 timer counter (TDR) gets reset and this modifies the 
//  dutycycle of all the TD1 PWM channels (TD1.0, TD1.1, TD1.2)
//  TD0 = 1KHz, 15% dutycycle; TD1 = 10kHz, 25%,50%,75% dutycycle 
//
//  Note: EXTCLR event in this case is High level on EXTCLR input. As long as, 
//       the input on EXTCLR is high, the timer counter will be held in reset
//
//  ACLK = REFO = 32kHz; SMCLK = MCLK = DCO Clock = 12MHz; 
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P2.0/TD0.2|--> CCR2 -| ~15% duty cycle; External Clear Pulses
//            |                 |          |  
//            |     P2.7/TEC1CLR|<---------| External Clear Input 
//            |                 |
//            |       P2.1/TD1.0|--> CCR0 - 50% dutycycle (Period/2)
//            |       P2.2/TD1.1|--> CCR1 - 25% dutycycle
//            |       P2.3/TD1.2|--> CCR2 - 75% dutycycle
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
  
  // Configure TD0 port pins
  P2SEL |= BIT0;                            // P2.0/TD0.2 options select
  P2DIR |= BIT0;                            // P2.0 output  
  
  // Configure TimerD0 to combine CCR0/1 block to control TD0CCR2 PWM output
  // TD0CCR2 PWM output = External clear pulses for TD1
  // Freq = 1kHz, Period = 1ms; dutycycle = 15% => ON period = 150us
  TD0CTL1 |= TD2CMB + TDCLKM_0;             // Combine TD0CCR1 and TD0CCR2
  TD0CCR0 = 32;
  TD0CCR1 = 20;  
  TD0CCR2 = 25;
  TD0CCTL2 |= OUTMOD_7;                     // TD0CCR2, Reset/Set
  TD0CTL0 |= TDSSEL_1 + MC_1 + TDCLR;       // TD0, ACLK, up mode, clear timer

  // Configure TD1 port pins
  P2SEL |= BIT1 + BIT2 + BIT3;
  P2DIR |= BIT1 + BIT2 + BIT3;
  
  // Configure TimerD1 to 10kHz with PWMs dutycycles = 25%, 50%, 75%
  TD1CCR0 = 1200;                           // Period = 1200*(1/12Mhz) = 0.1ms, 50% PWM
  TD1CCTL0 |= OUTMOD_4;                     // TD1CCR0, Toggle
  TD1CCR1 = 300;                            // Dutycycle = 25%
  TD1CCTL1 |= OUTMOD_7;                     // TD1CCR1, Reset/Set  
  TD1CCR2 = 900;                            // Dutycycle = 75%
  TD1CCTL2 |= OUTMOD_7;                     // TD1CCR2, Reset/Set
  TD1CTL0 = TDSSEL_2 + MC_1 + TDCLR;        // SMCLK, upmode, clear TDR  
  
  // Configure TD1 TEC External Clear
  // Need to physically connect P2.0/TD0.2 to P2.7/TEC1CLR
  P2SEL |= BIT7;
  TEC1XCTL2 = TECEXCLRLVS + TECEXCLRPOL + TECEXCLREN;   
                                            // High Level trigger, ext clear enable  
  
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
