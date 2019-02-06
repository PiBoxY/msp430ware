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
// Example User Configuration file for the CAPTIVATE-PROXIMITY demonstration.
//
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
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
// Sensor: proximityGroupSensor, Element: E00
uint16_t proximityGroupSensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proximityGroupSensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement proximityGroupSensor_E00 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 14,
    .pRawCount = proximityGroupSensor_E00_RawCnts,
    .pTuning = proximityGroupSensor_E00_Tuning,
};

// Sensor: proximityGroupSensor, Element: E01
uint16_t proximityGroupSensor_E01_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proximityGroupSensor_E01_Tuning[CAPT_SELF_FREQ_CNT];
tElement proximityGroupSensor_E01 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 14,
    .pRawCount = proximityGroupSensor_E01_RawCnts,
    .pTuning = proximityGroupSensor_E01_Tuning,
};

// Sensor: proximityGroupSensor, Element: E02
uint16_t proximityGroupSensor_E02_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proximityGroupSensor_E02_Tuning[CAPT_SELF_FREQ_CNT];
tElement proximityGroupSensor_E02 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 14,
    .pRawCount = proximityGroupSensor_E02_RawCnts,
    .pTuning = proximityGroupSensor_E02_Tuning,
};

// Sensor: proximityGroupSensor, Element: E03
uint16_t proximityGroupSensor_E03_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proximityGroupSensor_E03_Tuning[CAPT_SELF_FREQ_CNT];
tElement proximityGroupSensor_E03 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 14,
    .pRawCount = proximityGroupSensor_E03_RawCnts,
    .pTuning = proximityGroupSensor_E03_Tuning,
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
// Time Cycle: proximityGroupSensor_C00
tElement* proximityGroupSensor_C00_Elements[4] =
{
    &proximityGroupSensor_E00,
    &proximityGroupSensor_E01,
    &proximityGroupSensor_E02,
    &proximityGroupSensor_E03,
};
tCycle proximityGroupSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = proximityGroupSensor_C00_Elements,
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
//Sensor: proximityGroupSensor
const tCycle* proximityGroupSensor_Cycles[1] =
{
    &proximityGroupSensor_C00,
};

tButtonSensorParams proximityGroupSensor_Params;
tSensor proximityGroupSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = proximityGroupSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&proximityGroupSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 1200,
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
    .ui16ProxThreshold = 25,
    .ui16NegativeTouchThreshold = 30,
    .ui16ErrorThreshold = 1500,
    .ui16TimeoutThreshold = 3000,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 2,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 0,
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
    &proximityGroupSensor,
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
    .ui8AppLPM = LPM3_bits,
    .bElementDataTxEnable = true,
    .bSensorDataTxEnable = true,
    .ui16ActiveModeScanPeriod = 5,
    .ui16WakeOnProxModeScanPeriod = 33,
    .ui16InactivityTimeout = 375,
    .ui8WakeupInterval = 6,
};
