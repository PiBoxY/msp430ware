#ifndef _USSSWLIBCALIBRATION_H_
#define _USSSWLIBCALIBRATION_H_

//#############################################################################
//
//! \file   ussSwLibCalibration.h
//!
//! \brief  Contains the look-up table for the Gain Calibration.
//!         
//
//  Group:          MSP
//  Target Device:  Banshee
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: USSLib_02_20_00_16 
// Release Date: January 23, 2019
//#############################################################################


//*****************************************************************************
// includes
//*****************************************************************************
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "ussSwLib.h"
#include <math.h>
#include "common/ussSwLibCommonInterrupts.h"
#include "common/ussSwLibCommonSDHS.h"
#include "common/ussSwLibCommonUSS.h"
#include "common/ussSwLibCommonTimer.h"
#include "common/USSGenericRegDef.h"



//*****************************************************************************
//! \addtogroup ussSwLib 
//! @{
//*****************************************************************************

#ifdef __cplusplus

extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************

// Number of iterations used for averaging = 2^(RESONATOR_CALIB_ITERATION_COUNT)
// If RESONATOR_CALIB_ITERATION_COUNT == 0, Number of iterations used for
// averaging = 1.
// If RESONATOR_CALIB_ITERATION_COUNT == 1, Number of iterations used for
// averaging = 2
//
// Minimum value = 0
// Maximum value = 10
#define RESONATOR_CALIB_ITERATION_COUNT             2

// Number of ACLK cycles to monitor SAPH ASQCLK.
// Minimum value = 1
// Maximum value = 3000
#define RESONATOR_CALIB_MONITORING_ACLK             550


#define AGC_LOOP_MAX_ITERATIONS					5
#define AGC_LOOP_SETTLING						3
#define ADC_MINIMUM_GAIN_SETTING 				USS_Capture_Gain_Range_minus_6_5
#define AGC_GAIN_TABLE_SIZE						(47*2)
#define AGC_INDEX_OFFSET						17
#define AGC_MIN_DIFFERENCE						1.0


// Advanced / Derived parameters
// Number of ACLK cycles need to guarantee MSP will go to LPM
// minimum = 2 (recommended)
// maximum = 20
#define RESONATOR_CALIB_MONITORING_ACLK_MIN_COUNT   4

#define PLL_68_MHZ_TEMP_CONST      (uint64_t)((uint64_t)68000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_69_MHZ_TEMP_CONST      (uint64_t)((uint64_t)69000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_70_MHZ_TEMP_CONST      (uint64_t)((uint64_t)70000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_71_MHZ_TEMP_CONST      (uint64_t)((uint64_t)71000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_72_MHZ_TEMP_CONST      (uint64_t)((uint64_t)72000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_73_MHZ_TEMP_CONST      (uint64_t)((uint64_t)73000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_74_MHZ_TEMP_CONST      (uint64_t)((uint64_t)74000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_75_MHZ_TEMP_CONST      (uint64_t)((uint64_t)75000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_76_MHZ_TEMP_CONST      (uint64_t)((uint64_t)76000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_77_MHZ_TEMP_CONST      (uint64_t)((uint64_t)77000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_78_MHZ_TEMP_CONST      (uint64_t)((uint64_t)78000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_79_MHZ_TEMP_CONST      (uint64_t)((uint64_t)79000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)
#define PLL_80_MHZ_TEMP_CONST      (uint64_t)((uint64_t)80000000* \
                                   (uint64_t)RESONATOR_CALIB_MONITORING_ACLK)

#define ABSTOF_DTOF_MAX_ITERATIONS                      64999

//*****************************************************************************
// typedefs
//*****************************************************************************


//*****************************************************************************
// globals
//*****************************************************************************
// Contains pairs of (USS Capture Gain setting,the corresponding dB value)
const float gUSSGainLookUp[AGC_GAIN_TABLE_SIZE]=
{
	17,-6.5,
	18,-5.5,
	19,-4.6,
	20,-4.1,
	21,-3.3,
	22,-2.3,
	23,-1.4,
	24,-0.8,
	25,0.1,
	26,1,
	27,1.9,
	28,2.6,
	29,3.5,
	30,4.4,
	31,5.2,
	32,6,
	33,6.8,
	34,7.7,
	35,8.7,
	36,9,
	37,9.8,
	38,10.7,
	39,11.7,
	40,12.2,
	41,13,
	42,13.9,
	43,14.9,
	44,15.5,
	45,16.3,
	46,17.2,
	47,18.2,
	48,18.8,
	49,19.6,
	50,20.5,
	51,21.5,
	52,22,
	53,22.8,
	54,23.6,
	55,24.6,
	56,25,
	57,25.8,
	58,26.7,
	59,27.7,
	60,28.1,
	61,28.9,
	62,29.9,
	63,30.8
};

//*****************************************************************************
// the function prototypes
//*****************************************************************************
static float USS_findAGCIndex(int8_t leftIndex, int8_t rightIndex, float tempGain);

#ifdef __cplusplus
}
#endif // extern "C"
//@}  // ingroup

#endif // end of  _USSSWLIBCALIBRATION_H_ definition
