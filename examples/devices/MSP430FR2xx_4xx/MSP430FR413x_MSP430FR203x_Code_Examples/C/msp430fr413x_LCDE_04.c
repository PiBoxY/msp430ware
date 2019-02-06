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
//   MSP430FR4133 Demo -  LCD_E, Display "HELLO WORLD" using blink memory.
//
//   Description: Displays "HELLO" then "WORLD" using LCD blink memory while the
//                device is in LPM3.5.
//                f(LCD) = 32768Hz/((7+1)*16) = 256Hz,
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
//  LCD pin   FR4133_LineX
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
//  Kathryn Adamsky
//  Texas Instruments Inc.
//  June 2016
//  Built with Code Composer Studio v6.1.3
//******************************************************************************
#include <msp430.h>

void showChar(char c, int position);
void showCharB(char c, int position);
#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18
// Define word access definitions to LCD memories
#define LCDMEMW ((int*)LCDMEM)
#define LCDBMEMW ((int*)LCDBMEM)

// LCD memory map for numeric digits
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

//Writes to screen 1
 void showChar(char c, int position)
 {
     if (c == ' ')
     {
         // Display space
         LCDMEMW[position/2] = 0;
     }
     else if (c >= '0' && c <= '9')
     {
         // Display digit
         LCDMEMW[position/2] = digit[c-48][0] | (digit[c-48][1] << 8);
     }
     else if (c >= 'A' && c <= 'Z')
     {
         // Display alphabet
         LCDMEMW[position/2] = alphabetBig[c-65][0] | (alphabetBig[c-65][1] << 8);
     }
     else
     {
         // Turn all segments on if character is not a space, digit, or uppercase letter
         LCDMEMW[position/2] = 0xFFFF;
     }
 }

 //Writes to screen 2
 void showCharB(char c, int position)
 {
     if (c == ' ')
     {
         // Display space
         LCDBMEMW[position/2] = 0;
     }
     else if (c >= '0' && c <= '9')
     {
         // Display digit
         LCDBMEMW[position/2] = digit[c-48][0] | (digit[c-48][1] << 8);
     }
     else if (c >= 'A' && c <= 'Z')
     {
         // Display alphabet
         LCDBMEMW[position/2] = alphabetBig[c-65][0] | (alphabetBig[c-65][1] << 8);
     }
     else
     {
         // Turn all segments on if character is not a space, digit, or uppercase letter
         LCDBMEMW[position/2] = 0xFFFF;
     }
 }

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

    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                            // flcd ref freq is xtclk

    //LCD Operation - Mode 2
    LCDVCTL = LCDCPEN |                                        //Enable charge pump
            LCDSELVDD |                                        //Internally connect to Vcc
            VLCD_8 |                                           //internal 3.08v
           (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);//Charge pump frequency selection 256Hz

    LCDMEMCTL |= LCDCLRM | LCDCLRBM;                           // Clear LCD memory

    LCDCSSEL0 = 0x000F;                                        // Configure COMs and SEGs
    LCDCSSEL1 = 0x0000;                                        // L0, L1, L2, L3: COM pins
    LCDCSSEL2 = 0x0000;

    LCDM0 = 0x21;                                              // L0 = COM0, L1 = COM1
    LCDM1 = 0x84;                                              // L2 = COM2, L3 = COM3


    showChar('H', pos2);
    showChar('E', pos3);
    showChar('L', pos4);
    showChar('L', pos5);
    showChar('O', pos6);

    LCDBM0 = 0x21;
    LCDBM1 = 0x84;

    showCharB('W', pos1);
    showCharB('O', pos2);
    showCharB('R', pos3);
    showCharB('L', pos4);
    showCharB('D', pos5);


    LCDBLKCTL = LCDBLKPRE__512 |                               //Divide xtclk by 512
            LCDBLKMOD_3;                                       //Switch between memory contents of LCDM and LCDB

    LCDCTL0 |= LCD4MUX | LCDON;                                // Turn on LCD, 4-mux selected

    PMMCTL0_H = PMMPW_H;                                       // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF_L;                                  // and set PMMREGOFF

    __bis_SR_register(LPM3_bits | GIE);                        // Enter LPM3
    __no_operation();                                          // For debugger
}


