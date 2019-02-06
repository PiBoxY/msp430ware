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
//#############################################################################
//
//! \file   ping_pong_buffer.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include "ping_pong_buffer.h"

/*ROM_FUNC_START*/
bool PingPongBuffer_init(tPingPongBuffer *PPBuffer, uint8_t *pBuffer1,
		uint8_t *pBuffer2)
{
	if ((PPBuffer != 0) && (pBuffer1 != 0) && (pBuffer2 != 0))
	{
		PPBuffer->pEditBuffer = pBuffer1;
		PPBuffer->pTransmitBuffer = pBuffer2;
		return true;
	}

	return false;
}
/*ROM_FUNC_STOP*/

/*ROM_FUNC_START*/
bool PingPongBuffer_toggle(tPingPongBuffer *PPBuffer)
{
	uint8_t *pTempPtr;

	if (PPBuffer != 0)
	{
		pTempPtr = PPBuffer->pEditBuffer;
		PPBuffer->pEditBuffer = PPBuffer->pTransmitBuffer;
		PPBuffer->pTransmitBuffer = pTempPtr;
		return true;
	}

	return false;
}
/*ROM_FUNC_STOP*/
