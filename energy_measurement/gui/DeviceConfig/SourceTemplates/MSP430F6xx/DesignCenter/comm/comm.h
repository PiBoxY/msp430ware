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
#ifndef COMM_H_
#define COMM_H_
//#############################################################################
//
//! \file   comm.h
//!
//! \brief COMM module top level API.  These are the COMM functions that will
//!        be called directly by the application.
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
#include <msp430.h>
#include "protocol/packet.h"
#include "protocol/listener.h"

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Setup the COMM module for operation.
//!        Call this function before using the other communication fxn's,
//!        and after the digital IO muxes and clock system have been set up.
//!        The COMM module is statically configured via the comm_config files.
extern void Comm_setup(void);

//! \brief Write a bare-bones data buffer to the outbound serial stream.
//!        The required checksum will be appended automatically.
//!        After the function returns, the source buffer is no longer needed
//!        and may be overwritten with new data.
//! \param[in] sourceBuffer is a pointer to the source data to write
//! \param[in] sourceLength indicates the length of the source data in bytes
extern void Comm_writeBuffer(const uint8_t *sourceBuffer, uint16_t sourceLength);

//! \brief Write a packet object to the outbound serial stream.
//!        The required checksum will be appended automatically.
//!        After the function returns, the source buffer is no longer needed
//!        and may be overwritten with new data.
//! \param[in] packet is a pointer to the packet to write
extern void Comm_writePacket(const Packet_t *packet);

//! \brief Check for a packet in the inbound serial stream.  This function
//!        should be called periodically by the application's background loop
//!        to check for incoming packets from the host.  The passed packet
//!        structure is used for framing.  When a completed and validated
//!        packet is received, this funciton will return true and the
//!        packet itself will be contained in the packet structure that
//!        was passed by reference into this function.
//! \param[out] packet is a pointer to the packet to store results into.
//! \return true if a packet was framed, else false
extern bool Comm_checkForPacket(Packet_t *packet);

//! \brief Add a command listener that will be called when a CMD is
//!        found in the inbound data stream.  The first byte in a packet
//!        is the CMD byte.  This function will register a listener with the
//!        listener API.  When the passed cmd is received from the host,
//!        the passed handler function will be automatically called.
//!        This funciton is called once during setup to associate the handler
//!        function with the specified CMD.  Then, the Comm_listenForCmd
//!        function must be periodically called from the application background
//!        loop to check for CMDs.
//! \param[in] cmd0 is the cmd0 to look for
//! \param[in] cmd1 is the cmd1 to look for
//! \param[in] handler is the cmd handler function to call
//! \return true if successful, false if no space left for additional handlers
extern bool Comm_addCmdListener(uint8_t cmd0, uint8_t cmd1, Listener_handler_t handler);

//! \brief Listen for a cmd in a packet in the inbound serial stream.
//!        If a cmd is found, attempt to look up the cmd via the listner.
//!        If a CMD listener was added, this function must be called periodically
//!        in the application background loop to check the inbound stream for
//!        packets containing CMDs that are in the listener table.
//!        If a CMD is received that matches a registered CMD, the CMD handler
//!        is automatically called from within this process.  The application
//!        may then handle the receive event accordingly.
//! \return true if a packet with a valid cmd was received, else false
extern bool Comm_listenForCmd(void);

#endif /* COMM_H_ */
