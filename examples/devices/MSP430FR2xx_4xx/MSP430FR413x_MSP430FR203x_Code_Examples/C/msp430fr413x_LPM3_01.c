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
//  MSP430FR413x Demo - Enters LPM3 with ACLK = XT1CLK = 32768Hz.
//
//  Description: Configures ACLK to be sourced from XT1 crystal. Enter LPM3 until
//               WDT ISR is executed and toggles the LED on P1.0.
//
//           MSP430FR4133
//         ---------------
//     /|\|               |
//      | |               |
//      | |      XIN(P4.1)|--
//      --|RST            |  ~32768Hz
//        |     XOUT(P4.2)|--
//        |               |
//        |           P1.0|-->LED
//
//  William Goh
//  Texas Instruments Inc.
//  January 2014
//  Built with IAR Embedded Workbench v5.60 & Code Composer Studio v5.5
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDT_ADLY_1000;                 // WDT 1000ms, ACLK, interval timer

    SFRIE1 |= WDTIE;                        // Enable WDT interrupt

    P4SEL0 |= BIT2 + BIT1;                  // P4.2: XOUT; P4.1: XI1

    CSCTL4 = SELMS__DCOCLKDIV + SELA__XT1CLK;  // MCLK=SMCLK=DCO; ACLK=XT1

    // Port Configuration all un-used pins to output low
    P1OUT = 0x00;
    P2OUT = 0x00;
    P3OUT = 0x00;
    P4OUT = 0x00;
    P5OUT = 0x00;
    P6OUT = 0x00;
    P7OUT = 0x00;
    P8OUT = 0x00;

    P1DIR = 0xff;
    P2DIR = 0xff;
    P3DIR = 0xff;
    P4DIR = 0xff;
    P5DIR = 0xff;
    P6DIR = 0xff;
    P7DIR = 0xff;
    P8DIR = 0x0f;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3
    __no_operation();                       // For debugger
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) WDT_ISR (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT ^= 0x01;                          // Toggle P1.0 (LED) every 1s
}

