/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//  MSP430FR235x Demo - SAC-L3, Unity-Gain Buffer Mode
//
//  Description: Configure SAC-L3 for Unity-Gain Buffer Mode. The "+" terminal is
//  connected to OA0+. Amplifier's negative input is connected with its positive
//  input inside the silicon. SAC is selected to low speed and low power mode.
//  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
//
//                MSP430FR235x
//             -------------------
//         /|\|          P1.2/OA0-|
//          | |                |  |
//          --|RST       P1.1/OA0O|--
//            |                   |
//            |          P1.3/OA0+|<--
//            |                   |
//            |                   |
//            |                   |
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.50 & Code Composer Studio v6.2 
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watch dog timer

  P1SEL0 |= BIT1 + BIT3;                    // Select P1.1 P1.3 as external OA function
  P1SEL1 |= BIT1 + BIT3;                    // Select P1.1 P1.3 as external OA function


  SAC0OA |= NMUXEN + PMUXEN + NSEL_1 + OAPM;// Enable negative and positive input
                                            // Select PGA source as OA negative input
                                            // Select low speed and low power mode
  SAC0PGA |= MSEL_1;                        // Set as Unity-Gain Buffer Mode
  SAC0OA |= SACEN + OAEN;                   // Enable SAC and OA

  __bis_SR_register(LPM3_bits);             // Enter LPM3
}
