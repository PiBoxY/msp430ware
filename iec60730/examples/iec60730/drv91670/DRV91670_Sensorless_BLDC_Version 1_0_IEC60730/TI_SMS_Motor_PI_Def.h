/*=============================================================
 * @file:		TI_SMS_Motor_PI_Def.h
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

#ifndef TI_SMS_MOTOR_PI_DEF_H_
#define TI_SMS_MOTOR_PI_DEF_H_
#include "TI_SMS_Motor_Com_Def.h"

 typedef struct {  
 	INT16 Ref;   			// Input: Reference input 
    INT16 Fdb;   			// Input: Feedback input 
	SINT32 Err;				// Variable: Error
	UINT16 Kp;				// Parameter: Proportional gain
	SINT32 Up;				// Variable: Proportional output 
	UINT16 Ki;			    // Parameter: Integral gain
	SINT32 Ui;				// Variable: Integral output 
    SINT32 OutPreSat; 		// Variable: Pre-saturated output
	SINT32 OutMax;		    // Parameter: Maximum output limit
	SINT32 OutMin;	    	// Parameter: Minimum output limit
	SINT32 Out;   			// Output: PID output 
	SINT32 Up1;		   	    // History: Previous proportional output
	SINT32 Uip;				// Variable: Previous Integral output 
} PI_PARAM_S_NEW;	  
  
#define PI_INTEGRAL_INC_DEC_CONSTANT 10    // 5
       
#endif /*TI_SMS_MOTOR_PI_DEF_H_*/
