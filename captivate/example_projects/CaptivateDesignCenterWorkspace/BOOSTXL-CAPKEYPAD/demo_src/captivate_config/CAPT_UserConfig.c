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
//*****************************************************************************
// CAPT_UserConfig.c
//
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//
//*****************************************************************************

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
// Sensor: keypadSensor, Element: E00
uint16_t keypadSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E00 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 1,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E00_RawCnts,
    .pTuning = keypadSensor_E00_Tuning,
};

// Sensor: keypadSensor, Element: E01
uint16_t keypadSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E01 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 1,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E01_RawCnts,
    .pTuning = keypadSensor_E01_Tuning,
};

// Sensor: keypadSensor, Element: E02
uint16_t keypadSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E02 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 2,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E02_RawCnts,
    .pTuning = keypadSensor_E02_Tuning,
};

// Sensor: keypadSensor, Element: E03
uint16_t keypadSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E03 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 11,
    .pRawCount = keypadSensor_E03_RawCnts,
    .pTuning = keypadSensor_E03_Tuning,
};

// Sensor: keypadSensor, Element: E04
uint16_t keypadSensor_E04_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E04_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E04 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 1,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E04_RawCnts,
    .pTuning = keypadSensor_E04_Tuning,
};

// Sensor: keypadSensor, Element: E05
uint16_t keypadSensor_E05_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E05_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E05 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 1,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E05_RawCnts,
    .pTuning = keypadSensor_E05_Tuning,
};

// Sensor: keypadSensor, Element: E06
uint16_t keypadSensor_E06_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E06_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E06 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 2,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E06_RawCnts,
    .pTuning = keypadSensor_E06_Tuning,
};

// Sensor: keypadSensor, Element: E07
uint16_t keypadSensor_E07_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E07_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E07 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E07_RawCnts,
    .pTuning = keypadSensor_E07_Tuning,
};

// Sensor: keypadSensor, Element: E08
uint16_t keypadSensor_E08_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E08_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E08 =
{
    .ui8TxPin = 0,
    .ui8TxBlock = 0,
    .ui8RxPin = 1,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 11,
    .pRawCount = keypadSensor_E08_RawCnts,
    .pTuning = keypadSensor_E08_Tuning,
};

// Sensor: keypadSensor, Element: E09
uint16_t keypadSensor_E09_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E09_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E09 =
{
    .ui8TxPin = 0,
    .ui8TxBlock = 0,
    .ui8RxPin = 1,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 14,
    .pRawCount = keypadSensor_E09_RawCnts,
    .pTuning = keypadSensor_E09_Tuning,
};

// Sensor: keypadSensor, Element: E10
uint16_t keypadSensor_E10_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E10_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E10 =
{
    .ui8TxPin = 0,
    .ui8TxBlock = 0,
    .ui8RxPin = 2,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 11,
    .pRawCount = keypadSensor_E10_RawCnts,
    .pTuning = keypadSensor_E10_Tuning,
};

// Sensor: keypadSensor, Element: E11
uint16_t keypadSensor_E11_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E11_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement keypadSensor_E11 =
{
    .ui8TxPin = 0,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 12,
    .pRawCount = keypadSensor_E11_RawCnts,
    .pTuning = keypadSensor_E11_Tuning,
};

// Sensor: proximitySensor, Element: E00
uint16_t proximitySensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning proximitySensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement proximitySensor_E00 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 3,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 25,
    .pRawCount = proximitySensor_E00_RawCnts,
    .pTuning = proximitySensor_E00_Tuning,
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
// Time Cycle: keypadSensor_C00
tElement* keypadSensor_C00_Elements[2] =
{
    &keypadSensor_E00,
    &keypadSensor_E01,
};
tCycle keypadSensor_C00 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C00_Elements,
};

// Time Cycle: keypadSensor_C01
tElement* keypadSensor_C01_Elements[2] =
{
    &keypadSensor_E02,
    &keypadSensor_E03,
};
tCycle keypadSensor_C01 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C01_Elements,
};

// Time Cycle: keypadSensor_C02
tElement* keypadSensor_C02_Elements[2] =
{
    &keypadSensor_E04,
    &keypadSensor_E05,
};
tCycle keypadSensor_C02 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C02_Elements,
};

// Time Cycle: keypadSensor_C03
tElement* keypadSensor_C03_Elements[2] =
{
    &keypadSensor_E06,
    &keypadSensor_E07,
};
tCycle keypadSensor_C03 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C03_Elements,
};

// Time Cycle: keypadSensor_C04
tElement* keypadSensor_C04_Elements[2] =
{
    &keypadSensor_E08,
    &keypadSensor_E09,
};
tCycle keypadSensor_C04 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C04_Elements,
};

// Time Cycle: keypadSensor_C05
tElement* keypadSensor_C05_Elements[2] =
{
    &keypadSensor_E10,
    &keypadSensor_E11,
};
tCycle keypadSensor_C05 =
{
    .ui8NrOfElements = 2,
    .pElements = keypadSensor_C05_Elements,
};

// Time Cycle: proximitySensor_C00
tElement* proximitySensor_C00_Elements[1] =
{
    &proximitySensor_E00,
};
tCycle proximitySensor_C00 =
{
    .ui8NrOfElements = 1,
    .pElements = proximitySensor_C00_Elements,
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
//Sensor: keypadSensor
const tCycle* keypadSensor_Cycles[6] =
{
    &keypadSensor_C00,
    &keypadSensor_C01,
    &keypadSensor_C02,
    &keypadSensor_C03,
    &keypadSensor_C04,
    &keypadSensor_C05,
};

tButtonSensorParams keypadSensor_Params;
tSensor keypadSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 6,
    .pCycle = keypadSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&keypadSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 225,
    .ui16ConversionGain = 150,
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
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 30,
    .ui16ErrorThreshold = 400,
    .ui16TimeoutThreshold = 1500,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 4,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 1,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: proximitySensor
const tCycle* proximitySensor_Cycles[1] =
{
    &proximitySensor_C00,
};

tProxSensorParams proximitySensor_Params =
{
    .pSensor = NULL,
    .ui8NumberOfSensors = 0,
};

tSensor proximitySensor =
{
    // Basic Properties
    .TypeOfSensor = eProx,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = proximitySensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&proximitySensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 500,
    .ui16ConversionGain = 175,
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
    .ui16ProxThreshold = 12,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 3000,
    .ui16TimeoutThreshold = 1500,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 10,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 10,
    .bCountFilterEnable = true,
    .ui8CntBeta = 2,
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
	.projModeConversionStyle = eMultiFrequencyWithOutlierRemoval,
	
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
    &keypadSensor,
    &proximitySensor,
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
    .ui16ActiveModeScanPeriod = 14,
    .ui16WakeOnProxModeScanPeriod = 100,
    .ui16InactivityTimeout = 300,
    .ui8WakeupInterval = 6,
};
