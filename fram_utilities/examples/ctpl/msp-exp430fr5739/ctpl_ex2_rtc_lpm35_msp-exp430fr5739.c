/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  ctpl_ex2_rtc_lpm35 - LPM3.5 With RTC Wakeup.
//
//  This example is an adaptation of the C code example msp430fr57xx_lpm3-5_01
//  and demonstrates how to use RTC_B as an interval wakeup in LPM3.5. The
//  example will toggle PJ.0 after initialization to indicate a device start up
//  and then enter LPM3.5 with interrupts enabled. The RTC interrupt will wake
//  the device up every two seconds and toggle PJ.1.
// 
//  By using the compute through power loss (CTPL) library the original example
//  code is greatly simplified. The peripherals are initialized once at the
//  start of the application and the library will save the peripheral and
//  application state in FRAM before entering LPM. Upon wakeup from LPM the
//  peripheral and application state is restored and the code continues
//  execution from the next line of code.
// 
//  ACLK = 32.768kHz, MCLK = SMCLK = DCO = ~1MHz
//
//           MSP-EXP430FR5739
//           ---------------
//       /|\|            XIN|-
//        | |               | 32kHz
//        --|RST        XOUT|-
//          |               |
//          |           PJ.1|--> LED1
//          |           PJ.0|--> LED0
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  March 2016
//******************************************************************************
#include <msp430.h>

#include <ctpl.h>

void initGpio(void);
void initClocks(void);
void initRtc(void);

int main(void)
{
    /* Halt the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize GPIO, clocks and RTC_B */
    initGpio();
    initClocks();
    initRtc();

    /* Toggle PJ.0 LED to indicated device start up. */
    PJOUT ^= BIT0;
    __delay_cycles(100000);
    PJOUT ^= BIT0;

    /* Enable global interrupts. */
    __enable_interrupt();

    /*
     * Enter into LPM3.5 with restore on reset disabled. The RTC interrupt will
     * toggle PJ.1 each time the device wakes up.
     */
    while (1) {
        ctpl_enterLpm35(CTPL_DISABLE_RESTORE_ON_RESET);
    }
}

void initGpio(void)
{
    /* Configure GPIO to default state */
    P1OUT = 0; P1DIR = 0xFF;
    P2OUT = 0; P2DIR = 0xFF;
    P3OUT = 0; P3DIR = 0xFF;
    P4OUT = 0; P4DIR = 0xFF;
    PJOUT = 0; PJDIR = 0xFFFF;

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;
}

void initClocks(void)
{
    /* Configure XT1 pin */
    PJSEL0 |= BIT4 | BIT5;

    /* Clock System Setup, MCLK = SMCLK = DCO (1MHz), ACLK = LFXTCLK */
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;
    CSCTL2 = SELS__XT1CLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL4 &= ~XT1OFF;

    /* Wait for LFXT startup. */
    do {
        CSCTL5 &= ~XT1OFF;
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);
    CSCTL0_H = 0;
}

void initRtc(void)
{
    /* Configure RTC_B, interrupts every 2 seconds */
    RTCCTL01 = RTCBCD | RTCHOLD | RTCTEV_0;
    RTCPS1CTL = RT1IP_7 | RT1PSIE;

    /* Start RTC calendar mode */
    RTCCTL01 &= ~RTCHOLD;
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch (__even_in_range(RTCIV, RTCIV_RTCOFIFG)) {
        case RTCIV_NONE:      break;
        case RTCIV_RTCRDYIFG: break;
        case RTCIV_RTCTEVIFG: break;
        case RTCIV_RTCAIFG:   break;
        case RTCIV_RT0PSIFG:  break;
        case RTCIV_RT1PSIFG:
            /* Toggle PJ.1 */
            PJOUT ^= BIT1;
            break;
        case RTCIV_RTCOFIFG:  break;
    }
}
