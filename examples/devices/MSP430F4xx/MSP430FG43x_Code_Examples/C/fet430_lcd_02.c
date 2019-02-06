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
//  MSP-FET430P430 Demo - LCD, Display "430" on Varitronix VI-322 LCD
//
//  Description: Displays "430" on Varitronix VI-322 LCD as used on
//  Softbaugh D437 board.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//	
//               MSP430FG439
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |      Varitronix VI-322
//           |                 |     (As used on Softbaugh D437 board)
//           |             S0  |     ---------
//           |              -  |--> | + 3 2 1 |
//           |             S31 |     ---------
//           |             COM0|-----|
//           |                 |
//           |                 |
//           |                 |
//           |                 |
//
//
//  L. Westlund / H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************
#include <msp430.h>

#define a 0x001
#define b 0x010
#define c 0x002
#define d 0x020
#define e 0x004
#define f 0x040
#define g 0x008
#define h 0x080

const char digit[10] = {                // As used on Softbaugh D437 board
  a+b+c+d+e+f,                          // Displays "0"
  b+c,                                  // Displays "1"
  a+b+d+e+g,                            // Displays "2"
  a+b+c+d+g,                            // Displays "3"
  b+c+f+g,                              // Displays "4"
  a+c+d+f+g,                            // Displays "5"
  a+c+d+e+f+g,                          // Displays "6"
  a+b+c,                                // Displays "7"
  a+b+c+d+e+f+g,                        // Displays "8"
  a+b+c+d+f+g                           // Displays "9"
};

int toDisplay = 0x430;

int main(void)
{
  int i;
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  FLL_CTL0 |= XCAP18PF;                 // Configure load caps
  LCDCTL = LCDON + LCDSTATIC + LCDOGOFF;// STK LCD 4Mux, S0-S17
  BTCTL = BTFRFQ1+BTFRFQ0;              // STK LCD freq
  P5SEL = 0xFC;                         // Common and Rxx all selected
                                        // For P5.0, P5.1, P4.6, P4.7 setting
                                        // SEL bits selects Analog for the FG

  for( i = 0; i < 20; i ++)
  {
    LCDMEM[i] = 0;                      // Clear LCD
  }

  for (;;)
  {
    int data;                           // LCD display Data

    data = digit[toDisplay&0x0F];       // get the LCD data for 1st digit
    LCDMEM[0] = data;                   // write data to LCD Memory...
    data = data>>1;                     // shift data for next location...
    LCDMEM[1] = data;
    data = data>>1;
    LCDMEM[2] = data;
    data = data>>1;
    LCDMEM[3] = data;

    data = digit[(toDisplay>>4)&0x0F];  // get the LCD data for 2nd digit
    LCDMEM[4] = data;                   // write data to LCD Memory...
    data = data>>1;                     // shift data for next location...
    LCDMEM[5] = data;
    data = data>>1;
    LCDMEM[6] = data;
    data = data>>1;
    LCDMEM[7] = data;
                                        // skip two for this LCD
    data = digit[(toDisplay>>8)&0x0F];  // get the LCD data for 3rd digit
    LCDMEM[10] = data;                  // write data to LCD Memory...
    data = data>>1;                     // shift data for next location...
    LCDMEM[11] = data;
    data = data>>1;
    LCDMEM[12] = data;
    data = data>>1;
    LCDMEM[13] = data;

  }
}
