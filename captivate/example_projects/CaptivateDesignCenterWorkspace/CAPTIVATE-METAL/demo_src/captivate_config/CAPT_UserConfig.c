/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
//
// NOTE: This is an automatically generated source code file!  The Captivate
// Design Center generates the User Configuration file automatically based
// upon the sensor layout that was created.
//
// Changes to this file will be OVERWRITTEN if a you select
// "Update Existing Project" under "Generate Source Code" in the Design Center.
//
// To avoid interference with the code generation process, keep ALL application
// code external to this file.
//
//*****************************************************************************

#include "CAPT_UserConfig.h"

//*****************************************************************************
//
//! Captivate Element Definitions
//! All elements in this application are defined below.
//! Each element has 3 components:
//!  1) a raw count array (One index per freq. scanned) (uint16_t)
//!  2) a tuning array (One index per freq. scanned) (tCaptivateElementTuning)
//!  3) a element structure (tElement)
//
//*****************************************************************************
// Sensor: BUTTONS, Element: E00
uint16_t BUTTONS_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E00 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E00_RawCnts,
    .pTuning = BUTTONS_E00_Tuning,
};

// Sensor: BUTTONS, Element: E01
uint16_t BUTTONS_E01_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E01_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E01 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E01_RawCnts,
    .pTuning = BUTTONS_E01_Tuning,
};

// Sensor: BUTTONS, Element: E02
uint16_t BUTTONS_E02_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E02_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E02 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E02_RawCnts,
    .pTuning = BUTTONS_E02_Tuning,
};

// Sensor: BUTTONS, Element: E03
uint16_t BUTTONS_E03_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E03_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E03 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E03_RawCnts,
    .pTuning = BUTTONS_E03_Tuning,
};

// Sensor: BUTTONS, Element: E04
uint16_t BUTTONS_E04_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E04_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E04 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E04_RawCnts,
    .pTuning = BUTTONS_E04_Tuning,
};

// Sensor: BUTTONS, Element: E05
uint16_t BUTTONS_E05_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E05_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E05 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E05_RawCnts,
    .pTuning = BUTTONS_E05_Tuning,
};

// Sensor: BUTTONS, Element: E06
uint16_t BUTTONS_E06_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E06_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E06 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E06_RawCnts,
    .pTuning = BUTTONS_E06_Tuning,
};

// Sensor: BUTTONS, Element: E07
uint16_t BUTTONS_E07_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning BUTTONS_E07_Tuning[CAPT_SELF_FREQ_CNT];
tElement BUTTONS_E07 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 7,
    .pRawCount = BUTTONS_E07_RawCnts,
    .pTuning = BUTTONS_E07_Tuning,
};


//*****************************************************************************
//
//! Captivate Time Cycle Definitions
//! All time cycles in this application are defined below.  Time cycles are
//! groups of elements that are measured together in parallel in one time slot.
//! Each cycle has 2 components:
//!  1) an element pointer array to the member elements (tElement*)
//!  2) a cycle structure (tCycle)
//
//*****************************************************************************
// Time Cycle: BUTTONS_C00
tElement* BUTTONS_C00_Elements[4] =
{
    &BUTTONS_E00,
    &BUTTONS_E01,
    &BUTTONS_E02,
    &BUTTONS_E03,
};
tCycle BUTTONS_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = BUTTONS_C00_Elements,
};

// Time Cycle: BUTTONS_C01
tElement* BUTTONS_C01_Elements[4] =
{
    &BUTTONS_E04,
    &BUTTONS_E05,
    &BUTTONS_E06,
    &BUTTONS_E07,
};
tCycle BUTTONS_C01 =
{
    .ui8NrOfElements = 4,
    .pElements = BUTTONS_C01_Elements,
};


//*****************************************************************************
//
//! Captivate Sensor Definitions
//! All sensors in this application are defined below.  Sensors are
//! groups of time cycles that utilize raw measurement data to create an
//! abstract sensor type, such as a button, slider, wheel, or prox sensor.
//! Each sensor has 3 components:
//!  1) a cycle pointer array to the member time cycles (tCycle*)
//!  2) a sensor-specific parameter structure (tGenericSensorParams)
//!  3) a sensor structure (tSensor)
//
//*****************************************************************************
//Sensor: BUTTONS
const tCycle* BUTTONS_Cycles[2] =
{
    &BUTTONS_C00,
    &BUTTONS_C01,
};

tButtonSensorParams BUTTONS_Params;
tSensor BUTTONS =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 2,
    .pCycle = BUTTONS_Cycles,
    .pSensorParams = (tGenericSensorParams*)&BUTTONS_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 600,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 2,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 20,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 65535,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 0,
    .TouchDbThreshold.DbIn = 1,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};


#if (CAPT_CONDUCTED_NOISE_IMMUNITY_ENABLE==true)
//*****************************************************************************
//
//! \var g_EMCConfig
//! This structure stores the EMC configuration for this application.
//
//*****************************************************************************
const tEMCConfig g_EMCConfig =
{
	// Conversion Style
	.selfModeConversionStyle = eMultiFrequency,
	.projModeConversionStyle = eMultiFrequencyWithOutlierRemovalAndErrCorrection,
	
	// Oversampling Style
	.selfModeOversamplingStyle = eNoOversampling,
	.projModeOversamplingStyle = eNoOversampling,
	
	// Jitter Filter Enable
	.bJitterFilterEnable = true,
	
	// Noise Thresholds and Calibration Noise Limits
	.ui8NoiseThreshold = 20,
	.ui16CalibrationNoiseLimit = 10,
	.ui8CalibrationTestSampleSize = 8,
		
	// Dynamic Threshold Adjustment Parameters
	.bEnableDynamicThresholdAdjustment = true,
	.ui8MaxRelThreshAdj = 76,
	.ui8NoiseLevelFilterEntryThresh = 40,
	.ui8NoiseLevelFilterExitThresh = 0,
	.ui8NoiseLevelFilterDown = 6,
	.ui8NoiseLevelFilterUp = 1,
	.coeffA = _IQ31(0.0065),
	.coeffB = _IQ31(0.050)
};
#endif

//*****************************************************************************
//
//! \var g_pCaptivateSensorArray
//! This array allows for indexed access to any
//! sensor in the configuration.
//
//*****************************************************************************
tSensor* g_pCaptivateSensorArray[CAPT_SENSOR_COUNT] =
{
    &BUTTONS,
};


//*****************************************************************************
//
//! \var g_uiApp
//! This structure stores the global settings for this application.
//
//*****************************************************************************
tCaptivateApplication g_uiApp =
{
    .state = eUIActive,
	.pSensorList = &g_pCaptivateSensorArray[0],
    .ui8NrOfSensors = CAPT_SENSOR_COUNT,
    .ui8AppLPM = LPM0_bits,
    .bElementDataTxEnable = true,
    .bSensorDataTxEnable = true,
    .ui16ActiveModeScanPeriod = 20,
    .ui16WakeOnProxModeScanPeriod = 100,
    .ui16InactivityTimeout = 32,
    .ui8WakeupInterval = 5,
};
