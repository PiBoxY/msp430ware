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
//  MSP430FR57xx Demo - COMPD Hysteresis, CDOUT Toggle in LPM4; High speed mode
//
//  Description: Use Compd and shared reference to determine if input 'Vcompare'
//  is high of low.  Shared reference is configured to generate hysteresis.
//  When Vcompare exceeds Vcc*3/4 CDOUT goes high and when Vcompare is less 
//  than Vcc*1/4 then CDOUT goes low.
//  Connect P3.5/CDOUT to P1.0 externally to see the LED toggle accordingly.
//
//                 MSP430FR5739
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST      P1.1/CD0 |<--Vcompare            
//            |                  |                       
//            |        P3.5/CDOUT|----> 'high'(Vcompare>Vcc*3/4); 'low'(Vcompare<Vcc*1/4)
//            |                  |  |
//            |            P1.0  |__| LED 'ON'(Vcompare>Vcc*3/4); 'OFF'(Vcompare<Vcc*1/4)
//            |                  | 
//                                                       
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop WDT                     
 
// compare input
  P1SEL0 |= BIT1;               // P1.1/CD1
  P1SEL1 |= BIT1; 
// Compare output
  P3DIR |= BIT5; 
  P3SEL1 |= BIT5;
  P3SEL0 |= BIT5;

  // Setup Comparatord                                            
                                               
  CDCTL0 |= CDIPEN + CDIPSEL_1; // Enable V+, input channel CD0              
  CDCTL2 |= CDRSEL;             // VRef is applied to -terminal  
  CDCTL2 |= CDRS_1+CDREF13;     // VREF1 is Vcc*1/4             
  CDCTL2 |= CDREF04+CDREF03;    // VREF0 is Vcc*3/4             
  CDCTL3 |= BIT1;               // Input Buffer Disable @P1.1/Cd1    
  CDCTL1 |= CDON;               // Turn On Comparatord           

  __delay_cycles(400);           // delay for the reference to settle
  
  __bis_SR_register(LPM4_bits); // Enter LPM4
  __no_operation();             // For debug 
} 


