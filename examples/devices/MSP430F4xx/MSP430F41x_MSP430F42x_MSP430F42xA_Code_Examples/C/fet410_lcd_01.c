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
//  MSP-FET430P410 Demo - LCD, Display "6543210" on STK/EVK LCD
//
//  Description: Display "6543210" on MSP-EVK430S320 LCD.
//  ACLK = LFXT1 = 32768, MCLK = SMCLK = DCOCLK = (121+1) x 2 x ACLK = 7995392Hz
//  //* An external watch crystal between XIN & XOUT is required for ACLK *//	
//
//                                 Connections MSP430 -> LCD
//                                 -------------------------
//
//                                T.I.          T.I. MSP-EVK430S320
//                           MSP430x41x MCU   STK/EVK 6.5 digit 4 mux LCD
//                                                    #T218010
//                           ---------------       --------------
//                          |          COM3 |-----|2    COM4     |
//                          |          COM2 |-----|1    COM3     |
//                          |          COM1 |-----|3    COM2     |
//                          |          COM0 |-----|4,20 COM1     |
//                          |          SEG0 |-----|19            |
//                          |          SEG1 |-----|18            |
//                          |          SEG2 |-----|17            |
//                          |          SEG3 |-----|16            |
//                          |          SEG4 |-----|15            |
//                          |          SEG5 |-----|14            |
//                          |          SEG6 |-----|13            |
//                          |          SEG7 |-----|12            |
//                          |          SEG8 |-----|11            |
//                          |          SEG9 |-----|10            |
//                          |          SEG10|-----|9             |
//                          |          SEG11|-----|8             |
//                          |          SEG12|-----|7             |
//                          |          SEG13|-----|6             |
//                          |          SEG14|-----|5 (bits C,E,H |
//                          |               |     |   of digit 7)|
//                          |               |      --------------
//                          |               |
//                           ---------------
//
//                   NOTE: Pin R03 on the MSP430 must be connected to GND
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//*****************************************************************************
#include <msp430.h>

const char digit[10] =
{
  0xB7,  /* "0" LCD segments a+b+c+d+e+f */
  0x12,  /* "1" */
  0x8F,  /* "2" */
  0x1F,  /* "3" */
  0x3A,  /* "4" */
  0x3D,  /* "5" */
  0xBD,  /* "6" */
  0x13,  /* "7" */
  0xBF,  /* "8" */
  0x3F   /* "9" */
};

int main(void)
{
   int i;
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps
  LCDCTL = LCDP2 + LCD4MUX + LCDON;         // STK LCD 4-Mux, segments S0-S27
  BTCTL = BTFRFQ1;                          // STK LCD freq = ACLK/128
  P5SEL = 0xFC;                             // Set Rxx and COM pins for LCD

  for (;;)
  {
    for (i=0; i<7; ++i)                     // Display "6543210"
      LCDMEM[i] = digit[i];
  }
}


