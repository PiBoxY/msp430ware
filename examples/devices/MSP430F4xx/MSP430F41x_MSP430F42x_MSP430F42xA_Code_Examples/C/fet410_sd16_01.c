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
//  MSP-FET430P410 Demo - SD16, Continuous Conversion on a Group of Channels
//
//  Description: This program uses the SD16 module to perform continuous
//  conversions on a group of channels (0, 1, and 2). A SD16 interrupt occurs
//  whenever the conversions have completed. Test by applying voltages to the
//  three input channels and setting a breakpoint at the line indicated
//  below. Run program until it reaches the breakpoint, then use
//  the debugger's watch window to view the conversion results. Results
//  (upper 16 bits only) are stored in three arrays, one for each channel.
//  ACLK = LFXT1 = 32768 Hz, MCLK = SMCLK = DCO = 32 x ACLK = 1048576 Hz
//  //* An external watch crystal on XIN XOUT is required for ACLK     *//	
//  //* Minimum Vcc is required for SD16 module - see datasheet        *//
//  //* 100nF cap btw Vref and AVss is recommended when using 1.2V ref *//
//
//                MSP430F427
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//   Vin1+ -->|A0.0+            |
//   Vin1- -->|A0.0-            |
//   Vin2+ -->|A1.0+            |
//   Vin2- -->|A1.0-            |
//   Vin3+ -->|A2.0+            |
//   Vin3- -->|A2.0-            |
//            |                 |
//            |            VREF |---+
//            |                 |   |
//            |                 |  -+- 100nF
//            |                 |  -+-
//            |                 |   |
//            |            AVss |---+
//            |                 |
//
//  H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//*****************************************************************************
#include <msp430.h>

#define   Num_of_Results   8

/* Arrays to store SD16 conversion results */
/* NOTE: arrays need to be global to       */
/*       prevent removal by compiler       */
static volatile unsigned int Ch0results[Num_of_Results];
static volatile unsigned int Ch1results[Num_of_Results];
static volatile unsigned int Ch2results[Num_of_Results];

int main(void)
{
  volatile unsigned int i;                  // Use volatile to prevent removal
                                            // by compiler optimization

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps
  for (i = 0; i < 10000; i++);              // Delay for 32 kHz crystal to
                                            // stabilize

  SD16CTL = SD16REFON + SD16SSEL0;          // 1.2V ref, SMCLK
  SD16CCTL0 |= SD16GRP;                     // Group with CH1
  SD16CCTL1 |= SD16GRP;                     // Group with CH2
  SD16CCTL2 |= SD16IE;                      // Enable interrupt
  for (i = 0; i < 0x3600; i++);             // Delay for 1.2V ref startup

  __enable_interrupt();                     // Enable general interrupts
  SD16CCTL2 |= SD16SC;                      // Set bit to start conversion
  __bis_SR_register(LPM0_bits);             // Enter LPM0
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD16_VECTOR
__interrupt void SD16ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD16_VECTOR))) SD16ISR (void)
#else
#error Compiler not supported!
#endif
{
  static unsigned int index = 0;

  switch (SD16IV)
  {
  case 2:                                   // SD16MEM Overflow
    break;
  case 4:                                   // SD16MEM0 IFG
    break;
  case 6:                                   // SD16MEM1 IFG
    break;
  case 8:                                   // SD16MEM2 IFG
    Ch0results[index] = SD16MEM0;           // Save CH0 results (clears IFG)
    Ch1results[index] = SD16MEM1;           // Save CH1 results (clears IFG)
    Ch2results[index] = SD16MEM2;           // Save CH2 results (clears IFG)

    if (++index == Num_of_Results)
    {
      index = 0;                            // SET BREAKPOINT HERE
    }

    break;
  }
}




