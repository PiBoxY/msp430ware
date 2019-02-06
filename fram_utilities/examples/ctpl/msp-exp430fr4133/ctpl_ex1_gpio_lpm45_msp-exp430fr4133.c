/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  ctpl_ex1_gpio_lpm45 - LPM4.5 With GPIO Wakeup.
// 
//  This example is an adaptation of the C code example msp430fr413x_lpm4_5_01
//  and demonstrates how to enter LPM4.5 and wakeup from a GPIO interrupt. The
//  example will turn on P1.0 and enter into LPM4.5. When P1.2 (S1 on
//  MSP-EXP430FR4133) transitions from high to low the example will turn off
//  P1.0 to indicate the device is no longer in LPM4.5 and blink P4.0 forever.
// 
//  By using the compute through power loss (CTPL) library the original example
//  code is greatly simplified. The peripherals are initialized once at the
//  start of the application and the library will save the peripheral and
//  application state in FRAM before entering LPM. Upon wakeup from LPM the
//  peripheral and application state is restored and the code continues
//  execution from the next line of code.
// 
//  ACLK = VLOCLK, MCLK = SMCLK = DCO = ~1MHz
//
//           MSP-EXP430FR4133
//           ---------------
//       /|\|               |
//        | |           P4.0|---> LED2
//        --|RST        P1.0|---> LED1
//          |               |
//          |           P1.2|<--- S1 push-button
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  March 2016
//******************************************************************************

#include <msp430.h>

#include <ctpl.h>

void initGpio(void);

int main(void)
{
    /* Halt the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize GPIO */
    initGpio();

    /*
     * Turn on P1.0 (LED1) while in LPM4.5 with restore on reset disabled. The
     * device will wakeup when the P1.2 interrupt is triggered.
     */
    P1OUT |= BIT0;
    ctpl_enterLpm45(CTPL_DISABLE_RESTORE_ON_RESET);
    P1OUT &= ~BIT0;

    /* Now blink the LED in an endless loop. */
    while (1) {
        P4OUT ^= BIT0;
        __delay_cycles(100000);
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

    /* Configure P1.1 for hi/lo transition interrupt. */
    P1OUT |= BIT2;
    P1REN |= BIT2;
    P1DIR = 0xFF ^ BIT2;
    P1IES |= BIT2;
    P1IFG = 0;
    P1IE |= BIT2;

    /* Disable the GPIO power-on default high-impedance mode. */
    PM5CTL0 &= ~LOCKLPM5;
}
