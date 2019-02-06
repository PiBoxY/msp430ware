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
//  MSP430F51x2 Demo - Port Mapping Port4; Single runtime configuration
//
//  Description:  Port4 is port mapped to output TimerD0/1 digital signals.
//  TTDxCCR0-2 generate different PWM DutyCycles and have been output on Port1 
//  pins. TD0CCR0 and TD1CCR0 generates a square wave of freq ACLK/2^10 ~32Hz. 
//  MCLK = SMCLK = default DCO; ACLK = REFO~32kHz;                           
//                                                       
//                 MSP430F51x2
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST               |                                   
//            |       P1.0(TD0_0)|--> ACLK/2^10 ~ 32Hz                
//            |       P1.1(TD0_1)|--> 25%   
//            |       P1.2(TD0_2)|--> 50% 
//            |       P1.3(TD1_0)|--> ACLK/2^10 ~ 32Hz                
//            |       P1.4(TD1_1)|--> 75%                
//            |       P1.5(TD1_2)|--> 87.5%                 
//            |     P1.6(PM_None)|--> DVSS
//            |     P1.7(PM_None)|--> DVSS
//                 
//  B.Nisarga
//  Texas Instruments Inc.
//  Dec 2009
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

// Port1 Port Mapping definitions
const unsigned char P1mapping[8] = {
                                      PM_TD0_0,
                                      PM_TD0_1,             
                                      PM_TD0_2,             
                                      PM_TD1_0,             
                                      PM_TD1_1,             
                                      PM_TD1_2,             
                                      PM_NONE,
                                      PM_NONE  };
// Function Definition
void Port_Mapping(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  Port_Mapping();
  
  // Setup Port Pins              
  P1DIR |= 0xFF;                            // P1.0 - P1.7 output
  P1SEL |= 0xFF;                            // P1.0 - P1.6 Port Map functions
  
  // Setup TD0/TD1
  TD0CCTL0 = OUTMOD_4;                      // TD0CCR0 toggle
  TD0CCR0 = 256;                            // TD0CCR0 PWM Period/2
  TD0CCTL1 = OUTMOD_6;                      // TD0CCR1 toggle/set
  TD0CCR1 = 192;                            // TD0CCR1 PWM duty cycle
  TD0CCTL2 = OUTMOD_6;                      // TD0CCR2 toggle/set
  TD0CCR2 = 128;                            // TD0CCR2 PWM duty cycle
  
  TD1CCTL0 = OUTMOD_4;                      // TD1CCR0 toggle
  TD1CCR0 = 256;                            // TD1CCR0 PWM Period/2
  TD1CCTL1 = OUTMOD_6;                      // TD1CCR1 toggle/set
  TD1CCR1 = 64;                             // TD1CCR1 PWM duty cycle
  TD1CCTL2 = OUTMOD_6;                      // TD1CCR2 toggle/set
  TD1CCR2 = 32;                             // TD1CCR2 PWM duty cycle
  TD0CTL0 = TDSSEL_1 + MC_3;                // ACLK, up-down mode
  TD1CTL0 = TDSSEL_1 + MC_3;                // ACLK, up-down mode

  __bis_SR_register(LPM3_bits);             // Enter LPM3
  __no_operation();                         // For debugger
}

void Port_Mapping(void)
{
  unsigned char i;
  volatile unsigned char *ptr;
  __disable_interrupt();                    // Disable Interrupts before altering Port Mapping registers
  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
  
  #ifdef PORT_MAP_RECFG                     
  PMAPCTL = PMAPRECFG;                      // Allow reconfiguration during runtime
  #endif  
  
  ptr = &P1MAP0;
  for(i=0;i<8;i++)
  {
    *ptr = P1mapping[i];
    ptr++;
  } 
  
  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
  
}

  
