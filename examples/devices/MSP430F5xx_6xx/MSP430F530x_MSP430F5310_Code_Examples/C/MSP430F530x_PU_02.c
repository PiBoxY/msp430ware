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
//   MSP430F530x Demo  - Software Poll PU.0, Set P1.0 if PU.0 = 1
//
//  Description: Poll PU.0 in a loop, if hi P1.0 is set, if low, P1.0 reset.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//              MSP430F530x
//            -----------------
//        /|\|              VCC|--
//         | |                 |  |
//         --|RST          LDOO|<-
//     /|\   |                 |
//      --o--|PU.0         P1.0|-->LED
//     \|/
//
//  Note: 
//    1. PortU pins can only be configured together as either both inputs or both outputs
//    2. PortU is supplied by LDOO rail. Ensure 3.3V is available at LDOO, either by 
//       enabling the LDO OR by an external connection connecting device VCC to LDOO pin
//    3. In this example code, since the LDO is not enabled, an external connection between
//       the device VCC and LDOO pin is required for the PortU operation
//
//   K. Chen
//   Texas Instruments Inc.
//   Ver 2.00 - May 2012
//   Built with CCS Version: 5.2 and IAR Embedded Workbench Version: 5.40.3
//******************************************************************************
#include <msp430.h>         // To include latest F530x and F5310 header file v1.1, refer
                            // to the "ReadMe - Header" file in the code ex zip file

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  
  // Configure PU.0 as input pin
  LDOKEYPID = 0x9628;                       // Enable access to config registers 
  PUCTL |= PUIPE;                           // PortU input enable
  LDOKEYPID = 0x0;                          // Disbale access to config registers

  // Configure P1.0 as output 
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  
  while (1)                                 // Test PU.0
  {
    if (PUCTL & PUIN0)
      P1OUT |= BIT0;                        // if PU.0 is high, set P1.0
    else
      P1OUT &= ~BIT0;                       // else reset P1.0
  }
}


