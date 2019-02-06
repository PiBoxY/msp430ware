/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//  MSP430FR69x Demo - LCD_C, 4-Mux example, display "430" string on the LCD and
//                     blinks the string every 1Hz.
//
//  Description: This examples configures the LCD in 4-Mux mode and displays
//  "430" on a TI LCD and blinks it at 1Hz interval. This example was tested
//  on MSP-EXP430FR6989 Launchpad. This example would need to be modified based
//  on the final LCD and GPIO routing where lcd_num[] would need to be modified.
//  The internal voltage is sourced to v2 through v4 and v5 is connected to
//  ground. Charge pump is enabled.
//
//  It uses LCD pin L0~L21 and L26~L43 as segment pins.
//  f(LCD) = 32768Hz/((1+1)*2^4) = 1024Hz, ACLK = ~32768Hz,
//  MCLK = SMCLK = default DCODIV ~1MHz.
//
//      MSP430FR6989 / MSP-EXP430FR6989 Launchpad
//              -----------------
//          /|\|                 |
//           | |              XIN|--
//  GND      --|RST              |  32768Hz
//   |         |             XOUT|--
//   |         |                 |
//   |         |             COM3|----------------|
//   |         |             COM2|---------------||
//   |--4.7uF -|LCDCAP       COM1|--------------|||
//             |             COM0|-------------||||
//             |                 |    -------------
//             |           Sx~Sxx|---| 1 2 3 4 5 6 |
//             |                 |    -------------
//             |                 |       TI LCD
//                                 (See MSP-EXP430FR6989 Schematic)
//
//   William Goh
//   Texas Instruments Inc.
//   May 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//*****************************************************************************
#include "msp430.h"

const unsigned char lcd_num[10] = {
    0xFC,        // 0
    0x60,        // 1
    0xDB,        // 2
    0xF3,        // 3
    0x67,        // 4
    0xB7,        // 5
    0xBF,        // 6
    0xE4,        // 7
    0xFF,        // 8
    0xF7,        // 9
};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory

    // Display "4" on Segment 36,37
    LCDM19 = lcd_num[4];

    // Display "3" on " on Segment 28,29
    LCDM15 = lcd_num[3];

    // Display "0" on " on Segment 14,15
    LCDM8 = lcd_num[0];

    // Blink all segments, BLK_FREQ = 1Hz
    LCDCBLKCTL |= LCDBLKPRE1 | LCDBLKPRE2 | LCDBLKMOD_2;

    //Turn LCD on
    LCDCCTL0 |= LCDON;

    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();

    return 0;
}
