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
#ifndef PINGPONGBUFFER_H_
#define PINGPONGBUFFER_H_
//#############################################################################
//
//! \file   ping_pong_buffer.h
//!
//! \brief Serial communication ping-pong buffer management.
//
//! An edit buffer and a transmit buffer of equal size are used to allow
//! a new packet to be built while a previous packet is being transmitted.
//
//! Two contiguous byte arrays of
//! equal size in memory need to be established.  Pointers to these two arrays
//! need to be passed to to the CAPT_initPingPongBuffer() API.  From that point,
//! either buffer can be read to or written to by accessing the structure.
//! a toggle API switches the roles of the two buffers (edit and transmit).
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
//! typedef tPingPongBuffer
//!
//! tPingPongBuffer is the data type for a tPingPongBuffer container.
//! tPingPongBuffer APIs operate on tPingPongBuffer structures.
//! The structure is two bytes in size.  The arrays are pointed to and accessed
//! outside of the APIs provided here.
//
//*****************************************************************************
typedef struct
{
	//
	//! *pEditBuffer is a pointer to the buffer that can be written to.
	//
	uint8_t *pEditBuffer;

	//
	//! *pTransmitBuffer is a pointer to the buffer that can be read from.
	//
	uint8_t *pTransmitBuffer;
} tPingPongBuffer;

//
// Ping Pong Buffer APIs
//

//*****************************************************************************
//
//! This is the basic "constructor" function for a ping pong buffer.  
//! Call this once before the buffer is used.
//
//! \param *PPBuffer is a pointer to the Ping Pong buffer structure
//! to initialize.
//! \param *pBuffer1 is a pointer to an array of bytes to use for buffering.
//! \param *pBuffer2 is a pointer to an array of bytes to use for buffering.
//! pBuffer1 should point to a buffer that is the same length as pBuffer2.
//
//! \par Returns
//!		true if pointers are !=0, else false.
//
//*****************************************************************************
extern bool PingPongBuffer_init(tPingPongBuffer *PPBuffer,
		uint8_t *pBuffer1, uint8_t *pBuffer2);

//*****************************************************************************
//
//! This functions swaps the edit buffer with the transmit buffer.
//
//! \param *PPBuffer is a pointer to the Ping Pong buffer to toggle.
//
//! \par Returns
//!		if the buffer pointer is !=0, else false.
//
//*****************************************************************************
extern bool PingPongBuffer_toggle(tPingPongBuffer *PPBuffer);

#endif /* PINGPONGBUFFER_H_ */
//*****************************************************************************
//! Close the doxygen group
//! @}
//*****************************************************************************
