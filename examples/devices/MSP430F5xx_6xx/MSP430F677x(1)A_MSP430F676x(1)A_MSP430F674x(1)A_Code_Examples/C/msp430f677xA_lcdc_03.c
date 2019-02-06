/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//   MSP430F67791A Demo - LCD_C, Display 6 digit numbers
//
//   Description: Displays "000000" to "999999" in sequence on the LCD.
//                ACLK = REF0 = 32Khz, MCLK = SMCLK = DCO = Default
//
//               MSP430F67791A
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |    TI LCD
//            |              S0 |     -----------------
//            |               - |--> | +  5 4 3 2 1 0 |
//            |             S23 |     -----------------
//            |             COM0|-----||||
//            |             COM1|------|||
//            |             COM2|-------||
//            |             COM3|--------|
//            |                 |
//
//
//   E. Chen
//   Texas Instruments Inc.
//   January 2014
//   Built with CCS Version: 5.5.0 and IAR Embedded Workbench Version: 5.52
//******************************************************************************
#include <msp430.h>

// LCD Segment Mapping
const unsigned char LCD_Char_Map[] =
{
    BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7,                // '0' or 'O'
    BIT5 | BIT6,                                            // '1' or 'I'
    BIT0 | BIT1 | BIT3 | BIT4 | BIT6 | BIT7,                // '2' or 'Z'
    BIT0 | BIT1 | BIT4 | BIT5 | BIT6 | BIT7,                // '3'
    BIT0 | BIT1 | BIT2 | BIT5 | BIT6,                       // '4' or 'y'
    BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT7,                // '5' or 'S'
    BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7,         // '6' or 'b'
    BIT5 | BIT6 | BIT7,                                     // '7'
    BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7,  // '8' or 'B'
    BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6 | BIT7,         // '9' or 'g'
};

char hun_thou, ten_thou, thou, hun, ten, unit;

void main(void)
{
    volatile unsigned char i;

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup LCD_C
    // LCD_FREQ = ACLK/32/4, LCD Mux 4, turn on LCD
    LCDCCTL0 =  LCDDIV_31 | LCDPRE_1 | LCD4MUX | LCDON;

    //Charge pump generated internally at 2.96V, external bias (V2-V4) generation
    //Internal reference for charge pump
    LCDCVCTL = LCDCPEN | VLCD_2_96;
    REFCTL0 &= ~REFMSTR;

    LCDCPCTL0 = 0xFFFF;                     //Select LCD Segments 0-23
    LCDCPCTL1 = 0x00FF;                     //

    while (1)
    {
      for (hun_thou=0; hun_thou<=9; hun_thou++)
      {
       for (ten_thou=0; ten_thou<=9; ten_thou++)
       {
         for (thou =0; thou<=9; thou++)
         {
           for (hun=0; hun<=9; hun++)
           {
             for (ten=0; ten<=9; ten++)
             {
                for (unit=0; unit<=9; unit++)
                {
                  LCDM1 = LCD_Char_Map[hun_thou] & 0xFF;        // Lower byte
                  LCDM2 = (LCD_Char_Map[hun_thou] >> 8) & 0xFF; // Upper byte
                  LCDM3 = LCD_Char_Map[ten_thou] & 0xFF;        // Lower byte
                  LCDM4 = (LCD_Char_Map[ten_thou] >> 8) & 0xFF; // Upper byte
                  LCDM5 = LCD_Char_Map[thou] & 0xFF;          // Lower byte
                  LCDM6 = (LCD_Char_Map[thou] >> 8) & 0xFF;   // Upper byte
                  LCDM7 = LCD_Char_Map[hun] & 0xFF;          // Lower byte
                  LCDM8 = (LCD_Char_Map[hun] >> 8) & 0xFF;   // Upper byte
                  LCDM9 = LCD_Char_Map[ten] & 0xFF;          // Lower byte
                  LCDM10 = (LCD_Char_Map[ten] >> 8) & 0xFF;   // Upper byte
                  LCDM11 = LCD_Char_Map[unit] & 0xFF;          // Lower byte
                  LCDM12 = (LCD_Char_Map[unit] >> 8) & 0xFF;   // Upper byte
                  __delay_cycles(500000);
                }
             }
           }
         }
       }
     }
    }//Display the number's digits on the LCD
}
