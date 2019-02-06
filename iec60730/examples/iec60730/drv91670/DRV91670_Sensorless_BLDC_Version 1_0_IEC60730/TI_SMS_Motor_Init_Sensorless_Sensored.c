/*=============================================================
 * @file:		TI_SMS_Motor_Init.c
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

#ifndef TI_SMS_MOTOR_INIT_H_
#include "TI_SMS_Motor_Init.h"
#endif
#ifndef TI_SMS_MOTOR_UART_PACKET_API_H_
#include "TI_SMS_Motor_UART_Packet_API.h"
#endif

// Added for IEC60730 support
#include "TI_DRV_IEC60730.h"
extern uint16_t gInterruptCounter;


#define TOTAL_SAMPLE_ARRAY 10
#define TOTAL_SAMPLE_ARRAY_NEW 30
volatile UINT16 motor_electrical_comm_time [TOTAL_SAMPLE_ARRAY_NEW]; 
volatile UINT8 motor_electrical_states [TOTAL_SAMPLE_ARRAY];
volatile unsigned char comm_sample_count = 0;
volatile UINT8 elec_sample_count = 0;
UINT8 gReverseRotationDetected = FALSE;

UINT16 zeroCrossToCommutationDelay;

#define ILIM_AMPS 384				// Ilim setting calculation is as follows for 30A:
 									// DAC max voltage is 2.4V / 1023 or 2.34mV per bit
									// 30A * 1mOhm * Igain(30)/2.34mV = 384

// All look-up tables in the system.
UINT16 PWM_timeOutTable[PWMMAX_PWM_CHANNEL_NUM]={DEFAULT_DUTY_CYCLE,
	                                             DEFAULT_DUTY_CYCLE,
	                                             DEFAULT_DUTY_CYCLE,
	                                             DEFAULT_DUTY_CYCLE,
	                                             DEFAULT_DUTY_CYCLE,
	                                             DEFAULT_DUTY_CYCLE};

const UINT16 PWM_comutationSeqeunce_Anaheim_17V[48] =
{
	OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0,         // case 0  0-5
    OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  // case 1  6-11, A-C
    OUTMOD_0, OUTMOD_6,  OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_0 , // case 2  12-17, B-A
    OUTMOD_0, OUTMOD_0,  OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  // case 3  18-23, B-C
	OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  OUTMOD_0 | BIT2, OUTMOD_0,  // case 4  24-29, C-B
    OUTMOD_0 | BIT2, OUTMOD_0,   OUTMOD_0, OUTMOD_6,   OUTMOD_0, OUTMOD_0,// case 5  30-35, A-B   
    OUTMOD_0, OUTMOD_6,   OUTMOD_0, OUTMOD_0,  OUTMOD_0 | BIT2, OUTMOD_0, //case  6  36-41, C-A
    OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0          // case 7  42-47
};

const UINT16 PWM_comutationSeqeunce_Anaheim_15V[48] =
{
	OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_0,         // case 0  0-5
    OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  // case 1  6-11, A-C   
    OUTMOD_0, OUTMOD_6,  OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_0 , // case 2  12-17, B-A
    OUTMOD_0, OUTMOD_0,  OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_6 , // case 3  18-23, B-C
	OUTMOD_0, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  OUTMOD_0 | BIT2, OUTMOD_0,  // case 4  24-29, C-B
    OUTMOD_0 | BIT2, OUTMOD_0,  OUTMOD_0, OUTMOD_6,  OUTMOD_0, OUTMOD_0,  // case 5  30-35, A-B
    OUTMOD_0, OUTMOD_6,   OUTMOD_0, OUTMOD_0,  OUTMOD_0 | BIT2, OUTMOD_0, //case  6  36-41, C-A
    OUTMOD_0, OUTMOD_0, OUTMOD_0, OUTMOD_0, OUTMOD_0, OUTMOD_0            // case 7  42-47
};

const UINT16 PWM_commutationInterruptSequence_Anaheim_15V[8]=    // P3IES values for next commutation sequence( edge interrupt)
{
	0x00, /* P3IES, P3IFG case 0 */
    0x00, /* case 1 */
    0x00, /* case 2 */
    0x02, /* case 3 */
    0x00, /* case 4 */
    0x08, /* case 5 */
    0x04, /* case 6 */
    0x00, /* case 7 */
};

const UINT16 PWM_commutationInterruptSequence_Anaheim_17V_CW[8]=    // P3IES values for next commutation sequence
{
    PHW_INT_LO | PHV_INT_LO | PHU_INT_LO, /* P3IES, P3IFG case 0 */
    PHW_INT_HI | PHV_INT_HI | PHU_INT_LO,  /* case 1 */
    PHW_INT_HI | PHV_INT_LO | PHU_INT_HI,  /* case 2 */
    PHW_INT_HI | PHV_INT_LO | PHU_INT_LO, /* case 3 */
    PHW_INT_LO | PHV_INT_HI | PHU_INT_HI, /* case 4 */
    PHW_INT_LO | PHV_INT_HI | PHU_INT_LO, /* case 5 */
    PHW_INT_LO | PHV_INT_LO | PHU_INT_HI, /* case 6 */
    PHW_INT_LO | PHV_INT_LO | PHU_INT_LO, /* case 7 */
};

const UINT16 PWM_commutationInterruptSequence_Anaheim_17V_CCW[8]=    // P3IES values for next commutation sequence
{
    0x00, /* P3IES, P3IFG case 0 */
    0x00, /* case 1 */
    0x00, /* case 2 */
    0x02, /* case 3 */
    0x00, /* case 4 */
    0x08, /* case 5 */
    0x04, /* case 6 */
    0x00, /* case 7 */
};

const UINT16 PWM_commutationInterruptSequence_Anaheim_15V_CW[8]=    // P3IES values for next commutation sequence( edge interrupt)
{
	0x00, /* P3IES, P3IFG case 0 */
    0x00, /* case 1 */
    0x00, /* case 2 */
    0x02, /* case 3 */
    0x00, /* case 4 */
    0x08, /* case 5 */
    0x04, /* case 6 */
    0x00, /* case 7 */
};

const UINT16 PWM_commutationInterruptSequence_Anaheim_15V_CCW[8]=    // P3IES values for next commutation sequence( edge interrupt)
{
	0x00, /* P3IES, P3IFG case 0 */
    0x00, /* case 1 */
    0x00, /* case 2 */
    0x04, /* case 3 */
    0x00, /* case 4 */
    0x02, /* case 5 */
    0x08, /* case 6 */
    0x00, /* case 7 */
};

// this is for SENSORLESS operation. saving the present & previous comm states//
BEMF_PARAM_S sensorlessBEMF =
{
 	1,// UINT16 currentBackEMFState;
    2,// UINT16 previouBackEMFState;
    FALSE, // UINT16 comutationLeadAngleFlag; 
    FALSE,// UINT16 readyForBackEMF; 
    30,// UINT16 leadAngleDelay;	  
     0,//  UINT16 zeroCrossTimerInterval;
     0,//  UINT16 *leadAngleTable;	
     0,//  UINT16 comutationStateCount;
     0, //  UINT16 zeroCrossIntrruptCount; initialization
     FALSE, // UINT8  lastStateSaved;
};     



PWM_PARAM_S pwm3Phase =
{
    PWM_timeOutTable, //DutyCycle[MAX_PWM_CHANNEL_NUM]
 	1250,//1250,//1250,//Period timer B count, 1250 count represents 50uS
 	10,//deadTime for complementary PWM
 	10,// flyBackTime; delay interrupt enabling to prevent false trigger
 	ID_0,//clockDivider. 25MHz master clock divider
 	0x0,//clockDividerExtention
 	TBSSEL_0, // clockSource. main clock
#ifdef SENSORED_3_PHASE_BLDC
 	PWM_comutationSeqeunce_Anaheim_17V, // commuatation pair selection
 	PWM_commutationInterruptSequence_Anaheim_17V_CCW, // this set next hall interrupt
#endif
};

