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
//  MSP430FR235x Demo - SAC2+SAC0, Two-Stage amplifier,
//                      Use ADC to sample SAC0 output
//
//  Description: Configure SAC2 and SAC0 as two-stage amplifier.
//  SAC2 is set as first stage amplifier in non-inverting PGA mode and its gain is 3.
//  SAC0 is set as second stage amplifier in non-inverting PGA mode and its gain is 5.
//  SAC2 output is connected to SAC0 positive input internally by setting PSEL=0x10.
//  Use ADC CH A1 to sample SACO output. If higher than 1/2 VCC, turn on LED.
//  Both SAC0 and SAC2 are configured to low speed and low power mode.
//  No need to add external circuit to silicon. The gain is setting inside in silicon.
//  Add one adjustable DC voltage to 0A2+ pin and observe LED status.
//  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
//
//                MSP430FR235x
//             -------------------
//         /|\|          P1.2/OA0-|
//          | |          P1.1/OA0O|
//          | |          P1.3/OA0+|
//          --|RST                |
//            |          P3.1/OA2O|
//            |          P3.2/OA2-|
//            |          P3.3/OA2+|<--
//            |                   |
//            |               P1.0|----> LED
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

  P1DIR |= BIT0;                            // Select P1.0 as output
  P1OUT &= ~BIT0;                           // Set P1.0 output low
  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  //Select P1.1 P1.2 P1.3 as SAC0 function
  //Select P3.1 P3.2 P3.3 as SAC2 function
  P1SEL0 |= BIT1 + BIT2 + BIT3;
  P1SEL1 |= BIT1 + BIT2 + BIT3;
  P3SEL0 |= BIT1 + BIT2 + BIT3;
  P3SEL1 |= BIT1 + BIT2 + BIT3;

  SAC2OA = NMUXEN + PMUXEN + PSEL_0 + NSEL_1;//Select positive and negative pin input
  SAC2OA |= OAPM;                           // Select low speed and low power mode
  SAC2PGA = GAIN1 + MSEL_2;                 // Set Non-inverting PGA mode with Gain=3
  SAC2OA |= SACEN + OAEN;                   // Enable SAC2 and OA

  SAC0OA = NMUXEN + PMUXEN + PSEL_2 + NSEL_1;//Select Pair OA source
  SAC0OA |= OAPM;                           // Select low speed and low power mode
  SAC0PGA = GAIN0 + GAIN1 + MSEL_2;         // Set Non-inverting PGA mode with Gain=5
  SAC0OA |= SACEN + OAEN;                   // Enable SAC0 and OA



  // Configure ADC12
  ADCCTL0 &= ~ADCENC;                       // Disable ADC
  ADCCTL0 = ADCSHT_2 | ADCON;               // ADCON, S&H=16 ADC clks
  ADCCTL1 = ADCSHP;                         // ADCCLK = MODOSC; sampling timer
  ADCCTL2 = ADCRES_2;                       // 12-bit conversion results
  ADCIE = ADCIE0;                           // Enable ADC conv complete interrupt
  ADCMCTL0 = ADCINCH_1 | ADCSREF_0;         // A1 ADC input select = OA0 output
                                            // Vref = DVCC

  while(1)
  {
      ADCCTL0 |= ADCENC | ADCSC;          // Sampling and conversion start
      __bis_SR_register(LPM0_bits | GIE); // Enter LPM0, ADC_ISR will force exit
      __no_operation();                   // For debug only

      if (adcResult > 2047)                // OA output > 1/2 VCC
          P1OUT |= BIT0;                  // Set P1.0 LED on
      else
          P1OUT &= ~BIT0;                 // Clear P1.0 LED off
  }
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(ADCIV, ADCIV_ADCIFG))
  {
      case ADCIV_NONE:
          break;
      case ADCIV_ADCOVIFG:
          break;
      case ADCIV_ADCTOVIFG:
          break;
      case ADCIV_ADCHIIFG:
          break;
      case ADCIV_ADCLOIFG:
          break;
      case ADCIV_ADCINIFG:
          break;
      case ADCIV_ADCIFG:
          adcResult = ADCMEM0;            // Read ADC memory
          __bic_SR_register_on_exit(LPM0_bits);// Exit from LPM
          break;
      default:
          break;
  }
}
