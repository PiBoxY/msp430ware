/*=============================================================
 * @file:		TI_SMS_Motor_AlignGo_API.h
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

#ifndef TI_SMS_MOTOR_ALIGNNGO_API_H_
#define TI_SMS_MOTOR_ALIGNNGO_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "TI_SMS_Motor_PWM_API.h"
#include "TI_SMS_Motor_General_Timer_API.h"

typedef struct {  
     	UINT8   rotorPosition; // return value after go runs
     	UINT32  alignDelay;
     	UINT16  alignDutyCycle;
     	UINT16  goDelay;
     	UINT8   goLoops;
     	UINT16   goRamp; // go time decrental factor...
     	UINT16  goDutyCycle;
        const UINT16 *comSequence;
     	UINT8 CallAlignStateMachine; // set when we are in align and go state
     	UINT8 align_comm_index; // this saved last valid align index
       } AlignnGo_PARAM_S;

#endif /*TI_SMS_MOTOR_ALIGNNGO_API_H_*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// end of go routine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

