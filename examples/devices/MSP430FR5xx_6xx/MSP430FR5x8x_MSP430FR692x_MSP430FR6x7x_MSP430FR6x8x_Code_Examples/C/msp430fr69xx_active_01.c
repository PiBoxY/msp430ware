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
//   MSP430FR69xx Demo - Stay in active mode with MCLK = 8MHz
//
//   Description:
//   The activeModeTest() function is intended to mimic a typical cache hit
//   ratio for a real world application. The code itself is a combination of
//   move, jump and math operations performed in an infinite loop.
//   The active power for a typical cache hit ratio using MCLK=8MHz is measured
//
//   ACLK = VLO, MCLK = SMCLK = 8MHz
//
//
//          MSP430FR6989
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |
//        |          P1.0 |---> Disconnect JP9 for power meas.
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************

#include <msp430.h>

void activeModeTest(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    // Configure all un-used GPIO to lowest power state
    P1DIR = 0xFF;
    P1OUT = 0;

    P2DIR = 0xFF;
    P2OUT = 0;

    P3DIR = 0xFF;
    P3OUT = 0;

    P4DIR = 0xFF;
    P4OUT = 0;

    P5DIR = 0xFF;
    P5OUT = 0;

    P6DIR = 0xFF;
    P6OUT = 0;

    P7DIR = 0xFF;
    P7OUT = 0;

    P8DIR = 0xFF;
    P8OUT = 0;

    P9DIR = 0xFF;
    P9OUT = 0;

    P10DIR = 0xFF;
    P10OUT = 0;

    PJDIR = 0xFF;
    PJOUT = 0;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO setting for 8MHz
    CSCTL1 &= ~DCORSEL;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // MCLK = SMCLK = DCO = 8MHz
    CSCTL0_H = 0;                           // Lock CS registers

    // Endless loop. Function never returns...
    activeModeTest();
}

// This function executes the active mode power test.
void activeModeTest(void)
{
    __asm("IDD_AM_L:                            \n"
        "            MOV     #0x2000, R4        \n"
        "            MOV     #0x4, 0(R4)        \n"
        "            MOV     &0x2000, &0x2002   \n"
        "            ADD     @R4, 2(R4)         \n"
        "            SWPB    @R4+               \n"
        "            MOV     @R4, R5            \n"
        "IDD_AM_L1:                             \n"
        "            XOR     @R4+, &0x2020      \n"
        "            DEC     R5                 \n"
        "            JNZ     IDD_AM_L1          \n"
        "            XOR.B   #0x01, &0x0202     \n"
        "            JMP     IDD_AM_L             ");
}
