/*=============================================================
 * @file:		TI_SMS_Motor_General_Timer_API.h
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

#ifndef TI_SMS_MOTOR_GENERAL_TIMER_API_H_
#define TI_SMS_MOTOR_GENERAL_TIMER_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "DRV91620.h"
#include "TI_SMS_Motor_Init.h"
#include "TI_SMS_Motor_UserInPut_API.h"

typedef enum TIMERA_ID
{   TimerA0 = 0, 
    TimerA1 = 1,
	TimerA2 = 2,
	TimerA3 = 3,
	TimerA4 = 4
}	 TIMERA_ID;

typedef struct TIMERA_PARAM_S
{  
	//volatile struct TIMERA_REGS *regsAddr;
  /* TIMERA_REGS *regsAddr;*/
   TIMERA_ID timerA_ID;
   UINT16 cpuFreqencyDivid;
   UINT16 periodCount;
   UINT16 TimerCount;
   UINT16 ISRCount;
   UINT16 clockDivided;
} TIMERA_RARAM_S;

typedef enum 
{
	MotorStateAgoIdle,
	MotorStateAgo2on,								/*!< Engergizing two phases to center motor between them */
	MotorStateAgoBlanking,							/*!< Blanking time before hunting for comparator states */
	MotorStateAgoWaitComp,							/*!< Waiting for first comparator transition after turning 1 phase on */
	MotorStateAgoBemfBlanking,						/*!< Blanking time before handing over to BEMF */
    MotorStartUpDone                                /*!< Motor startup done, handing over to ISR based commutation */
}MotorState;

typedef enum AUTOCOMSTATE
{
    INIT_AND_COMMUTATION_START_TIMER = 0,              /*!< Init and do 1st commutation */          
    WAITFORZC1 = INIT_AND_COMMUTATION_START_TIMER + 1, /*!< Wait for zerocross */
    COMMUTATION = WAITFORZC1 + 1,                      /*!< Do commuataion */
    WAITFORZC2 =  COMMUTATION  + 1,                    /*!< Wait for zero cross */
    BLANKINGFORZC = WAITFORZC2 + 1                     /*!< Blanking for zero cross */
} AUTOCOMSTATE;

UINT32 TI_SMS_MOTOR_TIMERA_Init(TIMERA_RARAM_S *p);
 
UINT32 TI_SMS_MOTOR_TIMER_Read(TIMERA_RARAM_S *p);

UINT32 TI_SMS_MOTOR_TIMER_Start(TIMERA_RARAM_S *p);

UINT32 TI_SMS_MOTOR_TIMER_Stop(TIMERA_RARAM_S *p);   

void DelayCounts (UINT32 delay);
void DoMotorCommutationAsPerZerCross (UINT8 zeroCrossComparatorValue, UINT8 delayFlag);
void Motor_Control_Init_Start_Timer_A4_For_Commutation_Delay (UINT16 countValue);
void Motor_Control_Init_Start_Timer_A1_For_AlignAndGo (UINT16 countValue);
void InitTimer1Relatedvariables (void);
void SetCommuatationTimerStateMachine_Force_Commutation (AUTOCOMSTATE autoComState);
void SetCommuatationTimerStateMachine (MotorState alignstatevalue);
#endif /*TI_SMS_MOTOR_GENERAL_TIMER_API_H_*/
