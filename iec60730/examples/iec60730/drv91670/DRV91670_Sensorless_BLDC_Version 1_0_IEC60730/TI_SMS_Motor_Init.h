/*=============================================================
 * @file:		TI_SMS_Motor_Init.c
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

#ifndef TI_SMS_MOTOR_INIT_H_
#define TI_SMS_MOTOR_INIT_H_

#include "build.h"
#include "TI_SMS_Motor_Type_Def.h"
#include "TI_SMS_Motor_General_Timer_API.h"
#include "TI_SMS_Motor_PWM_API.h"
#include "TI_SMS_Motor_SPI_API.h"
#include "TI_SMS_Motor_ADC_API.h"
#include "TI_SMS_Motor_General_Timer_API.h"
#include "TI_SMS_Motor_BackEMF_API.h"
#include "TI_SMS_MOTOR_AlignGo_API.h"
//#include "TI_SMS_Motor_IPD_API.h"
#include "TI_SMS_Motor_Fault_API.h"
#include "TI_SMS_Motor_UserInPut_API.h"
#include "TI_SMS_Motor_UART_API.h"
#include "TI_SMS_Motor_PI_API.h"

#define MAXIMUM_TIME_TO_REACH_DESIRED_MOTOR_RPM 10000
#define MOTOR_SPEED_THRESHOLD 300
#define MAXIMUM_TIME_to_READ_MOTOR_SPEED 3000
#define PWMMAX_PWM_CHANNEL_NUM 6
#define START_INTERRUPT_DELAY 500

#define ADC_POLLING_MODE 1
#define ADC_INTERRUPT_MODE 2
#define ADC_PIPELINE_MODE 3 

#define ALIGN_DELAY 60000
#define PWM_TUNE_TABLE_LENGTH 21
typedef enum
{
	MOTOR_STATE_INITIALIZE = 0,
	MOTOR_STATE_CHECK_INPUTS = 1,
    MOTOR_STATE_START_UP = 2,
    MOTOR_STATE_RAMP_UP = 3,
	MOTOR_STATE_RUNNING = 4,
	MOTOR_STATE_BRAKING = 5,
	MOTOR_STATE_FAULT_STATE = 6,
	MOTOR_STATE_UNDERVOLTAGE_CHECK = 7,
	MOTOR_STATE_SETUP_MODE = 8,
	MOTOR_STATE_STOP = 9
}MOTOR_CONTROL_STATE; 

typedef enum
{
	NO_BRAKE = 0,
	BRAKE_25_PERECNT = NO_BRAKE + 1,
    BRAKE_50_PERCENT = BRAKE_25_PERECNT + 1,
    BRAKE_75_PERCENT = BRAKE_50_PERCENT + 1,
    BRAKE_100_PERCENT = BRAKE_75_PERCENT + 1
}MOTOR_BRAKING_SCHEME; 

typedef enum
{   
	ZERO_DEGREE_LED_ANGLE = 0,
	ONE_DEGREE_LED_ANGLE = ZERO_DEGREE_LED_ANGLE + 1,
	SEVEN_DEGREES_LEAD_ANGLE =  ONE_DEGREE_LED_ANGLE + 1,
    FIFTEEN_DEGREES_LEAD_ANGLE = SEVEN_DEGREES_LEAD_ANGLE + 1,
    THIRTY_DEGREES_LEAD_ANGLE = FIFTEEN_DEGREES_LEAD_ANGLE + 1
}MOTOR_LEAD_ANGLE; 

typedef enum
{
	PWM_FREQUENCY_8KHZ = 0,    // 0
	PWM_FREQUENCY_16KHZ =  PWM_FREQUENCY_8KHZ + 1, // 1
    PWM_FREQUENCY_32KHZ = PWM_FREQUENCY_16KHZ + 1, // 2
    PWM_FREQUENCY_12KHZ = PWM_FREQUENCY_32KHZ + 1, // 3
    PWM_FREQUENCY_20KHZ = PWM_FREQUENCY_12KHZ + 1, // 4
    PWM_FREQUENCY_24KHZ = PWM_FREQUENCY_20KHZ + 1, // 5
    PWM_FREQUENCY_28KHZ = PWM_FREQUENCY_24KHZ + 1  // 6
}MOTOR_PWM_FREQUENCY;

typedef enum
{
	SPEED_CONTROL_UART = 0,
	SPEED_CONTROL_PWM =  SPEED_CONTROL_UART + 1
} MOTOR_SPEED_CONTROL_TYPE;

typedef enum
{
	MOTOR_RUNNING_CW = 0,
	MOTOR_RUNNING_CCW =  MOTOR_RUNNING_CW + 1
} MOTOR_DIRECTION_TYPE;

typedef struct
{ 
	MOTOR_CONTROL_STATE currentState;
	MOTOR_CONTROL_STATE previousState;
	UINT16              motorSetSpeed;
 
} MOTOR_STATUS_S;

// use to initialize and calculate motor electrical speed 
typedef struct
{
    UINT8 speedReady;
    UINT8 stateCount;
    UINT8 multiplier; 
    UINT32 feedBackCount;
    UINT32 startTimerCount;
    UINT32 endTimerCount;
    UINT32 timerInterval; 	
} MOTOR_ELECTRICAL_SPEED;

// use to initialize user required parameters 
typedef struct
{
    UINT16 commutationLeadAngle;
    MOTOR_BRAKING_SCHEME brakingMethod;
    MOTOR_PWM_FREQUENCY pwmFrequency;
    UINT16 commutaionFreq;
    UINT16 batteryVolatge;    // battery voltage
    UINT16 temperatureReading; // temperature sensor value
    UINT16 dutyCycleIncrementValue; // use for duty cycle increment-decrement
    UINT16 motorpoles;  // use to calculate motor mechanical speed
    MOTOR_DIRECTION_TYPE motorDirection; // if set to 0 its CW direction, if 1 its CCW
    UINT16 hallSensorMode; // its for selecting different hall sensor sequences
    UINT16 motorStartStopFlag; // flag set when motor need to start
    UINT16 motorMaxSpeedFromGUI; // max speed received from GUI
    UINT16 motorCurrentSpeedDailPercent; // GUI dial percentage value
    UINT16 motorMaxVoltageSetting; // max default voltage
    UINT16 motorMaxCurrentSetting; // max default current
    UINT16 motorControllerMode; // use for Motor controller selection
} MOTOR_CONTROLLER_CONFIG;

// use to initialize motor ramp up ramp down curve parameters
typedef struct
{
    UINT16 rampUpValue; // its for motor ramp up
    UINT16 rampDownValue; // its for motor ramp down
    UINT16 rampPWMDutyCycle; // used as low pass filter to smooth pwmOutPutDutyCycle
    UINT16 PWMMinDutyCycle; // minimum PWM duty cycle value
    UINT16 PWMMaxDutyCycle; // maximum PWM duty cycle value
} MOTOR_RAMP_CONFIG;

// use to initialize sensorless motor startup code
typedef struct
{
    UINT16 motor_align_phase; // its motor align phase for sensorless mode
    UINT16 zerocorss_det_window; // use to this delay to detect zero cross
    UINT16 realign_window; // use this delay for next realign of motor phase
    UINT16 sensorless_startup_dutycycle; // use for starting motor in sensorless mode
} MOTOR_SENSORLESS_STARTUP_CONFIG;

typedef struct
{
    unsigned int errorDetected : 1;    // bit0
    unsigned int revesreRotationError : 1;    // bit1
    unsigned int underVoltageError : 1;    // bit2
    unsigned int overVoltageError : 1;    // bit3
    unsigned int overCurrentError : 1;   // bit5
    unsigned int motorUnderSpeedError : 1;    // bit6
    unsigned int motorOverSpeedError : 1;    // bit7
    unsigned int motorStartUpError : 1;    // bit8
    unsigned int motorGUIConnectionError : 1;    //  bit9
    unsigned int motorSpeedFeedbackError : 1;    // bit10
    unsigned int motorOverTempError : 1;    //bit11
}CONTROLLER_ERROR_FLAGS;

typedef enum
{
    ERROR_CON_DETECTED = 0x01,    // bit1
    REV_ROTATION_ERROR = 0x01 << 1,    // bit1
	UND_VOLTAGE_ERROR = 0x01 << 2,    // bit2
	OV_VOLTAGE_ERROR = 0x01 << 3,    // bit3
	OV_CUR_ERROR = 0x01 << 4, // bit5
	MOTOR_UND_SPEED_ERROR = 0x01 << 5,    // bit6
	MOTOR_OV_SPEED_ERROR = 0x01 << 6,    // bit7
	MOTOR_STARTUP_ERROR = 0x01 << 7,    // bit8
	MOTOR_GUI_CONN_ERROR = 0x01 << 8,
	MOTOR_SPEED_FEEDB_ERROR = 0x01 << 9,    // bit10
	MOTOR_OV_TEMP_ERROR = 0x01 << 10    //bit12
}CONTROLLER_ERROR_STATUS;

typedef struct
{
	UINT16 loadCurrentValue;    // use for motor load current
	UINT16 motorVCCValue;    // use for motor VCC
	UINT16 controllerErorFlags;    // use for motor controller related errors
}CONTROLLER_CUR_VOL_ERROR_FLAGS;

UINT32 motor_sys_init(void);
UINT32 Calculate_Motor_Electrical_Speed(MOTOR_ELECTRICAL_SPEED *p);
void Motor_Enable_Disable_Hall_ZeroCross_Interrupt(unsigned char flag);
void Port2_Enable_Disable_Pre_Driver(UINT8 enable_output, UINT8 skipingPWM);

#ifdef SYSTEM_DEBUG
// use to turn port4 bit 0 led off-on
void Port4_Bit_0_Init_For_Debugging(void);
void Port_4_Bit_0_Toggle(unsigned char toggle);
// use to turn port1 bit 4 led off-on
void Port1_Bit_4_Init_For_Debugging (void);
void Port_1_Bit_4_Toggle(UINT8 toggle);
void Port4_Bit_1_Init_For_Debugging(void);
#endif

UINT32 TI_SMS_Motor_Beep(UINT8 fetPairSel, UINT32 onTime, UINT32 offTime, UINT16 period);
UINT32 Init_Motor_Sensorless_Related_Variables(MOTOR_CONTROLLER_CONFIG* config);
void Motor_Control_Init_Check_And_Detect_Motor_Direction(void);
void Motor_Control_Init_Start_Timer_A2_For_Commutation_Delay(void);
// initialize all PWM period and duty cycle related struct with period percentage
UINT32 Init_Motor_PWM_Period_Related_Variables(void);
// return motor state machine current state
MOTOR_STATUS_S TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State(void);
// returns pointer to current motor config
MOTOR_CONTROLLER_CONFIG* TI_SMS_Init_Get_Motot_Config(void);
// use to turn port4 bit 6 power on LED
void Port1_Bit_6_Init_For_Debugging(void);
// use to turn port1 bit 6 led off-on
void Port_1_Bit_6_Toggle(unsigned char toggle);
void Port1_Bit_5_Init_For_Debugging(void);
void Port_1_Bit_5_Toggle(unsigned char toggle);
void Port1_Bit_2_Init_For_Debugging(void);

#endif /*TI_SMS_MOTOR_SYS_H_*/
