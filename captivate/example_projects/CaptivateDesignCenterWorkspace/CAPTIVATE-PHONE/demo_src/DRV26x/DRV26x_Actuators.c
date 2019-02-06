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
//
//! \file   DRV26x_Actuators.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.00.00.30
// Release Date: Thu, Oct 01, 2015  5:05:49 PM
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "DRV26x_Actuators.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

// \brief A macro to extract the lower 2 bits of a 4-bit constant.
//!
#define FOURBIT_LOWERHALF(input) (input & 0x03)

//! \brief A macro to extract the upper 2 bits of a 4-bit constant.
//!
#define FOURBIT_UPPERHALF(input) ((input >> 2) & 0x03)

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Default Actuator Configuration: AAC 1036 (LRA)
//!
const DRV26x_Actuator_t DRV26x_actuator_AAC1036 =
{
    // Basic Parameters
	.ratedVoltage.r = 0x60,
	.overdriveClampVoltage.r = 0xA4,

	// Auto-Calibration Parameters
	.autoCal_Compensation.r = 0x0C,
	.autoCal_BackEMF.r = 0x71,

	// Feedback Control Parameters
	.feedbackCtrl.b.bBackEMFGain = DRV26x_backEMFgain_3,
	.feedbackCtrl.b.bLoopGain = DRV26x_loopGain_medium,
	.feedbackCtrl.b.bFBBrakeFactor = DRV26x_feedbackBrakeFactor_3x,
	.feedbackCtrl.b.bActuatorType = DRV26x_actuator_LRA,

	// Control Register 1 Parameters
	.ctrlReg1.b.bDriveTime = 0,
	.ctrlReg1.b.bACCoupling = false,
	.ctrlReg1.b.bStartupBoost = true,

	// Control Register 2 Parameters
	.ctrlReg2.b.bCurrDissTimeLSB = FOURBIT_LOWERHALF(1),
	.ctrlReg2.b.bBlankingTimeLSB = FOURBIT_LOWERHALF(1),
	.ctrlReg2.b.bSamplingTime = DRV26x_LRASamplingTime_300us,
	.ctrlReg2.b.bBrakeStabilizer = false,
	.ctrlReg2.b.bBidirectional = false,

	// ControL Register 3 Parameters
	.ctrlReg3.b.bLRAOpenLoop = false,
	.ctrlReg3.b.bInputTriggerMode = DRV26x_inputTriggerMode_PWM,
	.ctrlReg3.b.bLRADriveMode = DRV26x_LRADriveMode_oncePerCycle,
	.ctrlReg3.b.bRTPDataFormat = DRV26x_RTPDataFormat_signed,
	.ctrlReg3.b.bDisableSupplyComp = false,
	.ctrlReg3.b.bERMOpenLoop = false,
	.ctrlReg3.b.bNoiseGateThresh = DRV26x_noiseGateThresh_off,

	// Control Register 4 Parameters
	.ctrlReg4.b.bOTPProgram = false,
	.ctrlReg4.b.bAutoCalTime = DRV26x_autoCalTime_500ms,

	// Control Register 5 Parameters
	.ctrlReg5.b.bCurrDissTimeMSB = FOURBIT_UPPERHALF(1),
	.ctrlReg5.b.bBlankingTimeMSB = FOURBIT_UPPERHALF(1),
	.ctrlReg5.b.bPlaybackInterval = DRV26x_playbackInterval_5ms,
	.ctrlReg5.b.bLRAAutoOpenLoop = false,
	.ctrlReg5.b.bAutoOpenLoopCnt = DRV26x_autoOpenLoopAttempts_4,
};

//! \brief Default Actuator Configuration: Samsung DMJBRN1030XX (Semco)
//!
const DRV26x_Actuator_t DRV26x_actuator_DMJBRN1030 =
{
    // Basic Parameters
	.ratedVoltage.r = 0x53,
	.overdriveClampVoltage.r = 0x83,

	// Auto-Calibration Parameters
	.autoCal_Compensation.r = 0x0F,
	.autoCal_BackEMF.r = 0x77,

	// Feedback Control Parameters
	.feedbackCtrl.b.bBackEMFGain = DRV26x_backEMFgain_3,
	.feedbackCtrl.b.bLoopGain = DRV26x_loopGain_medium,
	.feedbackCtrl.b.bFBBrakeFactor = DRV26x_feedbackBrakeFactor_3x,
	.feedbackCtrl.b.bActuatorType = DRV26x_actuator_LRA,

	// Control Register 1 Parameters
	.ctrlReg1.b.bDriveTime = 0x13,
	.ctrlReg1.b.bACCoupling = false,
	.ctrlReg1.b.bStartupBoost = true,

	// Control Register 2 Parameters
	.ctrlReg2.b.bCurrDissTimeLSB = FOURBIT_LOWERHALF(1),
	.ctrlReg2.b.bBlankingTimeLSB = FOURBIT_LOWERHALF(1),
	.ctrlReg2.b.bSamplingTime = DRV26x_LRASamplingTime_250us,
	.ctrlReg2.b.bBrakeStabilizer = false,
	.ctrlReg2.b.bBidirectional = false,

	// ControL Register 3 Parameters
	.ctrlReg3.b.bLRAOpenLoop = false,
	.ctrlReg3.b.bInputTriggerMode = DRV26x_inputTriggerMode_PWM,
	.ctrlReg3.b.bLRADriveMode = DRV26x_LRADriveMode_oncePerCycle,
	.ctrlReg3.b.bRTPDataFormat = DRV26x_RTPDataFormat_signed,
	.ctrlReg3.b.bDisableSupplyComp = false,
	.ctrlReg3.b.bERMOpenLoop = false,
	.ctrlReg3.b.bNoiseGateThresh = DRV26x_noiseGateThresh_off,

	// Control Register 4 Parameters
	.ctrlReg4.b.bOTPProgram = false,
	.ctrlReg4.b.bAutoCalTime = DRV26x_autoCalTime_500ms,

	// Control Register 5 Parameters
	.ctrlReg5.b.bCurrDissTimeMSB = FOURBIT_UPPERHALF(1),
	.ctrlReg5.b.bBlankingTimeMSB = FOURBIT_UPPERHALF(1),
	.ctrlReg5.b.bPlaybackInterval = DRV26x_playbackInterval_5ms,
	.ctrlReg5.b.bLRAAutoOpenLoop = false,
	.ctrlReg5.b.bAutoOpenLoopCnt = DRV26x_autoOpenLoopAttempts_4,
};
