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
//  This example is an adaptation of the C code example msp430fr413x_lpm3_5_01
//  and demonstrates how to use RTC as an interval wakeup in LPM3.5. The
//  example will toggle P1.0 after initialization to indicate a device start up
//  and then enter LPM3.5 with interrupts enabled. The RTC interrupt will wake
//  the device up every two seconds and toggle P2.0.
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
//           MSP-EXP430FR2311
//           ---------------
//       /|\|            XIN|-
//        | |               | 32kHz
//        --|RST        XOUT|-
//          |               |
//          |           P4.0|--> LED2
//          |           P1.0|--> LED1
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

    /* Initialize GPIO, clocks and RTC */
    initGpio();
    initClocks();
    initRtc();

    /* Toggle P1.0 LED to indicated device start up. */
    P1OUT ^= BIT0;
    __delay_cycles(100000);
    P1OUT ^= BIT0;

    /* Enable global interrupts. */
    __enable_interrupt();

    /*
     * Enter into LPM3.5 with restore on reset disabled. The RTC interrupt will
     * toggle P4.0 each time the device wakes up.
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
    P5OUT = 0; P5DIR = 0xFF;
    P6OUT = 0; P6DIR = 0xFF;
    P7OUT = 0; P7DIR = 0xFF;
    P8OUT = 0; P8DIR = 0xFF;

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;
}

void initClocks(void)
{
    /* Configure XT1 pins */
    P4SEL0 |= BIT1 | BIT2;

    /* Wait for LFXT startup. */
    do {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);
}

void initRtc(void)
{
    /* Configure RTC, interrupts every 2 seconds */
    RTCMOD = 64-1;
    RTCCTL = RTCSS__XT1CLK | RTCSR |RTCPS__1024;
    RTCCTL |= RTCIE;
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF)) {
        case RTCIV_NONE : break;
        case RTCIV_RTCIF:
            /* Toggle P4.0 */
            P4OUT ^= BIT0;
            break;
        default: break;
    }
}
