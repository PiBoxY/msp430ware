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
//*****************************************************************************
//
//! \file   TCA95xx.c
//
//! \brief  Seven Segment driver
//
//*****************************************************************************

//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************
#include "TCA95xx.h"
#include "I2CMaster.h"

#define TCA9534_IN			(0x00)
#define TCA9534_OUT			(0x01)
#define TCA9534_POLARITY	(0x02)
#define TCA9534_CONFIG		(0x03)

uint8_t TCA9534_TxBuf[1];
uint8_t TCA9534_RxBuf[1];

bool
TCA9534_Init(uint8_t ui8Address)
{

    I2CMaster_open();

	//
	// Init polarity to 1=1, 0=0 (Default Configuration)
	//
	*TCA9534_TxBuf = TCA9534_NONE;

    I2CMaster_writeReg8(ui8Address, TCA9534_POLARITY, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
    I2CMaster_readReg8(ui8Address, TCA9534_POLARITY, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif


	//
	// Init output registers to low (cleared) (Default Configuration)
	//
	*TCA9534_TxBuf = TCA9534_NONE;

    I2CMaster_writeReg8(ui8Address, TCA9534_OUT, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

    if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	//
	// Set All IO's as Hi-Z Inputs (Default Configuration)
	//
	*TCA9534_TxBuf = TCA9534_ALL;

    I2CMaster_writeReg8(ui8Address, TCA9534_CONFIG, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
	I2CMaster_readReg8(ui8Address, TCA9534_CONFIG, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

bool
TCA9534_SetAsOutputs(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Init IO Config
	//
    I2CMaster_readReg8(ui8Address, TCA9534_CONFIG, TCA9534_RxBuf, 1);

    *TCA9534_TxBuf &= ~(ui8Pins);

    I2CMaster_writeReg8(ui8Address, TCA9534_CONFIG, TCA9534_TxBuf, 1);


#ifdef TCA9534_VERIFY_WRITES
	//
	// If write verification is enabled, read back write and verify it
	//
    I2CMaster_readReg8(ui8Address, TCA9534_CONFIG, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

bool
TCA9534_SetAsInputs(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Init IO Config
	//
	I2CMaster_readReg8(ui8Address, TCA9534_CONFIG, TCA9534_RxBuf, 1);

	*TCA9534_TxBuf = (*TCA9534_RxBuf | ui8Pins);
	I2CMaster_writeReg8(ui8Address, TCA9534_CONFIG, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
	//
	// If write verification is enabled, read back write and verify it
	//
    I2CMaster_readReg8(ui8Address, TCA9534_CONFIG, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

bool
TCA9534_SetOutputPins(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Init IO Config
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	*TCA9534_TxBuf = (*TCA9534_RxBuf | ui8Pins);

	I2CMaster_writeReg8(ui8Address, TCA9534_OUT, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
	//
	// If write verification is enabled, read back write and verify it
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

bool
TCA9534_SetOutput(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Init IO Config
	//
	*TCA9534_TxBuf = ui8Pins;

	I2CMaster_writeReg8(ui8Address, TCA9534_OUT, TCA9534_TxBuf, 1);

	return true;
}

bool
TCA9534_ClearOutputPins(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Init IO Config
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	*TCA9534_TxBuf = (*TCA9534_RxBuf & ~ui8Pins);

	I2CMaster_writeReg8(ui8Address, TCA9534_OUT, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
	//
	// If write verification is enabled, read back write and verify it
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

bool
TCA9534_ToggleOutputPins(uint8_t ui8Address, uint8_t ui8Pins)
{
	//
	// Read back current pin output state, modify, and write back
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	*TCA9534_TxBuf = (*TCA9534_RxBuf ^ ui8Pins);

	I2CMaster_writeReg8(ui8Address, TCA9534_OUT, TCA9534_TxBuf, 1);

#ifdef TCA9534_VERIFY_WRITES
	//
	// If write verification is enabled, read back write and verify it
	//
    I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	if (*TCA9534_RxBuf != *TCA9534_TxBuf)
	{
		return false;
	}
#endif

	return true;
}

uint8_t
TCA9534_ReadInputPins(uint8_t ui8Address)
{
	//
	// Read back current pin output state, modify, and write back
	//
    I2CMaster_readReg8(ui8Address, TCA9534_IN, TCA9534_RxBuf, 1);

	return *TCA9534_RxBuf;
}

uint8_t
TCA9534_ReadOutputPins(uint8_t ui8Address)
{
	//
	// Read back current pin output state, modify, and write back
	//
	I2CMaster_readReg8(ui8Address, TCA9534_OUT, TCA9534_RxBuf, 1);

	return *TCA9534_RxBuf;
}