MOTOR_STATUS_S motorState = 
{ 
	MOTOR_STATE_INITIALIZE, //currentState
	MOTOR_STATE_INITIALIZE, //previousState
	10000                  // motorSetSpeed, motor set target speed in RPM
};

ADC_CHN_PARAMS_S analogPotSpeedIn =
{ 
	ADC_ANALOG_POT_FOR_TORQZ, // selected ADC channel range: [0,15]
	0, // return ADC data after * gain + offset
	FALSE, // data is ready or not.
	ADC_ANALOG_POT_FOR_TORQZ, //ADC_ANALOG_POT_SPEED_IN, // linked channel. not supported for now.
	 39, // this is 1250/1023 in Q 10.5 formate 
	 0
};

ADC_CHN_PARAMS_S motorCurrentSensing =
{
	ADC_ANALOG_POT_FOR_TORQZ_CURRENT, // selected ADC channel range: [0,15]
	0, // return ADC data after * gain + offset
	FALSE, // data is ready or not.
	ADC_ANALOG_POT_FOR_TORQZ_CURRENT, //ADC_ANALOG_POT_SPEED_IN, // linked channel. not supported for now.
	 39, // this is 1250/1023 in Q 10.5 formate
	 0
};

ADC_CHN_PARAMS_S motorVCCSensing =
{
	ADC_ANALOG_VCC_FOR_BUZZ, // selected ADC channel range: [0,15]
	0, // return ADC data after * gain + offset
	FALSE, // data is ready or not.
	ADC_ANALOG_VCC_FOR_BUZZ, //ADC_ANALOG_POT_SPEED_IN, // linked channel. not supported for now.
	 39, // this is 1250/1023 in Q 10.5 formate
	 0
};

TIMERA_RARAM_S timerA4 = 
{ 
	/*&timerA1Reg,* generate software timer */ 
	TimerA4,
	ID_3,       //cpuFrequenceyDivid; 
	0xFFFF, //periodCount;
	0x0, //TimerCount;
    10, //ISRCount
    1  //clockDvided
};

MOTOR_ELECTRICAL_SPEED motorElectricalSpeed =
{
    FALSE,//speedReady
    0,    // stateCount
    18,    // multiplier, use to multiply 1 mechanical rotaion time, to get correct 1 mech time
    0,    //feedBackCount
    0,    //startTimerCount
    0,    //endTimerCount
    20000,	  // timerInterval
};

AlignnGo_PARAM_S alignGo={
	
	0, //UINT8   rotorPosition; // return value after go runs
    4, //UINT32  alignDelay;
    200,//	UINT16  alignDutyCycle;
    1700*12,//1700*15,//UINT16  goDelay;
    20,//UINT8   goLoops;
    100*10,//100*5,// 15*10,//UINT16   goRamp; // go time decrental factor...
    200,//UINT16  goDutyCycle;
    &PWM_comutationSeqeunce_Anaheim_15V[0],//UINT16 *comSequence;
 	FALSE, //UINT8 CallAlignStateMachine; // set when we are in align and go state
 	0, //UINT8 align_comm_index; // this saved last valid align index
};

FAULT_CURRENT_PARAMS_S motorCurrentSetting = {
	   0xFF,  //UINT8 currentLsbValue;
	   0x03,  //UINT8 currentMsbValue;
	   FALSE, //UINT8 pwmPulseSkipEnabled;     // set TRUE will enable PWM pulse skiping.
	   FALSE,  // UINT8 overCurrentIntEnabled; // enabled over current interrupt
};
                
MOTOR_USERINPUT_PARAMS_S userSpeedInput = {
       200,    //UINT32 minMotorSetSpeed
	   15000,    //UINT32 maxMotorSetSpeed
	   0,        //UINT32 currentMotorSetSpeed
	   100,      //UINT16 motorStartThersold
	   10,      //UINT16 deadBandCount;         // use b/w start and stop switching
       FALSE,    //UINT8  motorStartStop;
       TRUE,     //UINT8  motorPowerUp;          // flag set when 1st time powerup   
       0,        //UINT16 pwmOutPutDutyCycle;    // pwm output duty cycle w.r.t pwm pulse in duty cycle
};

UART_PARAMS_S uartUCA0 = {
	        UCSSEL_2,    //UINT16  uartClk;
	            0x2C,//0xD9//0x8B,    //UINT8  baudRateDivLSB; // baud rate UCAxBR0 value 9600 BPS
	            0x0A,//0x00//0x02,    //UINT8  baudRateDivMSB; // baud rate UCAxBR1 value
	            0x00,    //UINT8  modControl; // modulation control UCAxMCTL
      UART_8BIT_DATA,    //UINT8  dataBits;
     UART_1_STOP_BIT,    //UINT8  stopBits;
                0x00,    //UINT8  paritySelect;
                0x00,         //UART_PARITY_ENABLED,//UART_PARITY_DISABLED,   //UINT8  parityEnable;
                UCRXIE,    //UINT8  rxIntEnable //UCRXIE
                UCTXIE,    //UINT8  txIntEnable //UCTXIE
                0x00,    //UINT8  txComplete;  // set TRUE when all data bytes were transmitted
                   0,    //UINT8  txBytesCount;  // total bytes need to tranmit
                   0,    //UINT8  txBuffer[UART_TX_BUFFER_SIZE]; // this buffer need to fill for transmission
                   0,    //UINT8  rxBuffer[UART_RX_BUFFER_SIZE]; // buffer filled during reception of data from UART
                   0,    //UINT8  rxBytesCount;  // totaol bytes received so far
    UART_RX_STATE_INIT_FIRST_MARKER,    //rx_receive_packet_state
               FALSE,    //rxPacketComplete; // set when we received valid packet
               FALSE,    //UINT8  rxPacketStarted; // set when we received 1st valid packet byte
               FALSE,    //UINT8  validPacketReceived; // set it to TRUE when valid data received
};

MOTOR_USERINPUT_PWM_PARAMS_S pwmPulseIn = {
	1,	//float dutyCycleRangeFactor;
	0,      //UINT32 pwmPulsePeriodInNSec;      // incoming PWM pulse in nano seconds
    0,      //UINT32 pwmPulseDutyCycleNSec;     // incoming PWM pulse in nano seconds  
	0,      //UINT32 pwmPulseScaledDutyCycleNSec;     // scaled output
	1000000, //PWM_IN_1MS,      //UINT32 dutyCycleLowLimit;
    2000000, //PWM_IN_2MS,      //UINT32 dutyCycleHigLimit;
    0,      //UINT16 pwmPulseInDutyCyle;        // PWM pulse in duty cycle
	0,      //UINT16 pwmPulseInDutyCycleCount;  // PWM pulse in duty cycle in terms of counter counts
	1,      //UINT16 pwmPulseInPeriodCount;     // PWM pulse in period in terms of counter counts
	0,      //UINT16 pwmPulseInDataReady;       // set when PWM pulse in data ready             
    FALSE,  //UINT8  pwmPulseInError;           // set when PWM pulse in erorr occours
    FALSE,  //UINT8  rescalingEnabled;          // set when rescaling   
    TRUE, //FALSE,   //UINT8  usePWMPulseIn;   
};


