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
//! The Captivate touch library UserConfig component defines the architecture
//! of the capacitive touch panel that is being implemented.
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

#ifndef CAPT_USERCONFIG_H_
#define CAPT_USERCONFIG_H_

//
// CapTIvate User Configuration Template Version
//
#ifndef __CAPTIVATE_CONFIG_VERSION__
#define __CAPTIVATE_CONFIG_VERSION__ 17000
#endif

//
// Include the CapTIvate Software Library
//
#include "captivate.h"

//
// Compile-Time System Configuration Definitions
//
#define CAPT_SENSOR_COUNT                     (1)
#define CAPT_INTERFACE  (__CAPT_UART_INTERFACE__)
#define CAPT_CONDUCTED_NOISE_IMMUNITY_ENABLE  (true)
#define CAPT_WAKEONPROX_ENABLE  (false)
#define CAPT_WAKEONPROX_SENSOR  (none)
#define CAPT_CAL_ALG (__CAPT_STD_CALIBRATION__)

//
// Captivate Frequency Count Definitions
// If conducted noise is enabled, multiple
// frequency scanning is enabled.  These definitions
// ensure that RAM space is set aside to store
// frequency specific information.
//
#if CAPT_CONDUCTED_NOISE_IMMUNITY_ENABLE
#define CAPT_SELF_FREQ_CNT        (4)
#define CAPT_MUTUAL_FREQ_CNT      (4)
#else
#define CAPT_SELF_FREQ_CNT        (1)
#define CAPT_MUTUAL_FREQ_CNT      (1)
#endif

//*****************************************************************************
//
//! \def CAPT_MS_TO_CYCLES is a macro to convert a period value in 
//! milliseconds to an approximate cycle count based on a 
//! 32.768 kHz clock frequency.
//
//*****************************************************************************
#define CAPT_MS_TO_CYCLES(ms)                                         (ms * 32)

//*****************************************************************************
//
//! \def CAPT_MS_TO_CYCLES_VLO is a macro to convert a period value in
//! milliseconds to an approximate cycle count based on a
//! 10 kHz clock frequency.  The VLO oscillator is approximately 10 kHz.
//
//*****************************************************************************
#define CAPT_MS_TO_CYCLES_VLO(ms)                                     (ms * 10)

//
// Captivate Sensor Prototypes
// These prototypes enable access to sensors
// from the application.
//
extern tSensor keypadSensor;

//*****************************************************************************
//
//! \var g_EMCConfig
//! This structure stores the EMC configuration for this application.
//
//*****************************************************************************
extern const tEMCConfig g_EMCConfig;

 //*****************************************************************************
 //
 //! \var g_pCaptivateSensorArray
 //! This array allows for indexed access to any
 //! sensor in the configuration.
 //
 //*****************************************************************************
extern tSensor* g_pCaptivateSensorArray[CAPT_SENSOR_COUNT];

//*****************************************************************************
//
//! \var g_uiApp
//! This structure stores the global settings for this application.
//
//*****************************************************************************
extern tCaptivateApplication g_uiApp;

//*****************************************************************************
//
//! Software Library / User Configuration Version Check
//
//*****************************************************************************
#ifndef __CAPTIVATE_LIB_VERSION__
#warning "CapTIvate library version not found by user configuration."
#elif __CAPTIVATE_LIB_VERSION__ != __CAPTIVATE_CONFIG_VERSION__
#warning "CapTIvate software library and user configuration version mismatch."
#endif

#endif /* CAPT_USERCONFIG_H_ */
