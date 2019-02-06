/*=============================================================
 * @file:		TI_SMS_Motor_UART_PACKET_API.h
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

#ifndef TI_SMS_MOTOR_UART_PACKET_API_H_
#define TI_SMS_MOTOR_UART_PACKET_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#ifndef TI_SMS_MOTOR_UART_API_H_
#include "TI_SMS_Motor_UART_API.h"
#endif

#define PACKET_HEADER_H1_ADD 0x00
#define PACKET_HEADER_H2_ADD 0x01
#define PACKET_LENGTH_ADD 0x02
#define PACKET_TAB_ADD 0x03
#define PACKET_FUNC_ADD 0x04
#define PACKET_DATA_LSB_ADD 0x05
#define PACKET_DATA_MSB_ADD 0x06
#define PACKET_CHECKSUM_LSB 0x07
#define PACKET_CHECKSUM_MSB 0x08
#define UART_RX_PACKET_COMPLETION_TIMEOUT 40

typedef enum
{
	WRITE_STATUS = 1,
	WRITE_SPEED_MAX = 2,
	WRITE_CURRENT_MAX = 3,
    WRITE_VOLTAGE_MAX = 4,
    WRITE_POLE_COUNT = 5,
    WRITE_SPEED = 6,
    WRITE_BRAKE = 7,
    WRITE_PWM_FREQ = 8,
    WRITE_HALL_SENSOR = 9,
    WRITE_DUTY_CYCLE = 10,
    WRITE_ALIGN_PHASE = 11,
    WRITE_ZERO_CROSS_WINDOW = 12,
    WRITE_REALIGN_WINDOW = 13,
    WRITE_RAMPUP  = 14,
    WRITE_RAMPDOWN = 15,

    WRITE_LEAD_ANGLE = 16,
    WRITE_SPEED_KP = 17,
    WRITE_SPEED_KI = 18,
    WRITE_CURRENT_KP = 19,
    WRITE_CURRENT_KI = 20,
    WRITE_DIRECTION = 21,
    WRITE_START = 22,
    WRITE_STOP = 23,
    WRITE_CONTROLLER_TYPE = 24,

    READ_STATUS = 127,
    READ_SPEED = 128,
    READ_CURRENT = 129,
    READ_POWEER = 130,
    READ_DEVEICE_NAME = 131,
    READ_PWM_FREQ = 132,
    READ_HALL_SENSOR = 133,
    READ_DUTY_CYCLE = 134,
    READ_ALIGN_PHASE = 135,
    READ_ZERO_CROSS_WINDOW = 136,
    READ_REALIGN_WINDOW = 137,
    READ_RAMPUP = 138,
    READ_RAMPDOWN = 139,
    READ_LEAD_ANGLE = 140,
    READ_SPEED_KP = 141,
    READ_SPEED_KI = 142,
    READ_CURRENT_KP = 143,
    READ_CURRENT_KI = 144,
    READ_GRAPH_SPEED_CURRENT_VOLTAGE = 145,
    READ_POLE_COUNT = 146,
	READ_CURRENT_MAX = 147,
    READ_VOLTAGE_MAX = 148,
    READ_SPEED_MAX = 149,
    READ_CONTROLLER_TYPE = 150
}CommFunctionCode;

void TI_SMS_MOTOR_Send_UART_Packet_Response (UART_PARAMS_S *p);

#endif /*TI_SMS_MOTOR_UART_API_H_*/

