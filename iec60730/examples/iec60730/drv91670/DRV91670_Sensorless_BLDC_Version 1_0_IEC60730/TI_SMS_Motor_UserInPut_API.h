/*=============================================================
 * @file:		TI_SMS_Motor_UserInPut_API.h
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

#ifndef TI_SMS_MOTOR_USERINPUT_API_H_
#define TI_SMS_MOTOR_USERINPUT_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "TI_SMS_Motor_PWM_API.h"
#include "TI_SMS_Motor_UART_API.h"

#define COUNTS_NEEDED_TO_SW_BW_START_STOP 20
#define PWM_IN_PERIOD_LOW_LIMIT      ((UINT32)13667840) // is in nsec
#define PWM_IN_PERIOD_HIGH_LIMIT     ((UINT32)26946568) // is in nsec
#define PWM_IN_DUTYCYCLE_LOW_LIMIT   ((UINT32)700000)     // is in nsec
#define PWM_IN_DUTYCYCLE_HIGH_LIMIT ((UINT32)2397440)    // is in nsec 

#define PWM_IN_SETUP_MODE_HIGH_LIMIT ((UINT32)2397440)    // is in nsec 
#define PWM_IN_SETUP_MODE_LOW_LIMIT  ((UINT32)2000000)     // is in nsec
#define PWM_IN_DUTYCYCLE_MULTIPLIER   ((UINT32)6274)
#define PWM_IN_DUTYCYCLE_SPEED_LOW_LIMIT  ((UINT32) 1000000)    // is in nsec
#define PWM_IN_DUTYCYCLE_SPEED_HIGH_LIMIT ((UINT32) 2000000)    // is in nsec 

#define PWM_IN_1MS ((UINT32) 1000000)    // is in nsec
#define PWM_IN_2MS ((UINT32) 2000000)    // is in nsec

#define PWM_IN_DUTYCYCLE_STANDARED_RANGE_NS   ((UINT32) 1000000)
#define PWM_IN_DUTYCYCLE_SPEED_LOW_LIMIT_FOR_MOTOR_STOP ((UINT32) 1200000)    //1200000

typedef struct { 
	             UINT32 minMotorSetSpeed;
	             UINT32 maxMotorSetSpeed;
	             UINT32 currentMotorSetSpeed;
	             UINT32 motorStartThreshold;   // set TRUE will enable PWM pulse skiping.
	             UINT16 deadBandCount;         // enabled over current interrupt
                 UINT8  motorStartStop;
                 UINT8  motorPowerUp;          // flag set when 1st time powerup     
                 UINT16 pwmOutPutDutyCycle;    // pwm output duty cycle w.r.t pwm pulse in duty cycle
                } MOTOR_USERINPUT_PARAMS_S;

typedef struct { 
	             //float dutyCycleRangeFactor;
	             UINT16 dutyCycleRangeFactor;
	             UINT32 pwmPulsePeriodInNSec;      // incoming PWM pulse in nano seconds
                 UINT32 pwmPulseDutyCycleNSec;     // incoming PWM pulse in nano seconds  
                 UINT32 pwmPulseScaledDutyCycleNSec;     // scaled output
	             UINT32 dutyCycleLowLimit;
                 UINT32 dutyCycleHigLimit;
                 UINT16 pwmPulseInDutyCyle;        // PWM pulse in duty cycle
	             UINT16 pwmPulseInDutyCycleCount;  // PWM pulse in duty cycle in terms of counter counts
	             UINT16 pwmPulseInPeriodCount;     // PWM pulse in period in terms of counter counts
	             UINT16 pwmPulseInDataReady;       // set when PWM pulse in data ready             
                 UINT8  pwmPulseInError;           // set when PWM pulse in erorr occours
                 UINT8  rescalingEnabled;          // set when rescaling   
                 UINT8  usePWMPulseIn;             // set when PWM Pulse in use for speed control esle UART   
                } MOTOR_USERINPUT_PWM_PARAMS_S;
               
UINT32 TI_SMS_MOTOR_UserInPut_Init(MOTOR_USERINPUT_PARAMS_S *p); //called in system init
UINT32 TI_SMS_MOTOR_UserInPut_PWM_SpeedInput_Init(void); // use to init PWM SpeedInPut
UINT8 TI_SMS_MOTOR_UserInPut_PWM_Period_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p); 
UINT8 TI_SMS_MOTOR_UserInPut_PWM_DutyCycle_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p); 
UINT8 TI_SMS_MOTOR_UserInPut_PWM_SetUpMode_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p); 
UINT32 TI_SMS_MOTOR_UserInPut_UpdateMotorSpeed(MOTOR_USERINPUT_PARAMS_S *p); //use to check is motor need to start of stop
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_Controller_Address(void);
UINT8 TI_SMS_MOTOR_UserInPut_Get_Battery_Voltage(void);
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_Current(void);
UINT8 TI_SMS_MOTOR_UserInPut_Get_Board_Temperature(void);
UINT16 TI_SMS_MOTOR_UserInPut_Get_Motor_Commutation_Frequency(void);
UINT16 TI_SMS_MOTOR_UserInPut_Get_Motor_PWM_Frequency(void);
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_LeadAngle(void);
UINT32 TI_SMS_MOTOR_UserInput_PWM_Rescaling(MOTOR_USERINPUT_PWM_PARAMS_S *p);
//use to check is motor need to start of stop and get user set speed
UINT32 TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(MOTOR_USERINPUT_PARAMS_S *p);

#endif /*TI_SMS_Motor_UserInPut_API.h*/

