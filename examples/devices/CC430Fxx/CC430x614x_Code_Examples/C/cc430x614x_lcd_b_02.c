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
//   CC430x614x Demo -  LCD blinks between two different strings
//
//   Description: The LCD blinks on a WDT interrupt, showing two different
//                character strings on the LCD. 
//                ACLK = REF0 = 32Khz, MCLK = SMCLK = DCO = Default
//
//                 C430F6147
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |           
//            |                 |    SBLCDA4
//            |              S0 |     -----------------
//            |               - |--> | + 7 6 5 4 3 2 1 |
//            |              S9 |     -----------------
//            |             COM0|-----||||
//            |             COM1|------|||
//            |             COM2|-------||
//            |             COM3|--------|
//            |                 |
//
//
//   G. Larmore
//   Texas Instruments Inc.
//   June 2012
//   Built with IAR Embedded Workbench V5.40.1 & Code Composer Studio V5.2
//******************************************************************************

#include <msp430.h>

// LCD Segments
#define LCD_A    BIT4
#define LCD_B    BIT5
#define LCD_C    BIT6
#define LCD_D    BIT7
#define LCD_F    BIT0
#define LCD_G    BIT1
#define LCD_E    BIT2
#define LCD_H    BIT3

// LCD Segment Mapping
const unsigned char  LCD_Char_Map[] =
{
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F,        // '0' or 'O'
  LCD_B+LCD_C,                                // '1' or 'I'
  LCD_A+LCD_B+LCD_D+LCD_E+LCD_G,              // '2' or 'Z'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_G,              // '3'
  LCD_B+LCD_C+LCD_F+LCD_G,                    // '4' or 'y'
  LCD_A+LCD_C+LCD_D+LCD_F+LCD_G,              // '5' or 'S'
  LCD_A+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,        // '6' or 'b'
  LCD_A+LCD_B+LCD_C,                          // '7'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_E+LCD_F+LCD_G,  // '8' or 'B'
  LCD_A+LCD_B+LCD_C+LCD_D+LCD_F+LCD_G,        // '9' or 'g'
  LCD_A+LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,        // 'A'    10
  LCD_A+LCD_D+LCD_E+LCD_F,                    // 'C'    11
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_G,              // 'd'    12
  LCD_A+LCD_D+LCD_E+LCD_F+LCD_G,              // 'E'    13
  LCD_A+LCD_E+LCD_F+LCD_G,                    // 'F'    14
  LCD_B+LCD_C+LCD_E+LCD_F+LCD_G,              // 'H'    15
  LCD_B+LCD_C+LCD_D+LCD_E,                    // 'J'    16
  LCD_D+LCD_E+LCD_F,                          // 'L'    17
  LCD_A+LCD_B+LCD_E+LCD_F+LCD_G,              // 'P'    18
  LCD_B+LCD_C+LCD_D+LCD_E+LCD_F               // 'U'    19
};

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  // Select LCD COM pins
  P5SEL |= (BIT5 | BIT6 | BIT7);
  P5DIR |= (BIT5 | BIT6 | BIT7);

  // Configure LCD_B
  // LCD_FREQ = ACLK/32/4, LCD Mux 4, turn on LCD
  // Charge pump generated internally at 3.44V, external bias (V2-V4) generation
  // Internal reference for charge pump
  LCDBCTL0 =  (LCDDIV0 + LCDDIV1 + LCDDIV2 + LCDDIV3 + LCDDIV4)| LCDPRE0 | LCD4MUX | LCDON | LCDSON;
  LCDBVCTL = LCDCPEN | VLCD_3_44;
  LCDBCTL0 |= LCDON + LCDSON;
  REFCTL0 &= ~REFMSTR;
  
  //Select LCD Segments 0-9
  LCDBPCTL0 = 0x03FF;
  LCDBPCTL1 = 0x0000;

  //LCD Memory
  LCDM5 = LCD_Char_Map[15];     //H
  LCDM4 = LCD_Char_Map[13];     //E
  LCDM3 = LCD_Char_Map[17];     //L
  LCDM2 = LCD_Char_Map[17];     //L
  LCDM1 = LCD_Char_Map[0];      //0
  
  // Blink Memory 
  LCDBM5 = LCD_Char_Map[11];    //C
  LCDBM4 = LCD_Char_Map[11];    //C
  LCDBM3 = LCD_Char_Map[4];     //4
  LCDBM2 = LCD_Char_Map[3];     //3
  LCDBM1 = LCD_Char_Map[0];     //0

  // Setup and enable WDT 250ms, ACLK, interval timer
  WDTCTL = WDT_ADLY_1000;
  SFRIE1 |= WDTIE;
  
  __bis_SR_register(LPM0_bits + GIE);		// Enter LPM3 w/interrupt
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
  LCDBMEMCTL ^= LCDDISP;
}
