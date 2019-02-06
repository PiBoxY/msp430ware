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
//! \file   TCA95xx.h
//
//! \brief  Seven Segment driver interface
//
//*****************************************************************************

//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef TCA9534_H_
#define TCA9534_H_

#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
// Definitions
//*****************************************************************************
#define NO_TCA9534_VERIFY_WRITES
#define TCA5934_ADDRS      (0x20)
#define TCA9534_NONE	   (0x00)
#define TCA9534_ALL		   (0xFF)

//*****************************************************************************
//
// \brief  TCA9534_Init() initilizes LED driver
//
// \param  Device I2C address
// \return success/failure
//
//*****************************************************************************
bool TCA9534_Init(uint8_t ui8Address);


//*****************************************************************************
//
// \brief  TCA9534_SetAsOutputs() configures pins as outputs
//
// \param  Device I2C address
// \param  Pins to be outputs
// \return success/failure
//
//*****************************************************************************
bool TCA9534_SetAsOutputs(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_SetAsInputs() configures pins as inputs
//
// \param  Device I2C address
// \param  Pins to be inputs
// \return success/failure
//
//*****************************************************************************
bool TCA9534_SetAsInputs(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_SetOutputPins() sets selected pins
//
// \param  Device I2C address
// \param  Pins to set
// \return success/failure
//
//*****************************************************************************
bool TCA9534_SetOutputPins(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_ClearOutputPins() clears selected pins
//
// \param  Device I2C address
// \param  Pins to clear
// \return success/failure
//
//*****************************************************************************
bool TCA9534_ClearOutputPins(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_SetOutput() sets selected pins
//
// \param  Device I2C address
// \param  Pins to set
// \return success/failure
//
//*****************************************************************************
bool TCA9534_SetOutput(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_SetOutput() sets selected pins
//
// \param  Device I2C address
// \param  Pins to toggle
// \return success/failure
//
//*****************************************************************************
bool TCA9534_ToggleOutputPins(uint8_t ui8Address, uint8_t ui8Pins);

//*****************************************************************************
//
// \brief  TCA9534_ToggleOutputPins() reads the state of selected input pins
//
// \param  Device I2C address
// \param  Pins to read
// \return success/failure
//
//*****************************************************************************
uint8_t TCA9534_ReadInputPins(uint8_t ui8Address);

//*****************************************************************************
//
// \brief  TCA9534_ReadOutputPins() reads the state of the output pins
//
// \param  Device I2C address
// \return 8-bit value of pins
//
//*****************************************************************************
uint8_t TCA9534_ReadOutputPins(uint8_t ui8Address);


#endif /* TCA9534_H_ */
