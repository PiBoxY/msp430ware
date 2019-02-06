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
//   MSP430F665x Demo -  Enabling MID protection for one MID block
//
//
//   Description: MSP430F6659 demo to enable MID protection for one MID block
//   The device has 512kB of flash, each MID block is 32kB. Note that for
//   device variants with lesser main memory the size of the MID block is scaled
//   proportionaly. Protection for the Info block is turned off using CW1.
//   In this case 0x8000 to 0xFFFF is the memory range that is protected.
//
//   Important Note: In order for this code example to work when using,
//   you will need to fill unused memory with a pattern such as "0xBE".
//
//   For IAR Embedded Workbench:
//   Project -> Options -> Linker -> Checksum Tab
//   Check the box that states "fill unused code memory" and provide a fill
//   pattern
//
//   Code Composer Studio:
//   Open lnk_msp430fxxxx.cmd (command linker) file.
//   Edit the following 2 lines by adding fill=0xBE
//    FLASH                   : origin = 0x8000, length = 0x7F80, fill=0xBE
//    FLASH2                  : origin = 0x10000,length = 0x78000, fill=0xBE
//
//   MCLK = default DCO = ~1MHz, ACLK = REFO = ~32kHZ
//
//           MSP430F665x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |               |
//
//   Priya Thanigai / William Goh
//   Texas Instruments Inc.
//   December 2012
//   Built with IAR Embedded Workbench Version 5.50 & CCS V5.2
//******************************************************************************
#include <msp430.h>

unsigned short parityVal;

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;              // Setup Watchdog
    P1OUT = 0;
    P1DIR |= BIT0;
    SYSBSLC &= ~SYSBSLPE;

    // Enable MID for main memory 0x8000 to 0xFFFF only
    // 512kB/16 = 32kB
    MidEnable(0x0001, 0x0000);

    // calculate the parity value across the one MID block
    parityVal = MidCalcVParity(0xA000, 0xA200);

    while(1)
    {
        if (parityVal == 0xBEBE)
        {
            P1OUT ^= BIT0;
            __delay_cycles(100000);
        }
    }
}
