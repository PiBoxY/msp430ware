/*=============================================================
 * @file:		TI_SMS_Motor_UserInPut_API.c
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

#include "TI_SMS_Motor_ADC_API.h"
#include "TI_SMS_Motor_UserInPut_API.h"

// Added for IEC60730 support
#include "TI_DRV_IEC60730.h"
extern uint16_t gInterruptCounter;

extern PWM_PARAM_S pwm3Phase;
extern MOTOR_USERINPUT_PWM_PARAMS_S pwmPulseIn;
extern UINT8 gSpeedFromMater;
extern ADC_CHN_PARAMS_S analogPotSpeedIn; // Create an instance of ADC module for analog pot monitoring
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // user specific config
UINT8 gCheckUserInputCount = 0;
UINT32 gUserInputFilteredValue = 0;

#define NUM_OF_PWM_SAMPLE 20
#define USING_GUI_SPEED
//#define USING_ADC_POT_SPEED_IN

// variables related to pwm speed pulse in
volatile unsigned int rising_edge_1st;
volatile unsigned int rising_edge_2nd;
volatile unsigned int falling_edge_1st;
volatile unsigned int pulse_duty_cycle;
volatile unsigned int pulse_on_time;
volatile unsigned char pulse_high_low_counts;
volatile unsigned char do_calculation = FALSE;
volatile unsigned char pwm_motor_set_speed;
volatile unsigned char valid_pulse_time = FALSE;
volatile float pwm_pulse_duty_cycle = 0;
// use to store current PWM Pulse In Duty Cycle
volatile unsigned char pwm_pulse_in_duty_cycle_for_speed = 0;
volatile unsigned long int pwm_pulse_in_timeout_value;

unsigned int gPWM_Period_Counts [NUM_OF_PWM_SAMPLE];
unsigned int gPWM_DutyCycle_Counts[NUM_OF_PWM_SAMPLE];
unsigned char gPWM_Sample_Count;
volatile UINT16 gUserInputValue;

UINT32 TI_SMS_MOTOR_UserInPut_Init(MOTOR_USERINPUT_PARAMS_S *p) //called in system init
{ 
	UINT32 errCode = 0;
	
	p->currentMotorSetSpeed = 0;    // set motor current speed to 0 on powerup
	p->motorStartStop = FALSE;   // set motor start flag to FALSE on powerup 
    return (errCode); 
}


UINT32 TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(MOTOR_USERINPUT_PARAMS_S *p) //use to check is motor need to start of stop
{
	UINT32 errCode = 0;

	volatile float temp_value1;

#ifdef USING_GUI_SPEED

	if(userControllerConfig.motorStartStopFlag)
	{
		temp_value1 =(float)userControllerConfig.motorCurrentSpeedDailPercent;
		temp_value1 = temp_value1/100.0;
		temp_value1 = temp_value1 * (userControllerConfig.motorMaxSpeedFromGUI);

		if (temp_value1 > 1)
		   p->currentMotorSetSpeed = temp_value1;
		else
			p->currentMotorSetSpeed = temp_value1;
		p->motorStartStop = TRUE;
	}
	else
	{
		p->currentMotorSetSpeed = 0;
		p->motorStartStop = FALSE;
	}
#endif

	//if (analogPotSpeedIn.data > p->motorStartThreshold)
#ifdef USING_ADC_POT_SPEED_IN
    TI_SMS_MOTOR_ADC_Chn_Read(&analogPotSpeedIn); // Adc_Inputs_Read_Analog_Pot_Value_In_Percentage
	gUserInputFilteredValue =  (gUserInputFilteredValue * 60)/100 + (((analogPotSpeedIn.data) * 40)/100);

	if (gUserInputFilteredValue > p->motorStartThreshold)
	{
	    if (gCheckUserInputCount >= COUNTS_NEEDED_TO_SW_BW_START_STOP)
	    {
	        temp_value1 = gUserInputFilteredValue;
	    	temp_value1 = (temp_value1/1024);
	    	temp_value1 = (temp_value1) * ((p->maxMotorSetSpeed - p->minMotorSetSpeed) + p->minMotorSetSpeed);

	        if (temp_value1 <= p->minMotorSetSpeed)
	            temp_value1 = p->minMotorSetSpeed;

	        if (temp_value1 > p->maxMotorSetSpeed)
	            temp_value1 = p->maxMotorSetSpeed;

	        p->currentMotorSetSpeed = temp_value1;
	    }
	    else
	        gCheckUserInputCount++;
	}
	else if (analogPotSpeedIn.data <= (p->motorStartThreshold - p->deadBandCount))
	{
	    if (gCheckUserInputCount)
	        gCheckUserInputCount--;

	    if (!gCheckUserInputCount)
	    {
	        p->currentMotorSetSpeed = p->minMotorSetSpeed;
	    }
	}
	else
	{
	}
#endif

    return (errCode);
}


UINT32 TI_SMS_MOTOR_UserInPut_UpdateMotorSpeed(MOTOR_USERINPUT_PARAMS_S *p) //use to check is motor need to start of stop
{
	UINT32 errCode = 0;
	volatile float temp_value1;

	TI_SMS_MOTOR_ADC_Chn_Read(&analogPotSpeedIn); // Adc_Inputs_Read_Analog_Pot_Value_In_Percentage

	gUserInputFilteredValue =  (gUserInputFilteredValue * 95)/100 + (((analogPotSpeedIn.data) * 5)/100);

	if (gUserInputFilteredValue > p->motorStartThreshold)
	{
	    if (gCheckUserInputCount >= COUNTS_NEEDED_TO_SW_BW_START_STOP)
	    {
	    	if (gUserInputFilteredValue > ADC_MAX_RANGE)
	    		gUserInputFilteredValue = ADC_MAX_RANGE;

	    	temp_value1 = gUserInputFilteredValue;
	    	temp_value1 = temp_value1/ADC_MAX_RANGE;
	        temp_value1 = temp_value1 * (pwm3Phase.Period);

	        if (temp_value1 <= (pwm3Phase.Period >> 4))
	            temp_value1 = pwm3Phase.Period >> 4;

	        if (temp_value1 >= (pwm3Phase.Period - PWM_HIGHER_LIMIT_DEAD_BAND))
	            temp_value1 = (pwm3Phase.Period - PWM_HIGHER_LIMIT_DEAD_BAND);

	        p->pwmOutPutDutyCycle = temp_value1;
	        p->currentMotorSetSpeed = temp_value1;
	    }
	    else
	        gCheckUserInputCount++;
	}
	else if (analogPotSpeedIn.data <= (p->motorStartThreshold - p->deadBandCount))
	{
	    if (gCheckUserInputCount)
	        gCheckUserInputCount--;

	    if (!gCheckUserInputCount)
	    {
	    	p->currentMotorSetSpeed = 0;
	    	p->pwmOutPutDutyCycle = 0;

	    }
	}
	else
	{
	}

    return (errCode);
}


/******************************************** PWM Speed In timer ************************/
void init_pwm_pulse_in_var (void)
{
    rising_edge_1st = 0;
    rising_edge_2nd = 0;
    falling_edge_1st = 0;
    pulse_duty_cycle = 0;
    pulse_on_time = 0;
    pulse_high_low_counts = 0;
    do_calculation = FALSE;
    pwm_motor_set_speed = 0;
    valid_pulse_time = FALSE;
}
void init_timer_a_0_for_pwm_pulse (void)
{
#ifdef PWM_SPEED_INPUT 
    TA0CCTL1 &= 0x0000;         
    TA0CCTL1 |= CAP + CM0 + CCIE + SCS;      // Setup Timer to Capture Rising Edge Interrupt           
    TA0CTL  = TASSEL_1 + MC_2 + TACLR +ID_1 + TAIE;
#endif
} 

