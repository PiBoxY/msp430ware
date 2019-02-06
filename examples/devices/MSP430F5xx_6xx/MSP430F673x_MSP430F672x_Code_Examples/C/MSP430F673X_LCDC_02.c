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
//   MSP430F673x Demo - LCD_C, Alternates Between Two Strings
//
//   Description: The LCD blinks on a WDT interrupt, showing two different
//                character strings on the LCD.
//                ACLK = REF0 = 32Khz, MCLK = SMCLK = DCO = Default
//
//                MSP430F673x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |    TI LCD
//            |              S0 |     -----------------
//            |               - |--> | + 7 6 5 4 3 2 1 |
//            |             S11 |     -----------------
//            |             COM0|-----||||
//            |             COM1|------|||
//            |             COM2|-------||
//            |             COM3|--------|
//            |                 |
//
//
//  W. Goh
//  Texas Instruments Inc.
//  January 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

// LCD Segment Mapping
const unsigned int LCD_Char_Map[] =
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
    BIT2 | BIT3 | BIT5 | BIT6 | BITD | BITF,                // 'M'        10
    BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT7,                // 'S'        11
    BIT0 | BIT1 | BIT2 | BIT3 | BIT6 |BIT7                  // 'P'        12
};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup LCD_C
    // LCD_FREQ = ACLK/32/2, LCD Mux 4, turn on LCD
    // LCD_BLK_FREQ = ACLK/512
    LCDCCTL0 =  LCDDIV_31 | LCDPRE_1 | LCD4MUX | LCDON;

    //Charge pump generated internally at 2.96V, external bias (V2-V4) generation
    //Internal reference for charge pump
    LCDCVCTL = LCDCPEN | VLCD_2_96;
    REFCTL0 &= ~REFMSTR;

    LCDCPCTL0 = 0x0FFF;                     //Select LCD Segments 0-11
    LCDCPCTL1 = 0x0000;                     //

    //LCD Memory
    LCDM1 = (unsigned char) (LCD_Char_Map[10] & 0xFF);          // M - Lower byte
    LCDM2 = (unsigned char) ((LCD_Char_Map[10] >> 8) & 0xFF);   // M - Upper byte
    LCDM3 = (unsigned char) (LCD_Char_Map[11] & 0xFF);          // S - Lower byte
    LCDM4 = (unsigned char) ((LCD_Char_Map[11] >> 8) & 0xFF);   // S - Upper byte
    LCDM5 = (unsigned char) (LCD_Char_Map[12] & 0xFF);          // P - Lower byte
    LCDM6 = (unsigned char) ((LCD_Char_Map[12] >> 8) & 0xFF);   // P - Upper byte

    // Blink Memory
    LCDBM1 = (unsigned char) (LCD_Char_Map[4] & 0xFF);          // 4 - Lower byte
    LCDBM2 = (unsigned char) ((LCD_Char_Map[4] >> 8) & 0xFF);   // 4 - Upper byte
    LCDBM3 = (unsigned char) (LCD_Char_Map[3] & 0xFF);          // 3 - Lower byte
    LCDBM4 = (unsigned char) ((LCD_Char_Map[3] >> 8) & 0xFF);   // 3 - Upper byte
    LCDBM5 = (unsigned char) (LCD_Char_Map[0] & 0xFF);          // 0 - Lower byte
    LCDBM6 = (unsigned char) ((LCD_Char_Map[0] >> 8) & 0xFF);   // 0 - Upper byte

    // Setup WDT
    WDTCTL = WDT_ADLY_1000;                 // WDT 1000ms, ACLK, interval timer
    SFRIE1 |= WDTIE;                        // Enable WDT interrupt

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM3 w/ interrupt
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
    LCDCMEMCTL ^= LCDDISP;                  // Change LCD character string
}

