/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//  MSP430FR235x Demo - Enters LPM3 with ACLK = VLO
//
//  Description: Configures ACLK to be sourced from VLO. Enter LPM3 until
//               WDT ISR is executed and toggles the LED on P1.0.
//
//           MSP430FR2355
//         ---------------
//     /|\|               |
//      | |               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.1|-->ACLK
//        |           P1.0|-->LED
//
//   Cash Hao
//   Texas Instruments Inc.
//   November 2016
//   Built with IAR Embedded Workbench v6.50.0 & Code Composer Studio v6.2.0
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTIS2 | WDTSSEL__ACLK;// WDT ~3s, ACLK, interval timer

    SFRIE1 |= WDTIE;                                              // Enable WDT interrupt

    CSCTL4 = SELMS__DCOCLKDIV | SELA__VLOCLK;                     // MCLK=SMCLK=DCO; ACLK=VLO


    // Port Configuration all pins to output low
    P1OUT = 0x00; P2OUT = 0x00;
    P3OUT = 0x00; P4OUT = 0x00;
    P5OUT = 0x00; P6OUT = 0x00;
    P1DIR = 0xff; P2DIR = 0xff;
    P3DIR = 0xff; P4DIR = 0xff;
    P5DIR = 0xff; P6DIR = 0xff;

    P1SEL1 |= BIT1;                                               // Output ACLK through P1.1

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);                           // Enter LPM3
    __no_operation();                                             // For debug
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
    P1OUT ^= BIT0;                                                // Toggle P1.0 (LED) every 3s
}
