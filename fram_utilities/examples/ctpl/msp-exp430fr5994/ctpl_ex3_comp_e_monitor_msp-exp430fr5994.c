/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  ctpl_ex3_comp_e_monitor - COMP_E Powerloss Monitor.
//
//  This example demonstrates how to use the COMP_E peripheral and an external
//  voltage divider to actively monitor supply voltage and detect when power is
//  lost. The comparator is configured with a 1.5V reference and an external
//  voltage divider provides Vcc/2 to the input pin (P1.5/C5). When Vcc/2 drops
//  below the 1.5V reference (meaning Vcc is below 3.0V) the comparator
//  interrupt service routine will disable the comparator monitor and invoke the
//  ctpl_enterShutdown API. This API will save the application and peripheral
//  state and waits for the device to enter BOR with a 64ms timeout. The device
//  will restore application and peripheral state when power is restored and
//  continue execution from the next line of code.
// 
//  The main application will blink LED1 with incremental counts, resetting
//  after four blinks. The power supply can be removed (by disconnecting the USB
//  cable or unplugging the jumpers connecting the on-board emulator to the
//  device) after a specific count of blink and then reapplied to verify that
//  context was saved.
// 
//  ACLK = VLOCLK, MCLK = SMCLK = DCO = ~1MHz
//
//           MSP-EXP430FR5994
//           ---------------
//       /|\|           P1.4|---> Vcc
//        | |       (C5)P1.5|---> Vcc/2 (350k/350k voltage divider)
//        --|RST        P4.7|---> GND
//          |               |
//          |           P1.0|---> LED1
//          |               |
//
//  Brent Peterson
//  Texas Instruments Inc.
//  March 2016
//******************************************************************************
#include <msp430.h>

#include <ctpl.h>

#ifndef __MSP430_HAS_COMP_E__
#error This example will only work on MSP430 devices with COMP_E.
#endif

#define MCLK_FREQUENCY          1000000

void initGpio(void);
void initClocks(void);
void initCompMonitor(void);

int main(void)
{
    uint16_t i, j;

    /* Halt the watchdog timer. */
    WDTCTL = WDTPW | WDTHOLD;

    /* Initialize GPIO and clock system. */
    initGpio();
    initClocks();

    /* Initialize the comparator monitor to actively monitor voltage. */
    initCompMonitor();

    /* Enable global interrupts. */
    __enable_interrupt();

    /* Blink P1.0 between 1-4 counts every 1 second. */
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    while (1) {
        for (i = 1; i < 5; i++) {
            for (j = i; j > 0; j--) {
                /* Blink the LED for 0.1s and delay for 0.2s */
                P1OUT |= BIT0;
                __delay_cycles(MCLK_FREQUENCY/10);
                P1OUT &= ~BIT0;
                __delay_cycles(MCLK_FREQUENCY/10);
            }
            /* Delay 1 second between counts. */
            __delay_cycles(MCLK_FREQUENCY);
        }
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
    PJOUT  = 0; PJDIR  = 0xFFFF;

    /* Disable the GPIO power-on default high-impedance mode. */
    PM5CTL0 &= ~LOCKLPM5;
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

void initCompMonitor(void)
{
    /* Set P1.4 to Vcc and P4.7 to GND. */
    P1OUT |= BIT4;
    P4OUT &= ~BIT7;

    /* Setup COMP_E for P1.5 (C9) input with 1.5v reference. */
    CECTL0 = CEIPEN | CEIPSEL_5;
    CECTL1 = CEPWRMD_1 | CEMRVS;
    CECTL2 = CEREFL_2 | CERS_2 | CERSEL | CEREF0_23;
    CECTL3 = BIT5;
    CEINT = CEIIE;
    CECTL1 |= CEON;

    /* Delay for the reference to settle */
    __delay_cycles(75);
}

#pragma vector=COMP_E_VECTOR
__interrupt void COMP_E_ISR(void)
{
    switch (__even_in_range(CEIV, CEIV_CERDYIFG)) {
        case CEIV_NONE:        break;
        case CEIV_CEIFG:    break;
        case CEIV_CEIIFG:
            /* Enter device shutdown with 64ms timeout. */
            ctpl_enterShutdown(CTPL_SHUTDOWN_TIMEOUT_64_MS);
            break;
        case CEIV_CERDYIFG:    break;
    }
}
