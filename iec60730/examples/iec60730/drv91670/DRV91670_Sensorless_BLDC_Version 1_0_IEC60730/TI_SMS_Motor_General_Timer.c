/*=============================================================
 * @file:		TI_SMS_Motor_General_Timer.c
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

#include "TI_SMS_Motor_General_Timer_API.h"

// Added for IEC60730 support
#include "TI_DRV_IEC60730.h"
extern uint16_t gInterruptCounter;

extern MOTOR_CONTROLLER_CONFIG dysonControllerConfig; // helang specific config
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // userspecific config
extern BEMF_PARAM_S sensorlessBEMF;
extern PWM_PARAM_S pwm3Phase; // Create an instance of the PWM module to driver U_H, U_L, V_H, V_L, W_H, W_L
extern PI_PARAM_S_NEW speedPID;
extern MOTOR_SENSORLESS_STARTUP_CONFIG motorSensorlessStartUp;
extern UART_PARAMS_S uartUCA0;

#define CURRENT_SAMPLING_AFTER_THIS_COUNT 50
UINT32 gTimerA4OverFlowCount = 0;
UINT32 gTimerCommutationElapse = 0;
volatile UINT8  gTimerBCCR1CompareCount = 0;
volatile UINT8 gCommutationState = 4;
volatile UINT8 gCommutationStateLast = 4;

extern ADC_CHN_PARAMS_S analogPotSpeedIn;
extern MOTOR_RAMP_CONFIG motorRampUpDownConfig;
extern AlignnGo_PARAM_S alignGo;
extern CONTROLLER_CUR_VOL_ERROR_FLAGS motorConCurVoltError;
extern UINT8 gStart_Current_Sampling;

MotorState AppMotorState = MotorStateAgoIdle;
AUTOCOMSTATE autoComState = INIT_AND_COMMUTATION_START_TIMER;

UINT32 TI_SMS_MOTOR_TIMERA_Init(TIMERA_RARAM_S *p)
{  
	UINT32 errCode = 0;
 
    switch (p->timerA_ID)
    {
    	case TimerA0:
    	    TA0CTL = TASSEL_2 | ID_3 | TACLR_L;
            TA0CCR0 = p->periodCount;
            TA0CCR1 = 0x0000;
    	    break;
    	case TimerA1:
    	    TA1CTL = TASSEL_2 | ID_3 | TACLR_L;
            TA1CCR0 = p->periodCount;
            TA1CCR1 = 0x0000;
            gTimerCommutationElapse = 0;                	    
    	    break;
    	case TimerA2:
    	    TA2CTL = TASSEL_2 | ID_3 | TACLR_L;
            TA2CCR0 = p->periodCount;
            TA2CCR1 = 0x0000;
            TA2CCR2 = 0x0000;
    	    break;    	
    	case TimerA3:
    	    TA3CTL = TASSEL_2 | ID_3 | TACLR_L;
            TA3CCR0 = p->periodCount;
            TA3CCR1 = 0x0000;
            TA3CCR2 = 0x0000;
    	    break;     	
    	case TimerA4:
    	    TA4CTL = TASSEL_2 | ID_3 | TACLR_L;
            TA4CCR0 = p->periodCount;
            TA4CCR1 = 0x0000;
            TA4CCR2 = 0x0000;
            gTimerA4OverFlowCount = 0;                	    
    	    break; 
    	default:
    	    break;
    }
    
	return(errCode);
} 

UINT32 TI_SMS_MOTOR_TIMER_Read(TIMERA_RARAM_S *p)
{ UINT32 errCode = 0;

    switch (p->timerA_ID)
    {
    	case TimerA0:
    	    p->ISRCount = 0;//gTimerA0OverFlowCount;
    	    p->TimerCount = TA0R;
    	    break;
    	case TimerA1:
    	    p->ISRCount = 0;//gTimerA1OverFlowCount;
    	    p->TimerCount = TA1R;                	    
    	    break;
    	case TimerA2:
    	    p->ISRCount = 0;//gTimerA2OverFlowCount;
    	    p->TimerCount = TA2R;                	    
    	    break;    	
    	case TimerA3:
    	    p->ISRCount = 0;//gTimerA3OverFlowCount;
    	    p->TimerCount = TA3R;                	    
    	    break;     	
    	case TimerA4:
    	    p->ISRCount = gTimerA4OverFlowCount;
    	    p->TimerCount = TA4R;
    	    break; 
    	default:
    	    break;
    }

	return(errCode);
}

 

UINT32 TI_SMS_MOTOR_TIMER_Start(TIMERA_RARAM_S *p)
{    UINT32 errCode = 0;

    switch (p->timerA_ID)
    {
    	case TimerA0:
    	    TA0CTL |= 0x0020 | TAIE;          
            TA0R = 0x0000;
    	    break;
    	case TimerA1:
    	    TA1CTL |= 0x0020 | TAIE;
            TA1R = 0x0000;
    	    break;
    	case TimerA2:
    	    TA2CTL |= 0x0020 | TAIE;
            TA2R = 0x0000;
    	    break;    	
    	case TimerA3:
    	    TA3CTL |= 0x0020 | TAIE;
            TA3R = 0x0000;
    	    break;     	
    	case TimerA4:
    	    TA4CTL |= 0x0020 | TAIE;
            TA4R = 0x0000;
            gTimerA4OverFlowCount = 0;               	    
    	    break; 
    	default:
    	    break;
    }

    p->ISRCount = 0;

	return(errCode);
}
	
UINT32 TI_SMS_MOTOR_TIMER_Stop(TIMERA_RARAM_S *p) 
{   
	UINT32 errCode = 0;

	return(errCode);
}

// Waiting to allow alignment
void DelayCounts (UINT32 delay)
{
	UINT32 delayCount;
	
	if (delay > 0)
	{
	    for (delayCount = 0; delayCount < delay; delayCount++)         
	    {
		    __no_operation();                   
	    }
	}           
}

// initialize timer for commutation delay time and also for electrical speed measurement
void Motor_Control_Init_Start_Timer_A4_For_Commutation_Delay (UINT16 countValue)
{
	TA4CCR0  = countValue;  
	TA4CCR1  = countValue - (countValue >> 1);    // offset 110  
	TA4CTL = 0x0000;        	// timer halted
	TA4R = 0x0000;          	// clear counter	 		
	TA4CTL = TASSEL_2 | TBCLR_L | ID_0| 0x0010;   
	TA4CCTL1 = CCIE;    // CCR0 interrupt 
}

// initialize timer for commutation delay time and also for electrical speed measurement
void Motor_Control_Init_Start_Timer_A1_For_AlignAndGo (UINT16 countValue)
{
	TA1CCR0  = countValue;  
    TA1CCR1 = pwm3Phase.DutyCycle[PWM_U_LOW_SIDE] >> 2;
	TA1CTL = 0x0000;        	// timer halted
	TA1R = 0x0000;          	// clear counter	 		
	TA1CTL = TASSEL_2 | TBCLR_L | ID_0| 0x0010;   
	TA1CCTL1 = CCIE;    // CCR0 interrupt 
}

void SetMotorNewForcedCommutationState (UINT8 commSatate)
{
    switch (commSatate)
    {
    	case 0x01:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 3;
    	    break;
            
    	case 0x02:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 6;
    	    break;
            
    	case 0x03:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 2;
    	    break;
            
    	case 0x04:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 5;
    	    break;
             	
    	case 0x05:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 1;
    	    break;
            
    	case 0x06:
    	    gCommutationStateLast = commSatate;
    	    gCommutationState = 4;
    	    break;
        	     
    	default:
    	    break;
    }
}

void DoMotorCommutationAsPerZerCross (UINT8 zeroCrossComparatorValue, UINT8 delayFlag)
{
    /* motor commutation sequences */ 
    switch(zeroCrossComparatorValue)
    { 
    	case 0:    //Sensorless               
            P3IES = 0x00;
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt           	
            if (delayFlag)
                pwmDelayCounts(COMMUTATION_DELAY);
          	break;
          
        case 1:    //Sensorless W-U
        	TBCCTL1 =  OUTMOD_0; // AHS fix high (OFF) . Port 4.1
        	TBCCTL3 = OUTMOD_0; // BHS fix high (Off) . Port 4.4
        	TBCCTL4 = OUTMOD_0;       // BLS fix low  (Off) . Port 4.4
        	TBCCTL6 = OUTMOD_0;       // CLS fix low  (Off) . Port 4.6
        	TBCCTL5 = OUTMOD_0 | BIT_2; // CHS fix low (On). Port 4.3
        	TBCCTL2 = OUTMOD_6 ; // ALS PWMed . Port 4.2
           	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES = PHW_INT_HI | PHV_INT_HI | PHU_INT_LO;  /* case 1 */
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt      	
          	break;
                
        case 2:  // U-V
          	// case 2  12-17, A-B, A is ON, B is PWM
        	TBCCTL2 = OUTMOD_0;       // ALS fix low  (OFF) . Port 4.1
        	TBCCTL3 = OUTMOD_0; // BHS fix high (Off) . Port 4.2
        	TBCCTL5 = OUTMOD_0; // CHS fix high (Off) . Port 4.3
        	TBCCTL6 = OUTMOD_0;       // CLS fix low  (Off) . Port 4.6
        	TBCCTL4 = OUTMOD_6 ;        // BLS PWMed . Port 4.4
        	TBCCTL1 = OUTMOD_0 | BIT_2;        // AHS fix Low (On). Port 4.5
           	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES = PHW_INT_HI | PHV_INT_LO | PHU_INT_HI;  /* case 2 */;
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
      
          	break;
           
        case 3:    //Sensorless W-V
        	TBCCTL1 = OUTMOD_0; // AHS fix high (OFF) . Port 4.1
        	TBCCTL2 = OUTMOD_0;       // ALS fix low  (Off) . Port 4.2
        	TBCCTL3 = OUTMOD_0; // BHS fix high (Off) . Port 4.3
        	TBCCTL6 = OUTMOD_0;       // CLS fix low  (Off) . Port 4.6
        	TBCCTL4 = OUTMOD_6 ; // BLS PWMed . Port 4.4
        	TBCCTL5 = OUTMOD_0 | BIT2; // CHS fix low (On). Port 4.5
        	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES = PHW_INT_HI | PHV_INT_LO | PHU_INT_LO;
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
      
          	break;
          
        case 4:    //Sensorless  V-W
        	TBCCTL1 = OUTMOD_0;          // AHS fix high (Off) . Port 4.3
        	TBCCTL2 = OUTMOD_0  ;        // ALS fix low  (Off) . Port 4.6
       		TBCCTL4 = OUTMOD_0  ;        // BLS fix low  (OFF) . Port 4.1
        	TBCCTL5 = OUTMOD_0;          // CHS fix high (Off) . Port 4.2
        	TBCCTL6 = OUTMOD_6 ;         // CLS PWMed . Port 4.4
        	TBCCTL3 = OUTMOD_0 | BIT_2;  // BHS fix Low (On). Port 4.5
        	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES = PHW_INT_LO | PHV_INT_HI | PHU_INT_HI;
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt     
          	break;
        
        case 5:    //Sensorless  V-U
        	TBCCTL1 = OUTMOD_0;       // AHS fix high (Off) . Port 4.3
        	TBCCTL4 = OUTMOD_0;       // BLS fix low  (Off) . Port 4.6
        	TBCCTL5 = OUTMOD_0;       // CHS fix high (Off) . Port 4.2
        	TBCCTL6 = OUTMOD_0;        // CLS fix low  (OFF) . Port 4.1
        	TBCCTL2 = OUTMOD_6 ;         // ALS PWMed . Port 4.4
        	TBCCTL3 = OUTMOD_0 | BIT_2;  // BHS fix Low (On). Port 4.5
        	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES =  PHW_INT_LO | PHV_INT_HI | PHU_INT_LO; /* case 5 */;
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          	break;
           
        case 6:    // U-W
        	TBCCTL2 = OUTMOD_0  ;       // ALS fix low  (OFF) . Port 4.1
        	TBCCTL3 = OUTMOD_0; // BHS fix high (Off) . Port 4.3
        	TBCCTL4 = OUTMOD_0;       // BLS fix low  (Off) . Port 4.6
        	TBCCTL5 = OUTMOD_0; // CHS fix high (Off) . Port 4.2
        	TBCCTL6 = OUTMOD_6 ;         // CLS PWMed . Port 4.4
        	TBCCTL1 = OUTMOD_0 | BIT_2;        // AHS fix Low (On). Port 4.5
        	if (delayFlag)
        	    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES =  PHW_INT_LO | PHV_INT_LO | PHU_INT_HI; /* case 6 */
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          	break;                              
           
       	case 7:    
       		if (delayFlag)
       		    pwmDelayCounts(COMMUTATION_DELAY);
            P3IES =  PHW_INT_LO | PHV_INT_LO | PHU_INT_LO; /* case 7 */
            P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
            
         	break;  
             
        default:             
        	break;
	}	
}

