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
#ifndef BYTEQUEUE_H_
#define BYTEQUEUE_H_
//#############################################################################
//
//! \file   byte_queue.h
//!
//! \brief Serial communication queue management.
//!        FIFO Array Based, data type is uint8_t, 8-bit unsigned integer
//!        Buffering is ring-based, i.e wrap-around is allowed to always allow
//!        maximum use of the buffer space.
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

//*****************************************************************************
//
//! typedef tByteQueue
//
//! tByteQueue is the data type for a ByteQueue container.
//! ByteQueue APIs operate on tByteQueue structures.
//
//*****************************************************************************
typedef struct
{
	//
	//! Pointer to queue data array
	//
	uint8_t* QueueArray;

	//
	//! Variable to store the size of the data array.
	//
	uint16_t ui16QueueArraySize;

	//
	//! Tail: Index of the Queue Tail in the Queue Array.
	//! This is where items are pulled from the queue
	//
	uint16_t ui16QueueHead;

	//
	//! Head: Index of the Queue Head in the Queue Array.
	//! This is where the next item is pushed onto the queue
	//
	uint16_t ui16QueueTail;

} tByteQueue;

//*****************************************************************************
//
//! \enum tByteQueueError
//
//! tByteQueueError enumerates possible Byte Queue function errors.
//
//*****************************************************************************
enum tByteQueueError
{
	//
	//! eByteQueue_Success when returned indicates that the function
	//! completed successfully.
	//
	eByteQueue_Success = 0,

	//
	//! eByteQueue_QueueEmpty when returned indicates that the function
	//! attempted to retrieve a byte but there was no data in the queue.
	//
	eByteQueue_QueueEmpty = -1,

	//
	//! eByteQueue_QueueOverrun indicates that the queue is full, and the oldest
	//! byte was dropped to make space for the newest byte.
	//
	eByteQueue_QueueOverrun = -2,
};

//
// Byte Queue External "Public" Function Prototypes
//

//*****************************************************************************
//
//! This is the basic "constructor" function for a queue.  Call this once
//! before a queue is used, or again to clear the queue and reset it.
//! Function is public
//
//! \param *pQueue is a pointer to the queue to initialize
//! \param *pBufferArray is a pointer to buffer space that can be used for
//! this queue.
//! \param ui16BufferArraySize is an integer specifying the length of the
//! memory (in bytes) pointed to by pBufferArray that can be used for the queue
//
//! \return true if parameters were valid, else false.
//
//*****************************************************************************
extern bool ByteQueue_init(tByteQueue *pQueue, uint8_t *pBufferArray,
		uint16_t ui16BufferArraySize);
		
//*****************************************************************************
//
//! Push a new item onto the end of the queue
//! Function is public
//!
//! \param *pQueue is a pointer to the queue of interest
//! \param ui8DataToAdd is the byte to add to the queue
//!
//! \return eByteQueue_QueueOverrun if queue was full and the oldest byte was
//! dropped; else eByteQueue_Success.
//
//*****************************************************************************
extern int8_t ByteQueue_push(tByteQueue *pQueue, uint8_t ui8DataToAdd);

//*****************************************************************************
//
//! Remove the next item from the front of the queue
//! Function is public
//!
//! \param *pQueue is a pointer to the queue of interest
//! \param *pDestination is a pointer to the destination
//!
//! \return eByteQueue_QueueEmpty if queue was empty (no bytes to pull),
//! else eByteQueue_Success.
//
//*****************************************************************************
extern int8_t ByteQueue_pull(tByteQueue *pQueue, uint8_t *pDestination);

//*****************************************************************************
//
//! Get the active size of the queue.
//! Function is public.  This is a helpful function for finding out how many
//! items are currently in the queue.
//!
//! \param *pQueue is a pointer to the queue of interest
//! \return number of items in the queue if not empty, else 0.
//
//*****************************************************************************
extern uint16_t ByteQueue_getLength(tByteQueue *pQueue);

//*****************************************************************************
//
//! Check whether queue is full.
//! Function is public.  This is a helpful function for internal (private) use.
//!
//! \param *pQueue is a pointer to the queue of interest
//! \return true if full, else false
//
//*****************************************************************************
extern bool ByteQueue_isFull(tByteQueue *pQueue);

//*****************************************************************************
//
//! Check whether queue is empty.
//! Function is public.  This is a helpful function for internal (private) use.
//!
//! \param *pQueue is a pointer to the queue of interest
//! \return true if empty, else false
//
//*****************************************************************************
extern bool ByteQueue_isEmpty(tByteQueue *pQueue);

#endif /* BYTEQUEUE_H_ */
//*****************************************************************************
//! Close the doxygen group
//! @}
//*****************************************************************************
