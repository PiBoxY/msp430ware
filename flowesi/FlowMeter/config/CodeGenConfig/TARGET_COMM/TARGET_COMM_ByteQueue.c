/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
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

//*****************************************************************************
//! TARGET_COMM_ByteQueue.c
//
//! A simple queue data structure for serial COM buffering
//! FIFO Array Based, data type is uint8_t, 8-bit unsigned integer
//! Buffering is ring-based, i.e wrap-around is allowed to always allow
//! maximum use of the buffer space.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup TARGET_COMM_ByteQueue
//! @{
//
//*****************************************************************************
//
// Include STDINT for C99 Types
//
#include <stdint.h>
#include <stdlib.h>

//
// Header Includes
//
#include "TARGET_COMM_ByteQueue.h"

//
// Queue methods are implemented below to operate on queue types.
//

//*****************************************************************************
//
//! This is the basic "constructor" function for a queue.  Call this once
//! before a queue is used, or again to clear the queue and reset it.
//! Function is public
//
//! \param *queue is a pointer to the queue to initialize
//! \param *pBufferArray is a pointer to buffer space that can be used for
//! this queue.
//! \param ui16BufferArraySize is an integer specifying the length of the
//! memory (in bytes) pointed to by pBufferArray that can be used for the queue
//
//! \return true if parameters were valid, else false.
//
//*****************************************************************************
bool TARGET_COMM_byteQueueInit(tByteQueue *queue, uint8_t *pBufferArray,
		uint16_t ui16BufferArraySize)
{
	uint16_t ui16Position;

	//
	// If buffer pointer or queue pointer are invalid, return false
	//
	if ((pBufferArray == 0) || (queue == 0))
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
	queue->ui16QueueTail = 0;
	queue->ui16QueueHead = 0;
	queue->QueueArray = pBufferArray;
	queue->ui16QueueArraySize = ui16BufferArraySize;

	//
	// Set all elements of the queue to zero.
	//
	for (ui16Position = 0; ui16Position < queue->ui16QueueArraySize;
			ui16Position++)
	{
		queue->QueueArray[ui16Position] = 0;
	}

	return true;
}

//*****************************************************************************
//
//! Push a new item onto the end of the queue
//! Function is public
//!
//! \param *queue is a pointer to the queue of interest
//! \param ui8DataToAdd is the byte to add to the queue
//!
//! \return true if old data was overwritten, else false
//
//*****************************************************************************
bool TARGET_COMM_byteQueuePush(tByteQueue *queue, uint8_t ui8DataToAdd)
{
	bool bDataOverwritten = false;

	//
	// Check for queue full case (overwrite oldest data point)
	//
	if (TARGET_COMM_byteQueueFull(queue) == true)
	{
		queue->QueueArray[queue->ui16QueueTail] = 0;
		queue->ui16QueueTail++;
		if (queue->ui16QueueTail == queue->ui16QueueArraySize)
		{
			queue->ui16QueueTail = 0;
		}
		bDataOverwritten = true;
	}

	//
	// Add item to queue
	//
	queue->QueueArray[queue->ui16QueueHead++] = ui8DataToAdd;

	//
	// Check for wrap-around case (ring)
	// Full case has already been checked above
	//
	if (queue->ui16QueueHead == queue->ui16QueueArraySize)
	{
		queue->ui16QueueHead = 0;
	}

	return bDataOverwritten;
}

//*****************************************************************************
//
//! Remove the next item from the front of the queue
//! TARGET_COMM_byteQueuePull(tByteQueue *queue)
//! Function is public
//!
//! \param *queue is a pointer to the queue of interest
//!
//! \return first item in queue if not empty, else 0.
//
//*****************************************************************************
uint8_t TARGET_COMM_byteQueuePull(tByteQueue *queue)
{
	uint8_t dataToPull;

	//
	// Check for empty queue case (no item to pull)
	//
	if (TARGET_COMM_byteQueueEmpty(queue) == true)
	{
		return 0;
	}

	//
	// Get data from queue tail and move tail index
	//
	dataToPull = queue->QueueArray[queue->ui16QueueTail];
	queue->QueueArray[queue->ui16QueueTail] = 0;
	queue->ui16QueueTail++;

	//
	// Check for wrap-around case (ring)
	//
	if (queue->ui16QueueTail == queue->ui16QueueArraySize)
	{
		queue->ui16QueueTail = 0;
	}

	return dataToPull;
}

//*****************************************************************************
//
//! Return the next item from the queue without removing it
//! Function is public
//!
//! \param *queue is a pointer to the queue of interest
//!
//! \return first item in queue if not empty, else 0.
//
//*****************************************************************************
uint8_t TARGET_COMM_byteQueuePeek(tByteQueue *queue)
{
	//
	// Check for empty queue case (no item to pull)
	//
	if (TARGET_COMM_byteQueueEmpty(queue) == true)
	{
		return 0;
	}

	//
	// Return data from queue tail
	//
	return queue->QueueArray[queue->ui16QueueTail];
}

//*****************************************************************************
//
//! Get the active size of the queue.
//! Function is public.  This is a helpful function for finding out how many
//! items are currently in the queue.
//!
//! \param *queue is a pointer to the queue of interest
//! \return number of items in the queue if not empty, else 0.
//
//*****************************************************************************
uint16_t TARGET_COMM_byteQueueSize(tByteQueue *queue)
{
	uint16_t size = 0;

	//
	// Check for empty case (0)
	//
	if (TARGET_COMM_byteQueueEmpty(queue) == true)
	{
		return 0;
	}

	//
	// Check for standard case (Tail is follows head)
	//
	if (queue->ui16QueueHead > queue->ui16QueueTail)
	{
		size = queue->ui16QueueHead - queue->ui16QueueTail;
	}
	//
	// Wrap-around case - Tail is greater than head
	//
	else
	{
		size = queue->ui16QueueArraySize - queue->ui16QueueTail
				+ queue->ui16QueueHead;
	}

	return size;
}

//*****************************************************************************
//
//! Check whether queue is full.
//! Function is public.  This is a helpful function for internal (private) use.
//!
//! \param *queue is a pointer to the queue of interest
//! \return 1 if full, else 0
//
//*****************************************************************************
bool TARGET_COMM_byteQueueFull(tByteQueue *queue)
{
	//
	// Check for head approaching tail full case
	//
	if (queue->ui16QueueTail != 0)
	{
		if (queue->ui16QueueHead == (queue->ui16QueueTail - 1))
		{
			return true;
		}
	}
	//
	// Check wrap-around full case
	//
	else if (queue->ui16QueueHead == (queue->ui16QueueArraySize - 1))
	{
		return true;
	}

	return false;
}

//*****************************************************************************
//
//! Check whether queue is empty.
//! Function is public.  This is a helpful function for internal (private) use.
//!
//! \param *queue is a pointer to the queue of interest
//! \return true if empty, else false
//
//*****************************************************************************
bool TARGET_COMM_byteQueueEmpty(tByteQueue *queue)
{
	//
	// Queue is empty when head index and tail index are the same.
	//
	if (queue->ui16QueueHead == queue->ui16QueueTail)
	{
		return true;
	}

	return false;
}

//*****************************************************************************
//
//! Close the doxygen group for TARGET_COMM_ByteQueue
//! @}
//
//*****************************************************************************
