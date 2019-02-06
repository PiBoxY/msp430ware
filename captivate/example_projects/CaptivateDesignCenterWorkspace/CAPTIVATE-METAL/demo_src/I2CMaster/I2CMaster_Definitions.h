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
#ifndef I2CMASTER_DEFINITIONS_H_
#define I2CMASTER_DEFINITIONS_H_
//#############################################################################
//
//! \file   I2CMaster_Definitions.h
//!
//! \brief  This file defines the configuration of the I2C Master driver
//!         implemented in I2CMaster.c/.h
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

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Controls whether the I2C master port is included during build.  
//!        To use the I2C master port, set this definition to true.
//!
#define I2CMASTER__ENABLE				                                 (true)

//! \brief Defines the MSP430 base address of the eUSCI_B instance
//!        being used with this I2C master port.
//!
#define I2CMASTER__EUSCI_B_PERIPHERAL	                        (EUSCI_B0_BASE)

//! \brief Defines the bit clock prescaler to use.
//!
#define I2CMASTER__PRESCALER	                                            (5)

//! \brief Defines the low power mode to enter when waiting for a transmission 
//!        to complete.
//!
#define I2CMASTER__LPMx_bits				                        (LPM0_bits)

#endif /* I2CMASTER_DEFINITIONS_H_ */
