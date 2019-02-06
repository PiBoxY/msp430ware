/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <msp430.h>

#include <ctpl.h>

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    return 0;
}
