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
//   MSP430F673x Demo - RTC_C, LPM3.5, & alarm
//
//   Description: The RTC_C module is used to set the time, start RTC operation,
//   and read the time from the respective RTC registers. Software will set the
//   original time to 11:59:45 am on Friday October 7, 2011. Then the RTC will
//   be activated through software, and an alarm will be created for the next
//   minute (12:00:00 pm). The device will then enter LPM3.5 awaiting
//   the event interrupt. Upon being woken up by the event, the LED on the board
//   will be set.
//
//   NOTE: To ensure that LPM3.5 is entered properly, you would need to use an
//   external power supply.
//
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F673x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//
//   W. Goh
//   Texas Instruments Inc.
//   January 2012
//   Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

int main (void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    if (SYSRSTIV == SYSRSTIV_LPM5WU)
    {
        // When woken up from LPM3.5, reinit since LPMx.5 will cause a BOR.
        PMMCTL0_H = PMMPW_H;                // open PMM
        PM5CTL0 &= ~LOCKIO;                 // Clear LOCKBAK and enable ports
        PMMCTL0_H = 0x00;                   // close PMM

        P1DIR |= BIT0;                      // Configure P1.0 as GPIO output

        // Reconfigure RTC to read RTC registers
        RTCCTL0_H = RTCKEY_H;               // Unlock RTC_C module
        RTCCTL1 |= RTCBCD | RTCHOLD;        // BCD mode, RTC hold
        RTCCTL1 &= ~(RTCHOLD);              // Start RTC calendar mode
        RTCCTL0_H = 0;                      // Lock RTC_C module

        // If woken up at noon, set LED and stay loop there
        if((RTCHOUR == 0x12)&&(RTCMIN == 0x00)&&(RTCSEC == 0x00))
        {
            P1OUT |= BIT0;                  // Set P1.0 on
            while(1);
        }
        // If values do not match, blink LED fast
        else
        {
            while(1)                        // continuous loop
            {
                P1OUT ^= BIT0;              // XOR P1.0
                __delay_cycles(20000);      // Delay
            }
        }
    }

    // GPIO Port Configuration for lowest power configuration
    P1OUT = 0x00;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;P5OUT = 0x00;P6OUT = 0x00;
    P7OUT = 0x00;P8OUT = 0x00;P9OUT = 0x00;PJOUT = 0x00;
    P1DIR = 0xFF;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;P5DIR = 0xFF;P6DIR = 0xFF;
    P7DIR = 0xFF;P8DIR = 0xFF;P9DIR = 0xFF;PJDIR = 0xFF;

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

    // Configure RTC_C
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC_C module
    RTCCTL0_L |= RTCTEVIE;                  // Enable RTC time event interrupt
    RTCCTL1 |= RTCBCD | RTCTEV_0 | RTCHOLD; // BCD mode, RTC hold, Set RTCTEV for 1 minute alarm
                                            // event interrupt
    RTCYEAR = 0x2011;                       // Year = 0x2011
    RTCMON = 0x10;                          // Month = 0x10 = October
    RTCDAY = 0x07;                          // Day = 0x07 = 7th
    RTCDOW = 0x05;                          // Day of week = 0x05 = Friday
    RTCHOUR = 0x11;                         // Hour = 0x11
    RTCMIN = 0x59;                          // Minute = 0x59
    RTCSEC = 0x45;                          // Seconds = 0x45

    RTCCTL1 &= ~(RTCHOLD);                  // Start RTC calendar mode
    RTCCTL0_H = 0;                          // Lock RTC_C module

    // Enter LPM3.5
    UCSCTL6 |= XT1OFF;                      // Turn off clock for LPMx.5 operation. ACLK will still
                                            // remain active
    PMMCTL0_H = PMMPW_H;                    // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF;                 // and set PMMREGOFF
    __bis_SR_register(LPM4_bits | GIE);     // Enter LPM3.5 mode with interrupts enabled
    __no_operation();

    // Code should NOT get here. This means that LPM3.5 was not properly entered.
    // Ensure that an external power supply was ued. Or else JTAG will put the CPU
    // in LPM0 mode.

    // Stop the RTC
    RTCCTL0_H = RTCKEY_H;                   // Unlock RTC_C module
    RTCCTL1 |= RTCHOLD;                     // RTC hold
    RTCCTL0_H = 0;                          // Lock RTC_C module

    // Blink LED slowly if LPM3.5 not entered properly
    while(1)
    {
        P1OUT ^= BIT0;                      // XOR P1.0
        __delay_cycles(500000);             // Delay
    }
}

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
            break;
        case RTCIV_RTCTEVIFG:               // RTCEVIFG
            PMMCTL0_H = PMMPW_H;            // open PMM
            PM5CTL0 &= ~LOCKIO;             // Clear LOCKBAK and enable ports
            PMMCTL0_H = 0x00;               // close PMM

            __bic_SR_register_on_exit(LPM4_bits); // Exit LPM4.5
            __no_operation();
            break;
        case RTCIV_RTCAIFG:                 // RTCAIFG
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
