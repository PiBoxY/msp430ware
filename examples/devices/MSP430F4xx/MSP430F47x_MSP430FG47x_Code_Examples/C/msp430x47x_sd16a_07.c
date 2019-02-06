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
//   MSP430x47x Demo - SD16_A, Using the Integrated Battery Sensor
//
//  Description: This example shows how to use the SD16_A module's integrated
//  Battery sensor (connected to input channel 5) to measure VCC.
//  A single conversion is initiated and then the CPU is disabled by entering
//  LPM0. Once the conversion has completed, a SD16_A interrupt occurs and
//  stores the result in variable "ADCresult".  LED lights if battery is low
//  ACLK = LFXT1 = 32768 Hz, MCLK = SMCLK = DCO = 32 x ACLK = 1048576 Hz
//  //* An external watch crystal on XIN XOUT is required for ACLK     *//
//  //* Minimum Vcc is required for SD16_A module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//                MSP430x47x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |A5+              | ( A5+, A5- connected internally )
//            |A5-              |
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//  M.Seamen/ P. Thanigai
//  Texas Instruments Inc.
//  September 2008
//  Built with IAR Embedded Workbench V4.11A and CCE V3.2
//*****************************************************************************
#include <msp430.h>

#define LOW_BAT 0xB999                      // ~3V

static unsigned int ADCresult;

int main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization
  P4OUT &= ~0x40;
  P4DIR |= 0x40;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
                                            // stabilize

  SD16CTL = SD16REFON+SD16SSEL0+SD16DIV_3;  // 1.2V ref, SMCLK/8
  SD16CCTL0 |= SD16SNGL+SD16IE ;            // Single conv, enable interrupt
  SD16INCTL0 |= SD16INCH_5;                 // Select Channel A5
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup

  while (1)
  {
    SD16CCTL0 |= SD16SC;                    // Set bit to start conversion
    __bis_SR_register(LPM0_bits+GIE);       // Enter LPM0
   if( ADCresult < LOW_BAT )
   {
    P4OUT |= 0x40;
   }
   else
   {
     P4OUT &= ~0x40;
   }
    __no_operation();                       //SET BREAKPOINT HERE
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD16A_VECTOR
__interrupt void SD16ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD16A_VECTOR))) SD16ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (SD16IV)
  {
  case 2:                                   // SD16MEM Overflow
    break;
  case 4:                                   // SD16MEM0 IFG
    ADCresult = SD16MEM0;                   // Save results (clears IFG)
    break;
  }

  __bic_SR_register_on_exit(LPM0_bits);                   // Exit LPM0
}


