/*=============================================================
 * @file:		TI_SMS_Motor_PI_API.c
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

#include "TI_SMS_Motor_PI_API.h"

extern PWM_PARAM_S pwm3Phase; // Create an instance of the PWM module to driver U_H, U_L, V_H, V_L, W_H, W_L
extern MOTOR_USERINPUT_PARAMS_S userSpeedInput; // Create an instance for user input.
extern MOTOR_RAMP_CONFIG motorRampUpDownConfig;
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // user specific config
volatile UINT16 PI_output=200;
UINT32 TI_SMS_MOTOR_PI_Control(PI_PARAM_S_NEW *p)
{
    UINT32 errorCode = 0;
    volatile float scaled_value;
    volatile UINT16 step;
    volatile UINT32 Temp;
    volatile float temp_100=100.0;

    if ((p->Kp == 0) && (p->Ki == 0))
    {
    	scaled_value = userControllerConfig.motorCurrentSpeedDailPercent;
    	scaled_value = (scaled_value/100) * (TBCCR0);
    	Temp=scaled_value;
        p->Out= Temp;
    	//p->Out= userControllerConfig.motorCurrentSpeedDailPercent *7;
    }
    else
    {
    // Compute the error
    p->Err = (SINT32)p->Ref - (SINT32)p->Fdb;

    scaled_value = p->Err;
    scaled_value = scaled_value/userSpeedInput.maxMotorSetSpeed;
    scaled_value = (scaled_value * pwm3Phase.Period);
    p->Err = scaled_value;

    // Compute the proportional output
    //    p->Up = ((p->Kp * p->Err) >> PI_KP_SHIFT5);

        //p->Up = p->Err;
        p->Up = ((p->Kp * p->Err) >> 3);
       // Compute the integral output
       // p->Ui = p->Ui + ((p->Ki * p->Err) >> PI_KI_SHIFT5);
       // scaled_value=(float)p->Err;
        //scaled_value*=0.1; // 0.02
        p->Err = (p->Err* p->Ki)>>6;

        p->Ui = p->Ui +  p->Err;

        if (p->Up > (p->OutMax))
          p->Up =  (p->OutMax);
#if 0
        if (p->Up < p->OutMin)
          p->Up =  p->OutMin;
#endif
        if (p->Up < -(p->OutMax))
          p->Up =  -(p->OutMax);

        if (p->Ui > (p->OutMax))
          p->Ui =  (p->OutMax);

    if (p->Ui < p->OutMin)
      p->Ui =  p->OutMin;

    // Compute the pre-saturated output
        p->OutPreSat = (p->Up + p->Ui);
    // Saturate the output
    if (p->OutPreSat > p->OutMax)
      p->Out =  p->OutMax;
    else if (p->OutPreSat < p->OutMin)
      p->Out =  p->OutMin;
    else
      p->Out = p->OutPreSat;

    // Update the previous proportional output
    p->Up1 = p->Up;

    }
    return (errorCode);
}

// use to initialize PI controller values
UINT32 TI_SMS_MOTOR_PI_Control_Init(PI_PARAM_S_NEW *p)
{
    UINT32 errorCode = 0;
	volatile float temp_value;

	temp_value = pwm3Phase.Period; // get PWM period value.
	temp_value =  temp_value * (DEFAULT_PWM_DUTY_CYCLE_IN_PERCENT * 0.01);

	if (temp_value < 1)
	    temp_value = 1;

    p->Uip = temp_value;
	p->Ui = temp_value;
    p->Out = temp_value;
    motorRampUpDownConfig.rampPWMDutyCycle = temp_value;

	temp_value = pwm3Phase.Period; // get PWM period value.
    temp_value = temp_value * (PID_MAXOUT_PERECNT_WRT_PWM_PERIOD * 0.01);
    if (temp_value < 1)
        temp_value = 1;
    p->OutMax = temp_value;

	temp_value = pwm3Phase.Period; // get PWM period value.
    temp_value = temp_value * (PID_MINOUT_PERECNT_WRT_PWM_PERIOD * 0.01);
    if (temp_value < 1)
        temp_value = 1;
    p->OutMin = temp_value;

    return errorCode;
}
