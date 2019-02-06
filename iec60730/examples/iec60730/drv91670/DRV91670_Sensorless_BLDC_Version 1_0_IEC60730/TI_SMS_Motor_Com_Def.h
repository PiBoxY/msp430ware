/*=============================================================
 * @file:		TI_SMS_Motor_Com_Def.h
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
#ifndef TI_SMS_MOTOR_COM_DEF_H_
#define TI_SMS_MOTOR_COM_DEF_H_

#include "TI_SMS_Motor_Type_Def.h"

#define _DEVICE_DRV_91620_ 

#ifdef _DEVICE_DRV_91620_
#include "DRV91620.h"
//#include "DRV91630.h"
#endif 
//#define ANAHEIM_BLY171S_15V_8000
//#define ANAHEIM_BLY171S_17V_8000
//#define SENSORED_3_PHASE_BLDC
#define SENSORLESS_3_PHASE_BLDC
#define PID_SPEED_CONTROL_ON

#define USE_DUTYCYCLE_WRT_PERIOD_COUNT
#define PWM_SPEED_INPUT 
#define COMMUTATION_DELAY 1 // used in commutaiton ISR
#define PID_MAXOUT_PERECNT_WRT_PWM_PERIOD 98//90 // in percentage, 70% of pwm period
#define PID_INTEGRAL_INITAIL_VALUE_PERECNT_WRT_PWM_PERIOD 10 //30 //10 // in percentage, 10% of period
#define PID_MINOUT_PERECNT_WRT_PWM_PERIOD 3 //20 //5 //3 // in percentage, 10% of pwm period

#define DEFAULT_PWM_DUTY_CYCLE_IN_PERCENT 10 //10 //20 // in percentage, 20% of pwm period use as default PWM duty cycle
#define LEAD_ANGLE_RIGHT_SHIFT_DIVIDER 8

#define DEFAULT_DUTY_CYCLE 250
 
#define SYSTEM_DEBUG
#define MAX_PWM_CHANNEL_NUM 7
#define ADC_GAIN_SHIF5 5 
#define PI_KI_SHIFT5 5
#define PI_KP_SHIFT5 5
#define NUM_OF_MOTOR_POLES 8
#define NUM_OF_TOTAL_ELECTRICAL_STATES  ((6 * NUM_OF_MOTOR_POLES) >> 1)
#define NUM_OF_BACK_EMF_STATES 6

#define TIME_OUT_FOR_MOTOR_RESTART  25               //100 ms
#define TIMEOUT_FOR_UPDATE_INITIAL_SPEED_RAMP_UP (TIME_OUT_FOR_MOTOR_RESTART/10)
#define RMAP_UP_DOWN_SET_SPEED_VALUE 25
#define DEFAULT_MOTOR_RAMP_UP_INITAIL_SPEED 500 
#define DEFAULT_MOTOR_SET_SPEED 1000 
#define MINIMUM_PWM_DUTY_CYCLE_FROM_ANALOG_POT 200
#define PID_INTEGRATOR_DEFAULT_DUTY_CYCLE 100
#define MIMIMUM_DEFUALT_MOTOR_RPM    100//500 //1000

#define RAMPUP_DELAY 1 //1  // 4
#define RAMPUP_DELAY_TO_REACH_MINIMUM_SPEED 40 //10 //1  // 4
#define MAX_TIMEOUT_FOR_RPM_FEEDBACK 10 * RAMPUP_DELAY //20* RAMPUP_DELAY

#define PWM_U_HIGH_SIDE 0
#define PWM_U_LOW_SIDE 1
#define PWM_V_HIGH_SIDE 2
#define PWM_V_LOW_SIDE 3
#define PWM_W_HIGH_SIDE 4
#define PWM_W_LOW_SIDE 5

// over current int led blinking delay
#define OVER_CURRENT_LED_BLINKING_DELAY 30000
// after motor spinning after align and go we need to get this minimum zero cross bemf count
#define MINIMUM_ZERO_CROSS_COUNT 20 
// speed error offset
#define SPEED_ERROR_OFFSET_VALUE 500

// TBCCR1 CCIE interrupt value, duty cycle alue divied by this factor and loaded into
// TBCCR1 for current sampling interrupt
#define TBCCR1_DIVIDER_FOR_CURRENT_INTERRUPT 1//8
#define MINIMUM_CURRENT_VALUE 100

// pre-driver on
#ifdef _DEVICE_DRV_91520 || _DEVICE_DRV_91168_//BREEZE	
#define PRE_DRIVER_ON 0xE0          // to turn pre-drivers on for BREEZE
#endif

#ifdef _DEVICE_DRV_91620_|| _DEVICE_DRV_92200 || _DEVICE_DRV_92250
#define PRE_DRIVER_ON 0x80          // to turn pre-drivers on for BUZZ
#endif	

#define ASSBST6P8 BIT2    // 0x04 

#ifdef _DEVICE_DRV_91620_|| _DEVICE_DRV_92200 || _DEVICE_DRV_92250
#define FET_OFF 0x00
#endif

#ifdef _DEVICE_DRV_91520 || _DEVICE_DRV_91168_//BREEZE	
#define FET_OFF 0x55
#endif

#define ALL_HIGH 0xFF
#define ALL_LOW 0x00

// use for PWM Pulse In frquency limit check
#define RC_PANE_PWM_PULSE_LOWER_LIMIT 20823    // 0.64 micro seconds is 1 count, counts equal to 75HZ
#define RC_PANE_PWM_PULSE_HIGHER_LIMIT 40127   // 0.64 mucro seconds is 1 count, counts equal to 35HZ
#define RC_PWM_PUSLE_IN_MULTIPLIER 6

// Packet length
#define DYSON_PACKET_LENGTH 5
// PWM counts for 8KHZ, 16KHZ, 32KHZ
#define PWM_PERIOD_COUNTS_8KHZ  3125  
#define PWM_PERIOD_COUNTS_16KHZ 1562
#define PWM_PERIOD_COUNTS_32KHZ 781 
#define PWM_PERIOD_COUNTS_12KHZ 2083
#define PWM_PERIOD_COUNTS_20KHZ 1250
#define PWM_PERIOD_COUNTS_24KHZ 1042
#define PWM_PERIOD_COUNTS_28KHZ 893

#define MOTOR_COMMUTATION_ANGLE_30_DEGREES 1
#define MOTOR_COMMUTATION_ANGLE_15_DEGREES 2
#define MOTOR_COMMUTATION_ANGLE_7_DEGREES 3
#define MOTOR_COMMUTATION_ANGLE_1_DEGREE 6
#define BRAKING_LOOP_COUNT 20
#define BRAKING_LOOP_COUNT_NEW 2//10//50
#define BRAKING_DELAY 30000

#define ADC_BATTERY_SENSING_IN 0x04
#define ADC_TEMP_SENSING_IN 0x05

#define MICRO_TO_NANO_MULTIPLIER 320

#define DUTY_CYCLE_INCREMENT_2_PERCENT 1
#define DUTY_CYCLE_INCREMENT_4_PERCENT 2
#define DUTY_CYCLE_INCREMENT_8_PERCENT 3
#define DUTY_CYCLE_INCREMENT_16_PERCENT 4

#define TIME_OUT_FOR_MOTOR_COMM_FREQ 20   // 191
#define OVER_CURRENT_LOOP_COUNT 50

#define DYSON_CONFIG_TABLE_LENGTH 128
#define DYSON_CODE1_BYTE1 0x00
#define DYSON_CODE1_BYTE2 0x01
#define DYSON_MAX_DUTYCYCLE_RANGE_BYTE 0x02
#define DYSON_MIN_DUTYCYCLE_RANGE_BYTE 0x03
// slave motor controller address
#define DRV9162_SLAVE_ADDRESS 0x30
#define DEFAULT_BATTRY_VOLATGE 0x31
#define DEFAULT_MOTOR_CURRENT_VALUE 0x32
#define DEFAULT_BOARD_TEMPERATURE 0x33
#define DEFAULT_MOTOR_LEADANGLE 0x34
#define DEFAULT_MOTOR_PWM_FREQUENCY (UINT16) 0x3435
#define DEFAULT_MOTOR_COMMUTATION_FREQUENCY (UINT16) 0x3637
#define DEFAULT_CRC_VALUE 0x38

#define CONFIG_BYTE_0_ADD 0x00
#define CONFIG_BYTE_1_ADD 0x01
#define CONFIG_BYTE_2_MAX_PWM_PULSE_WIDTH_ADD 0x02
#define CONFIG_BYTE_3_MIN_PWM_PULSE_WIDTH_ADD 0x03
#define CONFIG_BYTE_4_PWM_FREQ_ADD 0x04
#define CONFIG_BYTE_5_BRAKE_METHOD_ADD 0x05
#define CONFIG_BYTE_6_LEAD_ANGLE_ADD 0x06
#define CONFIG_BYTE_7_SPEED_CONTROL_PWM_OR_UART_ADD 0x07
#define CONFIG_BYTE_8_BAT_UVOLT_ADD 0x08
#define CONFIG_BYTE_9_OVER_CURRENT_ADD 0x09

#define ADC_START 0x04
#define ADC_START_TORQZ 0x01

#define BIT_2 0x04
#define ADC_MAX_RANGE 1024
#define PWM_HIGHER_LIMIT_DEAD_BAND 2
#define INCORRECT_ZEROCROSS_COUNT 3
#define CORRECT_ZEROCROSS_COUNT 1
#define ALIGN_TIMER_TICK_VALUE 20
#define ALIGN_AND_GO_GOOD_STARTUP_COUNT 25
#define ALIGN_TIMER_DELAY_COUNT 5000

#define BOOT_CAP_CHARGE_DELAY 30000

#define ADC_MODE ADC_POLLIING

#define ADC_ANALOG_POT_SPEED_IN 0x04 // we are using ADC channel 4. ADC channel range [0,15]
#define ADC_CURRENT_SENING_IN 0x0E   // we are using ADC channel 15. ADC channel range [0,15]

#ifdef SENSORLESS_3_PHASE_BLDC
#define ADC_ANALOG_POT_FOR_TORQZ 0x07   // ADC7/GPIO7/HS1 we are using ADC channel 7. ADC channel range [0,15]
#endif

#ifdef SENSORED_3_PHASE_BLDC
#define ADC_ANALOG_POT_FOR_TORQZ 0x02   // ADC2/GPIO2/WS we are using ADC channel 2. ADC channel range [0,15]
#endif

#define ADC_ANALOG_POT_FOR_TORQZ_CURRENT 14
#define ADC_ANALOG_VCC_FOR_BUZZ 7

#define BUZZ_FLASH_DEVICE_ID 1670
#define TORQZ_FLASH_DEVICE_ID 1680

#define SPEED_KP_MIN_VALUE 0
#define SPEED_KP_MAX_VALUE 10
#define SPEED_KI_MIN_VALUE 0
#define SPEED_KI_MAX_VALUE 10

#define MIN_POLE_COUNT_LIMIT 1
#define MAX_POLE_COUNT_LIMIT 24

#define HALL_SENSOR_MODE_A 0
#define HALL_SENSOR_MODE_B 1
#define HALL_SENSOR_MODE_C 2
#define HALL_SENSOR_MODE_D 3

#define MOTOR_CW_DIRECTION 1
#define MOTOR_CCW_DIRECTION 0
#define MOTOR_START_FLAG 1
#define MOTOR_STOP_FLAG 0

#define MIN_BRAKE_PERCENTAGE_LIMIT 0
#define MAX_BRAKE_PERCENTAGE_LIMIT 100
#define MIN_BRAKE_PERCENTAGE_LIMIT 0
#define BRAKE_25_PERCENT_SELECTED 25
#define BRAKE_50_PERCENT_SELECTED 50
#define BRAKE_75_PERCENT_SELECTED 75

#define MOTOR_MAX_SPEED_FROM_GUI 65000
#define MOTOR_MIN_SPEED_FROM_GUI 1000
#define MOTOR_MAX_DEFAULT_SPEED 8000
#define GUI_DIAL_DEFAULT_PERCENATGE 20
#define GUI_DIAL_MIN_PERCENATGE 0
#define GUI_DIAL_MAX_PERCENATGE 100

#define SLOW_SPEED_RAMP_SLOPE_VALUE 6
#define MEDIUM_SPEED_RAMP_SLOPE_VALUE 5
#define FAST_SPEED_RAMP_SLOPE_VALUE 4

#define MIN_ALIGN_PHASE_NUMBER 1
#define MAX_ALIGN_PHASE_NUMBER 6
#define ALIGN_TO_MOTOR_STATE_1 MIN_ALIGN_PHASE_NUMBER
#define ALIGN_TO_MOTOR_STATE_2 2
#define ALIGN_TO_MOTOR_STATE_3 2
#define ALIGN_TO_MOTOR_STATE_4 2
#define ALIGN_TO_MOTOR_STATE_5 2
#define ALIGN_TO_MOTOR_STATE_6 MAX_ALIGN_PHASE_NUMBER
#define ZERO_CROSS_DETECTION_COUNT 30000
#define MAX_ZERO_CROSS_DETECTION_COUNT 60000
#define MIN_ZERO_CROSS_DETECTION_COUNT 10000
#define MIN_ALIGN_TIMER_DELAY_COUNT 500
#define LEAD_ANGLE_MIN_VLAUE 5
#define LEAD_ANGLE_MAX_VLAUE 30

#define DEFAULT_MAX_VOLTAGE 25
#define DEFAULT_MAX_CURRENT 10

#define CONTROLLER_MODE_SENSORED 0
#define CONTROLLER_MODE_SENSORLESS 1
#define CONTROLLER_MODE_BRUSHED 2
// if speed feedback error is within 3% don't do any action
#define SPEED_FEED_BACK_ERROR_BAND 7//6

// lead angle value coming from GUI
#define LEAD_ANGLE_5_DEGREES 5
#define LEAD_ANGLE_10_DEGREES 10
#define LEAD_ANGLE_15_DEGREES 15
#define LEAD_ANGLE_20_DEGREES 20
#define LEAD_ANGLE_25_DEGREES 25
#define LEAD_ANGLE_30_DEGREES 30

#define ZERO_CROSS_WINDOW_MIN_TIME 10000
#define ZERO_CROSS_WINDOW_MAX_TIME 60000

#define REALIGN_TIMER_DELAY_MIN_COUNT 2000
#define REALIGN_TIMER_DELAY_MAX_COUNT 7000

#define SENSORLESS_DEFAULT_STARTUP_DUTYCYCLE 200
#define DELAY_COUNT_FOR_BOOT_CAP 1
#define VCC_OVER_19V 660
#define COMMTIME_OUT_NUM 25
#define COMM_COUNT_TO_CHECK_UNDER_SPEED 500
#define UNDER_SPEED_RPM_VALUE 1200
#define UNDER_SPEED_SAMPLE_COUNT 50
#define TURN_ON_VCC_DIVIDER 0x08

#endif /*TI_SMS_MOTOR_COM_DEF_H_*/
