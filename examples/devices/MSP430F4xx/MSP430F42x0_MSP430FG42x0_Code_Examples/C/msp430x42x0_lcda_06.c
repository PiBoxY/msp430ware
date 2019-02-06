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
//   MSP430x42x0 Demo - LCD_A run a 0-9 counter on TI LCD
//
//  Description; Count 0-9 on a TI LCD
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCO = 32xACLK = 1.048576MHz
//  Segments attached in accordance with Static LCD diagram in 4xx User's Guide
//  //*An external watch crystal is required on XIN/XOUT for ACLK*//	
//  //* WARNING: Do NOT enable Charge Pump with Static LCD *//
//  //* See Errata for more details: LCDA1 *//
//               MSP430F4270
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |                 |    TI LCD
//           |             S0  |     -----
//           |              -  |--> | 2 1 |
//           |             S6  |     -----
//           |             COM0|-----|
//           |                 |
//           |                 |
//           |                 |
//           |                 |
//
//  L. Westlund / S. Karthikeyan
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.30A
//*****************************************************************************
#include <msp430.h>

const char digit[40] =
{
  0x11, 0x11,  // '0'                   LCD segments b+a & d+c = lower two bytes
  0x11, 0x00,  // '0'                   LCD segments f+e & h+g = upper two bytes
  0x10, 0x01,  // '1'
  0x00, 0x00,  // '1'
  0x11, 0x10,  // '2'
  0x01, 0x01,  // '2'
  0x11, 0x11,  // '3'
  0x00, 0x01,  // '3'
  0x10, 0x01,  // '4'
  0x10, 0x01,  // '4'
  0x01, 0x11,  // '5'
  0x10, 0x01,  // '5'
  0x01, 0x11,  // '6'
  0x11, 0x01,  // '6'
  0x11, 0x01,  // '7'
  0x00, 0x00,  // '7'
  0x11, 0x11,  // '8'
  0x11, 0x01,  // '8'
  0x11, 0x11,  // '9'
  0x10, 0x01,  // '9'
};
int main(void)
{
  int digit_ptr;
  int value = 0;
  int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FLL_CTL0 |= XCAP14PF;                     // Set load cap for 32k xtal
  LCDACTL = LCDON + LCDSTATIC + LCDFREQ_128;// 4mux LCD, ACLK/128
  LCDAPCTL0 = 0x03;                         // S0-S6 used

  for( i = 0; i < 20; i ++)
  {
    LCDMEM[i] = 0;                          // Clear LCD
  }

  for (;;)
  {
    volatile unsigned int index;
    digit_ptr = value << 2;
    for (index = 0; index < 4; index++)     // Load 4 bytes of digit
    {
      LCDMEM[index] = digit[digit_ptr++];   // Byte of digit to byte of
    }

    for( index = 0; index < 50000; index++ ){} // delay loop
    if( value == 9 )
    {
      value = 0;                            // Reset counter
    }
    else
    {
      value++;
    }

  }
}
