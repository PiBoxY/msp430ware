/*=============================================================
 * @file:		TI_SMS_Motor_PWM_API.c
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

#include "TI_SMS_Motor_PWM_API.h"
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // user specific config
extern unsigned char gEnableCurrentSampling;


UINT32 TI_SMS_MOTOR_PWM_Update_Mode(PWM_PARAM_S *p, UINT8 hallState)
{
	UINT32 errCode = 0;
	volatile UINT8 registerValue = 0;

     registerValue = p->comSequence[hallState*6+0]; //(p->comSequence+hallState*6+0);
#ifdef SENSORLESS_CURRENT_CONTROL_ON
           TBCCTL1 = registerValue;
#endif

    registerValue = p->comSequence[hallState*6+0]; //*(p->comSequence+hallState*6+0);
    TBCCTL1 = registerValue;

	registerValue = p->comSequence[hallState*6+1]; //*(p->comSequence+hallState*6+0)
	TBCCTL2 = registerValue;

	registerValue = p->comSequence[hallState*6+2]; //*(p->comSequence+hallState*6+0)
	TBCCTL3 = registerValue;

	registerValue = p->comSequence[hallState*6+3]; //*(p->comSequence+hallState*6+0)
	TBCCTL4 = registerValue;

	registerValue = p->comSequence[hallState*6+4]; //*(p->comSequence+hallState*6+0)
	TBCCTL5 = registerValue;

	registerValue = p->comSequence[hallState*6+5]; //*(p->comSequence+hallState*6+0)
	TBCCTL6 = registerValue;

#ifdef SENSORLESS_3_PHASE_BLDC
	DelayCounts(p->flyBackTime);
#endif

	return(errCode);
}

UINT32 TI_SMS_MOTOR_PWM_Init(PWM_PARAM_S *p)
{
	UINT32 errCode = 0;
	volatile UINT8 registerValue;
	
    registerValue = P3IES & 0x01;    // mask off all bits except bit 0 ADC_DONE
    P3DIR &= 0xF1; 
    P3IES = registerValue;
    P3IFG &= 0x01;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
                      // P3 bit0 used for ADC Done interrupt
   
	TBCTL = 0x0000; // clear the control
	TBEX0 = p->clockDividerExtention;
    TBCTL |= p->clockSource; 
    TBCTL |= p->clockDivider;
    TBCCR0  = p->Period;              // Initial value for main CCR0 to 32767

    // Enable timers
    TBCTL = TBSSEL_2 | TBCLR_L | ID_0| 0x0010;    // Use SMCLK,Divide by 8, UP, Clear Counter gets reset
                                                  // Use SMCLK, Divide by 2, Clear Counter
                                                  // TASSEL_0 is for selecting TACLK as clock source 
                                                  // TASSEL_1 is for selecting ACLK as clock source
                                                  // TASSEL_2 is for selecting SMCLK as clock source 
                                                  // TASSEL_3 is for selecting INCLK as clock source
                                                  // TACLR_L will clear the timer
                                                  // use ID_0 = timer clock input/1
                                                  // use ID_1 = timer clock input/2
                                                  // use ID_2 = timer clock input/4
                                                  // use ID_3 = timer clock input/8}
   
#ifdef _DEVICE_DRV_91620_ || _DEVICE_DRV_92200 || _DEVICE_DRV_92250
      TBCCTL1 = OUTMOD_0;

      TBCCTL3 = OUTMOD_0 ;    // BHS fix low (OFF)
      TBCCTL5 = OUTMOD_0 ;    // CHS fix low (OFF)
      TBCCTL4 = OUTMOD_0 ;    // BLS fix low (OFF)
      TBCCTL6 = OUTMOD_0 ;    // CLS fix low (OFF)
      TBCCTL2 = OUTMOD_0 ;    // ALS fix low (OFF)
#endif 

	  // check the mode after 
      TBCCR1  = p->DutyCycle[0] >> TBCCR1_DIVIDER_FOR_CURRENT_INTERRUPT;    // Set CCR to 4905
      TBCCR2  = p->DutyCycle[1];    // Set CCR to 4905
      TBCCR3  = p->DutyCycle[2];    // Set CCR to 4905
      TBCCR4  = p->DutyCycle[3];    // Set CCR to 4905   
      TBCCR5  = p->DutyCycle[4];    // Set CCR to 4905
      TBCCR6  = p->DutyCycle[5];    // Set CCR to 4905   
 
      return (errCode);
}        
  
UINT32 TI_SMS_MOTOR_PWM_Update_Duty_Cycle(PWM_PARAM_S *p)
{   
	UINT32 errCode = 0;      
    TBCCR1  = p->DutyCycle[0];
	TBCCR2  = p->DutyCycle[1];    // Set CCR to 4905
    TBCCR3  = p->DutyCycle[2];    // Set CCR to 4905
    TBCCR4  = p->DutyCycle[3];    // Set CCR to 4905   
    TBCCR5  = p->DutyCycle[4];    // Set CCR to 4905
    TBCCR6  = p->DutyCycle[5];    // Set CCR to 4905   
    
    return(errCode);
}


UINT32 TI_SMS_MOTOR_PWM_Disable() // Return pre-defined number to show we disable error checking...
{
    UINT32 errCode = 0;	

    TBCCTL1 = OUTMOD_0;
    TBCCTL3 = OUTMOD_0 ;    // BHS fix low (OFF)
    TBCCTL5 = OUTMOD_0 ;    // CHS fix low (OFF)
    TBCCTL4 = OUTMOD_0 ;    // BLS fix low (OFF)
    TBCCTL6 = OUTMOD_0 ;    // CLS fix low (OFF)
    TBCCTL2 = OUTMOD_0 ;    // ALS fix low (OFF)

    return (errCode);
}

UINT32 TI_SMS_MOTOR_PWM_Update_Period(PWM_PARAM_S *p)
{
    UINT32 errCode = 0;	
    
    TBCCR0  = p->Period;              // Initial value for main CCR0 to 32767
            
    return (errCode);	
}


// update PWM duty cycle array
void TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(PWM_PARAM_S *p, UINT16 DutyCycleCount1,
    UINT16 DutyCycleCount2, UINT16 DutyCycleCount3, UINT16 DutyCycleCount4, UINT16 DutyCycleCount5,
    UINT16 DutyCycleCount6 )
{
   p->DutyCycle[PWM_U_HIGH_SIDE] = DutyCycleCount1;
   p->DutyCycle[PWM_U_LOW_SIDE] = DutyCycleCount2;
   p->DutyCycle[PWM_V_HIGH_SIDE] = DutyCycleCount3;
   p->DutyCycle[PWM_V_LOW_SIDE] = DutyCycleCount4;
   p->DutyCycle[PWM_W_HIGH_SIDE] = DutyCycleCount5;
   p->DutyCycle[PWM_W_LOW_SIDE] = DutyCycleCount6;
}
