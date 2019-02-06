/*=============================================================
 * @file:		TI_SMS_Motor_UART_API.h
 *
 * @brief:	Error message definition.
 *
 * @vers:		1.0.0.0	09-2012
 *
 *=============================================================
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
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
*/
#ifndef TI_SMS_MOTOR_UART_API_H_
#define TI_SMS_MOTOR_UART_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "TI_SMS_Motor_General_Timer_API.h"

#define UART_PARITY_ENABLED 0x80
#define UART_PARITY_DISABLED 0x00
#define UART_PARITY_ODD 0x00
#define UART_8BIT_DATA 0x00
#define UART_1_STOP_BIT 0x00

// packet received form Master
#define ADDRESS_ID 0x00
#define COMMAND_BYTE_ADDRESS 0x01
#define DATA_BYTE_1 0x02
#define DATA_BYTE_2 0x03
#define SLAVE_PACKET_CRC 0x04
#define MASTER_PACKET_LENGTH 5

// Slave packet send to Master 
#define SLAVE_ADDRESS_BYTE 0x00
#define PWM_FREQ_BYTE_1 0x01
#define PWM_FREQ_BYTE_2 0x02
#define COMMUTATION_FREQ_BYTE_1 0x03
#define COMMUTATION_FREQ_BYTE_2 0x04
#define CURRENT_BYTE 0x05
#define LEAD_ANGLE_BYTE 0x06
#define TEMP_MEASUREMENT_BYTE 0x07
#define BAT_VOLATGE_MEASUREMENT_BYTE 0x08
#define MASTER_PACKET_CRC 0x09
#define SLAVE_PACKET_LENGTH 10

// this delay is used when we are sending data without interrupt
#define UART_TX_SEND_FLAG_SET_DELAY 5000 

// UART rx tx data buffer size
#define UART_RX_BUFFER_SIZE 20
#define UART_TX_BUFFER_SIZE 20

#define FIRST_MARKER_FOR_RX_PACKET 84    // T
#define SECOND_MARKER_FOR_RX_PACKET 73   // I
#define RX_PACKET_MAX_LENGTH 9
#define DISABLE_RX_UART_INTERRUPT 0xFE

typedef enum
{
	UART_RX_STATE_INIT_FIRST_MARKER = 0,
    UART_RX_SECOND_MARKER_RECEIVED = UART_RX_STATE_INIT_FIRST_MARKER + 1,
    UART_RX_PACKET_LENGHT_RECEIVED = UART_RX_SECOND_MARKER_RECEIVED + 1,
    UART_RX_PROCESS_DATA = UART_RX_PACKET_LENGHT_RECEIVED + 1,
    UART_RX_CHECK_CHECKSUM = UART_RX_PROCESS_DATA + 1
}UART_Rx_Receive_Status;

typedef struct { 
	UINT8  uartClk;
	UINT8  baudRateDivLSB; // baud rate UCAxBR0 value
	UINT8  baudRateDivMSB; // baud rate UCAxBR1 value
	UINT8  modControl; // modulation control UCAxMCTL 
    UINT8  dataBits;
    UINT8  stopBits;
    UINT8  paritySelect;
    UINT8  parityEnable;
    UINT8  rxIntEnable;
    UINT8  txIntEnable;
    UINT8  txComplete;  // set TRUE when all data bytes were transmitted
    UINT8  txBytesCount;  // total bytes need to tranmit
    UINT8  txBuffer[UART_TX_BUFFER_SIZE]; // this buffer need to fill for transmission
    UINT8  rxBytesCount;  // totaol bytes received so far
    UINT8  rxBuffer[UART_RX_BUFFER_SIZE]; // buffer filled during reception of data from UART
    UINT32 rxTimeOutCount;  // use for RX time out count      
    UART_Rx_Receive_Status rx_receive_packet_state;
    UINT8  rxPacketComplete; // set when we received valid packet
    UINT8  rxPacketStarted; // set when we received 1st valid packet byte
    UINT8  validPacketReceived; // set it to TRUE when valid data received
} UART_PARAMS_S;

UINT32 TI_SMS_MOTOR_UART_Init(UART_PARAMS_S *p); //called in system init
UINT32 TI_SMS_MOTOR_UART_Send_Data(UART_PARAMS_S *p); // use for sending data
UINT32 TI_SMS_MOTOR_UART_Parse_Data(void);
UINT32 TI_SMS_MOTOR_UARRT_Send_Data_To_Master(UART_PARAMS_S *p);
UINT32  TI_SMS_MOTOR_Process_Rx_Packet(UART_PARAMS_S *p);
// Enable disable UART Rx interrupt
void TI_SMS_MOTOR_Enable_Disable_UART_Rx_Interrupt(UART_PARAMS_S *p, UINT8 flag);
// init UART packet for start data reception
UINT32 TI_SMS_MOTOR_UARRT_Init_For_Packet(UART_PARAMS_S *p);

#endif /*TI_SMS_MOTOR_UART_API_H_*/

