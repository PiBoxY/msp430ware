/* --COPYRIGHT--,BSD
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * --/COPYRIGHT--*/
#ifndef COMM_CONFIG_H_
#define COMM_CONFIG_H_
//#############################################################################
//
//! \file   comm_config.h
//!
//! \brief COMM module API configuration file
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************


#include "utils/byte_queue.h"
#include "utils/ping_pong_buffer.h"
#include "protocol/listener.h"

//*****************************************************************************
// COMM Interface Selection Definitions
//*****************************************************************************

//! \def No Serial Interface (Disable the COMM module)
//!
#define COMM_NONE (0)

//! \def UART Serial Interface
//!
#define COMM_UART (1)

//! \def I2C Slave Serial Interface
//!
#define COMM_I2CSLAVE (2)

//! \def Serial communication interface selection.  Set this to one of the valid
//!      interfaces or COMM_NONE to disable the COMM module.
//!
#define COMM_SERIAL_INTERFACE (COMM_UART)

//! \def Serial communication interface selection enable definitions
//!      These are set automatically to include the relevent modules.
//!
#if (COMM_SERIAL_INTERFACE==COMM_UART)
#define UART__ENABLE (true)
#define I2CSLAVE__ENABLE (false)
#define TIMEOUT__ENABLE (false)
#elif (COMM_SERIAL_INTERFACE==COMM_I2CSLAVE)
#define UART__ENABLE (false)
#define I2CSLAVE__ENABLE (true)
#define TIMEOUT__ENABLE	(true)
#endif

//*****************************************************************************
// COMM RAM Buffer Definitions and Global Variables
//*****************************************************************************

//! \def Receive queue buffer size
//!
#define COMM_RX_BUFF_SZ (64)

//! \def Transmit buffer size
//!
#define COMM_TX_BUFF_SZ  (64)

//! \def I2C buffer size
//!
#define COMM_I2C_BUFF_SZ  (64)

//! \var The transmit buffer
extern uint8_t Comm_transmitBuffer[COMM_TX_BUFF_SZ];

//! \var The receive buffer
extern uint8_t Comm_receiveBuffer[COMM_RX_BUFF_SZ];

//! \var The receive queue
extern tByteQueue Comm_receiveQueue;

//*****************************************************************************
// COMM Listener Table Definitions and Global Variables
//*****************************************************************************

//! \def The maximum number of command listeners
#define COMM_LISTENER_TABLE_SZ (8)

//! \var Listener configuration
extern Listener_config_t Comm_listenerConfig;


#endif /* COMM_CONFIG_H_ */
