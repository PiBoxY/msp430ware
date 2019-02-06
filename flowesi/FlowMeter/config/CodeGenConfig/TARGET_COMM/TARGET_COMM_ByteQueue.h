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
//! TARGET_COMM_ByteQueue.h
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
#ifndef BYTEQUEUE_H_
#define BYTEQUEUE_H_

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
//! \typedef tByteQueue
//!
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
	uint16_t ui16QueueTail;

	//
	//! Head: Index of the Queue Head in the Queue Array.
	//! This is where the next item is pushed onto the queue
	//
	uint16_t ui16QueueHead;

} tByteQueue;

//
// Byte Queue External "Public" Function Prototypes
//
extern bool TARGET_COMM_byteQueueInit(tByteQueue *queue, uint8_t *pBufferArray,
		uint16_t ui16BufferArraySize);

extern bool TARGET_COMM_byteQueuePush(tByteQueue *queue, uint8_t dataToAdd);

extern uint8_t TARGET_COMM_byteQueuePull(tByteQueue *queue);

extern uint8_t TARGET_COMM_byteQueuePeek(tByteQueue *queue);

extern uint16_t TARGET_COMM_byteQueueSize(tByteQueue *queue);

extern bool TARGET_COMM_byteQueueFull(tByteQueue *queue);

extern bool TARGET_COMM_byteQueueEmpty(tByteQueue *queue);

#endif /* QUEUE_H_ */
