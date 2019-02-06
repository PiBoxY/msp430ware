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
#ifndef DRV26X_ACTUATORS_H_
#define DRV26X_ACTUATORS_H_
//#############################################################################
//
//! \file   DRV26x_Actuators.h
//!
//! \brief The DRV26x_Actuators layer contains pre-populated DRV26x_Actuator_t
//!        containers for known, existing, haptic actuators.
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
#include "DRV26x_RegisterFile.h"

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief DRV26x_Actuator_t defines a container for all of the properties
//!        associated with a particular haptic actuator (ERM or LRA).  The 
//!        structure is composed of tDRV26x_Register instances, to allow for
//!        efficient reading and writing to the driver IC via I2C.  These 
//!        structures are used to configure the DRV26x driver for a specific 
//!        actuator.
//!        Default,out-of-box driver configurations are provided in the 
//!        DRV26x_Actuators.c/.h files.  If auto-calibration data does not 
//!        need to be read back, instances of DRV26x_Actuator_t may be
//!        read-only (const), saving RAM.  This is the default configuration 
//!        of the DRV26x_Acuators layer.
//!
typedef union
{
	uint8_t actuatorData[10]; /* Register Array (Sequential) */
	struct /* Individual Register Access */
	{
		DRV26x_RatedVoltageReg_t ratedVoltage;
		DRV26x_OvdrClampVoltageReg_t overdriveClampVoltage;
		DRV26x_AutocalCompResultReg_t autoCal_Compensation;
		DRV26x_AutocalBackEMFResultReg_t autoCal_BackEMF;
		DRV26x_FeedbackCtrlReg_t feedbackCtrl;
		DRV26x_Ctrl1Reg_t ctrlReg1;
		DRV26x_Ctrl2Reg_t ctrlReg2;
		DRV26x_Ctrl3Reg_t ctrlReg3;
		DRV26x_Ctrl4Reg_t ctrlReg4;
		DRV26x_Ctrl5Reg_t ctrlReg5;
	};
} DRV26x_Actuator_t;

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Default Actuator Configuration: AAC 1036 [LRA]
//!
extern const DRV26x_Actuator_t DRV26x_actuator_AAC1036;

//! \brief Default Actuator Configuration: Samsung DMJBRN1030XX (Semco) [LRA]
//!
extern const DRV26x_Actuator_t DRV26x_actuator_DMJBRN1030;

#endif /* DRV26X_ACTUATORS_H_ */
