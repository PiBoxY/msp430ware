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
#ifndef DRV26X_I2CPORT_H_
#define DRV26X_I2CPORT_H_
//#############################################################################
//
//! \file   DRV26x_I2CPort.h
//!
//! \brief The DRV26x_I2CPort header file defines macros for the 
//!        platform-specific I2C interface.  This file enables porting of 
//!        this library to multiple platforms.  There are 3 definitions that 
//!        must be defined here to enable access to the platform's I2C bus 
//!        master port: the address of the driver IC,a block read, and a 
//!        block write.
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.00.00.30
// Release Date: Thu, Oct 01, 2015  5:05:49 PM
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "DRV26x_RegisterFile.h"
#include "I2CMaster.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Defines the I2C device address of the driver
//!
#define DRV26X_I2C_ADDRESS  (0x5A)

//! \brief Defines a macro to write a block of registers.
//!
#define DRV26X_WRITEBLOCK(reg, buff, len) \
	I2CMaster_writeReg8(DRV26X_I2C_ADDRESS, reg, buff, len)

//! \brief Defines a macro to read a block of registers.
//!
#define DRV26X_READBLOCK(reg, buff, len) \
	I2CMaster_readReg8(DRV26X_I2C_ADDRESS, reg, buff, len)
	
//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Use DRV26x_getReg() to read a register on the DRV26x device.
//! \param reg specifies the register to read.  The available registers
//!        are defined in DRV26x_RegisterFile.h
//! \param dest is a pointer to the memory location to store the value
//!        that was read.
//! \return 0 if the operation was successful.
extern uint8_t DRV26x_getReg(DRV26x_Register_t reg, uint8_t *dest);

//! \brief Use DRV26x_setReg() to set the value of an 8-bit register on the
//!        DRV26x device.
//! \param reg specifies the register to write to.  The available registers
//!        are defined in DRV26x_RegisterFile.h
//! \param value specifies the 8-bit value to write to the register.
//! \return 0 if the operation was successful.
extern uint8_t DRV26x_setReg(DRV26x_Register_t reg, uint8_t value);

#endif /* DRV26X_I2CPORT_H_ */
