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
//*****************************************************************************
//   MSP430x21x1 Demo - Comp_A, Millivolt Meter
//
//   Description: This program demonstrates how to implement an MSP430F21x1/
//   F11x1 millivolt meter using Comparator_A. The measurement result can be
//   read out of variable 'ADCResult' using the watch window.
//    ACLK = n/a, MCLK = SMCLK = DCO=8MHZ
//
//             MSP430F21x1/11x1
//            -----------------
//       /|\ |              XIN|-
//        |  |                 |
//        ---|RST          XOUT|-
//           |                 |
//    AIN--->|P2.4             |
//           |                 |
//    +-10k--|P2.0             |
//    |      |                 |
//    +----- |P2.3             |
//    |      |                 |
//   ===2.2  |                 |
//    |      |                 |
//    ------ |VSS              |
//
//   F. Chen
//   Texas Instruments, Inc
//   November 2012
//   Built with CCS Version: 5.2.1 and IAR Embedded Workbench Version: 5.51.1
//*****************************************************************************

#include <msp430.h>

#define VCC_VALUE   (2980)                  // Actual system VCC (mV)
volatile unsigned int ADCResult;            // volatile to prevent opt.

void Init_Sys(void);                        // Function prototypes
unsigned int Meas_ADC(void);
unsigned int Meas_ADC2(void);

int main(void)
{
  Init_Sys();                               // Init system peripherals

  while (1)
  {
    ADCResult = Meas_ADC();                 // Measure ADC
    __no_operation();                       // >>SET BREAKPOINT HERE<<
  }                                         // And read out 'ADCResult'
}
//-----------------------------------------------------------------------------
// Initialize System Peripherals
//-----------------------------------------------------------------------------
void Init_Sys(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P2OUT = 0x20;                             // P2.5 high
  P2DIR = 0x21;                             // P2.0 = DAC, P2.5 powers pot
  CACTL2 = P2CA0 + P2CA1;                   // Setup CA, P2.3+ P2.4-
  
  if (CALBC1_8MHZ==0xFF)                    // If calibration constants erased
  {                                            
    while(1);                               // do not load, trap CPU!!    
  }


  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_8MHZ;                          // Set range              
  DCOCTL = CALDCO_8MHZ;                           // Set DCO step + modulation 
}
//-----------------------------------------------------------------------------
// Measure A/D Converter, return offset-compensated result
//-----------------------------------------------------------------------------
unsigned int Meas_ADC(void)
{
  unsigned int Result;

  CACTL1 = CAON;                            // Comparator on
  Result = Meas_ADC2();                     // Do 1st ADC conversion
  CACTL1 |= CAEX;                           // Invert comparator terminals
  Result += Meas_ADC2();                    // 2nd ADC, add inverted result
  CACTL1 = 0;                               // Comparator off

  return Result;
}
//-----------------------------------------------------------------------------
// A/D Converter Helper Function
//
// Attn: The loop cycle count has to be equal for both decision paths
//       of the if() statement, otherwise the measurement result will
//       not be accurate. It has been found with IAR V5.51.1, that the
//       code below will implement this, even independent of the
//       C compiler optimization settings. Another approach would be
//       implementing the entire function in assembly language.
//-----------------------------------------------------------------------------
unsigned int Meas_ADC2(void)
{
  unsigned int Counter = VCC_VALUE / 2;
  unsigned int Result = 0;

  P2OUT |= 0x01;                            // Set power to capacitor
  while (!(CACTL2 & CAOUT));                // Wait for CAOUT to get set

  do
  {
    if (CACTL2 & CAOUT)                     // Comparator high/low?
    {
      P2OUT &= ~0x01;                       // Remove power if high
    }
    else
    {
      P2OUT |= 0x01;                        // Set power if low
      Result++;                             // Measure the 'ON' time
      __no_operation();                     // Timing adjustment
    }
  } while (--Counter);                      // Decrement and loop

  P2OUT &= ~0x01;                           // Remove power from cap

  return Result;
}