// use to initialize user required parameters
MOTOR_CONTROLLER_CONFIG userControllerConfig = {
	LEAD_ANGLE_5_DEGREES,  //MOTOR_LEAD_ANGLE commutationLeadAngle;
    BRAKE_50_PERCENT,     //MOTOR_BRAKING_SCHEME brakingMethod;
    PWM_FREQUENCY_32KHZ, // PWM_FREQUENCY_16KHZ,   //MOTOR_PWM_FREQUENCY pwmFrequency;
    0,                  //UINT16 commutaionFreq;
    0,                     //UINT16 batteryVolatge;    // battery voltage
    0,                     //UINT16 temperatureReading; // temperature sensor value     
    DUTY_CYCLE_INCREMENT_2_PERCENT, //UINT16 dutyCycleIncrementValue; // use for duty cycle increment-decrement      
    NUM_OF_MOTOR_POLES, //UINT8 motorpoles;  // use to calculate motor mechanical speed
    MOTOR_RUNNING_CW, //UINT8 motorDirection; // if set to 0 its CW direction, if 1 its CCW
    HALL_SENSOR_MODE_A, //UINT8 hallSensorMode; // its for selecting different hall sensor sequences
    MOTOR_STOP_FLAG, //UINT8 motorStartStopFlag; // flag set when motor need to start
    MOTOR_MAX_DEFAULT_SPEED, //UNIT16 motorMaxSpeedFromGUI; // max speed received from GUI
    GUI_DIAL_DEFAULT_PERCENATGE, //UNIT16 motorCurrentSpeedDailPercent; // GUI dial percentage value
    DEFAULT_MAX_VOLTAGE, //UINT16 motorMaxVoltageSetting; // max default voltage
    DEFAULT_MAX_CURRENT, //UINT16 motorMaxCurrentSetting; // max default current
#ifdef SENSORLESS_3_PHASE_BLDC
    CONTROLLER_MODE_SENSORLESS, //UINT16 motorControllerMode; // use for Motor controller selection
#else
    CONTROLLER_MODE_SENSORED,
#endif
};

// use to initialize user required parameters
MOTOR_CONTROLLER_CONFIG userControllerConfigDefault = {
	LEAD_ANGLE_5_DEGREES,  //commutationLeadAngle;
    BRAKE_25_PERECNT,     //MOTOR_BRAKING_SCHEME brakingMethod;
    PWM_FREQUENCY_32KHZ,   //MOTOR_PWM_FREQUENCY pwmFrequency;
    0,                     //UINT16 batteryVolatge;    // battery voltage
    0,                     //UINT16 temperatureReading; // temperature sensor value     
    0,
    DUTY_CYCLE_INCREMENT_2_PERCENT, //UINT16 dutyCycleIncrementValue; // use for duty cycle increment-decrement      
    NUM_OF_MOTOR_POLES, //UINT8 motorpoles;  // use to calculate motor mechanical speed
    MOTOR_RUNNING_CW, //UINT8 motorDirection; // if set to 0 its CW direction, if 1 its CCW
    HALL_SENSOR_MODE_A, //UINT8 hallSensorMode; // its for selecting different hall sensor sequences
    MOTOR_STOP_FLAG, //UINT8 motorStartStopFlag; // flag set when motor need to start
    MOTOR_MAX_DEFAULT_SPEED, //UNIT16 motorMaxSpeedFromGUI; // max speed received from GUI
    GUI_DIAL_DEFAULT_PERCENATGE, //UNIT16 motorCurrentSpeedDailPercent; // GUI dial percentage value
    DEFAULT_MAX_VOLTAGE, //UINT16 motorMaxVoltageSetting; // max default voltage
    DEFAULT_MAX_CURRENT, //UINT16 motorMaxCurrentSetting; // max default current
#ifdef SENSORLESS_3_PHASE_BLDC
    CONTROLLER_MODE_SENSORLESS, //UINT16 motorControllerMode; // use for Motor controller selection
#else
    CONTROLLER_MODE_SENSORED,
#endif
};

PI_PARAM_S_NEW speedPID =
{
  4000,//  INT16 Ref;   		     Input: Reference input
   0,//  INT16 Fdb;   	    	 Input: Feedback input
   0,    //  SINT32 Err;		     	 Variable: Error
   10, //3,//	UINT16 Kp;		value = 1	     Parameter: Proportional gain use Q10.5 shit left by 5
    0,//	SINT32 Up;			     Variable: Proportional output
    3,//1,//32,//UINT16 Ki;		value = 0.1 	     Parameter: Integral gain use Q10.5 shit left by 5
    100,//200, //	SINT32 Ui;			     Variable: Integral output
    0,//  SINT32 OutPreSat; 	     Variable: Pre-saturated output
    600,//1250,//	SINT32 OutMax;		     Parameter: Maximum output limit
     100,//	SINT32 OutMin;	         Parameter: Minimum output limit
     0,//	SINT32 Out;   		     Output: PID output
     0,//	SINT32 Up1;		   	     History: Previous proportional output
     0 //	SINT32 Uip;				 Variable: Previous Integral output
};

PI_PARAM_S_NEW default_speedPID =
{
  4000,//  INT16 Ref;   		     Input: Reference input
   0,//  INT16 Fdb;   	    	 Input: Feedback input
   0,    //  SINT32 Err;		     	 Variable: Error
   10, //3,//	UINT16 Kp;		value = 1	     Parameter: Proportional gain use Q10.5 shit left by 5
    0,//	SINT32 Up;			     Variable: Proportional output
    3,//1,//32,//UINT16 Ki;		value = 0.1 	     Parameter: Integral gain use Q10.5 shit left by 5
    100,//200, //	SINT32 Ui;			     Variable: Integral output
    0,//  SINT32 OutPreSat; 	     Variable: Pre-saturated output
    600,//1250,//	SINT32 OutMax;		     Parameter: Maximum output limit
     100,//	SINT32 OutMin;	         Parameter: Minimum output limit
     0,//	SINT32 Out;   		     Output: PID output
     0,//	SINT32 Up1;		   	     History: Previous proportional output
     0 //	SINT32 Uip;				 Variable: Previous Integral output
};

MOTOR_RAMP_CONFIG motorRampUpDownConfig =
{
	SLOW_SPEED_RAMP_SLOPE_VALUE,  //UINT16 rampUpValue; // its for motor ramp up
	SLOW_SPEED_RAMP_SLOPE_VALUE,  //UINT16 rampDownValue; // its for motor ramp down
    0,                            //UINT16 rampPWMDutyCycle; // used as low pass filter to smooth pwmOutPutDutyCycle
};

// use to initialize sensorless motor startup code
MOTOR_SENSORLESS_STARTUP_CONFIG motorSensorlessStartUp =
{
	ALIGN_TO_MOTOR_STATE_1, //UINT16 motor_align_phase; // its motor align phase for sensorless mode
	ZERO_CROSS_WINDOW_MAX_TIME, //UINT16 zerocorss_det_window; // use to this delay to detect zero cross
	REALIGN_TIMER_DELAY_MAX_COUNT, //UINT16 realign_window; // use this delay for next realign of motor phase
	SENSORLESS_DEFAULT_STARTUP_DUTYCYCLE //UINT16 sensorless_startup_dutycycle; // use for starting motor in sensorless mode
};

CONTROLLER_CUR_VOL_ERROR_FLAGS motorConCurVoltError =
{
	0, //UINT16 loadCurrentValue;    // use for motor load current
	0, //UINT16 motorVCCValue;    // use for motor VCC
	0, //&controllerCurVccErFlags //CONTROLLER_ERROR_FLAGS controllerErorFlags;    // use for motor controller related errors
};

