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
//! \file   DRV26x.c
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

#include "DRV26x_I2CPort.h"
#include "DRV26x.h"

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void DRV26x_reset(void)
{
    DRV26x_setReg(DRV26x_reg_mode, 0);
}

void DRV26x_enterStandby(void)
{
    DRV26x_ModeReg_t modeReg;

    DRV26x_getReg(DRV26x_reg_mode, &(modeReg.r));
    modeReg.b.bStandby = true;
    DRV26x_setReg(DRV26x_reg_mode, modeReg.r);
}

void DRV26x_exitStandby(void)
{
    DRV26x_ModeReg_t modeReg;

    DRV26x_getReg(DRV26x_reg_mode, &(modeReg.r));
    modeReg.b.bStandby = false;
    DRV26x_setReg(DRV26x_reg_mode, modeReg.r);
}

DRV26x_DeviceID_t DRV26x_getDeviceID(void)
{
    DRV26x_StatusReg_t statusReg;

    DRV26x_getReg(DRV26x_reg_status, &(statusReg.r));
    return (DRV26x_DeviceID_t)statusReg.b.bDeviceID;
}

void DRV26x_setMode(DRV26x_Mode_t mode)
{
    DRV26x_ModeReg_t modeReg;

    DRV26x_getReg(DRV26x_reg_mode, &(modeReg.r));
    modeReg.b.bMode = (uint8_t)mode;
    DRV26x_setReg(DRV26x_reg_mode, modeReg.r);
}

DRV26x_Mode_t DRV26x_getMode(void)
{
    DRV26x_ModeReg_t modeReg;

    DRV26x_getReg(DRV26x_reg_mode, &(modeReg.r));
    return (DRV26x_Mode_t)(modeReg.b.bMode);
}

bool DRV26x_getDiagResult(void)
{
    DRV26x_StatusReg_t statusReg;

    DRV26x_getReg(DRV26x_reg_status, &(statusReg.r));
    return statusReg.b.bDiagResult;
}

bool DRV26x_getTempStatus(void)
{
    DRV26x_StatusReg_t statusReg;

    DRV26x_getReg(DRV26x_reg_status, &(statusReg.r));
    return statusReg.b.bOverTemp;
}

bool DRV26x_getOverCurrentStatus(void)
{
    DRV26x_StatusReg_t statusReg;

    DRV26x_getReg(DRV26x_reg_status, &(statusReg.r));
    return statusReg.b.bOverCurrent;
}

void DRV26x_loadActuatorConfig(const DRV26x_Actuator_t *actuator)
{
    DRV26X_WRITEBLOCK(
            DRV26x_reg_ratedVoltage,
            (uint8_t*)&(actuator->actuatorData[0]),
            10
        );
}

void DRV26x_go(void)
{
    DRV26x_GoReg_t goReg;

    goReg.b.bGo = true;
    DRV26x_setReg(DRV26x_reg_go, goReg.r);
}

void DRV26x_stop(void)
{
    DRV26x_GoReg_t goReg;

    goReg.b.bGo = false;
    DRV26x_setReg(DRV26x_reg_go, goReg.r);
}

void DRV26x_blockUntilGoIsCleared(void)
{
    DRV26x_GoReg_t goReg;

    do
    {
        DRV26x_getReg(DRV26x_reg_go, &(goReg.r));
    }
    while (goReg.b.bGo != false);
}

bool DRV26x_runAutoCalibration(void)
{
    DRV26x_Mode_t contextSavedMode;
    uint8_t diagnosticResult;

    DRV26x_blockUntilGoIsCleared();

    contextSavedMode = DRV26x_getMode();
    DRV26x_setMode(DRV26x_mode_autoCalibration);

    DRV26x_go();
    DRV26x_blockUntilGoIsCleared();
    diagnosticResult = DRV26x_getDiagResult();

    DRV26x_setMode(contextSavedMode);

    return diagnosticResult;
}

void DRV26x_storeAutoCalibrationResults(DRV26x_Actuator_t *actuator)
{
    uint8_t r;

    DRV26x_getReg(DRV26x_reg_autoCalibrationCompResult, &r);
    actuator->autoCal_Compensation.b.bAutocalCompResult = r;

    DRV26x_getReg(DRV26x_reg_autoCalibrationBackEMFResult, &r);
    actuator->autoCal_BackEMF.b.bAutocalBackEMFResult = r;
}

void DRV26x_selectEffectLibrary(DRV26x_Library_t library)
{
    DRV26x_LibSelectReg_t libSelectReg;

    DRV26x_getReg(DRV26x_reg_librarySelection, &(libSelectReg.r));
    libSelectReg.b.bLibrarySelection = (uint8_t)library;
    DRV26x_setReg(DRV26x_reg_librarySelection, libSelectReg.r);
}

void DRV26x_fireROMLibraryEffect(DRV26x_Effect_t effect, bool wait)
{
    DRV26x_WaveformSeqReg_t playlist[2];

    playlist[0].b.bParam = effect;
    playlist[0].b.bDelay = false;
    playlist[1].b.bParam = DRV26x_effect_stop;
    playlist[1].b.bDelay = false;

    if (wait == true)
    {
        DRV26x_blockUntilGoIsCleared();
    }
    DRV26X_WRITEBLOCK(DRV26x_reg_waveformSequence1, &(playlist[0].r), 2);
    DRV26x_go();
}

void DRV26x_setHighZOutputState(void)
{
    DRV26x_LibSelectReg_t libSelectReg;

    DRV26x_getReg(DRV26x_reg_librarySelection, &(libSelectReg.r));
    libSelectReg.b.bHighZ = true;
    DRV26x_setReg(DRV26x_reg_librarySelection, libSelectReg.r);
}

void DRV26x_clearHighZOutputState(void)
{
    DRV26x_LibSelectReg_t libSelectReg;

    DRV26x_getReg(DRV26x_reg_librarySelection, &(libSelectReg.r));
    libSelectReg.b.bHighZ = false;
    DRV26x_setReg(DRV26x_reg_librarySelection, libSelectReg.r);
}

void DRV26x_loadWaveformPlaylist(
        const DRV26x_WaveformPlaylist_t *playlist)
{
    DRV26x_blockUntilGoIsCleared();
    DRV26X_WRITEBLOCK(\
            DRV26x_reg_waveformSequence1,
            (uint8_t*)(&playlist->waveformPlaylistData[0]),
            8
        );
    DRV26X_WRITEBLOCK(
            DRV26x_reg_overdriveTimeOffset,
            (uint8_t*)(&playlist->waveformPlaylistData[8]),
            4
        );
}

void DRV26x_fireWaveformPlaylist(
        const DRV26x_WaveformPlaylist_t *playlist)
{
    DRV26x_loadWaveformPlaylist(playlist);
    DRV26x_go();
}
