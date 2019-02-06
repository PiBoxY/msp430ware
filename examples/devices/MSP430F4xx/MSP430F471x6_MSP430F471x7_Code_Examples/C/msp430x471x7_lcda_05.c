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
//    MSP430x471xx Demo - LCD_A Put "0123456" on SBLCDA4 LCD with charge pump
//
//  Description; Put "0123456" on SBLCDA4 LCD. Charge pump is enabled.
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCO = 32xACLK = 1.048576MHz
//  //*An external watch crystal is required on XIN/XOUT for ACLK*//	
// Note: A 4.7 uF or larger capacitor must be connected from pin LCDCAP to 
// ground when the internal charge pump is enabled. Otherwise, damage can occur.
//
//               MSP430x471xx
//           -----------------
//       /|\|              XIN|-
//        | |                 | 32kHz
//        --|RST          XOUT|-         SBLCDA4 
//          |                 |        ___________
//          |              S0 |------>|14         |
//          |              S1 |------>|13         | 
//          |             ..  | ....  |..         |
//          |             S13 |------>|1          | 
//    ------|LCDCAP/R33   COM0|------>|15         |
//   CAP    |             COM1|------>|16         | 
//  TO GND  |             COM2|------>|17         | 
//          |             COM3|------>|18         |
//          |             S14 |------>|19         | 
//          |             S19 |------>|20         | 
//          |              .. | ....  |..         | 
//    ------|LCDCAP/R33   S21 |------>|26         |     
//          |_________________|       |___________| 
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include <msp430.h>

// LCD segment definitions.
#define h 0x80
#define e 0x40
#define g 0x20
#define f 0x10
#define d 0x08
#define c 0x04
#define b 0x02
#define a 0x01

const char char_gen[] = {                   // definitions for digits
  a+b+c+d+e+f,                              // Displays "0"
  b+c,                                      // Displays "1"
  a+b+d+e+g,                                // Displays "2"
  a+b+c+d+g,                                // Displays "3"
  b+c+f+g,                                  // Displays "4"
  a+c+d+f+g,                                // Displays "5"
  a+c+d+e+f+g,                              // Displays "6"
  a+b+c,                                    // Displays "7"
  a+b+c+d+e+f+g,                            // Displays "8"
  a+b+c+d+f+g                               // Displays "9"
};
int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW +WDTHOLD;                  // Stop WDT
  FLL_CTL0 |= XCAP11PF;                     // Configure load caps

  // Wait for xtal to stabilize
  do
  {
    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
    for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
  
  P5SEL =BIT1+BIT2+BIT3+BIT4;               // Set COM pins for LCD
  LCDACTL = LCDON+LCD4MUX+ LCDFREQ_128;     // 4mux LCD, ACLK/32
  LCDAPCTL0 = 0x7F;                         // Segments 0-24
  LCDAVCTL0 = LCDCPEN;                      // charge pump enable
  LCDAVCTL1 = VLCD3;                        // to 3.02V

  for( i = 0; i < 20; i ++)
  {
    LCDMEM[i] = 0;                          // Clear LCD
  }

  for (i=0;i<7;i++)
  {
    LCDMEM[i] = char_gen[i];
  }
  
  __bis_SR_register(LPM3_bits);            // Enter LPM3

}

