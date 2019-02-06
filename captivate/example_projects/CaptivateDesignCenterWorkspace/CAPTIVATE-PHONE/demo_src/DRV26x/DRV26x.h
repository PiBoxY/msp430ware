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
#ifndef DRV26X_H_
#define DRV26X_H_
//#############################################################################
//
//! \file   DRV26x.h
//!
//! \brief The DRV26x.h/DRV26x.c source code file set makes up the core of the
//!        firmware driver for the DRV26xx haptic driver ICs.  This layer is 
//!        designed to be platform (host processor) independent.
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

#include <stdint.h>
#include <stdbool.h>
#include "DRV26x_RegisterFile.h"
#include "DRV26x_Actuators.h"
#include "DRV26x_ROMEffects.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief A macro to convert a waveform delay in ms into the proper register
//!        format.
#define DRV26X_WAVEFORMSEQ_DELAY_MS(ms) (0x80 | (ms/10))

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief Defines a container for a waveform playlist.  
//!        Up to 8 waveform objects (tDR26x_WaveformObj) may be queued
//!        in this structure to be played back sequentially as allowed by the 
//!        DRV26x haptic driver architecture.  In addition, the offset controls 
//!        are made available in this structure.
//!
typedef union
{
    uint8_t waveformPlaylistData[12];
    struct
    {
        DRV26x_WaveformSeqReg_t waveformList[8];
        DRV26x_OvdrTimeOffsetReg_t ovdrTimeOffset;
        DRV26x_SustTimePosOffsetReg_t sustTimePosOffset;
        DRV26x_SustTimeNegOffsetReg_t sustTimeNegOffset;
        DRV26x_BrakeTimeOffsetReg_t brakeTimeOffset;
    };
} DRV26x_WaveformPlaylist_t;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Perform a complete reset of the DRV26x device.
extern void DRV26x_reset(void);

//! \brief Put the DRV26x device into software standby.
extern void DRV26x_enterStandby(void);

//! \brief Take the DRV26x device out of software standby.
extern void DRV26x_exitStandby(void);

//! \brief Query the device ID.
//! \return the device ID. A list of valid IDs is given below:
//!         eDRV2604, eDRV2605, eDRV2604L, eDRV2605L  
extern DRV26x_DeviceID_t DRV26x_getDeviceID(void);

//! \brief Put the DRV26x device into a specific operational mode.
//! \param r specifies the register to read.  The available registers
//!        are defined in DRV26x_RegisterFile.h
//! \param mode defines the mode to set.  Valid parameters include the
//!        following:
//!        DRV26x_mode_internalTrigger, DRV26x_mode_externalEdgeTrigger,
//!        DRV26x_mode_externalLevelTrigger, DRV26x_mode_PWMandAnalog,
//!        DRV26x_mode_audioToVibe, DRV26x_mode_realtimePlayback,
//!        DRV26x_mode_diagnostics, DRV26x_mode_autoCalibration
extern void DRV26x_setMode(DRV26x_Mode_t mode);

//! \brief Query the current operational mode of the DRV26x.
//! \return the current mode.  Options include the following:
//!        DRV26x_mode_internalTrigger, DRV26x_mode_externalEdgeTrigger,
//!        DRV26x_mode_externalLevelTrigger, DRV26x_mode_PWMandAnalog,
//!        DRV26x_mode_audioToVibe, DRV26x_mode_realtimePlayback,
//!        DRV26x_mode_diagnostics, DRV26x_mode_autoCalibration
extern DRV26x_Mode_t DRV26x_getMode(void);

//! \brief Query whether a diagnostic or auto-calibration was successful.
//! \return true if a failure was detected, else false
extern bool DRV26x_getDiagResult(void);

//! \Brief Query whether the device is over-temp.
//! \return true if the device is exceeding the temperature threshold, else
//!         false
extern bool DRV26x_getTempStatus(void);

//! \brief Query whether the device load is below the impedance threshold,
//!        causing an over-current condition.
//! \return true if an overcurrent event was detected, else false
extern bool DRV26x_getOverCurrentStatus(void);

//! \brief Fire playback on the DRV26x device.
extern void DRV26x_go(void);

//! \brief Cancel playback on the DRV26x device.
extern void DRV26x_stop(void);

//! \brief Wait until the GO status has cleared, indicating the activity
//!        previously fired has completed and it is safe to again assert 
//!        the GO bit via DRV26x_go().
extern void DRV26x_blockUntilGoIsCleared(void);

//! \brief Load in an actuator configuration before firing any effects.
//! \param actuator is the actuator structure to load to the DRV26x device.
extern void DRV26x_loadActuatorConfig(const DRV26x_Actuator_t *actuator);

//! \brief Run an auto-calibration of the actuator attached to the DRV26x.  
//!        This will modify the DRV26x_reg_autoCalibrationCompResult and 
//!        DRV26x_reg_autoCalibrationBackEMFResult registers on the device.  
//!        The  calibrated values may be loaded back into the application 
//!        by calling DRV26x_storDRV26x_mode_autoCalibrationResults, if
//!        the actuator structure is stored in read/write capable memory.
extern bool DRV26x_runAutoCalibration(void);

//! \brief Read back the DRV26x_reg_autoCalibrationCompResult and 
//!        DRV26x_reg_autoCalibrationBackEMFResult registers on the device 
//!        into an  actuator data structure, if the actuator structure 
//!        is stored in read/write capable memory.
//! \param actuator is the actuator structure to store the results into.
void DRV26x_storeAutoCalibrationResults(
    DRV26x_Actuator_t *actuator);

//! \brief Select the haptic effect library to use.
//! \param library specifies the library to load.  Valid parameters include
//!        the following:
//!        DRV26x_lib_RAM = 0, DRV26x_lib_ROM_A, DRV26x_lib_ROM_B,
//!        DRV26x_lib_ROM_C, DRV26x_lib_ROM_D, DRV26x_lib_ROM_E,
//!        DRV26x_lib_ROM_LRA, DRV26x_lib_ROM_F
extern void DRV26x_selectEffectLibrary(DRV26x_Library_t library);

//! \brief Trigger the playback of a single effect from the ROM library.
//! \param effect specifies the effect to play.  Effects may be found in the
//!        DRV26x_ROMEffects.h file.
//! \param pass true to wait for all other effects to complete, else false
extern void DRV26x_fireROMLibraryEffect(DRV26x_Effect_t effect, bool wait);

//! \brief Set the output driver to a high-z state.
extern void DRV26x_setHighZOutputState(void);

//! Clear the output driver from a high-z state.
extern void DRV26x_clearHighZOutputState(void);

//! \brief Setup a playlist in the DRV26x without firing playback.
//! \param playlist is a pointer to the playlist to load.
extern void DRV26x_loadWaveformPlaylist(
        const DRV26x_WaveformPlaylist_t *playlist);

//! \brief Load a playlist and fires playback immediately.
//! \param playlist is a pointer to the playlist to load.
extern void DRV26x_fireWaveformPlaylist(
        const DRV26x_WaveformPlaylist_t *playlist);

#endif /* DRV26X_H_ */
