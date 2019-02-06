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
//  Target Device:  MSP430FR6989
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "board.h"

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Board_initSys(void)
{
    WDT_A_hold(WDT_A_BASE);
    PM5CTL0 &= ~LOCKLPM5;
}

void Board_initIO(void)
{
    // Configuring unused IOs
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, 0xFF);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, 0xFF);

    GPIO_setAsOutputPin(GPIO_PORT_P1, ~(GPIO_PIN1 | GPIO_PIN2));
    GPIO_setAsOutputPin(GPIO_PORT_P2, ~(GPIO_PIN4));
    GPIO_setAsOutputPin(GPIO_PORT_P3, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P4, ~(GPIO_PIN0 | GPIO_PIN1));
    GPIO_setAsOutputPin(GPIO_PORT_P5, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P6, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P7, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P8, 0xFF);
    GPIO_setAsOutputPin(GPIO_PORT_P9, 0xFF);

    // Configuring pins for crystal
    GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_PJ,
           GPIO_PIN4 + GPIO_PIN5,
           GPIO_PRIMARY_MODULE_FUNCTION
           );

    // Configuring pins for I2C
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P4,
            GPIO_PIN0 | GPIO_PIN1,
            GPIO_SECONDARY_MODULE_FUNCTION
            );

    // Configure button S1 (P1.1) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Configure button S2 (P1.2) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    // Configuring pin for IRQ
    GPIO_setAsInputPinWithPullUpResistor(
            BOARD_KEYPAD_IRQ_PORT,
            BOARD_KEYPAD_IRQ_PIN);
    GPIO_selectInterruptEdge(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN,
            GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN);

}

void Board_initClocks(void)
{
    // Set DCO frequency to default 8MHz
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);

    // Set MCLK to 8 MHz
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Set SMCLK to 2MHz
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_4);

    // Start the XT1 crystal oscillator
    CS_turnOnLFXT(CS_LFXT_DRIVE_3);
}
