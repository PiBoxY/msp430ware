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
//  This example is an adaptation of the C code example msp430fr59xx_lpm4-5_01
//  and demonstrates how to enter LPM4.5 and wakeup from a GPIO interrupt. The
//  example will turn on P4.6 and enter into LPM4.5. When P1.1 (S2 on
//  MSP-EXP430FR5969) transitions from high to low the example will turn off
//  P4.6 to indicate the device is no longer in LPM4.5 and blink P1.0 forever.
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
//           MSP-EXP430FR5969
//           ---------------
//       /|\|               |
//        | |           P1.0|---> LED2
//        --|RST        P4.6|---> LED1
//          |               |
//          |           P1.1|<--- S2 push-button
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

int main(void)
{
    /* Halt the watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize GPIO and clocks */
    initGpio();
    initClocks();

    /*
     * Turn on P4.6 (LED1) while in LPM4.5 with restore on reset disabled. The
     * device will wakeup when the P1.1 interrupt is triggered.
     */
    P4OUT |= BIT6;
    ctpl_enterLpm45(CTPL_DISABLE_RESTORE_ON_RESET);
    P4OUT &= ~BIT6;

    /* Now blink the LED in an endless loop. */
    while (1) {
        P1OUT ^= BIT0;
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
    PJOUT = 0; PJDIR = 0xFFFF;

    /* Configure P1.1 for hi/lo transition interrupt. */
    P1OUT |= BIT1;
    P1REN |= BIT1;
    P1DIR = 0xFF ^ BIT1;
    P1IES |= BIT1;
    P1IE |= BIT1;

    /* Disable the GPIO power-on default high-impedance mode. */
    PM5CTL0 &= ~LOCKLPM5;

    /* Clear pending interrupts. */
    P1IFG = 0;
}

void initClocks(void)
{
    /* Clock System Setup, MCLK = SMCLK = DCO (1MHz), ACLK = VLOCLK */
    CSCTL0_H = CSKEY >> 8;
    CSCTL1 = DCOFSEL_0;
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL0_H = 0;
}