UINT32 TI_SMS_MOTOR_UserInPut_PWM_SpeedInput_Init (void)
{
    UINT32 errCode = 0;
#ifdef PWM_SPEED_INPUT 
    volatile UINT16 Registervalue = 0;
     	
    init_pwm_pulse_in_var();
    init_timer_a_0_for_pwm_pulse();
#endif
    return errCode;
}

UINT32 TI_SMS_MOTOR_UserInPut_PWM_ConvertPWMCountToTime (MOTOR_USERINPUT_PWM_PARAMS_S *p)
{
    UINT32 errCode = 0;
#ifdef PWM_SPEED_INPUT 
    volatile UINT32 Registervalue = 0;
    
    Registervalue = p->pwmPulseInDutyCycleCount;
#endif
    return errCode;
}

/******************************************** PWM Speed In timer End ********************/

// check is pulse vaild
UINT8 TI_SMS_MOTOR_UserInPut_PWM_Period_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p) 
{
    UINT8 returnValue = FALSE;
    
    if ((p->pwmPulsePeriodInNSec >= PWM_IN_PERIOD_LOW_LIMIT) &&
        (p->pwmPulsePeriodInNSec <= PWM_IN_PERIOD_HIGH_LIMIT))
    {         
        returnValue = TRUE;
    }
    
    return returnValue; 		
}

