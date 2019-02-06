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

#ifndef __IEC60730_SYSTEM_CONFIG_H__
#define __IEC60730_SYSTEM_CONFIG_H__

//*****************************************************************************
//
//The following include contains all the users configuration to build
//the IEC60730 library and determine which modules are available in the
//selected MSP430 device.
//
//*****************************************************************************

#include "IEC60730_user_config.h"
//#include <msp430.h>
#include <DRV91670.h>


#ifdef __cplusplus
extern "C"
{
#endif



//***************************************************************************************
//
// Definitions for Oscillator test this section is used to determine MAX and MIN values
// for the given main frequency taking into account the allowable frequnecy drift %
// specified in "IEC60730_user_config.h" file
//
//***************************************************************************************
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
#define MAIN_CLOCK_FREQUENCY				1000000
#elif defined(MAIN_CLOCK_FREQUENCY_8MHz)
#define MAIN_CLOCK_FREQUENCY				8000000
#elif defined(MAIN_CLOCK_FREQUENCY_12MHz)
#define MAIN_CLOCK_FREQUENCY				12000000
#endif

#define ACLK_FREQUENCY						(LFXT1_FREQUENCY/LFXT1_FREQUENCY_DIVIDER)


#ifdef USE_TAxCLK
#ifdef USE_TA0
#define USE_TA0CLK_INPUT
#endif //USE_TA0
#ifdef USE_TA1
#define USE_TA1CLK_INPUT
#endif //USE_TA1
#ifdef USE_TA2
#define USE_TA2CLK_INPUT
#endif //USE_TA2
#endif // USE_TAxCLK


#ifdef START_UP_TIME
#define START_UP_DELAY	((MAIN_CLOCK_FREQUENCY)*(START_UP_TIME*0.001))
#endif // START_UP_TIME

//***************************************************************************************
//
// The following value is selected to have an approximate 10 msec interval to verify
// correct frequnecy of MCLK
//
//***************************************************************************************

#define	CCRn_VALUE_FOR_10_mSEC	ACLK_FREQUENCY/100
#define CYCLES_PER_PERIOD		((MAIN_CLOCK_FREQUENCY/MAIN_CLOCK_DIVIDER)/ACLK_FREQUENCY)
#define CYCLES_AVAILABLE_FOR_TEST	(CYCLES_PER_PERIOD*CCRn_VALUE_FOR_10_mSEC)


//***************************************************************************************
//
// If Compiling project with IAR
//
//***************************************************************************************
#ifdef __ICC430__
//***************************************************************************************
//
// If library is built for MSP430 CPUX architecture
//
//***************************************************************************************
#if __CORE__
#define NUMBER_OF_CYLES_TO_INCREMENT_COUNTER	6
//***************************************************************************************
//
// If library is built for MSP430 CPU architecture
//
//***************************************************************************************
#else
#define NUMBER_OF_CYLES_TO_INCREMENT_COUNTER	6
#endif
#else
//***************************************************************************************
//
// If Compiling project with CCS
//
//***************************************************************************************
#if defined(__MSP430X__)
//***************************************************************************************
//
// If library is built for MSP430 CPUX architecture
//
//***************************************************************************************
#define NUMBER_OF_CYLES_TO_INCREMENT_COUNTER	6
//***************************************************************************************
//
// If library is built for MSP430 CPU architecture
//
//***************************************************************************************
#elif defined(__MSP430__)
#define NUMBER_OF_CYLES_TO_INCREMENT_COUNTER	7
#endif
#endif


#define FREQUENCY_COUNT 		(CYCLES_AVAILABLE_FOR_TEST/NUMBER_OF_CYLES_TO_INCREMENT_COUNTER)
#define FREQUENCY_COUNT_MAX		(FREQUENCY_COUNT*(100 + PERCENT_FREQUENCY_DRIFT))/100
#define FREQUENCY_COUNT_MIN		(FREQUENCY_COUNT*(100 - PERCENT_FREQUENCY_DRIFT))/100


#ifdef __cplusplus
}
#endif

#endif
