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
//  This example is an adaptation of the C code example msp430fr59xx_lpm3-5_02
//  and demonstrates how to use RTC_B as an interval wakeup in LPM3.5. The
//  example will toggle P4.6 after initialization to indicate a device start up
//  and then enter LPM3.5 with interrupts enabled. The RTC interrupt will wake
//  the device up every two seconds and toggle P1.0.
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
//           MSP-EXP430FR5969
//           ---------------
//       /|\|            XIN|-
//        | |               | 32kHz
//        --|RST        XOUT|-
//          |               |
//          |           P1.0|--> LED2
//          |           P4.6|--> LED1
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  April 2015
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

    /* Toggle P4.6 LED to indicated device start up. */
    P4OUT ^= BIT6;
    __delay_cycles(100000);
    P4OUT ^= BIT6;

    /* Enable global interrupts. */
    __enable_interrupt();

    /*
     * Enter into LPM3.5 with restore on reset disabled. The RTC interrupt will
     * toggle P1.0 each time the device wakes up.
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
    PJOUT |= BIT4;
    PJSEL0 |= BIT4;

    /* Clock System Setup, MCLK = SMCLK = DCO (1MHz), ACLK = LFXTCLK */
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL4 &= ~LFXTOFF;

    /* Wait for LFXT startup. */
    do {
        CSCTL5 &= ~LFXTOFFG;
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
            /* Toggle P1.0 */
            P1OUT ^= BIT0;
            break;
        case RTCIV_RTCOFIFG:  break;
    }
}
