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
//   MSP430F530x Demo - Internal LDO enabled after device power-up
//
//  Description: This demo code shows example case where device is powered from 
//  an external supply and the LDO input coming on is detected to enable 
//  external system circuitry. When sufficient voltage at LDOI is present, P1.0 
//  is set, else reset; thus reflecting the status of the LDO being enabled or not
//    
//   Note: 
//    1.External supply is used to power the device VCC. If using TI target board 
//      (MSP-TS430RGC64B), remove jumper on JP4
//
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F530x
//             -----------------
//        /|\|              VCC|<- External Supply
//         | |                 |
//         --|RST              |  
//           |             LDOO|--> 3.3V LDO Output
//           |             LDOI|<-- LDO Input Voltage (3.76V to 5.5V - refer to the d/s specs for actual range)
//           |                 |          
//           |             P1.0|--> LED - Reflects LDO status
//           |                 |          
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
  volatile unsigned int i;

  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  
  // Configure P1.0 as output 
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  
  // Detect if sufficient voltage at LDOI is present
  if (LDOPWRCTL & LDOBGVBV)
    P1OUT |= BIT0;                          // Valid LDOI, LED on
  else
    P1OUT &= ~BIT0;
  
  // Enable interrupts
  LDOKEYPID = 0x9628;                       // Enable access to config registers
  LDOPWRCTL &= ~(LDOOVLIFG | LDOONIFG | LDOOFFIFG); 
                                            // Clear LDO-PWR IFGs before enabled IE bits 
  LDOPWRCTL |= LDOONIE | LDOOFFIE | LDOOVLIE;  
                                            // Enable LDO-PWR interrupts
  LDOKEYPID = 0x0;                          // Disbale access to config registers
  
  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3 w/ interrupts
  __no_operation();                         // For debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=LDO_PWR_VECTOR
__interrupt void LDOInterruptHandler(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(LDO_PWR_VECTOR))) LDOInterruptHandler (void)
#else
#error Compiler not supported!
#endif
{
  if(LDOPWRCTL & VBONIFG)
  {
      LDOKEYPID = 0x9628;                   // Enable access to config registers
      LDOPWRCTL &= ~(LDOOVLIFG | LDOONIFG); // Software clear IFG    
                                            // Its possible that current peaks due to charging cap 
                                            // at LDOO might cause the LDOOVLIFG to trigger
      LDOKEYPID = 0x0;                      // Disbale access to config registers       
      P1OUT |= BIT0;                        // Valid LDOI, LED on
  }
  else if(LDOPWRCTL & VUOVLIFG)
  {
      LDOKEYPID = 0x9628;                   // Enable access to config registers
      LDOPWRCTL &= ~VUOVLIFG;               // Software clear IFG
      LDOKEYPID = 0x0;                      // Disbale access to config registers   
      while(1);                             // Over load indication; take necessary steps in application firmware
  }
  else if(LDOPWRCTL & VBOFFIFG)
  {
      LDOKEYPID = 0x9628;                   // Enable access to config registers
      LDOPWRCTL &= ~VBOFFIFG;               // Software clear IFG
      LDOKEYPID = 0x0;                      // Disbale access to config registers       
      P1OUT &= ~BIT0;                       // Voltage LDOI < launch voltage, LED off
  }
}
