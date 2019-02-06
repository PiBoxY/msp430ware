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
//  MSP430FR60xx Demo - LCD_C, 4-Mux example, display "430" string on the LCD.
//
//  Description: This examples configures the LCD in 4-Mux mode and displays
//  "430" on a TI LCD.
//  This example would need to be modified based on the final LCD and GPIO
//  routing where lcd_num[] would need to be modified.
//  Charge pump is disabled. The external bias resistors are used.
//
//  It uses LCD pin L0~L26 as segment pins.
//  f(LCD) = 32768Hz/((1+1)*2^4) = 1024Hz, ACLK = ~32768Hz,
//  MCLK = SMCLK = default DCODIV ~1MHz.
//
//                 MSP430FR6047
//              -----------------
//          /|\|                 |
//           | |              XIN|--
//           --|RST              |  32768Hz
//             |             XOUT|--
//             |                 |
//             |             COM3|----------------|
//             |             COM2|---------------||
//             |             COM1|--------------|||
//             |             COM0|-------------||||
//             |                 |    -------------
//             |           Sx~Sxx|---| 1 2 3 4 5 6 |
//             |                 |    -------------
//             |                 |       TI LCD
//
//
//   Nima Eskandari
//   Texas Instruments Inc.
//   April 2017
//   Built with IAR Embedded Workbench V7.4 & Code Composer Studio V7.1
//*****************************************************************************

#include "msp430.h"

#define A1_DIGIT    LCDM1   //Digit A1
#define A1_AlPHA    LCDM2   //Alphanumeric part of A1
#define A2_DIGIT    LCDM3   //Digit A2
#define A2_AlPHA    LCDM4   //Alphanumeric part of A2
#define A3_DIGIT    LCDM5   //Digit A3
#define A3_AlPHA    LCDM6   //Alphanumeric part of A3
#define A4_DIGIT    LCDM7   //Digit A4
#define A4_AlPHA    LCDM8   //Alphanumeric part of A4
#define A5_DIGIT    LCDM9   //Digit A5
#define A5_AlPHA    LCDM10  //Alphanumeric part of A5

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

    //Initialize COMS pins and External bias resistor pins
    P6SEL0 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P6SEL1 = (BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    // Initialize LCD segments 0 - 26;
    LCDCPCTL0 = 0xFFFF;     //0-15
    LCDCPCTL1 = 0x07FF;     //16-26

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
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | (LCDMX1+LCDMX0+LCDSON) | LCDLP;

    // Charge pump disabled
    //Internal VLCD, external Rx3 pins, external contrast
    LCDCVCTL = LCDREXT | R03EXT;
    //To use internal bias resistors: LCDCVCTL = 0;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory

    A1_DIGIT = lcd_num[4];                  //Display number 4 on A1
    A2_DIGIT = lcd_num[3];                  //Display number 3 on A2
    A3_DIGIT = lcd_num[0];                  //Display number 0 on A3

    //Turn LCD on
    LCDCCTL0 |= LCDON;

    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();

    return 0;
}
