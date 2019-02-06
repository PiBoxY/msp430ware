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
//  MSP430F51x2 Demo - TimerD0, Dual Input capture mode, Normal timer mode, 
//                                Period measurement
//  Description: This code example implements input capture in dual capture 
//  mode using TimerD in normal timer mode. Dual capture mode of TimerD is 
//  demonstrated here. This example captures two consequent rising edges and 
//  computes the Period of the input signal. The TD0.0 input signal is generated 
//  by the Timer A0 module (TA0.0) and the freq of this PWM input signal can be 
//  changed by modifying the TA0CCR0 register. 
//  
//  ACLK = LFXT1 = 32kHz; SMCLK = MCLK = 12MHz 
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |       P1.6/TD0.0|<-- CCI0A <-| 
//            |       P3.7/TA0.0|--> CCR0 -->| 
//            |                 |
//            |             P3.0|--> MCLK = 12MHz DCO
//            |             P3.1|--> SMCLK = 12MHz DCO
//            |             P3.2|--> ACLK = 32kHz LFXT1
//
//  B. Nisarga
//  Texas Instruments Inc.
//  Nov 2010
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

unsigned int REdge1, REdge2, Period;

// Function Definition
void Port_Mapping(void);
void SetVcoreUp (unsigned int level);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // Configure XT1
  PJSEL |= BIT4+BIT5;                       // Port select XT1  
  UCSCTL6 &= ~(XT1OFF);                     // XT1 On
  UCSCTL6 |= XCAP_3;                        // Internal load cap  
  // Loop until XT1 fault flag is cleared
  do
  {
    UCSCTL7 &= ~XT1LFOFFG;                  // Clear XT1 fault flags
  }while (UCSCTL7&XT1LFOFFG);               // Test XT1 fault flag
  
  // Increase Vcore setting to level1 to support fsystem=12MHz
  // NOTE: Change core voltage one level at a time..
  SetVcoreUp (0x01);    
  
  // Initialize DCO to 12MHz
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
                                            // Clear XT1, DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  // Setup Port Pins 
  P3DIR |= 0x07;                            // P3.0 - P3.2 output
  P3SEL |= 0x07;                            // Port map P3.0 - P3.2  
  Port_Mapping();
  
  // Configure ports TD0.0 input and TA0.0 output
  P1DIR &= ~BIT6;                           // TD0.0 input
  P1SEL |= BIT6;                            // TD0.0 option select  
  P3DIR |= BIT7;                            // TA0.0 output           
  P3SEL |= BIT7;                            // TA0.0 option select
  
  // Configure LED on P1.0, output and driving low
  P1DIR |= BIT0;
  P1OUT &= ~BIT0;  
    
  // Configure TA0.0 compare output, 1kHz freq, 50% dutycycle 
  TA0CCR0 = 32;                            // Period = 2*164*/32khz = 2ms
  TA0CCTL0 |= OUTMOD_4;                    // TD01CCR0, Toggle
  TA0CTL = TASSEL_1 + MC_1 + TACLR;        // ACLK, upmode, clear TDR  

  // Configure TD0.0 input capture, dual capture mode 
  TD0CTL2 |= TDCAPM0;                       // Channel 0; dual capture mode
  TD0CCTL0 |= CAP + CM_1 + CCIE + SCS;      // TD0CCR0 Capture mode; Rising Edge;  
                                            // CCI0A; interrupt enable
  TD0CTL0 = TDSSEL_2 + MC_2 + TDCLR;        // SMCLK=12Mhz, Cont Mode; Start timer
  
  while(1)
  {
      __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0
      __no_operation();                     // For debugger  
      // On exiting LPM0
      if (TD0CCTL0 & COV)                   // Capture Overflow?    
          while(1);                         // Loop Forever
     
      Period = REdge2-REdge1;               // Measured Period 
      __no_operation();                     // BREAKPOINT HERE - measured period ~ 2ms 
                                            // Period ~ 2m*TD0 clock counts    
  }
}
  
// Timer0_D0 Interrupt Vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_D0_VECTOR
__interrupt void TIMER0_D0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_D0_VECTOR))) TIMER0_D0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  REdge1 = TD0CL0;
  REdge2 = TD0CCR0;
  __bic_SR_register_on_exit(LPM0_bits);      // Exit LPM0 
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

void Port_Mapping(void)
{
  __disable_interrupt();                    // Disable Interrupts before altering Port Mapping registers
  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
  
  #ifdef PORT_MAP_RECFG                     
  PMAPCTL = PMAPRECFG;                      // Allow reconfiguration during runtime
  #endif  
  
  P3MAP0 = PM_TD0CLKMCLK;                   // MCLK output
  P3MAP1 = PM_TD0_0SMCLK;                   // SMCLK output
  P3MAP2 = PM_TD1OUTH;                      // ACLK output
  
  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
}
