/*=============================================================
 * @file:		TI_SMS_Motor_BackEMP_API.h
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

#ifndef TI_SMS_MOTOR_BACKEMF_API_H_
#define TI_SMS_MOTOR_BACKEMF_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "TI_SMS_Motor_PWM_API.h"

typedef struct
{ 
  UINT16 currentBackEMFState;
  UINT16 previouBackEMFState;
  UINT16 comutationLeadAngleFlag; 
  UINT16 startBEMFSensing; 
  UINT16 leadAngleDelay;	  
  UINT16 zeroCrossTimerInterval;
  UINT16 *leadAngleTable;	
  UINT32 comutationStateCount;
  UINT16 zeroCrossIntrruptCount;
  UINT8  lastStateSaved;
  UINT8  leadAngleDelayCompleted;
  UINT32 sensorlessCommStartTimerCount;
  UINT32 sensorlessCommEndTimerCount;
  UINT32 sensorlessCommTimerInterval; 
} BEMF_PARAM_S;


UINT32 TI_SMS_MOTOR_BEMF_Init(BEMF_PARAM_S *p); 
UINT32 TI_SMS_MOTOR_BEMF_Update_Mode(void);  // this function will be called in current ISR

#endif /*TI_SMS_MOTOR_BACKEMF_API_H_*/
