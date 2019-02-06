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
//! \file   host_interface.h
//!
//! \brief  This is the host device interface for the BOOSTXL-CAPKEYPAD.
//!         The host interface is implemented as a basic I2C slave with IRQ
//!         capability.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef HOST_INTERFACE_H_
#define HOST_INTERFACE_H_

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief The host interface IRQ pin
//!
#define HOST_IRQ_PIN BIT4

//! \brief The host interface IRQ port direction register
//!
#define HOST_IRQ_DIR P2DIR

//! \brief The host interface IRQ port output register
//!
#define HOST_IRQ_OUT P2OUT

//! \brief The host interface IRQ pull low macro
//!
#define HOST_PULL_IRQ     HOST_IRQ_DIR |= HOST_IRQ_PIN

//! \brief The host interface IRQ release high macro
//!
#define HOST_RELEASE_IRQ  HOST_IRQ_DIR &= ~HOST_IRQ_PIN

//! \brief The host interface I2C slave address
//!
#define HOST_INTERFACE_I2C_SLAVE_ADDRESS 0x0B

//! \brief The host interface I2C slave invalid access return value
//!
#define HOST_INTERFACE_I2C_SLAVE_INVALID_RETURN 0xFF

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief Initialize the host interface module.
//! This init process enables the EUSCI_B0 module in I2C slave mode so that
//! a host processor on a LaunchPad may read out the keypad status and set
//! control settings.
//! \param none
//! \return none
extern void HostInterface_init(void);

#endif /* HOST_INTERFACE_H_ */
