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
#ifndef LISTENER_H_
#define LISTENER_H_
//#############################################################################
//
//! \file   listener.h
//!
//! \brief Listener module for providing CMD detection and association with
//!        a handler function
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include <stdint.h>
#include <stdbool.h>
#include "protocol/packet.h"

#ifndef NULL
#define NULL (0)
#endif

//! \typedef Function pointer to a listener handler function
//!
typedef void (*Listener_handler_t)(Packet_t *packet);

//! \typedef A listener object that associates a cmd with a handler fxn
typedef struct
{
	uint8_t cmd0;
	uint8_t cmd1;
	Listener_handler_t handler;
} Listener_t;

//! \typedef A listener configuration that stores a link to a table of
//!          listener objects
typedef struct
{
	Listener_t *table;
	uint8_t tableLength;
} Listener_config_t;

//! \brief Register a listener.  A listener is an association between a CMD in a
//! packet (the first byte of the packet) and a corresponding handler function
//! that may be called via the Listener lookup function when that CMD is seen.
//! \param[in] config is the listener configuration to add the listener to
//! \param[in] cmd is the cmd to listen for
//! \param[in] handler is the handler function to associated with the cmd
//! \return true if the listener was added successfully, else false
extern bool Listener_registerListener(Listener_config_t *config,
    uint8_t cmd0, uint8_t cmd1, Listener_handler_t handler);

//! \brief Lookup a handler based on a packet input.  If the cmd
//!        byte in the packet matches a listener in the config,
//!        the handler for that command is called from this function
//!        and is passed the packet containing the command.
extern uint8_t Listener_lookupHandler(Listener_config_t *config,
    Packet_t *packet);

#endif /* LISTENER_H_ */
