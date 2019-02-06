/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//#############################################################################
//
//! \file   board.c
//
//  Group:          MSP
//  Target Device:  MSP430F5529
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <driverlib.h>
#include "board.h"

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Board_initSys(void)
{
    volatile uint32_t index;

    WDT_A_hold(WDT_A_BASE);
    PMM_setVCore(PMM_CORE_LEVEL_3);
}

void Board_initIO(void)
{
    /* Configuring unused IOs */
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, 0xFF);

    GPIO_setAsOutputPin(GPIO_PORT_P1, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P2, ~(BOARD_KEYPAD_IRQ_PIN));
    GPIO_setAsOutputPin(GPIO_PORT_P3, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P4, ~(GPIO_PIN1 | GPIO_PIN2));
    GPIO_setAsOutputPin(GPIO_PORT_P5, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P6, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P7, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P8, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, 0xFF);

    /* Configuring pins for crystal */
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P5,
            GPIO_PIN4 + GPIO_PIN5
            );

    /* Configuring pins for I2C */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P4,
            GPIO_PIN1 | GPIO_PIN2);

    /* Configuring pin for IRQ */
    GPIO_setAsInputPinWithPullUpResistor(
            BOARD_KEYPAD_IRQ_PORT,
            BOARD_KEYPAD_IRQ_PIN);
    GPIO_selectInterruptEdge(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN,
            GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN);

    /* Configuring pin for LED1 */
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
}

void Board_initClocks(void)
{
    UCS_setExternalClockSource(32768, 0);

    /* Starting HFXT and LFXT in non-bypass mode without a timeout. */
    UCS_turnOnLFXT1(
           UCS_XT1_DRIVE_3,
           UCS_XCAP_3
       );

    /* Initializing the clock source as follows:
    *      MCLK = DCO = 24MHz
    *      SMCLK = DCO = 24MHz
    *      ACLK = LFXT = 32KHz
    *      BCLK  = REFO = 32kHz
    */
    UCS_initClockSignal(UCS_MCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_SMCLK, UCS_DCOCLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initClockSignal(UCS_ACLK, UCS_XT1CLK_SELECT, UCS_CLOCK_DIVIDER_1);
    UCS_initFLLSettle(
            24000,
            762
        );
}