#define CONFIG_BYTE_0_ADD 0x00
#define CONFIG_BYTE_1_ADD 0x01
#define CONFIG_BYTE_2_MAX_PWM_PULSE_WIDTH_ADD 0x02
#define CONFIG_BYTE_3_MIN_PWM_PULSE_WIDTH_ADD 0x03
#define CONFIG_BYTE_4_PWM_FREQ_ADD 0x04
#define CONFIG_BYTE_5_BRAKE_METHOD_ADD 0x05
#define CONFIG_BYTE_6_LEAD_ANGLE_ADD 0x06
#define CONFIG_BYTE_6_BAT_UVOLT_ADD 0x07
#define CONFIG_BYTE_6_OVER_CURRENT_ADD 0x08
#define CONFIG_BYTE_A_MOTOR_STARTUP_DUTYCYCLE 0x0A

// use to switch hall sensor sequence and hall interrupt settings too
UINT32 TI_SMS_Motor_Switch_Comm_Sequence_And_Interrupt_Table (PWM_PARAM_S *p, MOTOR_CONTROLLER_CONFIG *config_ptr)
{
	UINT32 errCode = 0;

	switch (config_ptr->hallSensorMode)
    {
    	case HALL_SENSOR_MODE_A:
    	    if (config_ptr->motorDirection)
    	    	p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CW[0];
    	    else
    	    	p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CCW[0];
    		p->comSequence = &PWM_comutationSeqeunce_Anaheim_17V[0];
    		break;

    	case HALL_SENSOR_MODE_B:
    		p->comSequence = &PWM_comutationSeqeunce_Anaheim_15V[0];
    		if (config_ptr->motorDirection)
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_15V_CW[0];
    		else
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_15V_CCW[0];
    	    break;

    	case HALL_SENSOR_MODE_C:
    		p->comSequence = &PWM_comutationSeqeunce_Anaheim_17V[0];
    		if (config_ptr->motorDirection)
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CW[0];
    		else
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CCW[0];
    		break;

    	case HALL_SENSOR_MODE_D:
    		p->comSequence = &PWM_comutationSeqeunce_Anaheim_17V[0];
    		if (config_ptr->motorDirection)
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CW[0];
    		else
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CCW[0];
    		break;


    	default:
    		p->comSequence = &PWM_comutationSeqeunce_Anaheim_17V[0];
    		if (config_ptr->motorDirection)
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CW[0];
    		else
    			p->interruptSequence = &PWM_commutationInterruptSequence_Anaheim_17V_CCW[0];
    		break;
    }
	return errCode;
}
// returns pointer to current motor config
MOTOR_CONTROLLER_CONFIG* TI_SMS_Init_Get_Motot_Config (void)
{
	return &userControllerConfig;
}

// motor beep function, unser need to restore the old PWM value
UINT32 TI_SMS_Motor_Beep(UINT8 fetPairSel, UINT32 onTime, UINT32 offTime, UINT16 period)
{
    UINT32 errorCode = 0;
    return errorCode;
}

UINT32 Init_Motor_Config_From_FRAM_Default (MOTOR_CONTROLLER_CONFIG* config, 
    MOTOR_CONTROLLER_CONFIG* default_config)
{
    UINT8 useDefault = TRUE;
    volatile UINT8 temp;
    UINT32 errorCode = 0;
    float floatTemp;
    
    pwmPulseIn.dutyCycleHigLimit = 2000000;
    pwmPulseIn.dutyCycleLowLimit = 1000000;
    floatTemp =(float)PWM_IN_DUTYCYCLE_STANDARED_RANGE_NS/(float)(pwmPulseIn.dutyCycleHigLimit-pwmPulseIn.dutyCycleLowLimit);
    floatTemp = floatTemp*256;
    pwmPulseIn.dutyCycleRangeFactor=(UINT16)floatTemp;

    pwmPulseIn.dutyCycleHigLimit = 2000000;
    pwmPulseIn.dutyCycleLowLimit = 1000000;
    floatTemp =(float)PWM_IN_DUTYCYCLE_STANDARED_RANGE_NS/(float)(pwmPulseIn.dutyCycleHigLimit-pwmPulseIn.dutyCycleLowLimit);
    floatTemp = floatTemp*256;
    pwmPulseIn.dutyCycleRangeFactor=(UINT16)floatTemp;

    if (useDefault)
    {
        config->brakingMethod = default_config->brakingMethod;
        config->pwmFrequency = default_config->pwmFrequency;
        config->commutationLeadAngle = default_config->commutationLeadAngle;	
    }
    return errorCode;	
}

// initialize all sensorless related struct
UINT32 Init_Motor_Sensorless_Related_Variables (MOTOR_CONTROLLER_CONFIG* config)
{
    UINT32 errorCode = 0;

    switch (config->pwmFrequency)
    {           
        case PWM_FREQUENCY_16KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_16KHZ;   //PWM_PERIOD_COUNTS_16KHZ;
            break;
            
        case PWM_FREQUENCY_32KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_32KHZ;    
            break;

        case PWM_FREQUENCY_12KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_12KHZ;
            break;

        case PWM_FREQUENCY_20KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_20KHZ;
            break;

        case PWM_FREQUENCY_24KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_24KHZ;
            break;

        case PWM_FREQUENCY_28KHZ:
            pwm3Phase.Period = PWM_PERIOD_COUNTS_28KHZ;
            break;

    	case PWM_FREQUENCY_8KHZ:
    	default:
    	    pwm3Phase.Period = PWM_PERIOD_COUNTS_8KHZ;   //PWM_PERIOD_COUNTS_8KHZ;
    	    break;
    }

    if (config->dutyCycleIncrementValue > DUTY_CYCLE_INCREMENT_16_PERCENT)
        config->dutyCycleIncrementValue = DUTY_CYCLE_INCREMENT_16_PERCENT;     
    
    return (errorCode);
}
                                               
// disable hall port interrupt 
void Motor_Enable_Disable_Hall_ZeroCross_Interrupt (unsigned char flag)
{
    if (flag)	
	    P3IE |= 0x0e;		    			           // P3.1, P3.2 and P3.3 interrupt is enabled
    else
        P3IE = 0x00;
}

// use to generate dummy port interrupt	
void Init_Motor_Starting_Sequence (void)
{
    volatile UINT8 count;
    
    P3IFG = 0x0E;
    __delay_cycles(START_INTERRUPT_DELAY);
}

/* Switches MCLK and SMCLK to 25Mhz. Since MCLK is changed, the FRAM access and precharge time
 * are also changed. */
void Clock_Switch(void)
{
	CCSCTL0 = SCSKEY + WSEN + DIVA__8 + DIVS__1 + DIVM__1; // SMCLK =  25Mhz = MCLK
}

void Port3_Init_For_Sensored_Sensorelss_Operation (void)
{
	volatile UINT8 registerValue = 0;

    P3DIR &= 0xF1;    // define P3.1, P3.2, P3.3 as an input

#ifdef _DEVICE_DRV_91520_ || _DEVICE_DRV_91168_ 	
    registerValue = SPI_Read (ASSIHALLSNS);
    spi_Write(ASSIHALLSNS, registerValue | 0x02);    // enable 3 phase hall sensors input, HS1, HS2, HS3
#endif

#ifdef _DEVICE_DRV_91620_ || _DEVICE_DRV_92200 || _DEVICE_DRV_92250
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    registerValue = SPI_Read(ASSIHALLSNS); 
#ifdef SENSORED_3_PHASE_BLDC 
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    SPI_Write(ASSIHALLSNS, registerValue & 0xFC);                                               // 0x0A is Hall Sns/Phase Cmp register address
#endif

#ifdef SENSORLESS_3_PHASE_BLDC 
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    SPI_Write(ASSIHALLSNS, (registerValue & 0xFC) | 0x01);    // enable sensorless operation                                               // 0x0A is Hall Sns/Phase Cmp register address
#endif
#endif                                             // 0x02 is for slecting 3 phase sensing mode 
}

