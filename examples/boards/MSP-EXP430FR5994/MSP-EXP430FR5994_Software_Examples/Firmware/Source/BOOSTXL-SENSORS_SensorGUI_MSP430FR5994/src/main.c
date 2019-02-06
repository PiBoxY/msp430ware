
//*****************************************************************************
//
// \file main.c
//
// \brief 
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 

//*****************************************************************************
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "driverlib.h"
#include "i2c_driver.h"
#include "demo_sysctl.h"
#include "bmi160_support.h"
#include "bme280_support.h"
#include "tmp007.h"
#include "opt3001.h"
#include "uart_driver.h"

//***** Definitions *****
#define USING_BOSCH_BP
#define SAMPLE_TIME_1				(53)
#define SAMPLE_TIME_2				(26)
#define SAMPLE_TIME_4				(13)
#define SAMPLE_TIME_6				(8)
#define SAMPLE_TIME_8				(6)
#define SAMPLE_TIME_10				(5)
#define NUM_AVGR_SUMS				2 //x^2 frames

//***** Function Prototypes *****
void configure_clocks(void);
int32_t movingAvg(int prevAvg, int16_t newValue);

//***** Global Data *****
uint32_t clockValue;			// Variable to store current clock values
uint16_t clockStatus;			// Variable to store status of Oscillator fault flags
uint32_t MCLKValue = 0;

const uint8_t wdtWakeUpPeriod [8] = {
		WDT_A_CLOCKDIVIDER_2G,
		WDT_A_CLOCKDIVIDER_128M,
		WDT_A_CLOCKDIVIDER_8192K,
		WDT_A_CLOCKDIVIDER_512K,
		WDT_A_CLOCKDIVIDER_32K,
		WDT_A_CLOCKDIVIDER_8192,
		WDT_A_CLOCKDIVIDER_512,
		WDT_A_CLOCKDIVIDER_64,
};

