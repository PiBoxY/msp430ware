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
//   MSP430FR59xx Demo - RTC_B in LPM3.5 with 2 second wake-up interval
//
//   Description: The RTC_B module is in counter mode with counter interrupt
//   triggers every 2 seconds. The device will then enter LPM3.5 waiting for
//   RT1PSIFG interrupt. Upon wake-up from LPMx.5, the device restores the
//   previous state of P1.0 LED based on the ledState variable. It then toggles
//   the P1.0 LED on the board and stores the LED state into FRAM memory.
//   This is a good example to demonstrate how to save the GPIO state before
//   entering LPMx.5 and restoring the GPIO state upon wake-up.
//   P4.6 is used as a heartbeat showcasing that the device is either waking
//   up from a BOR reset or LPMx.5.
//
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO = ~1MHz
//
//               MSP430FR59xx
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|--> LED
//            |             P4.6|--> Probe out/LED
//
//   William Goh
//   Texas Instruments Inc.
//   January 2013
//   Built with Code Composer Studio v5.5 and Embedded Workbench v5.60
//******************************************************************************
#include <msp430.h>

void initBoard(void);
void initRtc(void);
void WakeUpLPM35(void);

// Stores the state of the LED in FRAM where this variable
// is not C-initialized during start-up. This allows
// certain variables to easily be restored and managed
// in a non-volatile memory with no flash charge-pump required.
// This allows faster start-up time.
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(ledState);
unsigned char ledState = 0;
#elif  defined(__IAR_SYSTEMS_ICC__)
__persistent unsigned char ledState;
#else
// For other compilers, insert this variable into FRAM memory
unsigned char ledState;
#endif

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Initialize board
    initBoard();                            // Regular init code for GPIO and CS

    // Determine whether we are coming out of an LPMx.5 or a regular RESET.
    if (SYSRSTIV == SYSRSTIV_LPM5WU)
    {
        // When woken up from LPM3.5, reinit
        WakeUpLPM35();                      // LPMx.5 wakeup specific init code
        __enable_interrupt();               // The RTC interrupt should trigger now...
    }
    else
    {
        ledState = 0;                       // Initialize LED state variable
        P1OUT = 0x00;                       // For initialize P1OUT to zero initially
        initRtc();                          // Regular init code for RTC_B
    }

    // Use P4.6 LED as a heart beat for to represent device restarted either
    // from LPMx.5 or a complete power-up
    P4OUT |= BIT6;

    // Use Timer A for a short delay to blink P1.0 LED
    TA0CCTL0 = CCIE;                        // TACCR0 interrupt enabled
    TA0CCR0 = 1000;
    TA0CTL = TASSEL__ACLK | MC__UP;         // SMCLK, UP mode
    __bis_SR_register(LPM3_bits | GIE);
    TA0CTL &= ~(MC__UP);                    // Stop Timer

    P4OUT &= ~BIT6;                         // Clear P4.6 LED

    // Sequence to enter LPM3.5.
    PMMCTL0_H = PMMPW_H;                    // Open PMM Registers for write
    PMMCTL0_L |= PMMREGOFF;                 // and set PMMREGOFF

    // Enter LPM3.5 mode with interrupts enabled. Note that this operation does
    // not return. The LPM3.5 will exit through a RESET event, resulting in a
    // re-start of the code.
    __bis_SR_register(LPM3_bits | GIE);
    __no_operation();

    return 1;
}

void initBoard(void)
{
    // Port Configuration
    // Restore P1.0 LED state before reset from a variable
    // stored in FRAM
    P1OUT = ledState;
    P1DIR = 0xFF;
    P2OUT = 0x00;
    P2DIR = 0xFF;
    P3OUT = 0x00;
    P3DIR = 0xFF;
    P4OUT = 0x00;
    P4DIR = 0xFF;
    PJOUT = 0x00;
    PJSEL0 |= BIT4 | BIT5;
    PJDIR = 0xFFFF;

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Setup Clocks
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL1 = DCOFSEL_0;                     // Set DCO to 1MHz
    CSCTL2 = SELA_0 | SELS_3 | SELM_3;      // set ACLK = XT1; MCLK = DCO
    CSCTL3 = DIVA_0 | DIVS_0 | DIVM_0;      // Set all dividers to 1
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT1

    do
    {
        CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers
}

void initRtc(void)
{
    // Configure RTC_B
    // Configure BCD mode, stop the RTC, and enable RTC
    RTCCTL01 = RTCBCD | RTCHOLD | RTCTEV_0;
    // Set the RTC second stage divider to 256
    // RTC interrupts every 2 seconds
    RTCPS1CTL = RT1IP_7 | RT1PSIE;

    // Start RTC calendar mode
    RTCCTL01 &= ~RTCHOLD;
}

void WakeUpLPM35(void)
{
    // Re-configure / start RTC
    // Configure BCD mode, stop the RTC, and enable RTC
    RTCCTL01 |= RTCBCD | RTCHOLD | RTCTEV_0;
    // Set the RTC second stage divider to 256
    // RTC interrupts every 2 seconds
    RTCPS1CTL = RT1IP_7 | RT1PSIE;

    // Start RTC calendar mode
    RTCCTL01 &= ~RTCHOLD;
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(RTC_VECTOR))) RTC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(RTCIV, RTCIV_RTCOFIFG))
    {
        case RTCIV_NONE:      break;
        case RTCIV_RTCRDYIFG: break;
        case RTCIV_RTCTEVIFG: break;
        case RTCIV_RTCAIFG:   break;
        case RTCIV_RT0PSIFG:  break;
        case RTCIV_RT1PSIFG:
            // Toggle the LED on P1.0
            P1OUT ^= BIT0;

            // Store P1.0 output state to a variable
            ledState = (P1OUT & BIT0);
            break;
        case RTCIV_RTCOFIFG: break;
    }
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM4_bits);
}