void Port2_Enable_Disable_Pre_Driver (UINT8 enable_output, UINT8 skipingPWM)
{
    volatile UINT8 registerValue = 0;
    
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    registerValue = SPI_Read(ASSIICOMP);
    
    if (enable_output)
    {
        drv91670SpiResetAfeSpi();
        drv91670SpiReleaseAfeSpi();
        SPI_Write(ASSIICOMP, PRE_DRIVER_ON | registerValue | 0x04);    // enable/disable analog die output buffer [U,V,W]HDR & [UVW]LDR
    }
    else
    {
        drv91670SpiResetAfeSpi();
        drv91670SpiReleaseAfeSpi();
        SPI_Write(ASSIICOMP, (registerValue & 0x7F) | 0x04 );    // disable analog die output buffer [U,V,W]HDR & [UVW]LDR
    }
}

#ifdef SYSTEM_DEBUG
void Port4_Bit_0_Init_For_Debugging (void)
{
    P4DIR |= 0x01;                                 // bit 0 dir as output
	P4OUT &= 0xFE;                                // bit 0 set to low
}

// use to turn port4 bit 0 led off-on
void Port_4_Bit_0_Toggle (unsigned char toggle)
{
	if (toggle & 0x01)                                  // check for TRUE and FALSE  
	    P4OUT |= 0x01;                           // set P4.0 if toggle is non zero
	else
	    P4OUT &= 0xFE;                           // clear P4.0 if toggle is zero 
}

void Port1_Bit_4_Init_For_Debugging (void)
{
    P1DIR |= 0x10;                                 // bit 4 dir as output
	P1OUT &= 0xEF;                                // bit 4 set to low
}

// use to turn port1 bit 4 led off-on
void Port_1_Bit_4_Toggle (unsigned char toggle)
{
	if (toggle)                           // check for TRUE and FALSE  
	    P1OUT |= 0x10;                           // set P1.4 if toggle is non zero
	else
	    P1OUT &= 0xEF;                           // clear P1.4 if toggle is zero 
}

void Port1_Bit_5_Init_For_Debugging (void)
{
    P1DIR |= 0x20;                                 // bit 5 dir as output
	P1OUT &= 0xDF;                                // bit 5 set to low
}

// use to turn port1 bit 5 led off-on
void Port_1_Bit_5_Toggle (unsigned char toggle)
{
	if (toggle)                           // check for TRUE and FALSE  
	    P1OUT |= 0x20;                           // set P1.5 if toggle is non zero
	else
	    P1OUT &= 0xDF;                           // clear P1.5 if toggle is zero 
}

// use to turn port4 bit 6 power on LED
void Port1_Bit_6_Init_For_Debugging (void)
{
    P1DIR |= 0x40;                                 // bit 6 dir as output
	P1OUT &= 0xBF;                                // bit 6 set to low
}

// use to turn port1 bit 6 led off-on
void Port_1_Bit_6_Toggle (unsigned char toggle)
{
	if (toggle & 0x01)                           // check for TRUE and FALSE
	    P1OUT |= 0x40;                           // set P1.6 if toggle is non zero
	else
	    P1OUT &= 0xBF;                           // clear P1.6 if toggle is zero
}

#endif

// initialize port b as timer output
void Port2_Init_For_Motor_Control_Timer_Output (void)
{
    P2DIR |= 0x7E;    // Set P2.1, P2.2, P2.3, P2.4, P2.5 and P2.6 as an output
    P2SEL |= 0x7E;    // Port 2 used as timer outputs
}

// return total electrical states in one mechanical rotation
inline UINT16 Calculate_Motor_Electrical_State_From_Motor_Poles (MOTOR_CONTROLLER_CONFIG *p)
{
    return (p->motorpoles + p->motorpoles + p->motorpoles);
}

// calculate motor electrical speed
UINT32 Calculate_Motor_Electrical_Speed (MOTOR_ELECTRICAL_SPEED *p)
{
    UINT32 errorCode = 0;
    
    if (p->stateCount ==
        Calculate_Motor_Electrical_State_From_Motor_Poles(&userControllerConfig))
    {
    	TI_SMS_MOTOR_TIMER_Read(&timerA4);
        p->endTimerCount = timerA4.ISRCount;
        p->endTimerCount = (p->endTimerCount << 16) + timerA4.TimerCount;
        p->timerInterval = ((p->endTimerCount - p->startTimerCount) >> 4) * (p->multiplier);
        p->stateCount = 0;	                
        p->speedReady = TRUE;
     }
     
    if (p->stateCount == 0)
    {
    	p->startTimerCount = timerA4.ISRCount;
        p->startTimerCount = (p->startTimerCount << 16) + timerA4.TimerCount;
    }
                   
    p->stateCount = p->stateCount + 1;
    
    return (errorCode);
}

// calculate motor electrical speed
UINT32 Calculate_Motor_Electrical_Speed_In_Sensotrless (MOTOR_ELECTRICAL_SPEED *p, UINT16 commutationCount)
{
    UINT32 errorCode = 0;
    
    if (p->stateCount ==
        Calculate_Motor_Electrical_State_From_Motor_Poles(&userControllerConfig))
    {
        p->timerInterval = (p->startTimerCount); // divide by 8
        p->stateCount = 0;
        p->startTimerCount = 0;	                
        p->speedReady = TRUE;
     }
    else if (p->stateCount == 0)
    {
    	p->startTimerCount = commutationCount;
    }
    else
    {
        p->startTimerCount = p->startTimerCount + commutationCount;        
    }               
    p->stateCount = p->stateCount + 1;
    
    return (errorCode);
}

// initialize all pwm period and duty cycle related struct with period percentage
UINT32 Init_Motor_PWM_Period_Related_Variables (void)
{
    UINT32 errorCode = 0;
#ifdef USE_DUTYCYCLE_WRT_PERIOD_COUNT  
    float temp_value;
    
    // convert all PID data w.r.t PWM period   
    temp_value = pwm3Phase.Period;
    temp_value = temp_value * (PID_MAXOUT_PERECNT_WRT_PWM_PERIOD * 0.01);
    if (temp_value < 1)
        temp_value = 1;
    motorRampUpDownConfig.PWMMaxDutyCycle = temp_value;
    
    temp_value = pwm3Phase.Period;
    temp_value = temp_value * (PID_MINOUT_PERECNT_WRT_PWM_PERIOD * 0.01);
    
    if (temp_value < 1)
        temp_value = 1;    
    motorRampUpDownConfig.PWMMinDutyCycle = temp_value;
   
    temp_value = pwm3Phase.Period;
    temp_value = temp_value * (PID_INTEGRAL_INITAIL_VALUE_PERECNT_WRT_PWM_PERIOD * 0.01);
    if (temp_value < 1)
        temp_value = 1;
    motorRampUpDownConfig.rampPWMDutyCycle = temp_value;
    
    temp_value = pwm3Phase.Period; 
    temp_value = temp_value * (DEFAULT_PWM_DUTY_CYCLE_IN_PERCENT * 0.01); 
    if (temp_value < 1)
        temp_value = 1;
    pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE] = temp_value;
    pwm3Phase.DutyCycle[PWM_U_LOW_SIDE] = temp_value; 
    pwm3Phase.DutyCycle[PWM_V_HIGH_SIDE] = temp_value; 
    pwm3Phase.DutyCycle[PWM_V_LOW_SIDE] = temp_value;
    pwm3Phase.DutyCycle[PWM_W_HIGH_SIDE] = temp_value;
    pwm3Phase.DutyCycle[PWM_W_LOW_SIDE] = temp_value;
