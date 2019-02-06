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
//  MSP430FR235x Demo - SAC0+SAC2+eCOMP0, Use eCOMP0 to compare SAC0 PGA mode output
//                      and SAC2 DAC buffer mode output
//
//  Description: Configure SAC0 in Non-inverting PGA mode and its gain is 5. Configure
//  SAC2 in DAC buffer mode and DAC output is 1V. Select 1.5V internal Vref as SAC2
//  DAC voltage reference. Use eCOMP0 to compare SAC0 output and SAC2 output.
//  Add one adjustable DC voltage to OA0+ pin and observe LED status. When OA0+ input
//  voltage exceeds 200mV, CPOUT goes high and LED is turned on. When OA0+ input voltage
//  is less than 200mV then CPOUT goes low and LED is turned off. eCOMP interrupt enabled,
//  wake up from LPM3. Both SAC0 and SAC2 are configured to low speed and low power mode.
//  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
//
//                MSP430FR235x
//             -------------------
//         /|\|          P1.2/OA0-|
//          | |          P1.1/OA0O|
//          | |          P1.3/OA0+|<--
//          --|RST       P3.1/OA2O|
//            |          P3.2/OA2-|
//            |          P3.3/OA2+|
//            |                   |
//            |          P2.0/COUT|-->
//            |               P1.5|----> LED
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.50 & Code Composer Studio v6.2
//******************************************************************************
#include <msp430.h>

unsigned int adcResult;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1DIR |= BIT5;                            // Select P1.5 as output
  P1OUT &= ~BIT5;                           // Set P1.5 output low
  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  // Configure reference module
  PMMCTL0_H = PMMPW_H;                    // Unlock the PMM registers
  PMMCTL2 = INTREFEN | REFVSEL_0;         // Enable internal 1.5V reference
  while(!(PMMCTL2 & REFGENRDY));          // Poll till internal reference settles

  //Select P1.1 P1.2 P1.3 as SAC0 function
  //Select P3.1 P3.2 P3.3 as SAC2 function
  P1SEL0 |= BIT1 + BIT2 + BIT3;
  P1SEL1 |= BIT1 + BIT2 + BIT3;
  P3SEL0 |= BIT1 + BIT2 + BIT3;
  P3SEL1 |= BIT1 + BIT2 + BIT3;

  SAC0OA = NMUXEN + PMUXEN + PSEL_0 + NSEL_1;//Select positive and negative pin input
  SAC0OA |= OAPM;                           // Select low speed and low power mode
  SAC0PGA = GAIN0 + GAIN1 + MSEL_2;         // Set Non-inverting PGA mode with Gain=5
  SAC0OA |= SACEN + OAEN;                   // Enable SAC0 and OA0

  SAC2DAC = DACSREF_1;                      // Select 1.5V internal Vref as SAC2 DAC reference
  SAC2DAT = 0xAAA;                          // Set SAC2 DAC output = 1V
  SAC2DAC |= DACEN;                         // Enable SAC2 DAC

  SAC2OA = NMUXEN + PMUXEN + PSEL_1 + NSEL_1;//Select positive and negative pin input
  SAC2OA |= OAPM;                           // Select low speed and low power mode
  SAC2PGA = MSEL_1;                         // Set OA as buffer mode
  SAC2OA |= SACEN + OAEN;                   // Enable SAC2 and OA2


  // Setup eCOMP
  CP0CTL0 = CPPSEL_5 | CPNSEL_5;             // Select SAC0 and SAC2 output as eCOMP0 input
  CP0CTL0 |= CPPEN | CPNEN;                  // Enable eCOMP input
  CP0CTL1 |= CPIIE | CPIE;                   // Enable eCOMP dual edge interrupt
  CP0CTL1 |= CPEN | CPMSEL;                  // Turn on eCOMP, in low power mode


  __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3
  __no_operation();                         // For debug
}

// eCOMP interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ECOMP0_ECOMP1_VECTOR
__interrupt void ECOMP0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ECOMP0_ECOMP1_VECTOR))) ECOMP0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(CP0IV, CPIV__CPIIFG))
    {
        case CPIV__NONE:
            break;
        case CPIV__CPIFG:
            P1OUT |= BIT5;                  // OA0+ input > 200mV
            break;
        case CPIV__CPIIFG:
            P1OUT &= ~BIT5;                 // OA0+ input < 200mV
            break;
        default:
            break;
    }
}
