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
//! \file   keypad.c
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
#include "keypad.h"

//*****************************************************************************
// Function Implementations
//*****************************************************************************

//! \brief IRQ Status
//!
bool Keypad_IRQ = false;

void Keypad_start(void)
{
    USCI_B_I2C_initMasterParam USCIBConfig =
    {
        .selectClockSource = USCI_B_I2C_CLOCKSOURCE_SMCLK,
        .i2cClk = 24000000,
        .dataRate = USCI_B_I2C_SET_DATA_RATE_400KBPS,
    };

    USCI_B_I2C_initMaster(USCI_B1_BASE, &USCIBConfig);
    USCI_B_I2C_disable(USCI_B1_BASE);
    USCI_B_I2C_enable(USCI_B1_BASE);
    USCI_B_I2C_setSlaveAddress(USCI_B1_BASE, 0x0B);

    // Delay 500ms to allow BOOSTXL-CAPKEYPAD to start up
    __delay_cycles(12000000);

    // Perform write to BOOSTXL-CAPKEYPAD config word
    USCI_B_I2C_setMode(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_MODE);
    USCI_B_I2C_clearInterrupt(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
    while (USCI_B_I2C_isBusBusy(USCI_B1_BASE));
    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE, KEYPAD_CTRL_WORD);
    USCI_B_I2C_masterSendMultiByteFinish(USCI_B1_BASE, KEYPAD__CTRL_UART_ENABLE);

    GPIO_selectInterruptEdge(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN);
    GPIO_enableInterrupt(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN);
}

bool Keypad_getIRQStatus(void)
{
    if(Keypad_IRQ == true)
    {
        Keypad_IRQ = false;
        return true;
    }

    return false;
}

void Keypad_readStatus(uint16_t *currentStatus, uint16_t *previousStatus)
{
    uint8_t data = 0;

    USCI_B_I2C_setMode(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_MODE);
    USCI_B_I2C_clearInterrupt(USCI_B1_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_clearInterrupt(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
    while (USCI_B_I2C_isBusBusy(USCI_B1_BASE));

    USCI_B_I2C_masterSendMultiByteStart(USCI_B1_BASE, KEYPAD_STAT_WORD);
    while(!(USCI_B_I2C_getInterruptStatus(USCI_B1_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT)));

    USCI_B_I2C_masterReceiveMultiByteStart(USCI_B1_BASE);
    while(!(USCI_B_I2C_getInterruptStatus(USCI_B1_BASE, USCI_B_I2C_RECEIVE_INTERRUPT)));
    data = USCI_B_I2C_masterReceiveMultiByteNext(USCI_B1_BASE);
    *currentStatus = data & 0x00FF;
    while(!(USCI_B_I2C_getInterruptStatus(USCI_B1_BASE, USCI_B_I2C_RECEIVE_INTERRUPT)));
    data = USCI_B_I2C_masterReceiveMultiByteNext(USCI_B1_BASE);
    *currentStatus |= (data << 8) & 0xFF00;
    while(!(USCI_B_I2C_getInterruptStatus(USCI_B1_BASE, USCI_B_I2C_RECEIVE_INTERRUPT)));
    data = USCI_B_I2C_masterReceiveMultiByteFinish(USCI_B1_BASE);
    *previousStatus = data & 0x00FF;
    data = USCI_B_I2C_masterReceiveMultiByteNext(USCI_B1_BASE);
    *previousStatus |= (data << 8) & 0xFF00;
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2_ISR(void)
{
    if(GPIO_getInterruptStatus(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN) != 0)
    {
        GPIO_clearInterrupt(BOARD_KEYPAD_IRQ_PORT, BOARD_KEYPAD_IRQ_PIN);
        Keypad_IRQ = true;
    }

    __bic_SR_register_on_exit(LPM4_bits);
}
