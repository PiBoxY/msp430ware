/*=============================================================
 * @file:		TI_SMS_Motor_PWM_API.h
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

#ifndef TI_SMS_MOTOR_PWM_API_H_
#define TI_SMS_MOTOR_PWM_API_H_
#include "TI_SMS_Motor_Com_Def.h"
#include "TI_SMS_Motor_Init.h"

#define PHW_INT_HI 0x00                          // individual interrupt enables (high or low)
#define PHW_INT_LO 0x08
#define PHV_INT_HI 0x00
#define PHV_INT_LO 0x04
#define PHU_INT_HI 0x00
#define PHU_INT_LO 0x02
#define BLANKING_TIME 10*8                                                  // time to delay before enabling the interrupts


 typedef enum CHN_MODE
 { PWM_MODE = 0,
 	IO_MODE
 } CHN_MODE;
 
 typedef enum PWM_GENERATION_MODE
 { 
 	OUTPUT = 0,
 	SET,
 	TOGGLE_RESET,
 	SET_RESET,
 	TOGGLE,
 	RESET,
 	TOGGLE_SET,
 	RESET_SET
 } PWM_GENERATION_MODE;
 
typedef enum {
	TIMERBCLOCK = 0,
	ACLOCK,
	SMCLOCK,
	INCLOCK
 } CLOCK_SOURCE;
 
 typedef struct {  
     	UINT16 *DutyCycle; // Raw TimerCount of PWM On-time
        UINT16 Period; // Raw TimerCount of PWM period.
        UINT16 deadTime; // Raw Timercount of OWM period.  
        UINT16 flyBackTime; //we re-enable the intrrupts when flyBackTime expires // flyBackTimer will impact BEMF sensing.
        UINT16 clockDivider; // ID_0/ID_1/ID_2/ID_3                   (0*0x40u)      /* Timer A input divider: 0 - /1 */
        UINT16 clockDividerExtention; 
        UINT16 clockSource; //TBSSEL_0/TBSSEL_1/TBSSEL_2/TBSSEL_3 : use SMCLK, or AClK
        const UINT16 *comSequence;    // commutation pair selection
        const UINT16 *interruptSequence; // this set next hall interrupt
       } PWM_PARAM_S;
       
/*------------------------------------------------------------------------------
 Prototypes for the functions in .C
------------------------------------------------------------------------------*/

UINT32 TI_SMS_MOTOR_PWM_Init(PWM_PARAM_S *p); // Set up Controller & registers including PWM timer and 
UINT32 TI_SMS_MOTOR_PWM_Update_Duty_Cycle(PWM_PARAM_S *p); 
UINT32 TI_SMS_MOTOR_PWM_Update_Period(PWM_PARAM_S *p);
UINT32 TI_SMS_MOTOR_PWM_Disable(); // Return pre-defined number to show we disable error checking...

// Waiting to allow alignment
inline void  pwmDelayCounts (UINT32 delay)
{ 
	UINT32 delayCount = 0;
	    for (delayCount = 0; delayCount < delay; delayCount++)         
	    {
		    __no_operation();                   
	    }          
}

void TI_SMS_W_U_AND_W_V_Charge_BootCap(void);
void TI_SMS_V_U_AND_V_W_Charge_BootCap(void);
void TI_SMS_U_V_AND_U_W_Charge_BootCap(void);
void TI_SMS_U_V_W_Charge_BootCap(void);
// update PWM duty cycle array
void TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(PWM_PARAM_S *p, UINT16 DutyCycleCount1,
    UINT16 DutyCycleCount2, UINT16 DutyCycleCount3, UINT16 DutyCycleCount4, UINT16 DutyCycleCount5,
    UINT16 DutyCycleCount6 );
UINT32 TI_SMS_MOTOR_PWM_Update_Mode(PWM_PARAM_S *p, UINT8 hallState);

#endif /*TI_SMS_MOTOR_PWM_API_H_*/
