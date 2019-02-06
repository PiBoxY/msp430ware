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
//  MSP-FET430P430 Demo - ADC12, Using the Temperature Sensor
//
//  This example shows how to use the intergrated temperature sensor to measure
//  temperature. When the temperature diode channel (A10) is selected for
//  conversion, the internal reference is automatically turned on as the source
//  for the sensor. Note however, that it is NOT automatically selected for the
//  conversion.  Any available reference can be used for the conversion.  In
//  this example, a single conversion is performed of the temperature diode.
//  The temperature is then calculated in degrees C and F, based on the A/D
//  conversion value. Test by setting and running to a break point at "_NOP()".
//  To view the temperature open a watch window with the debugger and view DegC
//  and DegF.
//  ACLK = n/a, MCLK = SMCLK = default DCO, ADC12CLK = ADC12OSC
//
//  Note: This example does not perform a calibration on the temperature sensor
//  A calibration of the temperature senosr may be necessary in an application.
//  see the device datasheet for the temperature sensor specification.
//
//               MSP430FG439
//             ---------------
//            |               |
//            |A10 (Temp)     |
//            |               |
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************
#include <msp430.h>

static unsigned int ADCresult;
static volatile unsigned long int DegC, DegF;

int main(void)
{
  volatile unsigned int i;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  ADC12CTL0 = ADC12ON+REFON+REF2_5V+SHT0_6; // Setup ADC12, ref., sampling time
  ADC12CTL1 = SHP;                          // Use sampling timer
  ADC12MCTL0 = INCH_10+SREF_1;              // Select channel A10, Vref+
  ADC12IE = 0x01;                           // Enable ADC12IFG.0

  for (i=0; i<0x3600; i++)                  // Delay for reference start-up
  {
  }

  ADC12CTL0 |= ENC;                         // Enable conversions
  __enable_interrupt();                     // Enable interrupts

  while(1)
  {
    ADC12CTL0 |= ADC12SC;                   // Start conversion
    __bis_SR_register(LPM0_bits);           // Enter LPM0

    //  DegC = (Vsensor - 986mV)/3.55mV
    //  Vsensor = (Vref)(ADCresult)/4095)
    //  DegC -> ((ADCresult - 1615)*704)/4095
    DegC = ((((long)ADCresult-1615)*704)/4095);
    DegF = ((DegC * 9/5)+32);               // Calculate DegF
    __no_operation();                       // SET BREAKPOINT HERE
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12ISR (void)
#else
#error Compiler not supported!
#endif
{
  ADCresult = ADC12MEM0;                    // Move results, IFG is cleared
  __bic_SR_register_on_exit(LPM0_bits);     // Clear LPM0
}