#endif
    return (errorCode);
}
// initialize timer for commutation delay time and also for electrical speed measurement
void Motor_Control_Init_Start_Timer_A2_For_Commutation_Delay (void)
{
	TA2CTL = 0x0000;        	// timer halted
	TA2R = 0x0000;          	// clear counter	 		
	TA2CTL = TASSEL_2 + MC__CONTINOUS + ID__8; // start TA0	
}

// initialize timer for commutation delay time and also for electrical speed measurement
void Motor_Control_Init_Start_Timer_A3_For_Commutation_Delay (void)
{
	TA3CTL = 0x0000;        	// timer halted
	TA3R = 0x0000;          	// clear counter	 		
	TA3CTL = TASSEL_2 + MC__CONTINOUS + ID__8; // start TA3	
}

UINT32 motor_sys_init(void)
{
    UINT32 errorCode = 0;
 	volatile UINT16 Registervalue = 0;
 	volatile UINT8 temp_value = 0;

 	// Stop watchdog timer
 	WDTCTL = WDTPW + WDTHOLD; 
    // setting PIN 8 to be only a NMI not to function 
    SFRRPCR |= SYSNMI ;
 	Clock_Switch();
 	SPI_Init();
    // disable pre-driver, enable pre-driver skipping
    Port2_Enable_Disable_Pre_Driver(FALSE, FALSE); 	
 	// off P2 output
 	P2OUT = 0x00;
    P2SEL = 0x00;
    P2DIR = 0x7E;
 	// use for debugging port4 bit 0
#ifdef SYSTEM_DEBUG
 	Port4_Bit_0_Init_For_Debugging();
 	Port1_Bit_4_Init_For_Debugging();
 	Port1_Bit_5_Init_For_Debugging();
    Port_1_Bit_5_Toggle(FALSE);
#endif
 	// init port3 and AFE for processing hall sensor inputs
    Port3_Init_For_Sensored_Sensorelss_Operation();
    // do spi call to AFE to enable VBST to 6.8 v
    // initialize port b as timer output
    Port2_Init_For_Motor_Control_Timer_Output();
    // initialize PWM module
    TI_SMS_MOTOR_PWM_Init(&pwm3Phase);
    // init align and go state machine and related variables
    InitTimer1Relatedvariables();
    Motor_Enable_Disable_Hall_ZeroCross_Interrupt(FALSE);   
    // disable pre-driver, enable pre-driver skipping
    Port2_Enable_Disable_Pre_Driver(FALSE, FALSE);

    // enable global interrupt            
    __bis_SR_register(GIE); 
    TI_SMS_MOTOR_TIMERA_Init(&timerA4);
    TI_SMS_MOTOR_TIMER_Start(&timerA4);
    // init ADC
    TI_SMS_MOTOR_ADC_Init(); 
    // enable Buzz internal VCC voltage divider MOSFET
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    SPI_Write(ASSIMISC2, TURN_ON_VCC_DIVIDER);
    sensorlessBEMF.zeroCrossIntrruptCount = 0;
    Init_Motor_Config_From_FRAM_Default(&userControllerConfig, &userControllerConfigDefault);
    // initialize all sensorless related struct
    Init_Motor_Sensorless_Related_Variables(&userControllerConfig);
    // init all PWM related variables w.r.t PWM period count
    Init_Motor_PWM_Period_Related_Variables();
    // initialize PWM module
    TI_SMS_MOTOR_PWM_Init(&pwm3Phase);
    // Enable current limit
    TI_SMS_MOTOR_Fault_Init(&motorCurrentSetting); 
    // init user speed input
    TI_SMS_MOTOR_UserInPut_Init(&userSpeedInput);    
    // use to initialize PI controller values
    TI_SMS_MOTOR_PI_Control_Init(&speedPID);

    // init UART
    TI_SMS_MOTOR_UART_Init(&uartUCA0);

    TI_SMS_MOTOR_UARRT_Init_For_Packet(&uartUCA0);

    return (errorCode);
}

#ifdef SENSORLESS_3_PHASE_BLDC 

extern UINT16 currentAverageZCTime; 