// initialize timer for commutation delay time and also for electrical speed measurement
void Motor_Control_Init_Start_Timer_A3_For_Commutation_Delay_New (void)
{
	TA3CTL = 0x0000;        	// timer halted
	TA3R = 0x0000;          	// clear counter	 		
	TA3CTL = TASSEL_2 + MC__CONTINOUS + ID__8; // start TA3	
} 
/*
 *  \fn void align_state_machine(void)
 *  \brief Align and Go state machine, called during appropriate states
 */
UINT16 myLocalCount=0;
extern UINT16 gReturn2AlignGoFlag;
extern UINT8 gPredictZCIndex;
UINT16 gSwitch2AutoComFlag = FALSE;
UINT16 correctZCcount=0; 
UINT8 incorrectZCcount=0; 
UINT16 nextComDelay=200;
UINT16 startWindowForZCwaiting=20000;
UINT16 startupComCount=0;
UINT8 startupLastZCvalue=0;
UINT16  goodStartUpCount=0;
volatile UINT16 align_timer_ticks = 0;
UINT8 current_com_index = 0;
UINT8 gComSeq[6]={4,5,1,3,2,6};
UINT32 averageZCTime=0;

UINT32 debugCount=0;
UINT32 firstAutoComFlag = TRUE;
UINT32 delayForCommutation=0;
UINT32 previousAverageZCTime = 20000;
UINT32 currentAverageZCTime  = 0 ;
UINT8 validZeroCrossingComing=FALSE;
extern UINT8 gswitchToNormalRunning;
extern UINT16 zeroCrossToCommutationDelay;
 
