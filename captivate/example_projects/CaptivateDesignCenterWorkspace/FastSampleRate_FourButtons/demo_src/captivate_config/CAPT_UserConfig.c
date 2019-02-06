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
// CAPT_UserConfig.h
//
// Example User Configuration file for the FastSampleRate_FourButtons
// demonstration.
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
// Sensor: modeKeypadSensor, Element: E00
uint16_t modeKeypadSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E00 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = modeKeypadSensor_E00_RawCnts,
    .pTuning = modeKeypadSensor_E00_Tuning,
};

// Sensor: modeKeypadSensor, Element: E01
uint16_t modeKeypadSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E01 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 12,
    .pRawCount = modeKeypadSensor_E01_RawCnts,
    .pTuning = modeKeypadSensor_E01_Tuning,
};

// Sensor: modeKeypadSensor, Element: E02
uint16_t modeKeypadSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E02 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 6,
    .pRawCount = modeKeypadSensor_E02_RawCnts,
    .pTuning = modeKeypadSensor_E02_Tuning,
};

// Sensor: modeKeypadSensor, Element: E03
uint16_t modeKeypadSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E03 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 12,
    .pRawCount = modeKeypadSensor_E03_RawCnts,
    .pTuning = modeKeypadSensor_E03_Tuning,
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
// Time Cycle: modeKeypadSensor_C00
tElement* modeKeypadSensor_C00_Elements[4] =
{
    &modeKeypadSensor_E00,
    &modeKeypadSensor_E01,
    &modeKeypadSensor_E02,
    &modeKeypadSensor_E03,
};
tCycle modeKeypadSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = modeKeypadSensor_C00_Elements,
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
//Sensor: modeKeypadSensor
const tCycle* modeKeypadSensor_Cycles[1] =
{
    &modeKeypadSensor_C00,
};

tButtonSensorParams modeKeypadSensor_Params;
tSensor modeKeypadSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = modeKeypadSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&modeKeypadSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 150,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = true,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 5,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 8191,
    .ui16TimeoutThreshold = 10000,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 3,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 3,
    .bCountFilterEnable = true,
    .ui8CntBeta = 3,
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
    &modeKeypadSensor,
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
    .bSensorDataTxEnable = false,
    .ui16ActiveModeScanPeriod = 1,
    .ui16WakeOnProxModeScanPeriod = 100,
    .ui16InactivityTimeout = 32,
    .ui8WakeupInterval = 5,
};
