/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//   MSP430FR60xx Demo - Pulse generation using the MTIF
//
//   Description: Configure the MTIF for pulse generation. 8 pulses per 250ms.
//
//           MSP430FR6047
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|---> LED
//        |           PJ.4|-LFXIN
//        |           PJ.5|-LFXOUT
//        |    MTIF_OUT_IN|---> 8 pulses per 250ms
//
//   Katie Pier
//   Texas Instruments Inc.
//   June 2017
//   Built with IAR Embedded Workbench V6.50 & Code Composer Studio V7.1
//******************************************************************************
#include <msp430.h>

void RTC_init(void);
void CS_init(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure P1.0 as output for LED
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;

    // Configure PJ.4 & PJ.5 for LFXTIN
    PJSEL0 |=  BIT4 | BIT5;
    PJSEL1 &= ~(BIT4 | BIT5);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Initialize the clock system and RTC
    CS_init();
    RTC_init();
    // Initialize the MTIF
    // Enable the pulse generator, and set the initial grid frequency with a password
    MTIFPGCNF = 0x5A00 | PGFS_7 | PGCLR | PGEN;       // Pulse grid freq. = 1024Hz (frame freq. = 1024/256 = 4Hz)
    // Set up # of pulses
    MTIFPGKVAL = 0x5A00 | 4;                 // KVAL = 4. 8 pulses per 256 periods of 1024Hz.
    MTIFPGCTL = 0x5A00 | PKUR;                // K-count update request
    while(!(MTIFPGSR & PKUA));              // Wait for update acknowledge
    // Enable the MTIF terminal pins with a password
    MTIFTPCTL = TPPW | ACTIVATE_0 | TPOE;   // Enable test port output

    while(1)
    {
        __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/interrupt
        __no_operation();                   // For debug
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_C_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_C_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(RTCIV, RTCIV__RT1PSIFG))
    {
        case RTCIV__NONE:      break;       // No interrupts
        case RTCIV__RTCOFIFG:  break;       // RTCOFIFG
        case RTCIV__RTCRDYIFG: break;       // RTCRDYIFG
        case RTCIV__RTCTEVIFG:              // RTCEVIFG

            break;
        case RTCIV__RTCAIFG:   break;       // RTCAIFG
        case RTCIV__RT0PSIFG:  break;       // RT0PSIFG
        case RTCIV__RT1PSIFG:
            P1OUT ^= BIT0;                  // Toggle P1.0 LED
            break;       // RT1PSIFG
        default: break;
    }
}

void RTC_init(void)
{
    RTCCTL0_H   = RTCKEY_H;                 // Unlock RTC
    RTCCTL13_L |= RTCHOLD_L;                // Stop RTC
    RTCCTL0_L   = 0;                        // Clear all IFG
    RTCPS1CTL |= (RT1PSIE + RT1IP__128);    // Set Prescalar for 1Hz interval

    RTCTIM0 = 0;                            // clear second and min counter
    RTCPS = 0;                              // load PS counter

    RTCCTL13_L &= ~RTCHOLD_L;               // clear the hold and start RTC
}

void CS_init(void)
{
    // Clock System Setup
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3;                     // Set DCO to 8MHz
    // Set SMCLK = MCLK = DCO, ACLK = LFXTCLK
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__8;   // MCLK = SMCLK = 1MHz
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers
}
