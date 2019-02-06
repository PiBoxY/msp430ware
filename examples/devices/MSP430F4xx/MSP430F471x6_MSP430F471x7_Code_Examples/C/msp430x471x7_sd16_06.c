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
//  MSP430x471xx Demo - SD16, Using the Integrated Temperature Sensor
//
//  Description: This example shows how to use the SD16 module's integrated
//  temperature sensor (connected to input channel 2) to measure temperature.
//  A single conversion is initiated and then the CPU is disabled by entering
//  LPM0. Once the conversion has completed, a SD16 interrupt occurs and
//  stores the result in variable "ADCresult". Next, the CPU wakes up and
//  converts this value into two temperature values, one in degrees Celcius
//  and one in degrees Farenheit. Test by setting a breakpoint at the line
//  indicated below and running the program. Upon reaching the breakpoint,
//  use the debugger's watch window to view degrees C (DegC) and degrees F
//  (DegF).
//  ACLK = LFXT1 = 32768 Hz, MCLK = SMCLK = DCO = 32 x ACLK = 1048576 Hz
//  //* An external watch crystal on XIN XOUT is required for ACLK     *//	
//  //* Minimum Vcc is required for SD16 module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//  Sensor's temperature coefficient is 1.32mV/K (from 'F42x datasheet)
//  Sensor's offset voltage ranges from -100mv to 100mV (assume 0)
//  DegC =  DegK - 273
//  DegF = (DegC * 9/5) + 32
//  Vref = 1200mV
//  Vsensor = 1.32mV * DegK + Vsensor_offset (assuming 0mv for offset)
//  Vsensor = (SD16MEM2-32768)/32768 * Vref/2
//  DegK = (SD16MEM2 - 32768)/65536 * 1200/1.36
//  DegK = (SD16MEM2 * 909)/65536 - 454
//  DegC = (SD16MEM2 * 909)/65536 - 727   ( No need to simplify further,    )
//  DegF = (SD16MEM2 * 1636)/65536 - 1276 ( take advantage of HW multiplier )
//                                        ( and simple binary division      )
//
//                MSP430x471xx
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |A2.6+            | ( A2.6+, A2.6- connected internally )
//            |A2.6-            | ( to SD16's temperature sensor      )
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

unsigned int ADCresult;
unsigned long DegC = 0;
unsigned long DegF = 0;

int main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |= XCAP11PF;                     // Configure load caps
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
                                            // stabilize

  SD16CTL = SD16REFON+SD16SSEL0;            // 1.2V ref, SMCLK
  SD16CCTL2 |= SD16SNGL+SD16IE ;            // Single conv, enable interrupt
  SD16INCTL2 |= SD16INCH_6;                 // Select Channel A6
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup

  while (1)
  {
    SD16CCTL2 |= SD16SC;                    // Set bit to start conversion
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts

    DegC = ((unsigned long)ADCresult * 909)/65536 - 727;
                                            // Calculate degrees Celcius
    DegF = ((unsigned long)ADCresult * 1636)/65536 - 1276;
                                            // Calculate degrees Farenheit
    __no_operation();                       //SET BREAKPOINT HERE
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD16A_VECTOR
__interrupt void SD16AISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD16A_VECTOR))) SD16AISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (SD16IV)
  {
  case 2:                                   // SD16MEM Overflow
    break;
  case 4:                                   // SD16MEM0 IFG
    break;
  case 6:                                   // SD16MEM1 IFG    
    break;
  case 8:                                   // SD16MEM2 IFG
    ADCresult = SD16MEM2;                   // Save CH2 results (clears IFG)
    break;
  case 10:                                  // SD16MEM3 IFG
    break;
  case 12:                                  // SD16MEM4 IFG
    break;
  case 14:                                  // SD16MEM5 IFG   
    break;
  case 16:                                  // SD16MEM6 IFG   
    break;
  }

   __bic_SR_register_on_exit(LPM0_bits);    // Exit LPM0
}


