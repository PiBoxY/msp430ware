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
//! \file   LP3943.c
//
//  Group:          MSP
//  Target Device:  MSP430
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "LP3943.h"

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Copy data from one memory location to another.
//! \param *dest is the destination address
//! \param *src is the source address
//! \param cnt is the data size in bytes to copy from dest to src
//! \return none
static void LP3943_dataCopy(uint8_t *dest, const uint8_t *src, uint8_t cnt);

//*****************************************************************************
// Function Definitions
//*****************************************************************************

void LP3943_init(LP3943_device* device)
{
	uint8_t writeBuffer[9];
	SWI2C_I2CTransaction transaction;

	writeBuffer[0] = (LP3943_RW_REGISTERS | LP3943_AUTO_INCREMENT_ENABLE);
	LP3943_dataCopy(&writeBuffer[1], &(device->regFile[LP3943_RW_REGISTERS]), 8);

	transaction.address = device->address;
	transaction.numWriteBytes = 9;
	transaction.writeBuffer = writeBuffer;
	transaction.numReadBytes = 0;
	transaction.readBuffer = 0;
	transaction.repeatedStart = false;
	SWI2C_performI2CTransaction(&transaction);
}

void LP3943_updateLEDDriverFromMemory(LP3943_device* device)
{
    uint8_t writeBuffer[5];
    SWI2C_I2CTransaction transaction;

    writeBuffer[0] = (LP3943_REG_LS0 | LP3943_AUTO_INCREMENT_ENABLE);
    LP3943_dataCopy(&writeBuffer[1], &(device->ledReg.ledBuffer[0]), 4);

    // Write the local state (in memory) to the LED driver.
    transaction.address = device->address;
    transaction.numWriteBytes = 5;
    transaction.writeBuffer = writeBuffer;
    transaction.numReadBytes = 0;
    transaction.readBuffer = 0;
    transaction.repeatedStart = false;
    SWI2C_performI2CTransaction(&transaction);
}

void LP3943_updateLEDPWMFromMemory(LP3943_device* device)
{
    uint8_t writeBuffer[5];
    SWI2C_I2CTransaction transaction;

    writeBuffer[0] = (LP3943_REG_PSC0 | LP3943_AUTO_INCREMENT_ENABLE);
    LP3943_dataCopy(&writeBuffer[1], &(device->regFile[LP3943_REG_PSC0]), 4);

    transaction.address = device->address;
    transaction.numWriteBytes = 5;
    transaction.writeBuffer = writeBuffer;
    transaction.numReadBytes = 0;
    transaction.readBuffer = 0;
    transaction.repeatedStart = false;
    SWI2C_performI2CTransaction(&transaction);
}

void LP3943_setLEDStateInMemory(LP3943_device* device,
		uint8_t led, LP3943_ledMode mode)
{
	uint32_t mask;
	uint8_t bitPosition;

	// Bit positon is the LED ID * 2
	bitPosition = led << 1;

	// Clear previous state in the
	// device structure (local memory)
	mask = (uint32_t)0x03 << bitPosition;
	device->ledReg.ledBitField &= ~mask;

	// Save new state in the
	// device structure (local memory)
	mask = (uint32_t)(mode & 0x03) << bitPosition;
	device->ledReg.ledBitField |= mask;
}

void LP3943_setAllLEDsOnInMemory(LP3943_device* device)
{
	// Clear bit 1 and set bit 0 for each LED control bit
	// in the device structure (local memory)
	device->ledReg.ledBitField = 0x55555555;
}

void LP3943_setAllLEDsOffInMemory(LP3943_device* device)
{
	// Clear bit 1 and bit 0 for each LED control bit
	// in the device structure (local memory)
	device->ledReg.ledBitField = 0x00000000;
}

void LP3943_setLEDStateImmediately(LP3943_device* device, uint8_t led,
		LP3943_ledMode mode)
{
	LP3943_setLEDStateInMemory(device, led, mode);
	LP3943_updateLEDDriverFromMemory(device);
}

void LP3943_dataCopy(uint8_t *dest, const uint8_t *src, uint8_t cnt)
{
    while (cnt-- != 0)
    {
        *dest++ = *src++;
    }
}
