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

#ifndef I2CMASTER_H_
#define I2CMASTER_H_
//#############################################################################
//
//! \file   I2CMaster.h
//!
//! \brief  This module implements a standard I2C Master communication driver
//!         for the MSP eUSCI_B I2C peripheral.  Standard functions for open,
//!         close, read, and write are provided.  In addition, functions for
//!         reading and writing to 8-bit slave sub-registers are provided
//!         via readReg8 and writeReg8.
//!
//!         Configuration of the driver is done in the I2CMaster_Definitions.h
//!         file, which controls the peripheral and clock settings to use.
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "I2CMaster_Definitions.h"

#if (I2CMASTER__ENABLE==true)

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief This enumeration defines the error codes that will be returned
//!        by module functions.
//!
enum
{
    //! No Error Occurred! The operation was successful.
    eI2CMaster_noError = 0,
    
    //! The I2C bus is currently unavailable.
    eI2CMaster_unavailableError,
    
    //! The requested I2C bus slave was not available or nack'ed.
    eI2CMaster_slaveNackError,
    
    //! A clock low time-out occurred.
    eI2CMaster_busTimeoutError
};
typedef uint8_t tI2CMaster_error;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Open the I2C Master driver for operation.  It is expected that the
//! digital IO required for the eUSCI_B were muxed correctly by the caller.
void I2CMaster_open(void);

//! \brief Close the I2C Master driver.
void I2CMaster_close(void);

//! \brief Write a buffer to an I2C slave
//! \param ui8Addr is the address of the slave
//! \param pData is a pointer to the data to write
//! \param ui8Cnt is the length of pData in bytes
//! \return an error value to indicate the success of the operation
tI2CMaster_error I2CMaster_writeBuffer(uint8_t ui8Addr, uint8_t *pData,
    uint8_t ui8Cnt);

//! \brief Read to a buffer from an I2C slave
//! \param ui8Addr is the address of the slave
//! \param ui8Reg is the sub-register on the slave device to read from
//! \param ui8Cnt specifies the number of bytes to read from the slave.
//! \return an error value to indicate the success of the operation
tI2CMaster_error I2CMaster_readBuffer(uint8_t ui8Addr, uint8_t *pData,
    uint8_t ui8Cnt);

//! \brief Write to an I2C slave at the 8-bit slave sub-register indicated.
//! \param ui8Addr is the address of the slave
//! \param ui8Reg is the sub-register on the slave device to write to
//! \param pData is a pointer to the data to write
//! \param ui8Cnt is the length of pData in bytes
//! \return an error value to indicate the success of the operation
tI2CMaster_error I2CMaster_writeReg8(uint8_t ui8Addr, uint8_t ui8Reg,
    uint8_t *pData, uint8_t ui8Cnt);

//! \brief Read from an I2C slave at the 8-bit slave sub-register indicated.
//! \param ui8Addr is the address of the slave
//! \param ui8Reg is the sub-register on the slave device to read from
//! \param pData is a pointer to a memory location to store the read data
//! \param ui8Cnt specifies the number of bytes to read from the slave.
//! \return an error value to indicate the success of the operation
tI2CMaster_error I2CMaster_readReg8(uint8_t ui8Addr, uint8_t ui8Reg, 
    uint8_t *pData, uint8_t ui8Cnt);

#endif /* I2CMASTER__ENABLE==true */

#endif /* I2CMASTER_H_ */
