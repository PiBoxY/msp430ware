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
//   MSP430F51x2 Demo - Software Toggle P1.0 at Default DCO
//
//   Description: Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//   ACLK is rought out on pin P3.0, SMCLK is brought out on P3.1, and MCLK
//   is brought out on pin P3.2 by using the port mapping controller
//   ACLK = REFO = 32.768kHz, MCLK = SMCLK = Default DCO
//
//                 MSP430F51x2
//             -----------------
//         /|\|                 |
//          | |             P3.0|--> MCLK
//          --|RST          P3.1|--> SMCLK
//            |             P3.2|--> ACLK
//            |                 |
//            |             P1.0|-->LED
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   Dec 2009
//   Built with CCS v4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

// Function Definition
void Port_Mapping(void);

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  Port_Mapping();                           // Port Map port3
    
  P1DIR |= BIT0;                            // P1.0 output
  
  // Setup Port Pins 
  P3DIR |= 0x07;                            // P3.0 - P3.2 output
  P3SEL |= 0x07;                            // Port map P3.0 - P3.2                             

  while(1)
  {
    P1OUT ^= BIT0;                          // Toggle P1.0
    __delay_cycles(60000);                  // Delay
  }
}

void Port_Mapping(void)
{
  __disable_interrupt();                    // Disable Interrupts before altering Port Mapping registers
  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers
  
  #ifdef PORT_MAP_RECFG                     
  PMAPCTL = PMAPRECFG;                      // Allow reconfiguration during runtime
  #endif  
  
  P3MAP0 = PM_TD0CLKMCLK;
  P3MAP1 = PM_TD0_0SMCLK;
  P3MAP2 = PM_TD1OUTH;
  
  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif  
}
