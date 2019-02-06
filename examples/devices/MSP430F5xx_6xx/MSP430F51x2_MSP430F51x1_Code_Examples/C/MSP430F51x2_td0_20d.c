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
//  MSP430F51x2 Demo - TimerD0, Hi-Res Free Running mode, PWM TD0.0-2, TDCLK = 256Mhz
//
//  Description: This code example shows how to configure TimerD in Hi-Res 
//  Free Running mode. The TD0HCTL1 register with the TDHCLKTRIMx, TDHCLKSRx and 
//  TDHCLKRx bit settings are configured with the calibration data in the TLV 
//  table. 
//
//  SMCLK = MCLK = DCOclock = default DCO; TDCLK(TLV Cal Data)= 256Mhz
//  Note: TDHMx=01(16x) should be used to use 256Mhz TimerD TLV calibration data 
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P1.6/TD0.0|--> CCR0 - 50% dutycycle; PWM period = (2 x 128)/256MHz~ 1us = 1MHz
//            |       P1.7/TD0.1|--> CCR1 - 25% dutycycle; ON period ~ 32/256MHz~ 125ns
//            |       P2.0/TD0.2|--> CCR2 - 75% dutycycle; ON period ~ 96/256MHz~ 375ns
//
//  F. Chen
//  Texas Instruments Inc.
//  December 2012
//  Built with CCS v5.2.1 and IAR Embedded Workbench Version: 5.51.1
//******************************************************************************
#include <msp430.h>

#define CALTDH0CTL1_256        *((unsigned int *)0x1A36)
// Function Definitions
void SetVcoreUp (unsigned int level);

int main(void)
{
//  struct s_TLV_Timer_D_Cal_Data * pTD0CAL;  // Structure initialized in tlv.h
//  unsigned char bTD0CAL_bytes;                  
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Configure TimerD in Hi-Res Free Running Mode
 // Get_TLV_Info(TLV_TIMER_D_CAL, 0, &bTD0CAL_bytes, (unsigned int **) &pTD0CAL); 
                                            //Get TimerD0 Cal Values (instance 0)
 // if(bTD0CAL_bytes == 0x0)
 // {
      // No TimerD free running cal data found
  //    while(1);                             // Loop here
 // }  
 // TD0HCTL1 = pTD0CAL->TDH0CTL1_256;         // Read the 256Mhz TimerD TLV Data     
  TD0HCTL1 = CALTDH0CTL1_256;                 // Read the 256Mhz TimerD TLV Data  
  TD0CTL1 |= TDCLKM_1;                      // Select Hi-res local clock
  TD0HCTL0 = TDHEN + TDHM_1;                // CALEN=0 => free running mode; enable Hi-res mode 
                                            // THDMx = 01 => 16x
  // Configure TD0.x GPIO pins
  P1SEL |= BIT6 + BIT7;                     // P1.6,7 option select
  P1DIR |= BIT6 + BIT7;                     // P1.6,7 output
  P2SEL |= BIT0;                            // P2.0 options select
  P2DIR |= BIT0;                            // P2.0 output   
  
  // Configure the CCRx blocks
  TD0CCTL0 = OUTMOD_4;                      // CCR0 toggle
  TD0CCR0 = 128-1;                          // PWM Period/2
  TD0CCTL1 = OUTMOD_6;                      // CCR1 toggle/set
  TD0CCR1 = 32;                             // CCR1 PWM duty cycle
  TD0CCTL2 = OUTMOD_6;                      // CCR2 toggle/set
  TD0CCR2 = 96;                             // CCR2 PWM duty cycle
  TD0CTL0 |= MC_1 + TDCLR;                  // up-mode, clear TDR, Start timer
  
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0
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

