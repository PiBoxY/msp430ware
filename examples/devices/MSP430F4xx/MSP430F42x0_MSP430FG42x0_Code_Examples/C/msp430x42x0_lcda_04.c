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
//   MSP430x42x0 Demo - LCD_A Put "0123456" on SBLCDA4 LCD
//
//  Description; Put "0123456" on SBLCDA4 LCD.
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCO = 32xACLK = 1.048576MHz
//  //*An external watch crystal is required on XIN/XOUT for ACLK*//	
//               MSP430F4270
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |                 |    SBLCDA4
//           |             S0  |     -----------------
//           |              -  |--> | + 7 6 5 4 3 2 1 |
//           |             S13 |     -----------------
//           |             COM0|-----||||
//           |             COM1|------|||
//           |             COM2|-------||
//           |             COM3|--------|
//           |                 |
//
//  L. Westlund / S. Karthikeyan
//  Texas Instruments Inc.
//  June 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//*****************************************************************************
#include <msp430.h>


// LCD segment definitions.
#define d 0x80
#define c 0x40
#define b 0x20
#define a 0x10
#define h 0x08
#define e 0x04
#define g 0x02
#define f 0x01

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
  int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FLL_CTL0 |= XCAP14PF;                     // Set load cap for 32k xtal
  LCDACTL = LCDON + LCD4MUX + LCDFREQ_128;  // 4mux LCD, ACLK/128
  LCDAPCTL0 = 0x0F;                         // Segments 0-13

  P5SEL  = 0x1C;                            // Set COM pins for LCD

  for( i = 0; i < 20; i ++)
  {
    LCDMEM[i] = 0;                          // Clear LCD
  }

  for (i=0; i<7; ++i)                      // Display "0123456"
  {
    LCDMEM[i] = char_gen[i];
  }
  __bis_SR_register(LPM3_bits);            // Enter LPM3
}