UINT32 sensorless_3_Phase_ISR(void)
{    
	UINT32 errorCode = 0;
	volatile UINT32 zeroCrossToCommutationDelayGivenDegrees = 0;
	volatile UINT8 zeroCrossComparatorValue = 0;    
       
    zeroCrossComparatorValue = P3IN;                 //
    zeroCrossComparatorValue = ((zeroCrossComparatorValue & 0x0e) >> 1);  // get three bits

    //add lead angle delay 
    //motor_Control_motor_commutations(temp);
    sensorlessBEMF.currentBackEMFState= zeroCrossComparatorValue;
    
	 if (sensorlessBEMF.startBEMFSensing)
	 {
	    // motor_update_motor_comm_count(motor_get_motor_comm_count() + 1);
		sensorlessBEMF.comutationStateCount++;
	    // start lead angle after one commutation
	    if(sensorlessBEMF.lastStateSaved)
	    {
		    //motor_control_set_reset_change_commstate_status(0);	// set initial value
		    sensorlessBEMF.leadAngleDelayCompleted = FALSE;

            zeroCrossToCommutationDelayGivenDegrees = zeroCrossToCommutationDelay = TA3R & 0xFFFF; // read the value of TA3R
		    currentAverageZCTime=(currentAverageZCTime*15 +	zeroCrossToCommutationDelay)>>4;
				  
		    TA3CTL = 0x0000;        	// timer halted
	        TA3R = 0x0000;          	// clear counter	 		
	        TA3CTL = TASSEL_2 + MC__CONTINOUS + ID__8; // start TA0	
	
		    motor_electrical_comm_time[elec_sample_count++] = (UINT16)zeroCrossToCommutationDelay; 
		    
		    switch (userControllerConfig.commutationLeadAngle)
		    {
		    	case LEAD_ANGLE_5_DEGREES:
		    	    zeroCrossToCommutationDelayGivenDegrees =
		    	        zeroCrossToCommutationDelay >> 4 + (zeroCrossToCommutationDelay >> 5);
                    break;

		    	case LEAD_ANGLE_10_DEGREES:
		    		zeroCrossToCommutationDelayGivenDegrees =
		    		    zeroCrossToCommutationDelay >> 3 + (zeroCrossToCommutationDelay >> 4);
		    		break;

		    	case LEAD_ANGLE_15_DEGREES:
		    		zeroCrossToCommutationDelayGivenDegrees = zeroCrossToCommutationDelay >> 2;
		    		break;

		    	case LEAD_ANGLE_20_DEGREES:
		    		zeroCrossToCommutationDelayGivenDegrees =
		    		    zeroCrossToCommutationDelay >> 2 + (zeroCrossToCommutationDelay >> 4);
		    		    //+ (zeroCrossToCommutationDelay >> 5);

		    	case LEAD_ANGLE_25_DEGREES:
		    		zeroCrossToCommutationDelayGivenDegrees =
		    		    (zeroCrossToCommutationDelay >> 1) - (zeroCrossToCommutationDelay >> 4)
		    			- (zeroCrossToCommutationDelay >> 5);

		    	case LEAD_ANGLE_30_DEGREES:
		    		zeroCrossToCommutationDelayGivenDegrees = zeroCrossToCommutationDelay >> 1;
		    		break;
		    	default:
		    		zeroCrossToCommutationDelayGivenDegrees = 0;
		    		break;
		    }

			//Calculate_Motor_Electrical_Speed_In_Sensotrless(&motorElectricalSpeed, zeroCrossToCommutationDelay);
	       if (motorElectricalSpeed.stateCount == //NUM_OF_TOTAL_ELECTRICAL_STATES)
	           Calculate_Motor_Electrical_State_From_Motor_Poles(&userControllerConfig))
           {
               motorElectricalSpeed.timerInterval = ((motorElectricalSpeed.timerInterval * 3)+ (motorElectricalSpeed.startTimerCount)) >> 2; // divide by 8
               motorElectricalSpeed.stateCount = 0;
               motorElectricalSpeed.startTimerCount = 0;	                
               motorElectricalSpeed.speedReady = TRUE;
           }
           else if (motorElectricalSpeed.stateCount == 0)
           {
    	       motorElectricalSpeed.startTimerCount = zeroCrossToCommutationDelay;
           }
           else
           {
               motorElectricalSpeed.startTimerCount = motorElectricalSpeed.startTimerCount + zeroCrossToCommutationDelay;        
           }               
           motorElectricalSpeed.stateCount = motorElectricalSpeed.stateCount + 1;
	       		               

           if (zeroCrossToCommutationDelayGivenDegrees <= 0)
                sensorlessBEMF.leadAngleDelayCompleted = TRUE;	// set TRUE to call next state FET pair    			

      //     sensorlessBEMF.leadAngleDelayCompleted = TRUE;
            // once the time is satisfied advance and commutate
            while (!sensorlessBEMF.leadAngleDelayCompleted)
   	        { 	 				 			
		 		// check is TA3R exceed the lead angle delay
		 		if (TA3R >= zeroCrossToCommutationDelayGivenDegrees) 
	 			{
					sensorlessBEMF.leadAngleDelayCompleted = TRUE;	// set 1 to call next state FET pair
	 	    	}
	 		}		
		} 
	 }
    
    /* motor commutation sequences */ 
    //DoMotorCommutationAsPerZerCrossNoDelay(zeroCrossComparatorValue);
    if (elec_sample_count++ >= TOTAL_SAMPLE_ARRAY_NEW)
        elec_sample_count = 0; 
    motor_electrical_comm_time[elec_sample_count++] = (UINT16)TA3R && 0xFFFF; 

    if (elec_sample_count++ >= TOTAL_SAMPLE_ARRAY_NEW)
	    elec_sample_count = 0; 
    {

	    switch(zeroCrossComparatorValue)
    	{ 
    		case 0:    //Sensorless               
            	P3IES = 0x00;
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt           	
            	break;
          
        	case 1:    //Sensorless W-U
        		TBCCTL1 = OUTMOD_0;
 	        	TBCCTL3 = OUTMOD_0;
 	        	TBCCTL4 = OUTMOD_0;
 	        	TBCCTL6 = OUTMOD_0;
 	        	TBCCTL5 = OUTMOD_0 | BIT_2;
 	        	TBCCTL2 = OUTMOD_6;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
        	 	P3IES = PHW_INT_HI | PHV_INT_HI | PHU_INT_LO;  /* case 1 */
            	P3IFG = 0x00;
          		break;
                
        	case 2:    // U-V
        		TBCCTL2 = OUTMOD_0;
        	 	TBCCTL3 = OUTMOD_0;
        	 	TBCCTL5 = OUTMOD_0;
        	 	TBCCTL6 = OUTMOD_0;
        	 	TBCCTL4 = OUTMOD_6;
        	 	TBCCTL1 = OUTMOD_0 | BIT_2;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
            	P3IES = PHW_INT_HI | PHV_INT_LO | PHU_INT_HI;
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          		break;
           
        	case 3:    //Sensorless W-V
        		TBCCTL1 = OUTMOD_0;
            	TBCCTL2 = OUTMOD_0;
            	TBCCTL3 = OUTMOD_0;
            	TBCCTL6 = OUTMOD_0;
            	TBCCTL4 = OUTMOD_6;
            	TBCCTL5 = OUTMOD_0 | BIT2;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
            	P3IES = PHW_INT_HI | PHV_INT_LO | PHU_INT_LO;
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          		break;
          
        		case 4:    //Sensorless V-W
            	TBCCTL1 = OUTMOD_0;
            	TBCCTL2 = OUTMOD_0;
           		TBCCTL4 = OUTMOD_0;
            	TBCCTL5 = OUTMOD_0;
            	TBCCTL6 = OUTMOD_6;
            	TBCCTL3 = OUTMOD_0 | BIT_2;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
            	P3IES = PHW_INT_LO | PHV_INT_HI | PHU_INT_HI;
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt     
          		break;
        
        	case 5:    //Sensorless V-U
            	TBCCTL1 = OUTMOD_0;
            	TBCCTL4 = OUTMOD_0;
            	TBCCTL5 = OUTMOD_0;
            	TBCCTL6 = OUTMOD_0;
            	TBCCTL2 = OUTMOD_6;
            	TBCCTL3 = OUTMOD_0 | BIT_2;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
            	P3IES =  PHW_INT_LO | PHV_INT_HI | PHU_INT_LO;
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          		break;
           
        	case 6:    // U-W
            	TBCCTL2 = OUTMOD_0;
            	TBCCTL3 = OUTMOD_0;
            	TBCCTL4 = OUTMOD_0;
            	TBCCTL5 = OUTMOD_0;
            	TBCCTL6 = OUTMOD_6;
            	TBCCTL1 = OUTMOD_0 | BIT_2;
#ifdef SENSORLESS_3_PHASE_BLDC
 	        	DelayCounts(pwm3Phase.flyBackTime);
#endif
            	P3IES =  PHW_INT_LO | PHV_INT_LO | PHU_INT_HI; /* case 6 */
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
          		break;                              
           
       		case 7:    
            	P3IES =  PHW_INT_LO | PHV_INT_LO | PHU_INT_LO; /* case 7 */
            	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
            
         		break;  
             
        	default:             
        		P3IFG = 0x00;
        		break;
		}	
    }

// check for proper rotation here
// the first time through the routine, last_commutation_state_saved has not been set
// capture the last commutation state, and set the flag
//
	if (sensorlessBEMF.startBEMFSensing)
    {
    	if(sensorlessBEMF.lastStateSaved)
        {
        	sensorlessBEMF.zeroCrossIntrruptCount++; 	
            Motor_Control_Init_Check_And_Detect_Motor_Direction(); 
        }
        // store last commutation state for comparison
        
        if (comm_sample_count >= TOTAL_SAMPLE_ARRAY)
            comm_sample_count = 0;
                
        motor_electrical_states [comm_sample_count++] = sensorlessBEMF.currentBackEMFState;
        
        sensorlessBEMF.previouBackEMFState = sensorlessBEMF.currentBackEMFState;
        sensorlessBEMF.lastStateSaved = TRUE; 
    }

    P3IFG = 0x00; //Clear the interrupt flag   
    return (errorCode);
}
#endif

