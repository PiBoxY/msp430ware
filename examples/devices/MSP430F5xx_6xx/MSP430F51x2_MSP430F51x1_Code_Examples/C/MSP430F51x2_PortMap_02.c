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
// MSP430F51x2 Demo - Port Mapping single function to multiple pins; Single 
//                    runtime configuration. 
// Description:  Port1 is port mapped to output TimerA0 digital signals.  
// TACCR1-2 generate different DutyCycles and have been output on multiple pins. 
// ACLK = REFO~32kHz; MCLK = SMCLK = default DCO;                           
//                                                       
//                 MSP430F51x2
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST               |                                   
//            |     P1.0(TA0CCR0)|--> 50%   
//            |     P1.1(TA0CCR0)|--> 50% 
//            |     P1.2(TA0CCR1)|--> 25%                 
//            |     P1.3(TA0CCR1)|--> 25%                 
//            |     P1.4(TA0CCR2)|--> 75%                 
//            |     P1.5(TA0CCR2)|--> 75%   
//            |     P1.6(default)|--> DVSS                
//            |     P1.7(default)|--> DVSS  
//                 
//  B.Nisarga
//  Texas Instruments Inc.
//  Dec 2009
//  Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

void Port_Mapping(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  Port_Mapping();
  
  // Setup Port Pins              
  P1DIR |= 0x3F;                            // P1.0 - P1.5 output
  P1SEL |= 0x3F;                            // P1.0 - P1.6 Port Map functions
  
  // Setup TA0 
  TA0CCTL0 = OUTMOD_4;                      // TA3CCR0 toggle
  TA0CCR0 = 128;                            // TA3CCR0 PWM Period/2
  TA0CCTL1 = OUTMOD_6;                      // TA3CCR1 toggle/set
  TA0CCR1 = 96;                             // TA3CCR1 PWM duty cycle
  TA0CCTL2 = OUTMOD_6;                      // TA3CCR2 toggle/set
  TA0CCR2 = 32;                             // TA3CCR2 PWM duty cycle
  TA0CTL = TASSEL_1 + MC_3;                 // ACLK/2, up-down mode

  __bis_SR_register(LPM3_bits);		        // Enter LPM3
  __no_operation();                         // For debugger
}

void Port_Mapping(void)
{
  __disable_interrupt();                    // Disable Interrupts before altering Port Mapping registers
  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
  
  #ifdef PORT_MAP_RECFG                     
  PMAPCTL = PMAPRECFG;                      // Allow reconfiguration during runtime
  #endif  
  
  P1MAP0 = PM_TA0_0;
  P1MAP1 = PM_TA0_0;
  P1MAP2 = PM_TA0_1;
  P1MAP3 = PM_TA0_1;
  P1MAP4 = PM_TA0_2;
  P1MAP5 = PM_TA0_2;
  
  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
}

