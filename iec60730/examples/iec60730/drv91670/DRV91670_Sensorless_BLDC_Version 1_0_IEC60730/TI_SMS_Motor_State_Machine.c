/*=============================================================
 * @file:		TI_SMS_Motor_State_Machine.c
 *
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


// Added for IEC60730 support
#include "TI_DRV_IEC60730.h"


#include "TI_SMS_Motor_Init.h"
#ifndef TI_SMS_MOTOR_UART_PACKET_API_H_
#include "TI_SMS_Motor_UART_Packet_API.h"
#endif

//The following instances of various moduels are defined/configured in TI_SMS_Motor_Init.h
extern PWM_PARAM_S pwm3Phase; // Create an instance of the PWM module to driver U_H, U_L, V_H, V_L, W_H, W_L
extern MOTOR_STATUS_S motorState; //Create an instance of the motor state machine
extern TIMERA_RARAM_S timerA4; // Create an instance of timerA module
extern MOTOR_ELECTRICAL_SPEED motorElectricalSpeed; // Create an instance of the elctrcial speed module
extern AlignnGo_PARAM_S alignGo; // Create an instance of align & go for sensorless startup
extern BEMF_PARAM_S sensorlessBEMF; //Create an instance of bakc EMF
extern MOTOR_USERINPUT_PARAMS_S userSpeedInput; // Create an instance for user input.
extern MOTOR_CONTROLLER_CONFIG userControllerConfig; // user specific config
extern ADC_CHN_PARAMS_S analogPotSpeedIn; // Create an instance of ADC module for analog pot monitoring
extern MOTOR_USERINPUT_PWM_PARAMS_S pwmPulseIn;  // remote contorl -> RC receiver -> PWM-in -> control PWM-output UVW duty cycles
extern PI_PARAM_S_NEW speedPID;
extern UART_PARAMS_S uartUCA0;
extern MOTOR_RAMP_CONFIG motorRampUpDownConfig;
extern ADC_CHN_PARAMS_S motorVCCSensing;
extern ADC_CHN_PARAMS_S motorCurrentSensing;
extern CONTROLLER_CUR_VOL_ERROR_FLAGS motorConCurVoltError;
extern PI_PARAM_S_NEW default_speedPID;

// utility fucntions
extern void Motor_Enable_Disable_Hall_ZeroCross_Interrupt(unsigned char flag); // disable hall port interrupt 
extern void Init_Motor_Starting_Sequence(void); // use to generate dummy port interrupt	
// change PWM UVW dutycyle in order to adjust speed.
void TI_SMS_Motor_Increment_Decrement_DutyCycle (MOTOR_USERINPUT_PARAMS_S *userptr, MOTOR_RAMP_CONFIG *ptrRampConfig);
  //  MOTOR_CONTROLLER_CONFIG *motorptr, PWM_PARAM_S *pwmptr);
void TI_SMS_Motor_Commutation(UINT8 hall_effect_state); // sensorless BLDC motor commutation  

// align setupbfor motor startup
extern UINT32 Align_Setup(AlignnGo_PARAM_S *p, PWM_PARAM_S *pwmPtr);
// use to switch hall sensor sequence and hall interrupt settings too
extern UINT32 TI_SMS_Motor_Switch_Comm_Sequence_And_Interrupt_Table (PWM_PARAM_S *p, MOTOR_CONTROLLER_CONFIG *config_ptr);

// variable related to motor reverse rotation detection
extern UINT8 gReverseRotationDetected;

// below timer variables are used to measure speed between two commutation stages 
UINT16 gTimerElapse1st  = 0;
UINT16 gTimerElapse2nd = 0;
UINT16 gTimerElapse3rd = 0;
UINT16 gTimerPWMPulseInElapsed = 0;
UINT16 gTargetSpeed = DEFAULT_MOTOR_SET_SPEED;
UINT16 gSpeedRampUp = DEFAULT_MOTOR_RAMP_UP_INITAIL_SPEED;
UINT16 gCurrentMotorRPM = 0; 
UINT8 gswitchToNormalRunning;
float gPWMCalibrationFactor = 1;
extern UINT32 gUserInputFilteredValue;
UINT8 gStart_Current_Sampling = FALSE;
UINT16 underSpeedCount = 0;

void main_motor_controller_state_machine (MOTOR_STATUS_S *p) // motor state machine is called in while loop within main function. customers need to keep their foucs on.
{ 
	volatile UINT16 count; // used in various iteratios
	volatile UINT8 pwmInPulseReady; // PWM-in from RC receiver is ready.
	volatile UINT16 registerValue; // store register values
	volatile UINT16 soundCount=0;
    volatile float myRPM; // motor speed derived from the time difference between different commutation stages
    volatile float temp_value;
    volatile UINT8 motorStartFlag = TRUE;
    
    switch (p->currentState)
    {
        case MOTOR_STATE_INITIALIZE:  // init main state machine to move to next state
            motor_sys_init();
            motorState.motorSetSpeed = userSpeedInput.currentMotorSetSpeed;
            gReverseRotationDetected = FALSE;
            gUserInputFilteredValue = 0;
            gStart_Current_Sampling = FALSE;
            motorConCurVoltError.loadCurrentValue = 0;
            motorConCurVoltError.motorVCCValue = 0;
            // turn power led on
            Port_1_Bit_4_Toggle(FALSE);
            // enable pre-driver, need more currrent so enable PWM pusle skipping IN OC
            p->previousState = p->currentState;
            p->currentState = MOTOR_STATE_STOP;                 
            break;
               
        case MOTOR_STATE_CHECK_INPUTS:
            TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
        	// initialize all user related struct
            Init_Motor_Sensorless_Related_Variables(&userControllerConfig);
            // init all PWM related variables w.r.t PWM period count
            //Init_Motor_PWM_Period_Related_Variables();
            // use to initialize PI controller values
            default_speedPID.Ki = speedPID.Ki;
            default_speedPID.Kp = speedPID.Kp;
            speedPID.Ki = 15;
            speedPID.Kp = 2;

            TI_SMS_MOTOR_PI_Control_Init(&speedPID);
            // initialize PWM module
		    temp_value = pwm3Phase.Period; // get PWM period value.
		    temp_value =  temp_value * (DEFAULT_PWM_DUTY_CYCLE_IN_PERCENT * 0.01);
    		if (temp_value < 1)
                temp_value = 1;
     		// update PWM duty cycle array
     		TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, temp_value,
     				temp_value, temp_value, temp_value, temp_value, temp_value);
    		TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
    		underSpeedCount = 0;
            p->previousState = p->currentState;
            p->currentState=MOTOR_STATE_START_UP;
            break;
             
        case MOTOR_STATE_START_UP:
            TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);

            // Added for IEC60730 support
            IEC60730_RUN_PEST_test();

            // setup back EMF module
             gReverseRotationDetected = FALSE; 
             sensorlessBEMF.startBEMFSensing = FALSE;
             sensorlessBEMF.comutationStateCount = 0;
             sensorlessBEMF.lastStateSaved = FALSE;
             sensorlessBEMF.comutationStateCount = 0;
             sensorlessBEMF.sensorlessCommEndTimerCount = 0;
             sensorlessBEMF.sensorlessCommStartTimerCount = 0;
             sensorlessBEMF.sensorlessCommTimerInterval = 0;
             sensorlessBEMF.zeroCrossIntrruptCount = 0;
             sensorlessBEMF.previouBackEMFState = sensorlessBEMF.currentBackEMFState = 0;
             P3IE = 0x00;                          
             // enable pre-driver, need more current so enable PWM pulse skipping IN OC
             Port2_Enable_Disable_Pre_Driver(TRUE, TRUE); 
             TA3CTL = 0x00;
             InitTimer1Relatedvariables();
#ifdef SENSORLESS_3_PHASE_BLDC
             Align_Setup(&alignGo, &pwm3Phase); // Align and Go for motor start up.
             sensorlessBEMF.startBEMFSensing = TRUE;

             while (!gswitchToNormalRunning)
             {
            	 // check user speed input for motor start-stop-set speed
            	 TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(&userSpeedInput);
                 TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);

            	 if (!(userSpeedInput.motorStartStop))
            	 {
                 	 p->previousState = p->currentState;
                     p->currentState=MOTOR_STATE_STOP;
            		 break;
            	 }
             }
#endif

#ifdef SENSORED_3_PHASE_BLDC
             Align_Setup(&alignGo, &pwm3Phase); // Align and Go for motor start up.
             Motor_Enable_Disable_Hall_ZeroCross_Interrupt(TRUE);
             Init_Motor_Starting_Sequence();
#endif
             if (userSpeedInput.motorStartStop)
             {     
                 TI_SMS_MOTOR_TIMER_Read(&timerA4);
                 gTimerElapse1st = timerA4.ISRCount;
                 gTimerElapse2nd = timerA4.ISRCount;
#ifndef PID_SPEED_CONTROL_ON
                 // use max 25 percent period duty cycle for rampup 
                 userSpeedInput.pwmOutPutDutyCycle = (pwm3Phase.Period >> 2);
#else
                 userSpeedInput.pwmOutPutDutyCycle = motorRampUpDownConfig.rampPWMDutyCycle;
#endif
                 p->previousState = p->currentState;
                 p->currentState = MOTOR_STATE_RAMP_UP;
             }
             else
             {
             	 p->previousState = p->currentState;
                 p->currentState=MOTOR_STATE_STOP;
             }
             break;
            
        case MOTOR_STATE_RAMP_UP:  // we keep adding certain values to PWM dutyCyles to start up. 
            TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
        	TI_SMS_MOTOR_TIMER_Read(&timerA4);
        	// check user speed input for motor start-stop-set speed
        	TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(&userSpeedInput);

        	if((timerA4.ISRCount - gTimerElapse2nd) > RAMPUP_DELAY_TO_REACH_MINIMUM_SPEED)
        	{
        		// check is feedback speed ready
        		if (motorElectricalSpeed.speedReady)
        		{
        			myRPM = (motorElectricalSpeed.timerInterval)/1000;
        			myRPM =  (myRPM * 0.00032);
        			myRPM = (1/myRPM)*60;

        			speedPID.Ki = default_speedPID.Ki;
        			speedPID.Kp = default_speedPID.Kp;

            		speedPID.Ref = userSpeedInput.currentMotorSetSpeed;
        			speedPID.Fdb = 0;
        			TI_SMS_MOTOR_PI_Control(&speedPID);
        			registerValue = speedPID.Out;

            		userSpeedInput.pwmOutPutDutyCycle = speedPID.Out;
             		TI_SMS_Motor_Increment_Decrement_DutyCycle(&userSpeedInput, &motorRampUpDownConfig);

             		// update PWM duty cycle array
             		TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, registerValue,
             				registerValue, registerValue, registerValue, registerValue, registerValue);
             	    TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);

        			if (myRPM > MIMIMUM_DEFUALT_MOTOR_RPM)
        			{
        			     gTimerElapse1st = timerA4.ISRCount;
                         gTimerElapse2nd = timerA4.ISRCount;
                	     p->previousState = p->currentState;
                         p->currentState = MOTOR_STATE_RUNNING;
                         gStart_Current_Sampling = FALSE;
                         sensorlessBEMF.comutationStateCount = 0;
        			}
        			else
        			{
        				p->previousState = p->currentState;
        				p->currentState = MOTOR_STATE_STOP;
        			}
        		}
        		else
        		{
                    p->previousState = p->currentState;
                    p->currentState=MOTOR_STATE_STOP;
        		}
        	}
        	break;
           
        case MOTOR_STATE_RUNNING:
             // enable pre-driver, disable PWM pusle skipping in OC
            Port2_Enable_Disable_Pre_Driver(TRUE, FALSE); 
        
            gTimerElapse2nd = timerA4.ISRCount;
            gTimerElapse3rd =  gTimerElapse2nd;
            
            do
            {                
                TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
            	TI_SMS_MOTOR_TIMER_Read(&timerA4);
             
            	// check user speed input for motor start-stop-set speed
            	TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(&userSpeedInput);
            	speedPID.Ref = userSpeedInput.currentMotorSetSpeed;
            	// read value from user input and store in setspeed
            	motorState.motorSetSpeed = userSpeedInput.currentMotorSetSpeed;
            	//move setspeed to motor target speed for speed ramp up-down
            	gTargetSpeed = motorState.motorSetSpeed;

                if((timerA4.ISRCount - gTimerElapse2nd) > RAMPUP_DELAY)
             	{
                    // check user speed input for motor start-stop-set speed
                	TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(&userSpeedInput);

                 	TI_SMS_MOTOR_TIMER_Read(&timerA4);
                	// reset software timer for another timeout
                	gTimerElapse2nd = timerA4.ISRCount;
                    //gTimerElapse3rd = gTimerElapse2nd;
         
                    // check is feedback speed ready
                 	if (motorElectricalSpeed.speedReady)
                 	{
                 		gTimerElapse3rd = timerA4.ISRCount;
                 		motorElectricalSpeed.speedReady = FALSE;
                 		myRPM = (motorElectricalSpeed.timerInterval)/1000;
                 		myRPM =  (myRPM * 0.00032);
                 		myRPM = (1/myRPM)*60;

#ifdef PID_SPEED_CONTROL_ON
                 		speedPID.Fdb = myRPM;
#if 0
                 		if ( (speedPID.Fdb <= (speedPID.Ref - (speedPID.Ref >> SPEED_FEED_BACK_ERROR_BAND))) ||
                            (speedPID.Fdb >= (speedPID.Ref + (speedPID.Ref >> SPEED_FEED_BACK_ERROR_BAND))))
#endif
                        {
                        	// call PI controller
                        	TI_SMS_MOTOR_PI_Control(&speedPID);
                        	// move PI controller output to PWM duty cycle update function
                        	userSpeedInput.pwmOutPutDutyCycle = speedPID.Out;

                        	TI_SMS_Motor_Increment_Decrement_DutyCycle(&userSpeedInput, &motorRampUpDownConfig);
                        	// update PWM duty cycle
                        	registerValue = motorRampUpDownConfig.rampPWMDutyCycle;
                        	// update PWM duty cycle array
                        	TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, registerValue,
                        			registerValue, registerValue, registerValue, registerValue, registerValue);
                        	TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
                        }
#else
                 		TI_SMS_Motor_Increment_Decrement_DutyCycle(&userSpeedInput, &motorRampUpDownConfig);
                    	// update PWM duty cycle
                 		registerValue = motorRampUpDownConfig.rampPWMDutyCycle;
                 		// update PWM duty cycle array
                 		TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, registerValue,
                 				registerValue, registerValue, registerValue, registerValue, registerValue);
                 	    TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
#endif
                        TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
                 	}
                    else
                    {
                        // if this software timer will expires, we did not received any feed back speed 
                    	if((timerA4.ISRCount - gTimerElapse3rd) > MAX_TIMEOUT_FOR_RPM_FEEDBACK)
                        {
                        	userSpeedInput.motorStartStop = FALSE;
                 			p->previousState = p->currentState;
                 			p->currentState = MOTOR_STATE_STOP;
                 		    break;
                 		}
                    }
               } 
              // after every 500 commuation counts we check is motor running under speed
              if (sensorlessBEMF.comutationStateCount > COMM_COUNT_TO_CHECK_UNDER_SPEED)
              {
            	  if (speedPID.Fdb < UNDER_SPEED_RPM_VALUE)
            	  {
                      underSpeedCount++;
                      // if motor running under speed for 50 Consecutive
                      if (underSpeedCount > UNDER_SPEED_SAMPLE_COUNT)
                      {
            		      // disable PWM output
                          TI_SMS_MOTOR_PWM_Disable();
                          // disable the hall interrupt
                          Motor_Enable_Disable_Hall_ZeroCross_Interrupt(FALSE);
                          sensorlessBEMF.comutationStateCount = 0;
                 			p->previousState = p->currentState;
                 			p->currentState = MOTOR_STATE_STOP;
                 		    break;
                        }
                    }
            	  else
            	  {
            	      if (underSpeedCount)
            	    	  underSpeedCount = underSpeedCount - 1;
            	  }
              }

            // if this FALSE means motor need to stop
            }while(userSpeedInput.motorStartStop);
            // go to braking state
            if (p->currentState != MOTOR_STATE_STOP)
            {
                p->previousState = p->currentState;
                p->currentState = MOTOR_STATE_BRAKING;
            }
            break;
      
      case MOTOR_STATE_STOP:
    	  gStart_Current_Sampling = FALSE;
    	  // disable pre-driver, enable pre-driver skipping
    	  Port2_Enable_Disable_Pre_Driver(FALSE, FALSE);
    	  gUserInputFilteredValue = 0;
    	  // reset align and go state machines and associated variables
          InitTimer1Relatedvariables();
          // update PWM duty cycle array
   		  TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, 0, 0, 0, 0, 0, 0);
          TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);         
          // disable PWM output
          TI_SMS_MOTOR_PWM_Disable();
          // disable the hall interrupt
          Motor_Enable_Disable_Hall_ZeroCross_Interrupt(FALSE); 
          speedPID.Fdb = 0;
          motorConCurVoltError.loadCurrentValue = 0;
          // enable Buzz internal VCC voltage divider MOSFET
          drv91670SpiResetAfeSpi();
          drv91670SpiReleaseAfeSpi();
          SPI_Write(ASSIMISC2, TURN_ON_VCC_DIVIDER);
          // disable global interrupt
          // __bic_SR_register(GIE);

          do
          {
        	 TI_SMS_MOTOR_UserInPut_UpdateMotorSetSpeed(&userSpeedInput);
             TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
             // enable Buzz internal VCC voltage divider MOSFET
             drv91670SpiResetAfeSpi();
             drv91670SpiReleaseAfeSpi();
             SPI_Write(ASSIMISC2, TURN_ON_VCC_DIVIDER);
     		 TI_SMS_MOTOR_ADC_Chn_Read(&motorVCCSensing); // get motor VCC value
             motorConCurVoltError.motorVCCValue = motorVCCSensing.data;
          }while (!(userSpeedInput.motorStartStop));

#ifdef SENSORED_3_PHASE_BLDC
          TI_SMS_Motor_Switch_Comm_Sequence_And_Interrupt_Table(&pwm3Phase, &userControllerConfig);
#endif
          motorState.motorSetSpeed = userSpeedInput.currentMotorSetSpeed;
          motorConCurVoltError.loadCurrentValue = 0;
          // any reset code needed 
          p->previousState = p->currentState;
          p->currentState = MOTOR_STATE_CHECK_INPUTS;          
          break;
           
      case MOTOR_STATE_BRAKING:
          TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);
    	 //slow down motor speed -> 0
         //depend on compliler braking scheme we are using.
         // too fast breaking may damage motor and board
          // Disable the zero cross interrupts
          Motor_Enable_Disable_Hall_ZeroCross_Interrupt(FALSE);
          // turn all port3 interrupts off
          P3IES =  0x00;
          P3IFG = 0x00;
          P2IE = 0x00;

          // disable all PWM outputs
          TI_SMS_MOTOR_PWM_Disable();
          // update PWM output with minimum PWM output duty cycle
          
          registerValue = pwm3Phase.Period;
          
          switch (userControllerConfig.brakingMethod) // speed up with differnt strength/percentage
          {           
              case BRAKE_25_PERECNT:
                  registerValue = registerValue >> 2;
                  break;
            
              case BRAKE_50_PERCENT:
                  registerValue = registerValue >> 1;
                  break;
            
              case BRAKE_75_PERCENT:
                  registerValue = registerValue >> 1 + registerValue >> 2;
                  break;

              case BRAKE_100_PERCENT:
            	  registerValue = registerValue >> 1 + registerValue >> 2;
                  break;
        	
    	      case NO_BRAKE:
    	      default:
    	          registerValue = 0;
    	          break;
    	  }
          // reset align and go state machines and associated variables
          InitTimer1Relatedvariables();
   		  // update PWM duty cycle array
   		  TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, registerValue,
   		      registerValue, registerValue, registerValue, registerValue, registerValue);
          // update PWM duty cycle
   		  TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
          // Turn on low side UL,VL and WL FETS
          TBCCTL1 = OUTMOD_6;	// ULS PWM
          TBCCTL3 = OUTMOD_6;   // VLS PWM
          TBCCTL5 = OUTMOD_6;   // WLS PWM
          
          for (count = 0; count < BRAKING_LOOP_COUNT; count++)
              DelayCounts(BRAKING_DELAY >> 6);
          registerValue = 0;
          // update PWM duty cycle array
   		  TI_SMS_MOTOR_PWM_Update_Duty_Cycle_Array(&pwm3Phase, registerValue,
   		      registerValue, registerValue, registerValue, registerValue, registerValue);
          TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
          // disable all PWM outputs
          TI_SMS_MOTOR_PWM_Disable();
          TI_SMS_MOTOR_Send_UART_Packet_Response(&uartUCA0);

          // go to stop state
          p->previousState = p->currentState;
          p->currentState = MOTOR_STATE_STOP;
          break;

      case MOTOR_STATE_FAULT_STATE:
        // check if fault is hard fault or soft over current happens, have to stop the motor
        // overVoltage happens, we may be able to keep running the motor.
        //underVoltage happens, we stop the motor.
        //motor spin in reverse direciton, we top the motor as well. 
	      p->previousState = p->currentState;
          p->currentState = MOTOR_STATE_STOP; 
	      break;
	             
	  case MOTOR_STATE_SETUP_MODE:
	      // disable the hall interrupt
          Motor_Enable_Disable_Hall_ZeroCross_Interrupt(FALSE); 
          Port2_Enable_Disable_Pre_Driver(FALSE, FALSE);  
	      gTimerPWMPulseInElapsed = timerA4.ISRCount;
	      
	      pwmInPulseReady = FALSE;
	      
          p->previousState = p->currentState;
          p->currentState = MOTOR_STATE_STOP; 
          break;
                             
	  default:
	  // in case of ant error set main state machine controller to init state
	  p->currentState = MOTOR_STATE_INITIALIZE;
	  break;
    }        
}

void main ()
{
	// Added for IEC60730 support
	IEC60730_RUN_POST_test();
	motorState.currentState = MOTOR_STATE_INITIALIZE;
	motorState.previousState = MOTOR_STATE_INITIALIZE;
    
	while(TRUE)
	{
		main_motor_controller_state_machine(&motorState);
	}
}


void TI_SMS_Motor_Commutation(UINT8 hall_effect_state) // sensorless BLDC motor commutation  
{ 
}
//utility functions 
void TI_SMS_Motor_Increment_Decrement_DutyCycle (MOTOR_USERINPUT_PARAMS_S *userptr, MOTOR_RAMP_CONFIG *ptrRampConfig)
{    UINT16 tempValue=0;
	// increment PWM duty cycle
    if (ptrRampConfig->rampPWMDutyCycle < userptr->pwmOutPutDutyCycle)
    {
    	tempValue = ptrRampConfig->rampPWMDutyCycle + (pwm3Phase.Period >> ptrRampConfig->rampUpValue);

    	if(tempValue > userptr->pwmOutPutDutyCycle)
          ptrRampConfig->rampPWMDutyCycle = userptr->pwmOutPutDutyCycle;
        else
       	  ptrRampConfig->rampPWMDutyCycle = tempValue;
    }
    // decrement PWM duty cycle
    if (ptrRampConfig->rampPWMDutyCycle > userptr->pwmOutPutDutyCycle)
    {
    	tempValue = ptrRampConfig->rampPWMDutyCycle - (pwm3Phase.Period >> ptrRampConfig->rampDownValue);

    	if(tempValue < userptr->pwmOutPutDutyCycle)
    	    ptrRampConfig->rampPWMDutyCycle = userptr->pwmOutPutDutyCycle;
    	else
    	    ptrRampConfig->rampPWMDutyCycle = tempValue;
    }
    // check PWM duty cycle max limit
    if (ptrRampConfig->rampPWMDutyCycle > ptrRampConfig->PWMMaxDutyCycle)
    	ptrRampConfig->rampPWMDutyCycle = ptrRampConfig->PWMMaxDutyCycle;
    // check PWM duty cycle min limit    
    if (ptrRampConfig->rampPWMDutyCycle < ptrRampConfig->PWMMinDutyCycle)
    	ptrRampConfig->rampPWMDutyCycle = ptrRampConfig->PWMMinDutyCycle;
}


// return motor state machine current state
MOTOR_STATUS_S TI_SMS_Motor_Get_Main_Motor_Controller_State_Machine_State (void)
{
    return motorState;
}

