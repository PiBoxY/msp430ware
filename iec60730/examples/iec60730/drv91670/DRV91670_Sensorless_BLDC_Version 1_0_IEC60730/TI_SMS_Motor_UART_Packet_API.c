/*=============================================================
 * @file:		TI_SMS_Motor_UART_Packet_API.c
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
#include "TI_SMS_Motor_UART_Packet_API.h"
#endif
#include "TI_SMS_Motor_General_Timer_API.h"
#ifndef TI_SMS_MOTOR_INIT_H_
#include "TI_SMS_Motor_Init.h"
#endif

extern TIMERA_RARAM_S timerA4;
extern PI_PARAM_S_NEW speedPID;
extern PWM_PARAM_S pwm3Phase; // Create an instance of the PWM module to driver U_H, U_L, V_H, V_L, W_H, W_L
extern PI_PARAM_S_NEW speedPID;
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // user specific config
extern MOTOR_RAMP_CONFIG motorRampUpDownConfig; // motor speed ramp up ramp down add on value
extern MOTOR_SENSORLESS_STARTUP_CONFIG motorSensorlessStartUp; // use for motor sensorless startup
extern MOTOR_USERINPUT_PARAMS_S userSpeedInput;
extern CONTROLLER_CUR_VOL_ERROR_FLAGS motorConCurVoltError;
extern UINT16 PI_output;
extern MOTOR_STATUS_S motorState; //Create an instance of the motor state machine

void TI_SMS_MOTOR_Send_UART_Packet_Response (UART_PARAMS_S *p)
{
    volatile UINT8 sendPacket = FALSE;
    volatile UINT8 sendWriteAckPacket = FALSE;
    volatile UINT8 sendGraphAckPacket = FALSE;
    volatile UINT16 totalCheckSum = 0;
	volatile float temp_value;

    MOTOR_STATUS_S temp_state;

    TI_SMS_MOTOR_TIMER_Read(&timerA4);

	if ((p->rxPacketComplete == TRUE) &&
	    ((timerA4.ISRCount - p->rxTimeOutCount) < UART_RX_PACKET_COMPLETION_TIMEOUT))
    {
        switch (p->rxBuffer[PACKET_FUNC_ADD])
        {
        	case WRITE_STATUS:
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_SPEED_MAX:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		    if (totalCheckSum <= MOTOR_MAX_SPEED_FROM_GUI &&
        		        totalCheckSum >= MOTOR_MIN_SPEED_FROM_GUI)
        		        userControllerConfig.motorMaxSpeedFromGUI = totalCheckSum;
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_CURRENT_MAX:
    			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
   		        userControllerConfig.motorMaxCurrentSetting = totalCheckSum;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_VOLTAGE_MAX:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		userControllerConfig.motorMaxVoltageSetting = totalCheckSum;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_POLE_COUNT:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		    if (totalCheckSum > MIN_POLE_COUNT_LIMIT &&
        		        totalCheckSum <= MAX_POLE_COUNT_LIMIT)
        		        userControllerConfig.motorpoles = totalCheckSum;
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_SPEED:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum <= GUI_DIAL_MAX_PERCENATGE)
        		    userControllerConfig.motorCurrentSpeedDailPercent = totalCheckSum;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_BRAKE:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			if (totalCheckSum <= MAX_BRAKE_PERCENTAGE_LIMIT)
        			{
        				if (totalCheckSum >= BRAKE_75_PERCENT_SELECTED)
        				    userControllerConfig.brakingMethod = BRAKE_75_PERCENT;
        				else if (totalCheckSum >= BRAKE_50_PERCENT_SELECTED)
                        	userControllerConfig.brakingMethod = BRAKE_50_PERCENT;
                        else if ((totalCheckSum >= 5) && (totalCheckSum <= BRAKE_25_PERCENT_SELECTED))
                        	userControllerConfig.brakingMethod = BRAKE_25_PERECNT;
                        else
                        	userControllerConfig.brakingMethod = NO_BRAKE;
        			}
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_PWM_FREQ:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			if (totalCheckSum <= 6)
        			    userControllerConfig.pwmFrequency = totalCheckSum & 0xFF;
        		}
        		sendWriteAckPacket = TRUE;
        		 break;
        	case WRITE_HALL_SENSOR:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			if (totalCheckSum <= HALL_SENSOR_MODE_D)
        			    userControllerConfig.hallSensorMode = totalCheckSum & 0xFF;
        		}
        		sendWriteAckPacket = TRUE;
        		 break;
        	case WRITE_DUTY_CYCLE:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
       				motorSensorlessStartUp.sensorless_startup_dutycycle = totalCheckSum;
        		}
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_ALIGN_PHASE:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			if (totalCheckSum >= MIN_ALIGN_PHASE_NUMBER &&
        			    totalCheckSum <= MAX_ALIGN_PHASE_NUMBER)
        			{
        				totalCheckSum = totalCheckSum - 1;
        				motorSensorlessStartUp.motor_align_phase = totalCheckSum & 0xFF;
        			}
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_ZERO_CROSS_WINDOW:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			motorSensorlessStartUp.zerocorss_det_window = ZERO_CROSS_WINDOW_MIN_TIME + (totalCheckSum*550);
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_REALIGN_WINDOW:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        			motorSensorlessStartUp.realign_window = REALIGN_TIMER_DELAY_MIN_COUNT + (50 * totalCheckSum);
        		}
        		sendWriteAckPacket = TRUE;
        		break;

        	case WRITE_RAMPUP:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum >= FAST_SPEED_RAMP_SLOPE_VALUE &&
        		    totalCheckSum <= SLOW_SPEED_RAMP_SLOPE_VALUE)
        			motorRampUpDownConfig.rampUpValue = totalCheckSum & 0xFF;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_RAMPDOWN:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum >= FAST_SPEED_RAMP_SLOPE_VALUE &&
        		    totalCheckSum <= SLOW_SPEED_RAMP_SLOPE_VALUE)
        			motorRampUpDownConfig.rampDownValue = totalCheckSum & 0xFF;
        		sendWriteAckPacket = TRUE;
                break;
        	case WRITE_LEAD_ANGLE:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum >= LEAD_ANGLE_MIN_VLAUE &&
        		    totalCheckSum <= LEAD_ANGLE_MAX_VLAUE)
        		    userControllerConfig.commutationLeadAngle = totalCheckSum & 0xFF;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_SPEED_KP:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum >= SPEED_KP_MIN_VALUE &&
        		    totalCheckSum <= SPEED_KP_MAX_VALUE)
        		    speedPID.Kp = totalCheckSum & 0xFF;
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_SPEED_KI:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if (totalCheckSum >= SPEED_KI_MIN_VALUE &&
        		    totalCheckSum <= SPEED_KI_MAX_VALUE)
        		    speedPID.Ki = totalCheckSum & 0xFF;
                sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_CURRENT_KP:
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_CURRENT_KI:
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_DIRECTION:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		    if ((totalCheckSum <= 1))
        		    	userControllerConfig.motorDirection = totalCheckSum & 0xFF;
        		}
        		sendWriteAckPacket = TRUE;
        		break;
        	case WRITE_START:
        		temp_state = TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State();
        		if (temp_state.currentState == MOTOR_STATE_STOP)
        		{
        			totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		    if ((totalCheckSum == MOTOR_START_FLAG))
        		    	userControllerConfig.motorStartStopFlag = totalCheckSum & 0xFF;
        		}
        		sendWriteAckPacket = TRUE;
        	    break;
        	case WRITE_STOP:
        		totalCheckSum = p->rxBuffer[PACKET_DATA_LSB_ADD] + p->rxBuffer[PACKET_DATA_MSB_ADD] * 256 ;
        		if ((totalCheckSum == MOTOR_STOP_FLAG))
        			userControllerConfig.motorStartStopFlag = totalCheckSum & 0xFF;
        		sendWriteAckPacket = TRUE;
                break;
        	case READ_STATUS:
        	    p->rxBuffer[PACKET_DATA_LSB_ADD]= 0x00;
        	    p->rxBuffer[PACKET_DATA_MSB_ADD]= 0xF2;
        	    sendPacket = TRUE;
        	    break;
        	case READ_SPEED:
            	totalCheckSum = speedPID.Fdb;
        		p->rxBuffer[PACKET_DATA_LSB_ADD]= totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	p->rxBuffer[PACKET_DATA_MSB_ADD]= totalCheckSum;
        	    sendPacket = TRUE;
        	    break;
        	case READ_CURRENT:
        	    p->rxBuffer[PACKET_DATA_LSB_ADD]= p->rxBuffer[PACKET_DATA_LSB_ADD] + 1;
        	    p->rxBuffer[PACKET_DATA_MSB_ADD]= 0x03;
        	    sendPacket = TRUE;
        	    break;
        	case READ_POWEER:
        	    p->rxBuffer[PACKET_DATA_LSB_ADD]= p->rxBuffer[PACKET_DATA_LSB_ADD] + 1;
        	    p->rxBuffer[PACKET_DATA_MSB_ADD]= 0x04;
        	    sendPacket = TRUE;
        	    break;
        	case READ_DEVEICE_NAME:
                // fill LSB
        		totalCheckSum = TORQZ_FLASH_DEVICE_ID;
        		p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
        		totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
        		// fill MSB
        		p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
        	    sendPacket = TRUE;
        	    break;
            case READ_PWM_FREQ:
            	totalCheckSum = pwm3Phase.Period;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
        	    break;
            case READ_HALL_SENSOR:
            	// Its only 8 bit number
            	totalCheckSum = (userControllerConfig.hallSensorMode) & 0xFF;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  0x00;
        	    sendPacket = TRUE;
        	    break;
            case READ_DUTY_CYCLE:
        		totalCheckSum = motorSensorlessStartUp.sensorless_startup_dutycycle;
        		p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
        		totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
        		// fill MSB
        		p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
        	    sendPacket = TRUE;
        	    break;

            case READ_ALIGN_PHASE:
                totalCheckSum = 1 + motorSensorlessStartUp.motor_align_phase ;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] = totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
            	break;
            case READ_ZERO_CROSS_WINDOW:
                totalCheckSum = 0;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
            case READ_REALIGN_WINDOW:
                totalCheckSum = 0;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
            case READ_RAMPUP:
            	totalCheckSum = (motorRampUpDownConfig.rampUpValue) & 0xFF;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
                break;
            case READ_RAMPDOWN:
            	totalCheckSum = (motorRampUpDownConfig.rampDownValue) & 0xFF;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
                break;
            case READ_LEAD_ANGLE:
            	totalCheckSum = userControllerConfig.commutationLeadAngle;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
        	    break;
            case READ_SPEED_KP:
            	totalCheckSum = speedPID.Kp;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
        	    sendPacket = TRUE;
        	    break;
            case READ_SPEED_KI:
            	totalCheckSum = speedPID.Ki;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
        	    break;
            case READ_CURRENT_KP:
        	    p->rxBuffer[PACKET_DATA_LSB_ADD]= 0x00;
        	    p->rxBuffer[PACKET_DATA_MSB_ADD]= 0x00;
        	    sendPacket = TRUE;
        	    break;
            case READ_CURRENT_KI:
        	    p->rxBuffer[PACKET_DATA_LSB_ADD]= 0x00;
        	    p->rxBuffer[PACKET_DATA_MSB_ADD]= 0x00;
        	    sendPacket = TRUE;
        	    break;
            case READ_GRAPH_SPEED_CURRENT_VOLTAGE:
            	// fill LSB
            	// totalCheckSum=userControllerConfig.motorCurrentSpeedDailPercent;
            	totalCheckSum = motorConCurVoltError.motorVCCValue;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum;
            	// fill LSB
            	totalCheckSum = motorConCurVoltError.loadCurrentValue;
            	//  totalCheckSum=pwm3Phase.Period;
        	    // totalCheckSum = speedPID.Out; 
            	
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 1] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 2] =  totalCheckSum;
            	// fill LSB
            	//totalCheckSum = MotorSpeedFeedBack;
            	totalCheckSum = speedPID.Fdb;
            	//totalCheckSum = speedPID.Ref;
            	//totalCheckSum=pwm3Phase.Period;
            	//totalCheckSum = speedPID.Ref;
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 3] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 4] =  totalCheckSum;
                // PWM frequency
            	totalCheckSum = pwm3Phase.Period;
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 5] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 6] =  totalCheckSum;
            	// PWM dutycycle
            	totalCheckSum = pwm3Phase.DutyCycle[PWM_W_LOW_SIDE];
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 7] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD + 8] =  totalCheckSum;
            	p->rxBytesCount = 17;
            	p->rxBuffer[PACKET_LENGTH_ADD] = 17;
            	sendPacket = TRUE;
            	break;
            case READ_POLE_COUNT:
            	totalCheckSum = userControllerConfig.motorpoles;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  0x00;
            	sendPacket = TRUE;
            	break;
            case READ_CURRENT_MAX:
            	totalCheckSum = userControllerConfig.motorMaxCurrentSetting;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
                break;
            case READ_VOLTAGE_MAX:
            	totalCheckSum = userControllerConfig.motorMaxVoltageSetting;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
                break;
            case READ_SPEED_MAX:
            	totalCheckSum = userControllerConfig.motorMaxSpeedFromGUI;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  totalCheckSum & 0xFF;
            	sendPacket = TRUE;
                break;
            case READ_CONTROLLER_TYPE:
            	totalCheckSum = userControllerConfig.motorControllerMode;
            	p->rxBuffer[PACKET_DATA_LSB_ADD] =  totalCheckSum & 0xFF;
            	totalCheckSum = (totalCheckSum >> 8 ) & 0xFF;
            	// fill MSB
            	p->rxBuffer[PACKET_DATA_MSB_ADD] =  0x00;
            	sendPacket = TRUE;
            	break;

            default:
                break;
        }
        // response for read packet received from master
        if (sendPacket)
        {
        	p->validPacketReceived = TRUE;

	    	for (sendPacket = 0; sendPacket < (p->rxBytesCount - 2); sendPacket++)
	    		p->txBuffer[sendPacket] = p->rxBuffer[sendPacket];

        	p->txBytesCount = p->rxBytesCount;

        	totalCheckSum = 0;
        	// calculate checksum
        	for (sendPacket = 0; sendPacket < (p->txBytesCount - 2); sendPacket++)
        	    totalCheckSum = totalCheckSum + p->txBuffer[sendPacket];

        	// move LSB checksum byte
        	p->txBuffer[p->txBytesCount - 2] = totalCheckSum & 0xFF;
        	// move MSB checksum byte
        	p->txBuffer[p->txBytesCount - 1] = (totalCheckSum >> 8) & 0xFF;
        	TI_SMS_MOTOR_UART_Send_Data(p);
        }
        // response for write packet received from master
        if (sendWriteAckPacket)
        {
            p->txBuffer[0] = p->rxBuffer[PACKET_TAB_ADD];
            p->txBuffer[1] = p->rxBuffer[PACKET_FUNC_ADD];
        	p->txBytesCount = 2;
        	TI_SMS_MOTOR_UART_Send_Data(p);
        }
        // make UART RX  ready for next packet
        p->rxPacketComplete = FALSE;
    	p->rxPacketStarted = FALSE;
    	p->rxBytesCount = 0;
    	p->rx_receive_packet_state = UART_RX_STATE_INIT_FIRST_MARKER;
    	TI_SMS_MOTOR_Enable_Disable_UART_Rx_Interrupt(p, TRUE);
    }
}