const uint8_t timeSamplesBMI [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesBMM [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesBME [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesTMP [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

const uint8_t timeSamplesOPT [6] = {
		SAMPLE_TIME_1, 		//Sample at 1 time per second
		SAMPLE_TIME_2, 		//Sample at 2 times per second
		SAMPLE_TIME_4, 		//Sample at 4 times per second
		SAMPLE_TIME_6, 		//Sample at 6 times per second
		SAMPLE_TIME_8, 		//Sample at 8 times per second
		SAMPLE_TIME_10, 	//Sample at 10 times per second
};

//Default time sample values for each sensor
volatile uint8_t sampleTimePeriodBMI = 2; //2
volatile uint8_t sampleTimePeriodBMM = 5; //5
volatile uint8_t sampleTimePeriodBME = 0;
volatile uint8_t sampleTimePeriodTMP = 0;
volatile uint8_t sampleTimePeriodOPT = 0;

// Changed by the GUI -  default ~ 0.0156 seconds  1/32KHz * WDT_A_CLOCKDIVIDER_512
volatile uint8_t wdtWakeUpPeriodIndex = 6;

// BMI160
BMI160_RETURN_FUNCTION_TYPE returnValue;
struct bmi160_gyro_t        s_gyroXYZ;
struct bmi160_accel_t       s_accelXYZ;
struct bmi160_mag_xyz_s32_t s_magcompXYZ;

// BME280
s32 returnRslt = 0;
s32 g_s32ActualTemp   = 0;
u32 g_u32ActualPress  = 0;
u32 g_u32ActualHumity = 0;

// OPT3001
uint16_t rawData;
float    convertedLux;

// TMP007
uint16_t rawTemp;
uint16_t rawObjTemp;
float    tObjTemp;
float    tObjAmb;

//Timer Counter
uint16_t WDTcount = 0;

//Receive UART Variables
#define NUM_RX_CHARS 64
char rxMsgData[NUM_RX_CHARS] = "";
int numMsgsRx = 0;
int tempIndex = 5;
int numChars = 0;

//Calibration off-sets
int8_t accel_off_x;
int8_t accel_off_y;
int8_t accel_off_z;
int16_t gyro_off_x;
int16_t gyro_off_y;
uint16_t gyro_off_z;

//gesture recognition
int getGestures = 1;
int dominant = 0;
uint16_t gyroAbsX, gyroAbsY, gyroAbsZ;
uint16_t deltaAccelX, deltaAccelY, deltaAccelZ;
int16_t prevAccelX = 0;
int16_t prevAccelY = 0;
int16_t prevAccelZ = 0;
int16_t prevGyroX = 0;
int16_t prevGyroY = 0;
int16_t prevGyroZ = 0;
int16_t stillCount = 0;
int32_t gyroAvgX = 0.0;
int32_t gyroAvgY = 0.0;
int32_t gyroAvgZ = 0.0;
int32_t accelAvgX = 0.0;
int32_t accelAvgY = 0.0;
int32_t accelAvgZ = 0.0;

//Sensor Status Variables
bool BME_on = true;
bool BMI_on = true;
bool TMP_on = true;
bool OPT_on = true;

/***********************************************************
  Function: main
*/

void main(void)
{
    // Stop Watchdog Timer
	WDT_A_hold(WDT_A_BASE);

    // Setup clock configuration
    configure_clocks();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= 0x01;                          // Set P1.0 to output direction

#ifdef USING_BOSCH_BP
	//Initialize uart
	uartInit();

	// Initialize i2c
	initI2C();
#endif

	// Enable interrupts
	__enable_interrupt();

#ifdef USING_BOSCH_BP
//	// Initialize bmi160 sensor
	bmi160_initialize_sensor();

	returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
	bmi160_accel_foc_trigger_xyz(0x03, 0x03, 0x01, &accel_off_x, &accel_off_y, &accel_off_z);
	bmi160_set_foc_gyro_enable(0x01, &gyro_off_x, &gyro_off_y, &gyro_off_z);

	// Initialize bme280 sensor
	bme280_data_readout_template();
	returnRslt = bme280_set_power_mode(BME280_SLEEP_MODE);

	// Initialize opt3001 sensor
	sensorOpt3001Init();

	// Initialize tmp007 sensor
	sensorTmp007Init();
#endif

	//Enable TMP, OPT, and BME sensors
	sensorTmp007Enable(true);
	sensorOpt3001Enable(true);
	returnRslt = bme280_set_power_mode(BME280_NORMAL_MODE);
	while(1)
	{
//		P1OUT |= 0x01;
//      DelayMs(1000);
//		P1OUT &= ~0x01;
//		clockValue++;

		WDT_A_hold(WDT_A_BASE);
		WDT_A_resetTimer(WDT_A_BASE);
		SFR_disableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

		//Set frequency back to full speed if reading/converting data
		if((WDTcount%timeSamplesBME[sampleTimePeriodBME] == 0) ||
				(WDTcount%timeSamplesBMM[sampleTimePeriodBMM] == 0) ||
				(WDTcount%timeSamplesBMI[sampleTimePeriodBMI] == 0) ||
				(WDTcount%timeSamplesTMP[sampleTimePeriodTMP] == 0) ||
				(WDTcount%timeSamplesOPT[sampleTimePeriodOPT] == 0) ||
				(WDTcount == 53))
		{
			if (getGestures)
			{

				gyroAbsX = abs(s_gyroXYZ.x);
				gyroAbsY = abs(s_gyroXYZ.y);
				gyroAbsZ = abs(s_gyroXYZ.z);

				gyroAvgX = movingAvg(gyroAvgX, gyroAbsX );
				gyroAvgY = movingAvg(gyroAvgY, gyroAbsY );
				gyroAvgZ = movingAvg(gyroAvgZ, gyroAbsZ );



				if( gyroAvgX > 1000 || gyroAvgY > 1000 || gyroAvgZ > 1000 )
				{
					stillCount = 0;
					if ( gyroAvgY > gyroAvgX )
					{
						if ( gyroAvgY > gyroAvgZ )
						{
							dominant = 2; //gyro Y
						}
						else
						{
							dominant = 3; //gyro Z
						}

					}
					else if ( gyroAvgZ > gyroAvgX )
					{
						dominant = 3; //gyro Z
					}
					else
					{
						dominant = 1; //gyro X
					}
				}
				else{

					deltaAccelX = abs(s_accelXYZ.x - prevAccelX);
					deltaAccelY = abs(s_accelXYZ.y - prevAccelY);
					deltaAccelZ = abs(s_accelXYZ.z - prevAccelZ);

					accelAvgX = movingAvg(accelAvgX, deltaAccelX);
					accelAvgY = movingAvg(accelAvgY, deltaAccelY);
					accelAvgZ = movingAvg(accelAvgZ, deltaAccelZ);


					if ( accelAvgX > 100 || accelAvgY > 100 || accelAvgZ > 100 )
					{
						stillCount = 0;

						if ( accelAvgY > accelAvgX )
						{
							if ( accelAvgY > accelAvgZ )
							{
								dominant = 5; //accel Y
							}
							else
							{
								dominant = 6; //accel Z
							}

						}
						else if ( accelAvgZ > accelAvgX )
						{
							dominant = 6; //accel Z
						}
						else
						{
							dominant = 4; //accel X
						}

					}
					else{
						stillCount++;
						if (stillCount>3)
						{
							dominant = 0; //still condition
						}

					}

				}
//				//contrsuct JSON string for GUI
//				snprintf(test.txString, 30,
//						"{\"dominant\":{\"code\":%d}}\n",
//						dominant);
//
//				//Send UART data JSON string
//				sendText();

				prevAccelX = s_accelXYZ.x;
				prevAccelY = s_accelXYZ.y;
				prevAccelZ = s_accelXYZ.z;
			}
		}

		//Mag update frequency
		if (WDTcount%timeSamplesBMM[sampleTimePeriodBMM] == 0)
		{
			if(BMI_on)
			{
				//Read Mag value (BMM) through BMI
				returnValue = bmi160_bmm150_mag_compensate_xyz(&s_magcompXYZ);

				//contrsuct JSON string for BMM
				snprintf(test.txString, 60,
						"{\"mag\":{\"x\":%ld,\"y\":%ld,\"z\":%ld}}\n",
						s_magcompXYZ.x, s_magcompXYZ.y, s_magcompXYZ.z);

				//Send UART data JSON string
				sendText();
			}
		}
		//Accel and Gyro update frequency
		if (WDTcount%timeSamplesBMI[sampleTimePeriodBMI] == 0)
		{
			if(BMI_on)
			{
				//Read Accel and Gyro values
				returnValue = bmi160_read_accel_xyz(&s_accelXYZ);
				returnValue = bmi160_read_gyro_xyz(&s_gyroXYZ);

				//contrsuct JSON string for BMI
				snprintf(test.txString, 120,
						"{\"dominant\":{\"code\":%d},"
						"\"gyro\":{\"x\":%d,\"y\":%d,\"z\":%d},"
						"\"accel\":{\"x\":%d,\"y\":%d,\"z\":%d}}\n",
						dominant,
						s_gyroXYZ.x, s_gyroXYZ.y, s_gyroXYZ.z,
						s_accelXYZ.x, s_accelXYZ.y, s_accelXYZ.z);

				//Send UART data JSON string
				sendText();
			}
		}
		//**Will need to separate each sensor into own if statements to allow GUI to change each sensor's sampling rates
		//TMP update frequency
		if (WDTcount%timeSamplesTMP[sampleTimePeriodTMP] == 0)
		{
			if(TMP_on)
			{
				// Read/convert tmp007 and opt3001 data
				sensorTmp007Read(&rawTemp, &rawObjTemp);
				sensorTmp007Convert(rawTemp, rawObjTemp, &tObjTemp, &tObjAmb);

				//contrsuct JSON string for TMP
				snprintf(test.txString, 60,
						"{\"ir_temp\":{\"obj_temp\":%5.2f}}\n",
						tObjTemp);

				//Send UART data JSON string
				sendText();
			}
		}
		//OPT update frequency
		if (WDTcount%timeSamplesOPT[sampleTimePeriodOPT] == 0) {
			if(OPT_on)
			{
				//Read and convert OPT values
				sensorOpt3001Read(&rawData);
				sensorOpt3001Convert(rawData, &convertedLux);

				//contrsuct JSON string for OPT
				snprintf(test.txString, 60,
						"{\"light\":{\"lux\":%5.2f}}\n",
						convertedLux);

				//Send UART data JSON string
				sendText();
			}
		}
		//BME update frequency
		if (WDTcount%timeSamplesBME[sampleTimePeriodBME] == 0) {
			if(BME_on)
			{
				//Read BME environmental data
				returnRslt = bme280_read_pressure_temperature_humidity(
						&g_u32ActualPress, &g_s32ActualTemp, &g_u32ActualHumity);

				//contrsuct JSON string for BME
				snprintf(test.txString, 60,
						"{\"enviro\":{\"humid\":%ld,\"press\":%ld,\"amb_temp\":%u}}\n",
						(uint32_t)g_u32ActualHumity, (uint32_t)g_u32ActualPress, (uint16_t)g_s32ActualTemp);

				//Send UART data JSON string
				sendText();
			}
		}
		//Reset WDTcount when max count has been reached
		if (WDTcount == 53) {
			//Reset WDT counter after reaching roughly one second
			WDTcount = 0;
		}
		WDTcount++;

		// Check to see if there is any message from the host
		if (receiveText(rxMsgData,NUM_RX_CHARS)){
			numMsgsRx++;
			numChars = strlen(rxMsgData);

			//Parses through UART received data and changes sampling rates and on/off statuses accordingly
			if (numChars > 6) {
				switch(rxMsgData[2]){
				case 's':  // e.g. {"s":10} or {"s":8} or {"s":4) to set sampling rates of 10Hz, 8Hz or 4Hz
					switch(rxMsgData[3]){
					case 'i':
						tempIndex = 6;
						if (rxMsgData[tempIndex] == '"'){
							tempIndex = 7;  // this accommodates values that have been converted to strings
						}
						switch (rxMsgData[tempIndex]){
						case '0':
							sampleTimePeriodBMI = 0;
							break;
						case '1':
							sampleTimePeriodBMI = 1;
							break;
						case '2':
							sampleTimePeriodBMI = 2;
							break;
						case '3':
							sampleTimePeriodBMI = 3;
							break;
						case '4':
							sampleTimePeriodBMI = 4;
							break;
						case '5':
							sampleTimePeriodBMI = 5;
							break;
						default:
							break;
						}
						break;
						case 'm':
							tempIndex = 6;
							if (rxMsgData[tempIndex] == '"'){
								tempIndex = 7;
							}
							switch (rxMsgData[tempIndex]){
							case '0':
								sampleTimePeriodBMM = 0;
								break;
							case '1':
								sampleTimePeriodBMM = 1;
								break;
							case '2':
								sampleTimePeriodBMM = 2;
								break;
							case '3':
								sampleTimePeriodBMM = 3;
								break;
							case '4':
								sampleTimePeriodBMM = 4;
								break;
							case '5':
								sampleTimePeriodBMM = 5;
								break;
							default:
								break;
							}
							break;
							case 't':
								tempIndex = 6;
								if (rxMsgData[tempIndex] == '"'){
									tempIndex = 7;
								}
								switch (rxMsgData[tempIndex]){
								case '0':
									sampleTimePeriodTMP = 0;
									break;
								case '1':
									sampleTimePeriodTMP = 1;
									break;
								case '2':
									sampleTimePeriodTMP = 2;
									break;
								case '3':
									sampleTimePeriodTMP = 3;
									break;
								case '4':
									sampleTimePeriodTMP = 4;
									break;
								case '5':
									sampleTimePeriodTMP = 5;
									break;
								default:
									break;
								}
								break;
								case 'o':
									tempIndex = 6;
									if (rxMsgData[tempIndex] == '"'){
										tempIndex = 7;
									}
									switch (rxMsgData[tempIndex]){
									case '0':
										sampleTimePeriodOPT = 0;
										break;
									case '1':
										sampleTimePeriodOPT = 1;
										break;
									case '2':
										sampleTimePeriodOPT = 2;
										break;
									case '3':
										sampleTimePeriodOPT = 3;
										break;
									case '4':
										sampleTimePeriodOPT = 4;
										break;
									case '5':
										sampleTimePeriodOPT = 5;
										break;
									default:
										break;
									}
									break;
									case 'e':
										tempIndex = 6;
										if (rxMsgData[tempIndex] == '"'){
											tempIndex = 7;
										}
										switch (rxMsgData[tempIndex]){
										case '0':
											sampleTimePeriodBME = 0;
											break;
										case '1':
											sampleTimePeriodBME = 1;
											break;
										case '2':
											sampleTimePeriodBME = 2;
											break;
										case '3':
											sampleTimePeriodBME = 3;
											break;
										case '4':
											sampleTimePeriodBME = 4;
											break;
										case '5':
											sampleTimePeriodBME = 5;
											break;
										default:
											break;
										}
										break;

					}
					break;
					case 'i': //e.g.: {"i":0} or {"i":1}
						if (rxMsgData[5] == '1'){
							returnRslt = bmi160_config_running_mode(APPLICATION_NAVIGATION);
							BMI_on = true;
						} else {
							returnRslt = bmi160_config_running_mode(ACCEL_PEDOMETER);
							BMI_on = false;
						}
						break;
					case 't': //e.g.: {"t":0} or {"t":1}
						if (rxMsgData[5] == '1'){
							sensorTmp007Enable(true);
							TMP_on = true;
						} else {
							sensorTmp007Enable(false);
							TMP_on = false;
						}
						break;
					case 'o': //e.g.: {"o":0} or {"o":1}
						if (rxMsgData[5] == '1'){
							sensorOpt3001Enable(true);
							OPT_on = true;
						} else {
							sensorOpt3001Enable(false);
							OPT_on = false;
						}
						break;
					case 'e': //e.g.: {"e":0} or {"e":1}
						if (rxMsgData[5] == '1'){
							returnRslt = bme280_set_power_mode(BME280_NORMAL_MODE);
							BME_on = true;
						} else {
							returnRslt = bme280_set_power_mode(BME280_SLEEP_MODE);
							BME_on = false;
						}
						break;
					case 'r':
						if (rxMsgData[5] == '1'){
							getGestures = 1;
						} else {
							getGestures = 0;
							dominant = 0;
						}
						break;
				}
			}
		}
		// Configure WDT
		WDT_A_initIntervalTimer(WDT_A_BASE,
				WDT_A_CLOCKSOURCE_ACLK,
				wdtWakeUpPeriod[wdtWakeUpPeriodIndex]);

		//Enable Watchdog Interupt
		SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
		SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

		// Start WDT
		WDT_A_start(WDT_A_BASE);

		// Go to LPM0
		__bis_SR_register(LPM0_bits + GIE);
	}
}

/***********************************************************
  Funtion:

   Works as a simple moving averager. Used for gesture recognition.

 */
int32_t movingAvg(int prevAvg, int16_t newValue)
{
	return (((prevAvg << NUM_AVGR_SUMS) + newValue - prevAvg) >> NUM_AVGR_SUMS);
}

/***********************************************************
  Function: configure_clocks
*/
void configure_clocks(void)
{
    // Sets DCO to 8 MHz
	CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);

    // Sets SMCLK, MCLK source as DCO
	CS_initClockSignal(CS_MCLK,  CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	
    // Sets ACLK source as VLO (~9.4 kHz)	
//	CS_initClockSignal(CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Sets ACLK source as LFMODCLK (~38kHz)
	CS_initClockSignal(CS_ACLK, CS_LFMODOSC_SELECT, CS_CLOCK_DIVIDER_1);

    // Clear and enable global oscillator fault flag
	SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
	SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    __bis_SR_register(GIE);		// Enable global interrupt

	clockValue = CS_getACLK();
    clockValue = CS_getSMCLK();
    MCLKValue = CS_getMCLK();
}


// NMI interrupt service routine
#pragma vector=UNMI_VECTOR
__interrupt void NMI_ISR(void)
{
	do
	{
		clockStatus = CS_clearAllOscFlagsWithTimeout( 1000 );
	}
	// If it still can't clear the oscillator fault flags after the timeout, trap and wait here.
	while(clockStatus != 0);
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_A_ISR(void)
{
	__bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
}

#if 0
int _system_pre_init(void)
{
  // stop WDT
  WDTCTL = WDTPW + WDTHOLD;

  // Perform C/C++ global data initialization
  return 1;
}
#endif