void InitTimer1Relatedvariables (void)
{
	alignGo.CallAlignStateMachine = FALSE;
    gswitchToNormalRunning = 0;
    align_timer_ticks = 0;
    AppMotorState = MotorStateAgoIdle;
    autoComState = INIT_AND_COMMUTATION_START_TIMER;
}

void SetCommuatationTimerStateMachine (MotorState alignstatevalue)
{
    AppMotorState = alignstatevalue;	
}

void SetCommuatationTimerStateMachine_Force_Commutation (AUTOCOMSTATE autoComStateValue)
{
    autoComState = autoComStateValue;	
}

void align_state_machine(void)
{	
    UINT16 temp_value;

    if (!(alignGo.CallAlignStateMachine))
        return;


    // Check comparator for appropriate states	
	switch (AppMotorState)
	{
		case MotorStateAgoIdle:
			if (align_timer_ticks >= motorSensorlessStartUp.realign_window)
			{
				Motor_Control_Init_Start_Timer_A3_For_Commutation_Delay_New();
				alignGo.align_comm_index = (alignGo.align_comm_index +1)%6;
				startWindowForZCwaiting=20000;
                temp_value = pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE];
                
                 if ((temp_value + 1) > (pwm3Phase.Period >> 2))
                	temp_value = pwm3Phase.Period >> 2;
                else
                	temp_value = temp_value + (pwm3Phase.Period >> 6);
                 
                motorRampUpDownConfig.rampPWMDutyCycle = temp_value;
                pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE] = temp_value; // init PWM duty cycle.
    		    pwm3Phase.DutyCycle[PWM_U_LOW_SIDE] = temp_value; 
    		    pwm3Phase.DutyCycle[PWM_V_HIGH_SIDE] = temp_value; 
    		    pwm3Phase.DutyCycle[PWM_V_LOW_SIDE] = temp_value;
    		    pwm3Phase.DutyCycle[PWM_W_HIGH_SIDE] = temp_value;
    		    pwm3Phase.DutyCycle[PWM_W_LOW_SIDE] = temp_value;
		        TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
					
					
				// Energize zero phase				
				DoMotorCommutationAsPerZerCross(gComSeq[alignGo.align_comm_index], FALSE);
				pwmDelayCounts(COMMUTATION_DELAY);
				startupComCount=0;
				AppMotorState = MotorStateAgo2on;
				align_timer_ticks = 0;
				goodStartUpCount=0;
				motorSensorlessStartUp.motor_align_phase = alignGo.align_comm_index;
			}
			break;
				
		case MotorStateAgo2on:
			// 2 on state - drive U + W
			if (align_timer_ticks >= 200)
			{
				alignGo.align_comm_index=(alignGo.align_comm_index+1)%6;
				// Drop first phase, keep 2nd on to position wheel forward					
				DoMotorCommutationAsPerZerCross(gComSeq[alignGo.align_comm_index], FALSE);
     			TA3R=0;
				pwmDelayCounts(COMMUTATION_DELAY);
     			// Set state to wait on comparator 1
				AppMotorState = MotorStateAgoBlanking;
				align_timer_ticks = 0; 
			}
			break;
				
		case MotorStateAgoBlanking:
			// Wait for blanking time before hunting for comparators
			if (align_timer_ticks >= 50)
			{
				AppMotorState = MotorStateAgoWaitComp;
				align_timer_ticks = 0;
			}
			break;
				
		case MotorStateAgoWaitComp:
			// waiting on comparator 1 to transition. Check to see if too much time has elapsed
			if(TA3R > motorSensorlessStartUp.zerocorss_det_window)
		    {
				AppMotorState = MotorStateAgoIdle;
				align_timer_ticks = 0;	
			}
			else 
			{   
				if(((P3IN >> 1) & 0x07) == gComSeq[(alignGo.align_comm_index+1)%6])
				{
					correctZCcount++;
				    incorrectZCcount=0;
				}
			    else
			    { 
			    	correctZCcount=0;
			          
			   		if(((P3IN >> 1) & 0x07) != gComSeq[alignGo.align_comm_index%6])
			        {
			   			if(incorrectZCcount++ > INCORRECT_ZEROCROSS_COUNT)
			          	{
			          		AppMotorState = MotorStateAgoIdle;
			          	    align_timer_ticks = 0;
			          	    incorrectZCcount=0;
			          	 }
			        }
			    }
				    
				if( correctZCcount >= CORRECT_ZEROCROSS_COUNT)
				{   
				
				    zeroCrossToCommutationDelay=TA3R;
				    startupComCount++;
				    
				    currentAverageZCTime=(previousAverageZCTime-(previousAverageZCTime>>4))+(zeroCrossToCommutationDelay>>4);	
				    
				    if(startupComCount>10)
				        startWindowForZCwaiting=currentAverageZCTime +(currentAverageZCTime>>1);

					gCommutationState = (P3IN >> 1) & 0x07;
					align_timer_ticks = 0;
				    AppMotorState = MotorStateAgoBemfBlanking;
                    DoMotorCommutationAsPerZerCross(gCommutationState, FALSE);
                    TA3R=0;
				    previousAverageZCTime=TA2R;
				   
				    pwmDelayCounts(COMMUTATION_DELAY);
				
				}
			} 
			break;
		        
		case MotorStateAgoBemfBlanking:			 
			if (align_timer_ticks > ALIGN_TIMER_TICK_VALUE) // 20
            {
            	AppMotorState = MotorStateAgoWaitComp;
                align_timer_ticks=0;
                alignGo.align_comm_index=(alignGo.align_comm_index+1)%6;
                previousAverageZCTime = currentAverageZCTime;
                
                if(goodStartUpCount++> ALIGN_AND_GO_GOOD_STARTUP_COUNT)
                {
#ifdef PID_SPEED_CONTROL_ON
                	speedPID.Ui = motorRampUpDownConfig.rampPWMDutyCycle;
                	speedPID.Out = motorRampUpDownConfig.rampPWMDutyCycle;
#endif
                	AppMotorState = MotorStartUpDone;
				    gswitchToNormalRunning = TRUE;
				    P3IE = 0x0E;
                }	
            }
			break;
                
         case  MotorStartUpDone:
             break;
	}				
	// Increment number of timer ticks
	++align_timer_ticks;	
}	


