/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//*****************************************************************************
//  MSP430FG662x Demo - OA1, Unity-Gain Buffer Mode
//
//  Description: Configure OA0 for rail-to-rail input mode. The "+" terminal is
//  connected to OA1IP0 and the "-" terminal is connected to ground via ground
//  switch G1SW1.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430FG662x
//             -------------------
//         /|\|                XIN|-
//          | |                   |
//          --|RST            XOUT|-
//            |                   |
//     "+" -->|P6.3/OA1IP0        |
//            |                   |
//            |          P7.4/OA1O|--> OA1 Output
//            |                   |
//
//  M. Swanson
//  Texas Instruments Inc.
//  April 2014
//  Built with Code Composer Studio v5.5
//*****************************************************************************
#include <msp430.h>  

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P6SEL |= BIT3;                            // Select P6.3 OA11IP0 function

  OA1GSW = GSW1;                            // Close G1SW1 ground switch
  OA1PSW = PSW0;                            // Select OA1IP0 as "+" input
  OA1NSW = NSW1;                            // Select GSW1 as "-" input
  OA1CTL0 = OARRI;     	                    // Select rail-to-rail input mode
  while(OA1CTL0&OARRIRDY);                  // Wait for rail-to-rail input to be ready
  OA1CTL0 = OAM;                            // Enable OA1 amplifier

  __bis_SR_register(LPM3_bits);             // Enter LPM3
  __no_operation();                         // Required only for debugger
}
