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
//  This example is an adaptation of the C code example msp430fr243x_lpm3_5_01
//  and demonstrates how to use RTC as an interval wakeup in LPM3.5. The
//  example will toggle P1.0 after initialization to indicate a device start up
//  and then enter LPM3.5 with interrupts enabled. The RTC interrupt will wake
//  the device up every two seconds and toggle P1.1.
// 
//  By using the compute through power loss (CTPL) library the original example
//  code is greatly simplified. The peripherals are initialized once at the
//  start of the application and the library will save the peripheral and
//  application state in FRAM before entering LPM. Upon wakeup from LPM the
//  peripheral and application state is restored and the code continues
//  execution from the next line of code.
// 
//  ACLK = VLO = ~10kHz, MCLK = SMCLK = DCO = ~1MHz
//
//           MSP-EXP430FR2433
//           ---------------
//       /|\|               |
//        | |               |
//        --|RST            |
//          |               |
//          |           P1.1|--> LED2
//          |           P1.0|--> LED1
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  October 2017
//******************************************************************************
#include <msp430.h>

#include <ctpl.h>

void initGpio(void);
void initRtc(void);

int main(void)
{
    /* Halt the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize GPIO and RTC */
    initGpio();
    initRtc();

    /* Toggle P1.0 LED to indicated device start up. */
    P1OUT ^= BIT0;
    __delay_cycles(100000);
    P1OUT ^= BIT0;

    /* Enable global interrupts. */
    __enable_interrupt();

    /*
     * Enter into LPM3.5 with restore on reset disabled. The RTC interrupt will
     * toggle P1.1 each time the device wakes up.
     */
    while (1) {
        ctpl_enterLpm35(CTPL_DISABLE_RESTORE_ON_RESET);
    }
}

void initGpio(void)
{
    /* Configure GPIO to default state */
    PAOUT = 0x0000; PADIR = 0xFFFF;

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;
}

void initRtc(void)
{
    /* Configure RTC, interrupts every 2 seconds */
    RTCMOD = 20-1;
    RTCCTL = RTCSS__VLOCLK | RTCSR |RTCPS__1024;
    RTCCTL |= RTCIE;
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF)) {
        case RTCIV_NONE : break;
        case RTCIV_RTCIF:
            /* Toggle P1.1 */
            P1OUT ^= BIT1;
            break;
        default: break;
    }
}