// check is pulse vaild
UINT8 TI_SMS_MOTOR_UserInPut_PWM_DutyCycle_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p) 
{
    UINT8 returnValue = FALSE;
    
    if ((p->pwmPulseDutyCycleNSec >= PWM_IN_DUTYCYCLE_LOW_LIMIT) &&
        (p->pwmPulseDutyCycleNSec <= PWM_IN_DUTYCYCLE_HIGH_LIMIT))
    {         
        returnValue = TRUE;
    }
    
    return returnValue; 		
}

// check is pulse vaild
UINT8 TI_SMS_MOTOR_UserInPut_PWM_SetUpMode_Valid(MOTOR_USERINPUT_PWM_PARAMS_S *p) 
{
    UINT8 returnValue = FALSE;
    
    if ((p->pwmPulseDutyCycleNSec >= PWM_IN_SETUP_MODE_LOW_LIMIT) &&
        (p->pwmPulseDutyCycleNSec <= PWM_IN_SETUP_MODE_HIGH_LIMIT))
    {         
        returnValue = TRUE;
    }
    
    return returnValue; 		
}

// get slave controller addresst
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_Controller_Address(void) 
{
    UINT8 motorControllerAddress = DRV9162_SLAVE_ADDRESS;
       
    return motorControllerAddress; 		
}

// get battery voltage
UINT8 TI_SMS_MOTOR_UserInPut_Get_Battery_Voltage(void) 
{
    UINT8 batteryVoltage = DEFAULT_BATTRY_VOLATGE;
       
    return batteryVoltage; 		
}

// get motor current
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_Current(void) 
{
    UINT8 motorCurrent = DEFAULT_MOTOR_CURRENT_VALUE;
       
    return motorCurrent; 		
}

// get battery voltage
UINT8 TI_SMS_MOTOR_UserInPut_Get_Board_Temperature(void) 
{
    UINT8 boardTemperature = DEFAULT_BOARD_TEMPERATURE;
       
    return boardTemperature; 		
}

// get lead angle
UINT8 TI_SMS_MOTOR_UserInPut_Get_Motor_LeadAngle(void) 
{
    UINT8 motorLeadAngle = DEFAULT_MOTOR_LEADANGLE;
       
    return motorLeadAngle; 		
}

// get motor PWM frequency
UINT16 TI_SMS_MOTOR_UserInPut_Get_Motor_PWM_Frequency(void) 
{
    UINT16 motorPWMFreq = DEFAULT_MOTOR_PWM_FREQUENCY;
       
    return motorPWMFreq; 		
}

// get motor commutation frequency
UINT16 TI_SMS_MOTOR_UserInPut_Get_Motor_Commutation_Frequency(void) 
{
    UINT16 motorCommutationFreq = DEFAULT_MOTOR_COMMUTATION_FREQUENCY;
       
    return motorCommutationFreq; 		
}
UINT32 lastPwmPulseScaledDutyCycleNSec = 1000000; //04/04/2012
/******************************************** PWM pulse in for speed measurement user usage ends ********************/
/* Timer0_A1 Interrupt Vector (TAIV) handler
This ISR will capture two rising and one falling edge of the SPEED in signal. 
Once captured they are used by the main program to calculate the pulse width
and period. Also when the Falling edge is captured, a flag called Calculate is
generated which enables the main program to start the compuation of the pulse
width and period. */

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{	
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T0A1_INTERRUPT;
		TA0CCTL1 &= ~(CCIE +CCIFG);
	}else{
		switch(__even_in_range(TA0IV,14))
		{
			case  0:    // No interrupt
				break;
			case  2:    // TA0.1 external interrupt processing
				if (!do_calculation)
				{
					pulse_high_low_counts++;
										 // count number of times you come into the interrupt
					TA0CCTL1 = CAP + SCS;    // Clear the CCIE and CCIFG

					if(pulse_high_low_counts == 1)    //  first positive edge
					{
						rising_edge_1st = TA0CCR1;    // get period because its 2nd rising edge
						TA0CCTL1 |= CM0 + CCIE;    //Re-enable timer to capture Rising Edge
					}
					else if(pulse_high_low_counts == 2)    // second positive edge
					{
						rising_edge_2nd = TA0CCR1;    // get period because its 2nd rising edge
						TA0CCTL1 |= CM1 + CCIE;    // Re-enable Timer to Capture Falling Edge Interrupt
					}
					else if(pulse_high_low_counts == 3)    // 2nd falling edge
					{
						falling_edge_1st = TA0CCR1;    // get time because its 1st falling edge
						TA0CCTL1 = 0x0000;
						do_calculation = TRUE;
					}
					else // nothing to process
					{
					}
				}
				break;    // CCR1  used
			default:    // cover all the cases except 0 and 2
				break;
		}
	}
}

