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
// MSP430FR57xx Demo - COMPD interrupt capability; Vcompare is compared against 
//                    internal 1.5V reference
//
// Description: Use CompD and internal reference to determine if input'Vcompare'
// is high of low.  For the first time, when Vcompare exceeds the 1.5V internal
// reference, CDIFG is set and device enters the CompD ISR. In the ISR CDIES is
// toggled such that when Vcompare is less than 1.5V internal reference, 
// CDIFG is set.
// LED is toggled inside the CompD ISR
//                                                   
//                 MSP430FR5739
//             ------------------                        
//         /|\|                  |                       
//          | |                  |                       
//          --|RST      P1.1/CD1 |<--Vcompare            
//            |                  |                                         
//            |            P1.0  |--> LED 'ON'(Vcompare>1.5V); 'OFF'(Vcompare<1.5V)
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
  P1DIR |= BIT0;                // P1.0/LED output direction
// compare input
  P1SEL0 |= BIT1;               // P1.1/CD1
  P1SEL1 |= BIT1; 
 
// Setup ComparatorB                           
  CDCTL0 |= CDIPEN + CDIPSEL_1; // Enable V+, input channel CD1            
  CDCTL2 |= CDRSEL;             // VREF is applied to -terminal 
  CDCTL2 |= CDRS_3+CDREFL_1;    // R-ladder off; bandgap ref voltage (1.2V)
                                // supplied ref amplifier to get Vcref=1.5V (CDREFL_2)            
  CDCTL3 |= BIT1;               // Input Buffer Disable @P1.1/CD1    

  __delay_cycles(400);           // delay for the reference to settle
  
  CDINT &= ~(CDIFG + CDIIFG);   // Clear any errant interrupts  
  CDINT  |= CDIE;               // Enable CompB Interrupt on rising edge of CDIFG (CDIES=0)
  CDCTL1 |= CDON;               // Turn On ComparatorB    
  
  __bis_SR_register(LPM4_bits+GIE);         // Enter LPM4 with inetrrupts enabled
  __no_operation();                         // For debug 
}

// Comp_d ISR - LED Toggle
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=COMP_D_VECTOR
__interrupt void Comp_D_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(COMP_D_VECTOR))) Comp_D_ISR (void)
#else
#error Compiler not supported!
#endif
{
  CDCTL1 ^= CDIES;              // Toggles interrupt edge
  CDINT &= ~CDIFG;              // Clear Interrupt flag
  P1OUT ^= 0x01;                // Toggle P1.0
}
