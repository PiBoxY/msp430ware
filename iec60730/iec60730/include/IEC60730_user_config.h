/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

#ifndef _IEC60730_USER_CONFIG_H_
#define _IEC60730_USER_CONFIG_H_

#include <msp430.h>

#ifdef __cplusplus
extern "C"
{
#endif

//***************************************************************************************
// Start of User Options
//***************************************************************************************

//**************************************************************************************
// FOR WATCHDOG TIMER options
// Section to define if WDT is enabled 0=disabled 1= enabled
//**************************************************************************************
#define ENABLED_WDT				1


//**************************************************************************************
// Define for handle test failures
//
// Section to define if JUMP_TO_FAILSAFE is enabled 0=disabled 1= enabled
//
// If JUMP_TO_FAILSAFE is enabled the user must create a function called
// "IEC60730_FAIL_SAFE_failSafe(void)" to handle the failure of any test
//
// If JUMP_TO_FAILSAFE is disabled all test will return TEST_FAILURE. TEST_FAILURE is
// define in "IEC60730_system_config.h"
//**************************************************************************************

#define JUMP_TO_FAILSAFE		1


//***************************************************************************************
// Defines for OSCILLATOR_TEST
//***************************************************************************************

//***************************************************************************************
// Defines to specify which Timer_Ax and CCRn to be used for Oscillator fault test
// You must define one one USE_TAx and and one USE_CCRn. Otherwise, the test may
// result in unexpected beahvior.
//
// NOTE: The selected Timer_Ax and CCRn must be available in the device. If the selected
//		 timer is not available in the device, the project will throw compilation errors
//***************************************************************************************

#define USE_TA0
//#define USE_TA1
//#define USE_TA2


#define USE_CCR0
//#define USE_CCR1
//#define USE_CCR2
//#define USE_CCR3
//#define USE_CCR4

//***************************************************************************************
// Define to specify the MCLK frequency
//
// Uncomment the frequency at which the MCLK will be running.
//
// If the desired MCLK frequency is not available you can select
// MAIN_CLOCK_FREQUENCY_USER_DEFINED.
//
// MAIN_CLOCK_EXTERNAL_XT2 if a external high speed oscillator or crystal will be used.
//
//
// NOTE: If MAIN_CLOCK_EXTERNAL_XT2 or MAIN_CLOCK_FREQUENCY is defined you must define
// MAIN_CLOCK_FREQUENCY and provide the frequency in Hz.
//***************************************************************************************

//#define MAIN_CLOCK_EXTERNAL_XT2
//#define MAIN_CLOCK_USER_DEFINED
//#define MAIN_CLOCK_FREQUENCY_1MHz
//#define MAIN_CLOCK_FREQUENCY_8MHz
#define MAIN_CLOCK_FREQUENCY_12MHz

//***************************************************************************************
// Uncomment if MAIN_CLOCK_EXTERNAL_XT2 or MAIN_CLOCK_USER_DEFINED option was
// selected above.
//
// NOTE: Make sure to specify the frequency in HZ
//
// CAUTION: If using predefined frequency DO NOT uncomment this define. Uncommenting
// this define may result in unexpected behavior for OSCILLATOR_TEST
//***************************************************************************************
//#define MAIN_CLOCK_FREQUENCY	4000000

//***************************************************************************************
// Define to specify divider of Main Clock
//***************************************************************************************
#define MAIN_CLOCK_DIVIDER			1

//***************************************************************************************
//Defines to provide information regarding independent clock source that will be used to
//monitor the main clock source
//***************************************************************************************
#define LFXT1_FREQUENCY				    32768
#define LFXT1_FREQUENCY_DIVIDER		        1

//***************************************************************************************
//Defines to provide determine the allowed frequency drift of the main clock (in +/- %)
//***************************************************************************************
#define PERCENT_FREQUENCY_DRIFT		3

//***************************************************************************************
// Defines for RAM MEMORY test
//***************************************************************************************

//***************************************************************************************
// The following defines determine the parameters that will be passed to
// IEC60730_MARCH_TEST_testRam(). The values for RAM_START_ADDRESS and RAM_SIZE
// can be obtain in the device datasheet. The stack size can be determine from your
// compiler configuration setting.
//***************************************************************************************

#if defined(__MSP430F5529__)
#define RAM_START_ADDRESS		        0x2400
#define RAM_SIZE				0x1FF0
#define STACK_SIZE				160
#elif defined(__MSP430G2553__)
#define RAM_START_ADDRESS		0x0200
#define RAM_SIZE				0x01F0
#define STACK_SIZE				80
#elif defined(__MSP430FR5739__)
#define RAM_START_ADDRESS			0x1C00
#define RAM_SIZE				0x03F0
#define STACK_SIZE				160
#else
//***************************************************************************************
// IF you your device is not listed in above please specify the RAM properties
//***************************************************************************************
#define RAM_START_ADDRESS
#define RAM_SIZE
#define STACK_SIZE
#endif

//***************************************************************************************
// The following defines is calculated based on the defines above. This define should
// not be modified.
//***************************************************************************************
#define RAM_END_ADDRESS			((RAM_START_ADDRESS + RAM_SIZE)-STACK_SIZE)


//***************************************************************************************
// The following define selects the type of MARCH test to be implemented in the RAM
// TEST.
//
// NOTE: Only one test should be selected.
//***************************************************************************************
//#define MARCH_C_TEST
#define MARCH_X_TEST

//***************************************************************************************
// Uncomment this define if you want to run the RAM test in  NON-DESTRUCTIVE mode.
//***************************************************************************************
#define NON_DESTRUCTIVE

//***************************************************************************************
// The following define is used if NON-DESTRUCTIVE for march test is selected
//***************************************************************************************
#define RAM_TEST_BUFSIZE        8




//***************************************************************************************
// Defines for NON VOLATILE MEMORY test
//***************************************************************************************


//***************************************************************************************
//
// Definitions required for software implementation of CRC16-CCITT the default seed is
// (0xFFFF) and the polynomial used is x^16+x^12+x^5+1
//
//***************************************************************************************
#define CRC16_CCITT_SEED 			0xFFFF

//***************************************************************************************
//
// If library is being complied for an MSP430 device that does not have a CRC module
// CRC16_CCITT_POLY will be automatically defined
//
//***************************************************************************************
#ifndef __MSP430_HAS_CRC__
#define CRC16_CCITT_POLY 			0x1021
#endif

//***************************************************************************************
// The following define specifies the start location in Flash where CRC check sumsums
// are stored.
// Specify the address where CRC result is located.
// Note: Ideally the location will be in the information memory section of the device.
// To determine the address for the information memory for each device please reffer to
// the device datasheet and locate the "Memory Organization" section in the datasheet.
//
//***************************************************************************************
#if defined(__MSP430F5529__)
#define CRC_CHECKSUM_LOCATION		0x1800
#elif defined(__MSP430G2553__)
#define CRC_CHECKSUM_LOCATION		0x1004
#elif defined(__MSP430FR5739__)
#define CRC_CHECKSUM_LOCATION		0x1880
#else
#define CRC_CHECKSUM_LOCATION
#endif
//***************************************************************************************
// Defines for ADC test
//***************************************************************************************

//***************************************************************************************
//
// The following defines specify the maximum and minimum ADC conversion count drift
// allowed in the application
//
//***************************************************************************************
#define MINIMUM_ADC_COUNT_DRIFT		-50
#define MAXIMUM_ADC_COUNT_DRIFT		50

//***************************************************************************************
//
// Section to define all the sucess codes
//
//***************************************************************************************

#define SIG_CPU_REG_TEST		0xCC
#define SIG_PC_REG_TEST			0xCD
#define SIG_CLOCK_TEST			0xCE
#define SIG_RAM_TEST            0xCF
#define SIG_NV_MEM_CRC_TEST     0xD0
#define SIG_ADC_TEST            0xD1
#define SIG_GPIO_TEST			0xD2
#define SIG_INTERRUPT_TEST		0xD3

//***************************************************************************************
//
// Section to define failure code.
//
// The following failure code will be return by IEC60730 APIs.
//
//***************************************************************************************
#define TEST_FAILURE		0x00

//***************************************************************************************
//End user Options
//***************************************************************************************
#ifdef __cplusplus
}
#endif

#endif
