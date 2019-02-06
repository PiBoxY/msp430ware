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
// Example User Configuration file for the Backchennel_UART demonstration.
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
// Sensor: keypadSensor, Element: E00
uint16_t keypadSensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E00 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E00_RawCnts,
    .pTuning = keypadSensor_E00_Tuning,
};

// Sensor: keypadSensor, Element: E01
uint16_t keypadSensor_E01_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E01_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E01 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E01_RawCnts,
    .pTuning = keypadSensor_E01_Tuning,
};

// Sensor: keypadSensor, Element: E02
uint16_t keypadSensor_E02_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E02_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E02 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E02_RawCnts,
    .pTuning = keypadSensor_E02_Tuning,
};

// Sensor: keypadSensor, Element: E03
uint16_t keypadSensor_E03_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E03_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E03 =
{
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E03_RawCnts,
    .pTuning = keypadSensor_E03_Tuning,
};

// Sensor: keypadSensor, Element: E04
uint16_t keypadSensor_E04_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E04_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E04 =
{
    .ui8RxPin = 1,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E04_RawCnts,
    .pTuning = keypadSensor_E04_Tuning,
};

// Sensor: keypadSensor, Element: E05
uint16_t keypadSensor_E05_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E05_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E05 =
{
    .ui8RxPin = 1,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E05_RawCnts,
    .pTuning = keypadSensor_E05_Tuning,
};

// Sensor: keypadSensor, Element: E06
uint16_t keypadSensor_E06_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E06_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E06 =
{
    .ui8RxPin = 1,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E06_RawCnts,
    .pTuning = keypadSensor_E06_Tuning,
};

// Sensor: keypadSensor, Element: E07
uint16_t keypadSensor_E07_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning keypadSensor_E07_Tuning[CAPT_SELF_FREQ_CNT];
tElement keypadSensor_E07 =
{
    .ui8RxPin = 1,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 10,
    .pRawCount = keypadSensor_E07_RawCnts,
    .pTuning = keypadSensor_E07_Tuning,
};

// Sensor: proximitySensor, Element: E00
uint16_t proximitySensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proximitySensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement proximitySensor_E00 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 13,
    .pRawCount = proximitySensor_E00_RawCnts,
    .pTuning = proximitySensor_E00_Tuning,
};

// Sensor: sliderSensor, Element: E00
uint16_t sliderSensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning sliderSensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement sliderSensor_E00 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 18,
    .pRawCount = sliderSensor_E00_RawCnts,
    .pTuning = sliderSensor_E00_Tuning,
};

// Sensor: sliderSensor, Element: E01
uint16_t sliderSensor_E01_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning sliderSensor_E01_Tuning[CAPT_SELF_FREQ_CNT];
tElement sliderSensor_E01 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 19,
    .pRawCount = sliderSensor_E01_RawCnts,
    .pTuning = sliderSensor_E01_Tuning,
};

// Sensor: sliderSensor, Element: E02
uint16_t sliderSensor_E02_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning sliderSensor_E02_Tuning[CAPT_SELF_FREQ_CNT];
tElement sliderSensor_E02 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 19,
    .pRawCount = sliderSensor_E02_RawCnts,
    .pTuning = sliderSensor_E02_Tuning,
};

// Sensor: sliderSensor, Element: E03
uint16_t sliderSensor_E03_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning sliderSensor_E03_Tuning[CAPT_SELF_FREQ_CNT];
tElement sliderSensor_E03 =
{
    .ui8RxPin = 2,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 19,
    .pRawCount = sliderSensor_E03_RawCnts,
    .pTuning = sliderSensor_E03_Tuning,
};

// Sensor: wheelSensor, Element: E00
uint16_t wheelSensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning wheelSensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement wheelSensor_E00 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 20,
    .pRawCount = wheelSensor_E00_RawCnts,
    .pTuning = wheelSensor_E00_Tuning,
};

// Sensor: wheelSensor, Element: E01
uint16_t wheelSensor_E01_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning wheelSensor_E01_Tuning[CAPT_SELF_FREQ_CNT];
tElement wheelSensor_E01 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 21,
    .pRawCount = wheelSensor_E01_RawCnts,
    .pTuning = wheelSensor_E01_Tuning,
};