#ifdef SENSORED_3_PHASE_BLDC
void Hall_sensor_ISR()
{
    volatile UINT8 hallEffectState;
    volatile UINT16 hallPulseFilter;
    MOTOR_CONTROLLER_CONFIG* config_ptr;

    config_ptr = TI_SMS_Init_Get_Motot_Config();

    hallEffectState = (P3IN & 0x0e) >> 1; // get hall effect pin status and move

    Calculate_Motor_Electrical_Speed(&motorElectricalSpeed);

    sensorlessBEMF.zeroCrossIntrruptCount++;

    switch(hallEffectState)
    {
      case 0:
            P3IFG = 0x00;
            break;
        case 1:
        	if (config_ptr->motorDirection)
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        	    P3IES =   pwm3Phase.interruptSequence[hallEffectState]; /* case 1 */
        	}
        	else
        	{
        	     TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 6);
        		 P3IES =  pwm3Phase.interruptSequence[hallEffectState]; /* case 6 */
        	}
        	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
        	break;

        case 2:
        	if (config_ptr->motorDirection)
        	{
        	    TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        	    P3IES = pwm3Phase.interruptSequence[hallEffectState];
        	}
        	else
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 5);
        		P3IES =  pwm3Phase.interruptSequence[hallEffectState]; /* case 2 */
        	}
        	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
        	break;

        case 3:
        	if (config_ptr->motorDirection)
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 3 */
        	}
        	else
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 4);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 3 */
        	}
        	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
        	break;

        case 4:
        	if (config_ptr->motorDirection)
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 4 */
        	}
        	else
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 3);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 4 */
        	}
        	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
        	break;

        case 5:
        	if (config_ptr->motorDirection)
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 5 */
        	}
        	else
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 2);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 5 */
        	}
        	P3IFG = 0x00;    // P3 bit1 to bit3 used for hall interrupt or bemf interrupt
        	break;

        case 6:
        	if (config_ptr->motorDirection)
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, hallEffectState);
        		P3IES = pwm3Phase.interruptSequence[hallEffectState]; /* case 6 */
        	}
        	else
        	{
        		TI_SMS_MOTOR_PWM_Update_Mode(&pwm3Phase, 1);
        	    P3IES =  pwm3Phase.interruptSequence[hallEffectState]; /* case 6 */
        	}
        	P3IFG = 0x00;
        	break;

        case 7:
            P3IFG = 0x00;
            break;

        default:
            P3IFG = 0x00;
            break;
    }
	P3IFG = 0x00;
}
#endif

// check motor comm state for correct rotation direction
void Motor_Control_Init_Check_And_Detect_Motor_Direction (void) 
{
	switch (sensorlessBEMF.previouBackEMFState)
	{
		case 3:
	 		if (sensorlessBEMF.currentBackEMFState != 0x02)
	 	 	    gReverseRotationDetected = TRUE;
	 	 	break;
	 	case 2:
	 		if (sensorlessBEMF.currentBackEMFState != 0x06)
	 		    gReverseRotationDetected = TRUE;
	 		break;
	 	case 6:
	 		if (sensorlessBEMF.currentBackEMFState != 0x04)
	 		    gReverseRotationDetected = TRUE;
	 		break;
	 	case 4:
	 		if (sensorlessBEMF.currentBackEMFState != 0x05)
	 		    gReverseRotationDetected = TRUE;
	 		break;
	 	case 5:
	 		if (sensorlessBEMF.currentBackEMFState != 0x01)
	 		    gReverseRotationDetected = TRUE;
	 		break;
	 	case 1:
	 		if (sensorlessBEMF.currentBackEMFState != 0x03)
	 		    gReverseRotationDetected = TRUE;
	 		break;
	 	default:
	 		gReverseRotationDetected = TRUE; 
	 		break; // case 0 and 7 should not occur
	} // end of switch
}
void Motor_Control_Init_Check_And_Detect_Motor_Direction_Old (void) 
{
	switch (sensorlessBEMF.previouBackEMFState)
	{
		case 3:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x02) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x06))
	 	 	    gReverseRotationDetected = FALSE;
	 	 	else
	 	 	    gReverseRotationDetected = TRUE; 
	 	 	break;
	 	case 2:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x06) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x04))
	 		    gReverseRotationDetected = FALSE;
	 		else
	 	 	    gReverseRotationDetected = TRUE; 
	 		break;
	 	case 6:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x04) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x05))
	 		    gReverseRotationDetected = FALSE;
	 		else
	 	 	    gReverseRotationDetected = TRUE; 
	 		break;
	 	case 4:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x05) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x01))
	 		    gReverseRotationDetected = FALSE;
	 		else
	 	 	    gReverseRotationDetected = TRUE; 
	 		break;
	 	case 5:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x01) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x03))
	 		    gReverseRotationDetected = FALSE;
	 		else
	 	 	    gReverseRotationDetected = TRUE; 
	 		break;
	 	case 1:
	 		if ((sensorlessBEMF.currentBackEMFState == 0x03) ||
	 		    (sensorlessBEMF.currentBackEMFState == 0x02))
	 		    gReverseRotationDetected = FALSE;
	 		else
	 	 	    gReverseRotationDetected = TRUE; 
	 		break;
	 	default:
	 		gReverseRotationDetected = TRUE; 
	 		break; // case 0 and 7 should not occur
	} // end of switch
}

// function used to set or clear different error condition
void Motor_Control_Set_Clear_Error_Condition_Flag (CONTROLLER_ERROR_STATUS errorStatus, CONTROLLER_CUR_VOL_ERROR_FLAGS *ptr,
		                                              UINT8 setClr)
{
	switch (errorStatus)
	{
		case ERROR_CON_DETECTED:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~ERROR_CON_DETECTED);
			break;
	 	case REV_ROTATION_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~REV_ROTATION_ERROR);
	 		break;
	 	case UND_VOLTAGE_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~UND_VOLTAGE_ERROR);
	 		break;
	 	case OV_VOLTAGE_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~OV_VOLTAGE_ERROR);
	 		break;
	 	case OV_CUR_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~OV_CUR_ERROR);
	 		break;
	 	case MOTOR_UND_SPEED_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~MOTOR_UND_SPEED_ERROR);
	 		break;
	 	case MOTOR_OV_SPEED_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~MOTOR_OV_SPEED_ERROR);
	 		break;
	 	case MOTOR_STARTUP_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~MOTOR_STARTUP_ERROR);
	 		break;
	 	case MOTOR_GUI_CONN_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~MOTOR_GUI_CONN_ERROR);
	 		break;
	 	case MOTOR_SPEED_FEEDB_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                ptr->controllerErorFlags &= (~MOTOR_SPEED_FEEDB_ERROR);
	 		break;
	 	case MOTOR_OV_TEMP_ERROR:
		    if (setClr)
                ptr->controllerErorFlags |= errorStatus;
            else
                //ptr->controllerErorFlags &= (~MOTOR_OV_TEMP_ERROR);
            break;
	 	default:
	 		break;
	} // end of switch
}


// Port 3 interrupt service routine for hall effect sensors
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{   
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_PORT3_INTERRUPT;
		P3IE  &= ~BIT7;
		P3IFG &= ~BIT7;
	}else{
#ifdef SENSORLESS_3_PHASE_BLDC
		sensorless_3_Phase_ISR();
#endif

#ifdef SENSORED_3_PHASE_BLDC
		Hall_sensor_ISR();
#endif
	}
}

// over current ISR
#pragma vector = PORT1_VECTOR
__interrupt void AFTE_OVER_CURRENT_ISR(void)
{
	// Check if interrupt was triggered by Interrupt Sweep Function in
	// POST routine.
	if(gInterruptCounter& BITF){
		gInterruptCounter += TEST_PORT1_INTERRUPT;
		P1IE  &= ~BIT7;
		P1IFG &= ~BIT7;
	}else{
		pwm3Phase.DutyCycle[PWM_U_HIGH_SIDE] = 0x00;
		pwm3Phase.DutyCycle[PWM_U_LOW_SIDE] = 0x00;
		pwm3Phase.DutyCycle[PWM_V_HIGH_SIDE] = 0x00;
		pwm3Phase.DutyCycle[PWM_V_LOW_SIDE] = 0x00;
		pwm3Phase.DutyCycle[PWM_W_HIGH_SIDE] = 0x00;
		pwm3Phase.DutyCycle[PWM_W_LOW_SIDE] = 0x00;
		TI_SMS_MOTOR_PWM_Update_Duty_Cycle(&pwm3Phase);
		// disable all PWM outputs
		TI_SMS_MOTOR_PWM_Disable();
	}
}