///////////////////////////// all Timer ISR

/* Timer0_A0 interrupt service routine
 This is for only CCRO. 
*/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T0A0_INTERRUPT;
		TA0CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA0IV;
	}
}


#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{     
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T1A0_INTERRUPT;
		TA1CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA1IV;
	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{  
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T1A1_INTERRUPT;
		TA1CCTL1 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA1IV;

		align_state_machine();
	}
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
{	
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T2A0_INTERRUPT;
		TA2CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA2IV;
	}
}

// Timer 2_a1 isr
#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T2A1_INTERRUPT;
		TA2CCTL1 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA2IV;
	}
}

// Timer 3_a0 isr
#pragma vector=TIMER3_A0_VECTOR
__interrupt void TIMER3_A0_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T3A0_INTERRUPT;
		TA3CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA3IV;
	}
}

// Timer 3_a1 isr
#pragma vector=TIMER3_A1_VECTOR
__interrupt void TIMER3_A1_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T3A1_INTERRUPT;
		TA3CCTL1 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA3IV;
	}
}

/* Timer4_A0 interrupt service routine
 This is for only CCRO. 
*/
// use to genearte 12 step inter comm waveform 

#pragma vector=TIMER4_A0_VECTOR
__interrupt void TIMER4_A0_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T4A0_INTERRUPT;
		TA4CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA4IV;
	}
}