// Sensor: wheelSensor, Element: E02
uint16_t wheelSensor_E02_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning wheelSensor_E02_Tuning[CAPT_SELF_FREQ_CNT];
tElement wheelSensor_E02 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 21,
    .pRawCount = wheelSensor_E02_RawCnts,
    .pTuning = wheelSensor_E02_Tuning,
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
tElement* keypadSensor_C00_Elements[4] =
{
    &keypadSensor_E00,
    &keypadSensor_E01,
    &keypadSensor_E02,
    &keypadSensor_E03,
};
tCycle keypadSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = keypadSensor_C00_Elements,
};

// Time Cycle: keypadSensor_C01
tElement* keypadSensor_C01_Elements[4] =
{
    &keypadSensor_E04,
    &keypadSensor_E05,
    &keypadSensor_E06,
    &keypadSensor_E07,
};
tCycle keypadSensor_C01 =
{
    .ui8NrOfElements = 4,
    .pElements = keypadSensor_C01_Elements,
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

// Time Cycle: sliderSensor_C00
tElement* sliderSensor_C00_Elements[4] =
{
    &sliderSensor_E00,
    &sliderSensor_E01,
    &sliderSensor_E02,
    &sliderSensor_E03,
};
tCycle sliderSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = sliderSensor_C00_Elements,
};

// Time Cycle: wheelSensor_C00
tElement* wheelSensor_C00_Elements[3] =
{
    &wheelSensor_E00,
    &wheelSensor_E01,
    &wheelSensor_E02,
};
tCycle wheelSensor_C00 =
{
    .ui8NrOfElements = 3,
    .pElements = wheelSensor_C00_Elements,
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
const tCycle* keypadSensor_Cycles[2] =
{
    &keypadSensor_C00,
    &keypadSensor_C01,
};

tButtonSensorParams keypadSensor_Params;
tSensor keypadSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 2,
    .pCycle = keypadSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&keypadSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 250,
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
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 1980,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 1,
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
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = proximitySensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&proximitySensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 800,
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
    .ui16ProxThreshold = 12,
    .ui16NegativeTouchThreshold = 15,
    .ui16ErrorThreshold = 2000,
    .ui16TimeoutThreshold = 7920,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 1,
    .TouchDbThreshold.DbOut = 4,
    .bCountFilterEnable = true,
    .ui8CntBeta = 2,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: sliderSensor
const tCycle* sliderSensor_Cycles[1] =
{
    &sliderSensor_C00,
};

tSliderSensorParams sliderSensor_Params =
{
    .ui8TotalElements = 4,
    .ui16Resolution = 1000,
    .SliderBeta = 175,
    .SliderLower = 45,
    .SliderUpper = 960,
    .SliderAlgorithm = eVectors,
    .SliderFilterEnable = true,
};

tSensor sliderSensor =
{
    // Basic Properties
    .TypeOfSensor = eSlider,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = sliderSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&sliderSensor_Params,
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
    .ui16TimeoutThreshold = 3960,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: wheelSensor
const tCycle* wheelSensor_Cycles[1] =
{
    &wheelSensor_C00,
};

tWheelSensorParams wheelSensor_Params =
{
    .ui8TotalElements = 3,
    .ui16Resolution = 100,
    .SliderBeta = 150,
    .SliderUpper = 100,
    .SliderLower = 0,
    .SliderAlgorithm = eVectors,
    .SliderFilterEnable = true,
};

tSensor wheelSensor =
{
    // Basic Properties
    .TypeOfSensor = eWheel,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = wheelSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&wheelSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 500,
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
    .ui16TimeoutThreshold = 3960,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = false,
    .ui8CntBeta = 0,
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
    &sliderSensor,
    &wheelSensor,
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
    .ui16ActiveModeScanPeriod = 33,
    .ui16WakeOnProxModeScanPeriod = 100,
    .ui16InactivityTimeout = 32,
    .ui8WakeupInterval = 5,
};
