/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//  MSP430FR231x Demo - TRI0+ADC10, Use ADC to sample TRI output,
//
//  Description: Configure TRI0 as unity-gain buffer Mode.
//  As the code example practice, TRI0- and TRI0O are connected directly. 
//  The input signal which is in the range 0~1/2Vcc is input through TRI0+,
//  Use ADC CH A5 to sample TRI output. If higher than 1/4 VCC, turn on LED.
//  TRI0 is configured to low speed and low power mode.
//  Need to add external circuit to silicon, make sure TRI0 work in amplifier mode.
//
//  ACLK = n/a, MCLK = SMCLK = default DCODIV ~1MHz.
//  Note: Please select SBW interface instead of JTAG interface while debugging 
//        the TRI because the TRI pins are alternatively used with JTAG pins.
//
//                MSP430FR231x
//             -------------------
//         /|\|                   |
//          | |                   |
//          | |                   |
//          --|RST                |
//            |         P1.5/TRI0O|----||
//            |         P1.6/TRI0-|----||
//            |         P1.7/TRI0+|<------0~1/2*Vcc
//            |                   |
//            |               P1.0|----> LED
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
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

  //Select P1.5 P1.6 P1.7 as TRI fucntion
  P1SEL0 |= BIT5 + BIT6 + BIT7;
  P1SEL1 |= BIT5 + BIT6 + BIT7;

  TRI0CTL = TRIPM + TRIEN;                  // Enable TRI, Select low speed and low power mode

  // Configure ADC10
  ADCCTL0 &= ~ADCENC;                       // Disable ADC
  ADCCTL0 = ADCSHT_2 | ADCON;               // ADCON, S&H=16 ADC clks
  ADCCTL1 = ADCSHP;                         // ADCCLK = MODOSC; sampling timer
  ADCCTL2 = ADCRES;                         // 10-bit conversion results
  ADCIE = ADCIE0;                           // Enable ADC conv complete interrupt
  ADCMCTL0 = ADCINCH_5 | ADCSREF_0;         // A5 ADC input select = TRI output
                                            // Vref = DVCC

  while(1)
  {
      ADCCTL0 |= ADCENC | ADCSC;          // Sampling and conversion start
      __bis_SR_register(LPM0_bits | GIE); // Enter LPM0, ADC_ISR will force exit
      __no_operation();                   // For debug only

      if (adcResult > 256)                // OA output > 1/4 VCC
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
