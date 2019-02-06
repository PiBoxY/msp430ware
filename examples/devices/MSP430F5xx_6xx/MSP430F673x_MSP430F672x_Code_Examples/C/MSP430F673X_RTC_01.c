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
//  MSP430F673x Demo - RTC_C, Calendar Mode with Time Event and Alarm Interrupts
//
//  Description: This program demonstrates the RTC_C mode by triggering an
//  interrupt every second and minute. This code toggles P1.0 every second.
//  This code recommends an external LFXT1 crystal for RTC accuracy.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO = 32 x ACLK = 1048576Hz
//
//                MSP430F673x
//             -----------------
//        /|\ |              XIN|-
//         |  |                 | 32kHz
//         ---|RST          XOUT|-
//            |                 |
//            |            P1.0 |--> Toggles every second
//            |                 |
//
//   M. Swanson
//   Texas Instruments Inc.
//   December 2011
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WTD

    // Initialize LFXT1
    UCSCTL6 &= ~(XT1OFF);                   // Enable XT1
    UCSCTL6 |= XCAP_3;                      // Internal load cap

    // Loop until XT1, XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG | XT1LFOFFG | DCOFFG);
        // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                  // Clear fault flags
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

    P1OUT &= ~BIT0;                         // Clear P1.0 output
    P1DIR |= BIT0;                          // Set P1.0 as output

    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC_C module
    RTCCTL0_L |= RTCTEVIE | RTCAIE | RTCRDYIE; // Enable RTC time event, alarm event,
                                            // read ready interrupt
    RTCCTL1 |= RTCBCD | RTCHOLD;            // RTC enable BCD mode, RTC hold

    RTCYEAR = 0x2011;                       // Year = 0x2011 = 2011
    RTCMON = 0x12;                          // Month = 0x12 = December
    RTCDAY = 0x05;                          // Day = 0x05 = 5th
    RTCDOW = 0x03;                          // Day of week = 0x03 = Wednesday
    RTCHOUR = 0x24;                         // Hour = 0x12
    RTCMIN = 0x59;                          // Minute = 0x57
    RTCSEC = 0x55;                          // Seconds = 0x36

    RTCADOWDAY = 0x3;                       // RTC Day of week alarm = 0x2
    RTCADAY = 0x22;                         // RTC Day Alarm = 0x22
    RTCAHOUR = 0x23;                        // RTC Hour Alarm
    RTCAMIN = 0x45;                         // RTC Minute Alarm

    RTCCTL1 &= ~(RTCHOLD);                  // Start RTC calendar mode
    RTCCTL0_H = 0;                          // Lock RTC_C module

    __bis_SR_register(LPM3_bits | GIE);     // Enter LPM3 w/ interrupts
                                            // enabled
    __no_operation();
}

// RTC Interrupt Service Routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void rtc_isr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) rtc_isr (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(RTCIV, 16))
    {
        case RTCIV_NONE:                    // No interrupts
            break;
        case RTCIV_RTCOFIFG:                // RTCOFIFG
            break;
        case RTCIV_RTCRDYIFG:               // RTCRDYIFG
            P1OUT ^= 0x01;                  // Toggles P1.0 every second
            break;
        case RTCIV_RTCTEVIFG:               // RTCEVIFG
            __no_operation();               // Interrupts every minute
            break;
        case RTCIV_RTCAIFG:                 // RTCAIFG
            __no_operation();               // Interrupts every alarm event
            break;
        case RTCIV_RT0PSIFG:                // RT0PSIFG
            break;
        case RTCIV_RT1PSIFG:                // RT1PSIFG
            break;
        case 14: break;                     // Reserved
        case 16: break;                     // Reserved
        default: break;
    }
}

