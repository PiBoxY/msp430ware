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
//  MSP430F51x2 Demo - TimerD0/1-TEC, External Fault, TECIV
//
//  Description: This code example shows how to configure the TEC module for
//  external fault input interrupts. In this example code, TD0 is used to 
//  generate the external fault pulses. TD1 is configured to output PWM of
//  75% ducytcycle on TD1.1. With the EXTFLT event on TD1.1 fault inputs, the
//  FLT interrupt is set. The fault status is checked in the TECIV ISR and the 
//  LED is toggled only if the FLT1 status bit is set.
//  Note: Rissing edge on TEC1FLT1 input sets the EXTFLT interrupt 
//
//  TD0 = 1KHz, 15% dutycycle; TD1 = 10kHz, 75% dutycycle 
//  ACLK = REFO = 32kHz; SMCLK = MCLK = DCO Clock = 12MHz; 
//
//                 MSP430F51x2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P2.0/TD0.2|--> CCR2 -| ~15% duty cycle; External fault Pulses
//            |                 |          |  
//            |    P2.7/TEC1FLT1|<---------| External fault Input 
//            |                 |
//            |       P2.2/TD1.1|--> CCR1 - 75% dutycycle
//            |             P1.0|--> LED toggled on Fault Event
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
  
  // Configure P1.0/LED
  P1DIR |= BIT0;                            // P1.0 output
  P1OUT &= ~BIT0;                           // P1.0 Output Low
  
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
  TD0CCR1 = 8;  
  TD0CCR2 = 13;
  TD0CCTL2 |= OUTMOD_7;                     // TD0CCR2, Reset/Set
  TD0CTL0 |= TDSSEL_1 + MC_1 + TDCLR;       // TD0, ACLK, up mode, clear timer

  // Configure TD1.1 port pins
  P2SEL |= BIT2;
  P2DIR |= BIT2;
  
  // Configure TimerD1 to 10kHz with PWMs dutycycles = 75%
  TD1CCR0 = 1200;                           // Period = 1200*(1/12Mhz) = 0.1ms
  TD1CCR1 = 900;                            // Dutycycle = 75%
  TD1CCTL1 |= OUTMOD_7;                     // TD1CCR1, Reset/Set  
  TD1CTL0 = TDSSEL_2 + MC_1 + TDCLR;        // SMCLK, upmode, clear TDR  
  
  // Configure TD1 TEC External Clear
  // Need to physically connect P2.0/TD0.2 to P2.7/TEC1FLT1
  P2SEL |= BIT7;
  TEC1XCTL1 = TECXFLTPOL1;                  // EXTFLT1 rising edge trigger
  TEC1XINT = TECXFLTIE;                     // Enable Fault Interrupt                                            
  TEC1XCTL0 = TECXFLTEN1;                   // EXTFLT1 enable for controlling TD1.1
  
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger  
  
}

// TEC1 (Timer Event Control) Interrupt Vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TEC1_VECTOR
__interrupt void TEC1_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TEC1_VECTOR))) TEC1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TEC1IV,0x6))
  {
      case  0x0: break;                             // Vector  0:  No interrupt
      case  0x2:                                    // Vector  2:  External Fault        
                if (TEC1STA & TECXFLT1STA)
                {
                    // Toggle LED 
                    P1OUT ^= BIT0;
                 }
                 break;                 
      case  0x4: break;                             // Vector  4:  External Clear
      case  0x6: break;                             // Vector  6:  Auxiliary Clear
      default:  break;
  }
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
