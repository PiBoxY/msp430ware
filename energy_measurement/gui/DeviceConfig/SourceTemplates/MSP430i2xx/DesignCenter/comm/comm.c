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
//! \file   comm.c
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
#include "drivers/i2cslave.h"
#include "drivers/uart.h"
#include "comm_config.h"
#include "comm.h"
#include <string.h>

Packet_t packet={0};

//*****************************************************************************
// UART Interface Event Handlers and Configuration
//*****************************************************************************

#if COMM_SERIAL_INTERFACE==COMM_UART
bool Comm_uartReceiveHandler(uint8_t ui8Data)
{
	//
	// Push the received byte onto the receive queue.
	// Return true to exit active from the receive ISR foreground thread.
	//
	ByteQueue_push(&Comm_receiveQueue, ui8Data);
	return true;
}

//! \var The UART port definition for the UART driver.
//!
const tUARTPort Comm_uartPort =
{
	.pbReceiveCallback = Comm_uartReceiveHandler,
	.pbErrorCallback = 0,
	.peripheralParameters.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
	.peripheralParameters.clockPrescalar = UART__PRESCALER,
	.peripheralParameters.firstModReg = UART__FIRST_STAGE_MOD,
	.peripheralParameters.secondModReg = UART__SECOND_STAGE_MOD,
	.peripheralParameters.parity = EUSCI_A_UART_NO_PARITY,
	.peripheralParameters.msborLsbFirst = EUSCI_A_UART_LSB_FIRST,
	.peripheralParameters.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,
	.peripheralParameters.uartMode = EUSCI_A_UART_MODE,
	.peripheralParameters.overSampling = UART__SAMPLING_MODE
};

//*****************************************************************************
// I2C Slave Interface Event Handlers and Configuration
//*****************************************************************************
#elif COMM_SERIAL_INTERFACE==COMM_I2CSLAVE

//! \var The I2C slave receive buffer.
//!
uint8_t Comm_i2cSlaveRxBuffer[COMM_I2C_BUFF_SZ];

bool Comm_i2cReceiveHandler(uint16_t cnt)
{
	uint16_t i;

	//
	// Push each received byte onto the receive queue.
	// Return true to exit active from the receive ISR foreground thread.
	//
	for (i=0; i<cnt; i++)
	{
		(void)ByteQueue_push(
				&Comm_receiveQueue,
				Comm_i2cSlaveRxBuffer[i]
			);
	}
	return true;
}

//! \var The I2C slave port definition for the I2C slave driver.
//!
const tI2CSlavePort Comm_i2cSlavePort =
{
	.pbReceiveCallback = Comm_i2cReceiveHandler,
	.pvErrorCallback = 0,
	.pReceiveBuffer = Comm_i2cSlaveRxBuffer,
	.ui16ReceiveBufferSize = COMM_I2C_BUFF_SZ,
	.bSendReadLengthFirst = true
};
#endif

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Comm_setup(void)
{
	//
	// Setup communication buffers
	//
	(void)ByteQueue_init(
			&Comm_receiveQueue,
			Comm_receiveBuffer,
			COMM_RX_BUFF_SZ
		);

	//
	// Open serial driver
	//
#if COMM_SERIAL_INTERFACE==COMM_UART
	UART_openPort(&Comm_uartPort);
#elif COMM_SERIAL_INTERFACE==COMM_I2CSLAVE
	I2CSlave_openPort(&Comm_i2cSlavePort);
#endif
}

void Comm_writeBuffer(const uint8_t *sourceBuffer, uint16_t sourceLength)
{
#if (COMM_SERIAL_INTERFACE==COMM_UART)

    while (1)
    {
        __bic_SR_register(GIE);
        if (UART_getPortStatus() != eUARTIsIdle)
        {
            __bis_SR_register(UART__LPMx_bits | GIE);
        }
        else
        {
            __bis_SR_register(GIE);
            break;
        }
    };

	//
	// Because UART is being used, convert the source buffer into stream format.
	// This adds headers, byte stuffing, and the checksum.
	//
	sourceLength = Packet_convertToStream(
			&Comm_transmitBuffer[0],
			sourceBuffer,
			sourceLength
		);
	//
	// Transmit the packet via the UART interface.
	//
	UART_transmitBuffer(&Comm_transmitBuffer[0], sourceLength);
#elif (COMM_SERIAL_INTERFACE==COMM_I2CSLAVE)
	//
	// Because I2C Slave is being used, copy the packet as-is into the transmit buffer.
	// Then append the checksum directly.  Headers and byte stuffing are not needed
	// in I2C slave mode, as the master uses I2C frames to interpret packets.
	//
	memcpy(&Comm_transmitBuffer[0], sourceBuffer, sourceLength);
	sourceLength = Packet_appendChecksum(&Comm_transmitBuffer[0], sourceLength);

	//
	// Update the I2C slave transmit buffer, then set the request flag to signal
	// the host that data is waiting.  This operation will time out if the master
	// does not access the data.
	//
	I2CSlave_setTransmitBuffer(&Comm_transmitBuffer[0], sourceLength);
	I2CSlave_setRequestFlag();
#endif
}

void Comm_writePacket(const Packet_t *packet)
{
	//
	// Simply call the write buffer function, passing the packet parameters.
	//
	Comm_writeBuffer(packet->payload, packet->length);
}



bool Comm_checkForPacket(Packet_t *packet)
{
	bool gotPacket;

	/*
	// Attempt to extract a packet from the inbound data stream.
	*/
	gotPacket = Packet_extractFromStream(&Comm_receiveQueue, packet);

	return gotPacket;
}

bool Comm_addCmdListener(uint8_t cmd0, uint8_t cmd1, Listener_handler_t handler)
{
	bool status;

	/*
	// Add a listener to the listener table
	*/
	status = Listener_registerListener(
			&Comm_listenerConfig,
			cmd0,
			cmd1,
			handler
        );

	return status;
}

bool Comm_listenForCmd(void)
{
	bool Listenstatus;

	Listenstatus = Comm_checkForPacket(&packet);

	if(true == Listenstatus)
	{
		Listenstatus = Listener_lookupHandler(&Comm_listenerConfig, &packet);
	}

	return Listenstatus;
}