#pragma vector=TIMER4_A1_VECTOR
__interrupt void TIMER4_A1_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T4A1_INTERRUPT;
		TA4CCTL1 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TA4IV;
		static UINT8 test = 0x00;
		static UINT8 counter = 0;

		gTimerA4OverFlowCount++;
	#ifdef PWM_SPEED_INPUT
		if (counter++ > COMMTIME_OUT_NUM)
		{
			if (uartUCA0.validPacketReceived)
			{
				uartUCA0.validPacketReceived = FALSE;
				test = ~test;
				if (test)
					Port_1_Bit_6_Toggle(TRUE);
				else
					Port_1_Bit_6_Toggle(FALSE);
			}
			counter = 0;
		}
	#endif

		if ((gTimerA4OverFlowCount - gTimerCommutationElapse) > TIME_OUT_FOR_MOTOR_COMM_FREQ) //Timeout-> move to fault state
		{
			userControllerConfig.commutaionFreq = sensorlessBEMF.comutationStateCount;
			sensorlessBEMF.comutationStateCount = 0;
			gTimerCommutationElapse = gTimerA4OverFlowCount;
		}
	}
}

/* Timer0_B0 interrupt service routine
 This is for only CCRO. 
*/
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter & BITF){
		gInterruptCounter += TEST_T0B0_INTERRUPT;
		TB0CCTL0 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TBIV;
	}
}

/* Timer0_B1 interrupt service routine
 This is for CCR1, CCR2, CCR3, CCR4, CCR5, CCR6 and over flow timer. 
*/
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
{
	// Added for IEC60730 support
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_T0B1_INTERRUPT;
		TB0CCTL1 &= ~(CCIE +CCIFG);
	}else{
		UINT16 interruptValue = TBIV;

		  switch(interruptValue & 0x02)
		  {
			case 0x2:    // use for CCR1 interrupt
				break;
			case 0x4:    // use for CCR2 interrupt
				break;
			case 0x8:    // use for CCR3 interrupt
				break;
			case 0x10:   // use for CCR4 interrupt
				break;
			case 0x20:   // use for CCR5 interrupt
				break;
			case 0x40:   // use for CCR6 interrupt
				break;
			default:
				  break;
		  }
	}
}



