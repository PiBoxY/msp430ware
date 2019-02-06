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
//   MSP430FR413x Demo -  LCD_E, Display "123456" and blink in LPM3.5 mode.
//
//   Description: Displays "123456" in sequence to the LCD display.
//                f(LCD) = 32768Hz/((7+1)*64) = 256Hz,
//                f(BLINK) = f(LCD)/((3+1)*64) = 1Hz.
//                MSP430 works in LPM3.5 mode for ultra low power.
//                ACLK = default REFO ~32768Hz, 
//                MCLK = SMCLK = default DCODIV ~1MHz.
//
//                MSP430FR4133
//             -----------------
//         /|\|                 |
//          | |              XIN|--
// GND      --|RST              |  ~32768Hz
//  |         |             XOUT|--
//  |--0.1uF--|R13              |
//  |--0.1uF--|R23      (L3)COM3|----------------|
//  |--0.1uF--|R33      (L2)COM2|---------------||
//          --|LCDC2    (L1)COM1|--------------|||
//     4.7uF  |         (L0)COM0|-------------||||
//          --|LCDC1            |    -------------
//            |           L4~L39|---| 1 2 3 4 5 6 |
//            |   except L27~L35|    -------------
//            |                 |       TI LCD
//            |                 |
//
//  LCD pin - Port Pin Map
//  LCD pin   G6021_LineX
//    1         L8  (P3.0)
//    2         L9  (P3.1)
//    3         L10 (P3.2)
//    4         L11 (P3.3)
//    5         L12 (P3.4)
//    6         L13 (P3.5)
//    7         L14 (P3.6)
//    8         L15 (P3.7)
//    9         L16 (P6.0)
//    10        L17 (P6.1)
//    11        L18 (P6.2)
//    12        L19 (P6.3)
//    13        L20 (P6.4)
//    14        L21 (P6.5)
//    15        L22 (P6.6)
//    16        L23 (P6.7)
//    17        L4  (P7.4)
//    18        L5  (P7.5)
//    19        L6  (P7.6)
//    20        L7  (P7.7)
//    21        L3  (P7.3)
//    22        L2  (P7.2)
//    23        L1  (P7.1)
//    24        L0  (P7.0)
//    25        -
//    26        -
//    27        -
//    28        -
//    29        -
//    30        -
//    31        -
//    32        L24 (P2.0)
//    33        L25 (P2.1)
//    34        L26 (P2.2)
//    35        L36 (P5.4)
//    36        L37 (P5.5)
//    37        L38 (P5.6)
//    38        L39 (P5.7)
//
//  Cen Fang
//  Wei Zhao
//  Texas Instruments Inc.
//  Oct 2013
//  Built with IAR Embedded Workbench v5.60 & Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18

const char digit[10] =
{
    0xFC,                                                      // "0"
    0x60,                                                      // "1"
    0xDB,                                                      // "2"
    0xF3,                                                      // "3"
    0x67,                                                      // "4"
    0xB7,                                                      // "5"
    0xBF,                                                      // "6"
    0xE4,                                                      // "7"
    0xFF,                                                      // "8"
    0xF7                                                       // "9"
};

int main( void )
{
    WDTCTL = WDTPW | WDTHOLD;                                  // Stop watchdog timer

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2;                                     // P4.2~P4.1: crystal pins
    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                         // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);                                  // Test oscillator fault flag
    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2;            // Higher drive strength and current consumption for XT1 oscillator

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure LCD pins
    SYSCFG2 |= LCDPCTL;                                        // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

    LCDPCTL0 = 0xFFFF;
    LCDPCTL1 = 0x07FF;
    LCDPCTL2 = 0x00F0;                                         // L0~L26 & L36~L39 pins selected

    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);

    LCDCSSEL0 = 0x000F;                                        // Configure COMs and SEGs
    LCDCSSEL1 = 0x0000;                                        // L0, L1, L2, L3: COM pins
    LCDCSSEL2 = 0x0000;

    LCDM0 = 0x21;                                              // L0 = COM0, L1 = COM1
    LCDM1 = 0x84;                                              // L2 = COM2, L3 = COM3

    LCDBLKCTL |= LCDBLKPRE2 | LCDBLKMOD_2;                     // BLK_FREQ = 1Hz, Blinking mode2

    // Display "123456"
    LCDMEM[pos1] = digit[1];
    LCDMEM[pos2] = digit[2];
    LCDMEM[pos3] = digit[3];
    LCDMEM[pos4] = digit[4];
    LCDMEM[pos5] = digit[5];
    LCDMEM[pos6] = digit[6];

    LCDCTL0 |= LCD4MUX | LCDON;                                // Turn on LCD, 4-mux selected
  
    PMMCTL0_H = PMMPW_H;                                       // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF_L;                                  // and set PMMREGOFF

    __bis_SR_register(LPM3_bits | GIE);                        // Enter LPM3.5
    __no_operation();                                          // For debugger
}

