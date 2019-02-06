/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
// MSP430F524x Demo - Port Mapping Port4; Single runtime configuration
//
// Description:  Port4 is port mapped to output TimerB digital signals. TBCCR1-TBCCR6 
// generate different PWM DutyCycles and have been output on Port4 pins. TBCCR0 
// generates a square wave of freq ACLK/2^10 ~32Hz. 
// MCLK = SMCLK = default DCO; ACLK = REFO~32kHz;                           
//                                                       
//                 MSP430F524x
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST               |                                   
//            |     P4.0(TB0CCR0)|--> ACLK/2^10 ~ 32Hz                
//            |     P4.1(TB0CCR1)|--> 25%   
//            |     P4.2(TB0CCR2)|--> 50% 
//            |     P4.3(TB0CCR3)|--> 62.5%                 
//            |     P4.4(TB0CCR4)|--> 75%                
//            |     P4.5(TB0CCR5)|--> 87.5%                 
//            |     P4.6(TB0CCR6)|--> 94% 
//            |     P4.7(PM_None)|--> DVSS
//                 
//   E. Chen
//   Texas Instruments Inc.
//   August 2013
//   Built with IAR Embedded Workbench v5.52 & Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

// Port4 Port Mapping definitions
const unsigned char P4mapping[8] = {
                                      PM_TB0CCR0A,
                                      PM_TB0CCR1A,             
                                      PM_TB0CCR2A,             
                                      PM_TB0CCR3A,             
                                      PM_TB0CCR4A,             
                                      PM_TB0CCR5A,             
                                      PM_TB0CCR6A,
                                      PM_NONE     };
// Function Definition
void Port_Mapping(void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  Port_Mapping();
  
  // Setup Port Pins              
  P4DIR |= 0xFF;                            // P4.0 - P4.7 output
  P4SEL |= 0xFF;                            // P4.0 - P4.6 Port Map functions
  
  // Setup TB0 
  TB0CCTL0 = OUTMOD_4;                      // CCR0 Toggle
  TB0CCR0 = 256;                            // PWM Period/2
  TB0CCTL1 = OUTMOD_6;                      // CCR1 toggle/set
  TB0CCR1 = 192;                            // CCR1 PWM duty cycle
  TB0CCTL2 = OUTMOD_6;                      // CCR2 toggle/set
  TB0CCR2 = 128;                            // CCR2 PWM duty cycle
  TB0CCTL3 = OUTMOD_6;                      // CCR3 toggle/set
  TB0CCR3 = 96;                             // CCR3 PWM duty cycle
  TB0CCTL4 = OUTMOD_6;                      // CCR4 toggle/set
  TB0CCR4 = 64;                             // CCR4 PWM duty cycle
  TB0CCTL5 = OUTMOD_6;                      // CCR5 toggle/set
  TB0CCR5 = 32;                             // CCR5 PWM duty cycle
  TB0CCTL6 = OUTMOD_6;                      // CCR6 toggle/set
  TB0CCR6 = 16;                             // CCR6 PWM duty cycle  
  TB0CTL = TBSSEL_1 + MC_3;                 // ACLK, up-down mode

  __bis_SR_register(LPM3_bits);       		// Enter LPM3
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
  
  ptr = &P4MAP0;
  for(i=0;i<8;i++)
  {
    *ptr = P4mapping[i];
    ptr++;
  } 
  
  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
  
}

  
