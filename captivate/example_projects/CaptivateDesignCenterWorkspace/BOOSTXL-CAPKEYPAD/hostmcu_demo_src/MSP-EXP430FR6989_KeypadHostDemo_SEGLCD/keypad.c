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

#include <driverlib.h>
#include "board.h"
#include "keypad.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \def BOOSTXL-CAPKEYPAD IRQ Port
//!
#define KEYPAD_EUSCI (EUSCI_B1_BASE)

//*****************************************************************************
// Function Implementations
//*****************************************************************************

//! \brief IRQ Status
//!
bool Keypad_IRQ = false;

void Keypad_start(void)
{
    EUSCI_B_I2C_initMasterParam eUSCIBConfig =
    {
        .selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .i2cClk = 2000000,
        .dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS,
        .byteCounterThreshold = 0,
        .autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP
    };

    EUSCI_B_I2C_initMaster(KEYPAD_EUSCI, &eUSCIBConfig);
    EUSCI_B_I2C_disable(KEYPAD_EUSCI);
    EUSCI_B_I2C_enable(KEYPAD_EUSCI);
    EUSCI_B_I2C_setSlaveAddress(KEYPAD_EUSCI, 0x0B);

    // Delay 500ms to allow BOOSTXL-CAPKEYPAD to start up
    __delay_cycles(4000000);

    // Perform write to BOOSTXL-CAPKEYPAD config word
    EUSCI_B_I2C_setMode(KEYPAD_EUSCI,EUSCI_B_I2C_TRANSMIT_MODE);
    EUSCI_B_I2C_clearInterrupt(KEYPAD_EUSCI, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    while (EUSCI_B_I2C_isBusBusy(KEYPAD_EUSCI));
    EUSCI_B_I2C_masterSendMultiByteStart(KEYPAD_EUSCI, KEYPAD_CTRL_WORD);
    EUSCI_B_I2C_masterSendMultiByteFinish(KEYPAD_EUSCI, KEYPAD__CTRL_WOP_ENABLE);

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

    EUSCI_B_I2C_setMode(KEYPAD_EUSCI, EUSCI_B_I2C_TRANSMIT_MODE);
    EUSCI_B_I2C_clearInterrupt(KEYPAD_EUSCI, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    EUSCI_B_I2C_clearInterrupt(KEYPAD_EUSCI, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    while (EUSCI_B_I2C_isBusBusy(KEYPAD_EUSCI));

    EUSCI_B_I2C_masterSendMultiByteStart(KEYPAD_EUSCI, KEYPAD_STAT_WORD);
    while(!(EUSCI_B_I2C_getInterruptStatus(KEYPAD_EUSCI, EUSCI_B_I2C_TRANSMIT_INTERRUPT0)));

    EUSCI_B_I2C_masterReceiveStart(KEYPAD_EUSCI);
    while(!(EUSCI_B_I2C_getInterruptStatus(KEYPAD_EUSCI, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));
    data = EUSCI_B_I2C_masterReceiveMultiByteNext(KEYPAD_EUSCI);
    *currentStatus = data & 0x00FF;
    while(!(EUSCI_B_I2C_getInterruptStatus(KEYPAD_EUSCI, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));
    data = EUSCI_B_I2C_masterReceiveMultiByteNext(KEYPAD_EUSCI);
    *currentStatus |= (data << 8) & 0xFF00;
    while(!(EUSCI_B_I2C_getInterruptStatus(KEYPAD_EUSCI, EUSCI_B_I2C_RECEIVE_INTERRUPT0)));
    data = EUSCI_B_I2C_masterReceiveMultiByteNext(KEYPAD_EUSCI);
    *previousStatus = data & 0x00FF;
    data = EUSCI_B_I2C_masterReceiveMultiByteFinish(KEYPAD_EUSCI);
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
