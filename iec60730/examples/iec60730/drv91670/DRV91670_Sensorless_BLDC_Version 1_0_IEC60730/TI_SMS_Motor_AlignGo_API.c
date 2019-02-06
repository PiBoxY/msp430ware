/*=============================================================
 * @file:		TI_SMS_Motor_AlignGo_API.c
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

#include "TI_SMS_Motor_AlignGo_API.h"
extern PWM_PARAM_S pwm3Phase; // Create an instance of the PWM module to driver U_H, U_L, V_H, V_L, W_H, W_L
extern MOTOR_SENSORLESS_STARTUP_CONFIG motorSensorlessStartUp; // use for motor sensorless startup
extern AlignnGo_PARAM_S alignGo;

UINT32 Align_Setup(AlignnGo_PARAM_S *p, PWM_PARAM_S *pwmPtr)
{  
    UINT32 errorCode = 0;

    P1DIR |= 0x70;   

    P2OUT = 0x00;
    P2SEL = 0x7E;

    TBCCTL1 = OUTMOD_0;
    TBCCTL2 = OUTMOD_6;
    TBCCTL3 = OUTMOD_0;
	TBCCTL4 = OUTMOD_6;
    TBCCTL5 = OUTMOD_0;
    TBCCTL6 = OUTMOD_6;

#ifdef SENSORLESS_3_PHASE_BLDC
    Motor_Control_Init_Start_Timer_A1_For_AlignAndGo(TBCCR0);
#endif
    // Enable timers
    TBCCR0  = pwm3Phase.Period;
    TBCTL = TBSSEL_2 | TBCLR_L | ID_0| 0x0010;
    TA4R = 0x0000;        
         
 	// update PWM duty cycle                    		   
    //align_comm_index = 2;
    if (motorSensorlessStartUp.motor_align_phase > (MAX_ALIGN_PHASE_NUMBER - 1))
    	motorSensorlessStartUp.motor_align_phase = 2;
    alignGo.align_comm_index = motorSensorlessStartUp.motor_align_phase;

   	if (motorSensorlessStartUp.sensorless_startup_dutycycle >= (pwm3Phase.Period >> 1))
   		pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE] =
   				motorSensorlessStartUp.sensorless_startup_dutycycle = pwm3Phase.Period >> 1;

   	pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE] = motorSensorlessStartUp.sensorless_startup_dutycycle;
   	pwm3Phase.DutyCycle[PWM_U_LOW_SIDE] = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
    pwm3Phase.DutyCycle[PWM_V_HIGH_SIDE] = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
    pwm3Phase.DutyCycle[PWM_V_LOW_SIDE] = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
    pwm3Phase.DutyCycle[PWM_W_HIGH_SIDE] = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
    pwm3Phase.DutyCycle[PWM_W_LOW_SIDE] = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
	TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);

#ifdef SENSORLESS_3_PHASE_BLDC
    if (p->alignDelay != 0)
    {
    	DelayCounts(p->alignDelay);
    	p->CallAlignStateMachine = TRUE;
    }
#endif

    return(errorCode);
}      
