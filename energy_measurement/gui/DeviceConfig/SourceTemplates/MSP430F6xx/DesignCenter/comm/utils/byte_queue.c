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
//! \file   byte_queue.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//#############################################################################

#include "byte_queue.h"

bool ByteQueue_init(tByteQueue *pQueue, uint8_t *pBufferArray,
		uint16_t ui16BufferArraySize)
{
	//
	// If buffer pointer or queue pointer are invalid, return false
	//
	if ((pBufferArray == 0) || (pQueue == 0))
	{
		return false;
	}

	//
	// If buffer size is less than 2, a queue cannot be created! Return false.
	//
	if (ui16BufferArraySize < 2)
	{
		return false;
	}

	//
	// Set tail and head indexes to zero, and store off buffer pointer
	// and buffer size.
	//
	pQueue->ui16QueueHead = 0;
	pQueue->ui16QueueTail = 0;
	pQueue->QueueArray = pBufferArray;
	pQueue->ui16QueueArraySize = ui16BufferArraySize;

	return true;
}

int8_t ByteQueue_push(tByteQueue *pQueue, uint8_t ui8DataToAdd)
{
	int8_t i8ReturnStatus = eByteQueue_Success;

	//
	// Check for queue full case (overwrite oldest data point)
	//
	if (ByteQueue_isFull(pQueue) == true)
	{
		pQueue->QueueArray[pQueue->ui16QueueHead] = 0;
		pQueue->ui16QueueHead++;
		if (pQueue->ui16QueueHead == pQueue->ui16QueueArraySize)
		{
			pQueue->ui16QueueHead = 0;
		}
		i8ReturnStatus = eByteQueue_QueueOverrun;
	}

	//
	// Add item to queue
	//
	pQueue->QueueArray[pQueue->ui16QueueTail++] = ui8DataToAdd;

	//
	// Check for wrap-around case (ring)
	// Full case has already been checked above
	//
	if (pQueue->ui16QueueTail == pQueue->ui16QueueArraySize)
	{
		pQueue->ui16QueueTail = 0;
	}

	return i8ReturnStatus;
}

int8_t ByteQueue_pull(tByteQueue *pQueue, uint8_t *pDestination)
{
	//
	// Check for empty queue case (no item to pull)
	//
	if (ByteQueue_isEmpty(pQueue) == true)
	{
		return eByteQueue_QueueEmpty;
	}

	//
	// Get data from queue head and move head index
	//
	*pDestination = pQueue->QueueArray[pQueue->ui16QueueHead];
	pQueue->QueueArray[pQueue->ui16QueueHead] = 0;
	pQueue->ui16QueueHead++;

	//
	// Check for wrap-around case (ring)
	//
	if (pQueue->ui16QueueHead == pQueue->ui16QueueArraySize)
	{
		pQueue->ui16QueueHead = 0;
	}

	return eByteQueue_Success;
}

uint16_t ByteQueue_getLength(tByteQueue *pQueue)
{
	uint16_t size = 0;

	//
	// Check for empty case (0)
	//
	if (ByteQueue_isEmpty(pQueue) == true)
	{
		return 0;
	}

	//
	// Check for standard case (head is follows tail)
	//
	if (pQueue->ui16QueueTail > pQueue->ui16QueueHead)
	{
		size = pQueue->ui16QueueTail - pQueue->ui16QueueHead;
	}
	//
	// Wrap-around case - Head is greater than tail
	//
	else
	{
		size = pQueue->ui16QueueArraySize - pQueue->ui16QueueHead
				+ pQueue->ui16QueueTail;
	}

	return size;
}

bool ByteQueue_isFull(tByteQueue *pQueue)
{
	//
	// Check for tail approaching head full case
	//
	if (pQueue->ui16QueueHead != 0)
	{
		if (pQueue->ui16QueueTail == (pQueue->ui16QueueHead - 1))
		{
			return true;
		}
	}
	//
	// Check wrap-around full case
	//
	else if (pQueue->ui16QueueTail == (pQueue->ui16QueueArraySize - 1))
	{
		return true;
	}

	return false;
}

bool ByteQueue_isEmpty(tByteQueue *pQueue)
{
	//
	// pQueue is empty when head index and tail index are the same.
	//
	if (pQueue->ui16QueueTail == pQueue->ui16QueueHead)
	{
		return true;
	}

	return false;
}
