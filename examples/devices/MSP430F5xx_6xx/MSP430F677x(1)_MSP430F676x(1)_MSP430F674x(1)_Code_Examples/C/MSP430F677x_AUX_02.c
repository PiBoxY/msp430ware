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
//  MSP430F67791 Demo - AUX, Software-Controlled Switching, AUXVCC2
//
//  Description: Maintain 1-MHz system frequency when switching the AUX2.
//  ACLK is brought out on pin P1.2, SMCLK is brought out on P1.5, and MCLK
//  is brought out on pin P1.4.
//  ACLK = REFO = 32.768kHz, MCLK = SMCLK = (Default DCO)/2 = (2MHz/2) = 1MHz
//
//                MSP430F67791
//             -----------------
//         /|\|                 |
//          | |             P1.2|-->ACLK
//          --|RST          P1.4|-->MCLK
//            |             P1.5|-->SMCLK
//            |                 |
//
//   C. Fu
//   Texas Instruments Inc.
//   October 2012
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.51
//******************************************************************************
#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    
    // Clear the LOCKAUX bit, enable the write access to the AUXCTLx registers
    AUXCTL0 = AUXKEY;
    
    // Enable software control of DVCC, AUXVCC1, AUXVCC2
    AUXCTL1 |= AUX0MD | AUX1MD | AUX2MD;
    AUXCTL1 |= AUX0OK | AUX1OK | AUX2OK;
       
    // Select AUXVCC2 to use
    AUXCTL1 |= AUX2PRIO;
    AUXCTL1 &= ~ AUX0OK;
	
    // Setup P1.5 SMCLK, P1.4 MCLK, P1.2 ACLK
    P1DIR |= BIT2 | BIT4 | BIT5;            // ACLK, MCLK, SMCLK set out to pins
    P1SEL0 |= BIT2 | BIT4 | BIT5;            // P1.2,4,5 for debugging purposes.

    while (1);
}



























