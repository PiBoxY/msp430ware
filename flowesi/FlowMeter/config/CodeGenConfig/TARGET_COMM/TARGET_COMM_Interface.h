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
//! TARGET_COMM_Interface.h
//
//! This is the top-level communication  layer.  It interfaces
//! between the application level, protocol layer, and the low-level 
//! serial interface.
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//! \author Modified by: MSP430 Embedded Software
//
//
//*****************************************************************************
#ifndef TARGET_COMM_INTERFACE_H_
#define TARGET_COMM_INTERFACE_H_

//
// Include ByteQueue and PingPongBuffer for help managing serial communications
//
#include "TARGET_COMM_PingPongBuffer.h"
#include "TARGET_COMM_ByteQueue.h"

//
// Include the Captivate protocol layer for packet generation and interpretation
//
#include "TARGET_COMM_Protocol.h"

//
// Include the low level serial driver (eUSCI_A0 UART in this case)
//
#include "Serial_Drivers\eusci_uart_driver.h"

//*****************************************************************************
//
//! \def Serial_init defines a macro for interfacing to the low level serial
//! interface.  
//! \def Serial_write defines a macro for interfacing to the low level serial
//! interface transmit function.
//
//! By using generic init commands here, other interfaces,
//! such as SPI or I2C, can be implemented easily.
//
//*****************************************************************************
#define Serial_init(receive_queue)							EUSCI_UART_init(receive_queue)
#define Serial_write(transmit_buffer, transmit_length)		EUSCI_UART_write(transmit_buffer, transmit_length)

extern void TARGET_COMM_init(void);
extern void TARGET_COMM_sendSensorData(void);
extern void TARGET_COMM_sendTSMData(void);
extern void TARGET_COMM_sendDACData(void);
extern void TARGET_COMM_sendDebugData(void);
extern void TARGET_COMM_sendRecalStartCmd(void);
extern void TARGET_COMM_sendRecalValues(void);
extern void TARGET_COMM_checkForReceivedPacket(void);

#endif /* TARGET_COMM_INTERFACE_H_ */
