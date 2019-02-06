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

<CDC_DATE>

//*****************************************************************************
// CAPT_UserConfig.c
//
// \version 1.70.00.03
// Released on July 30, 2018
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
<CDC_ELEMENT_STRUCTURE>

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
<CDC_CYCLE_STRUCTURE>

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
<CDC_SENSOR_STRUCTURE>

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
<CDC_SENSOR_ARRAY>

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
	<CDC_CAPTIVATE_SETTINGS>
};
