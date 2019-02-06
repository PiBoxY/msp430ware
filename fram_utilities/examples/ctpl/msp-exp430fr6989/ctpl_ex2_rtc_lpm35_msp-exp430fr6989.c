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
//  and demonstrates how to use RTC as an interval wakeup in LPM3.5. The
//  example will toggle P9.7 after initialization to indicate a device start up
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
//           MSP-EXP430FR6989
//           ---------------
//       /|\|            XIN|-
//        | |               | 32kHz
//        --|RST        XOUT|-
//          |               |
//          |           P1.0|---> LED1
//          |           P9.7|---> LED2
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

    /* Initialize GPIO, clocks and RTC_C */
    initGpio();
    initClocks();
    initRtc();

    /* Toggle P9.7 LED to indicated device start up. */
    P9OUT ^= BIT7;
    __delay_cycles(100000);
    P9OUT ^= BIT7;

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
    P1OUT  = 0; P1DIR  = 0xFF;
    P2OUT  = 0; P2DIR  = 0xFF;
    P3OUT  = 0; P3DIR  = 0xFF;
    P4OUT  = 0; P4DIR  = 0xFF;
    P5OUT  = 0; P5DIR  = 0xFF;
    P6OUT  = 0; P6DIR  = 0xFF;
    P7OUT  = 0; P7DIR  = 0xFF;
    P8OUT  = 0; P8DIR  = 0xFF;
    P9OUT  = 0; P9DIR  = 0xFF;
    P10OUT = 0; P10DIR = 0xFF;
    PJOUT  = 0; PJDIR  = 0xFFFF;

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     */
    PM5CTL0 &= ~LOCKLPM5;
}

void initClocks(void)
{
    /* Configure LFXT pins */
    PJSEL0 |= BIT4 | BIT5;

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
    /* Configure RTC_C, interrupts every 2 seconds */
    RTCCTL0_H = RTCKEY_H;
    RTCCTL0_L = RTCTEVIE;
    RTCCTL1 = RTCSSEL_2 | RTCTEV_0 | RTCHOLD;
    RTCPS0CTL = RT0PSDIV0 | RT0PSDIV1;
    RTCPS1CTL = RT1SSEL1 | RT1PSDIV0 | RT1PSDIV1;

    /* Start RTC calendar mode */
    RTCCTL1 &= ~(RTCHOLD);
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RT1PSIFG))
    {
        case RTCIV_NONE:      break;        // No interrupts
        case RTCIV_RTCOFIFG:  break;        // RTCOFIFG
        case RTCIV_RTCRDYIFG: break;        // RTCRDYIFG
        case RTCIV_RTCTEVIFG:               // RTCEVIFG
            P1OUT ^= BIT0;                  // Toggle P1.0 LED
            break;
        case RTCIV_RTCAIFG:   break;        // RTCAIFG
        case RTCIV_RT0PSIFG:  break;        // RT0PSIFG
        case RTCIV_RT1PSIFG:  break;        // RT1PSIFG
        default: break;
    }
}
