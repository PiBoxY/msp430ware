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
#ifndef DRV26X_REGISTERFILE_H_
#define DRV26X_REGISTERFILE_H_
//#############################################################################
//
//! \file   DRV26x_RegisterFile.h
//!
//! \brief The DRV26x_RegisterFile header file contains an enumeration of the 
//!        DRV26xx register file, as well as bit field definitions for each 
//!        register where appropriate.  This can be thought of as the 
//!        definition of the DRV26xx I2C register map.
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

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief A helpful reminder that the bit field is read only.
//!
#define __R

//! \brief A helpful reminder that the bit field is write only.
//!
#define __W

//! \brief A helpful reminder that the bit field is read/write.
//!
#define __RW

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief An enumeration of the tDRV2605 I2C register file.
//!
typedef enum
{
	DRV26x_reg_status = 0x00,
	DRV26x_reg_mode,
	DRV26x_reg_realTimePlaybackInput,
	DRV26x_reg_librarySelection,
	DRV26x_reg_waveformSequence1,
	DRV26x_reg_waveformSequence2,
	DRV26x_reg_waveformSequence3,
	DRV26x_reg_waveformSequence4,
	DRV26x_reg_waveformSequence5,
	DRV26x_reg_waveformSequence6,
	DRV26x_reg_waveformSequence7,
	DRV26x_reg_waveformSequence8,
	DRV26x_reg_go,
	DRV26x_reg_overdriveTimeOffset,
	DRV26x_reg_sustainTimePositiveOffset,
	DRV26x_reg_sustainTimeNegativeOffset,
	DRV26x_reg_brakeTimeOffset,
	DRV26x_reg_audioToVibeControl,
	DRV26x_reg_audioToVibeMinInputLevel,
	DRV26x_reg_audioToVibeMaxInputLevel,
	DRV26x_reg_audioToVibeMinOutputDrive,
	DRV26x_reg_audioToVibeMaxOutputDrive,
	DRV26x_reg_ratedVoltage,
	DRV26x_reg_overdriveClampVoltage,
	DRV26x_reg_autoCalibrationCompResult,
	DRV26x_reg_autoCalibrationBackEMFResult,
	DRV26x_reg_feedbackControl,
	DRV26x_reg_control1,
	DRV26x_reg_control2,
	DRV26x_reg_control3,
	DRV26x_reg_control4,
	DRV26x_reg_control5,
	DRV26x_reg_LRAOpenLoopPeriodReg,
	DRV26x_reg_voltageMonitorReg,
	DRV26x_reg_LRAResonancePeriodReg
} DRV26x_Register_t;

//! \brief STATUS REGISTER (0x00) (DRV26x_reg_status)
//!
typedef union {
    __R  uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __R  uint8_t bOverCurrent         :	1;  /* Over Current Flag */
      __R  uint8_t bOverTemp            :	1;  /* Over Temperature Flag */
      __R  uint8_t bRESERVED0           :	1;  /* Reserved */
      __R  uint8_t bDiagResult          :   1;  /* Diagnostic Result */
      __R  uint8_t bRESERVED1           :	1;  /* Reserved */
      __R  uint8_t bDeviceID            :	3;  /* Device ID */
    } b;
} DRV26x_StatusReg_t;

typedef enum {
	DRV26x_deviceID_DRV2604,
	DRV26x_deviceID_DRV2605,
	DRV26x_deviceID_DRV2604L,
	DRV26x_deviceID_DRV2605L
} DRV26x_DeviceID_t;

//! \brief MODE REGISTER (0x01) (DRV26x_reg_mode)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bMode                :	3;  /* Mode */
      __R  uint8_t bRESERVED0           :	3;  /* Reserved */
      __RW uint8_t bStandby             :   1;  /* Standby */
      __RW uint8_t bDeviceReset         :	1;  /* Reset */
    } b;
} DRV26x_ModeReg_t;

//! \brief Mode bit options
//!
typedef enum {
	DRV26x_mode_internalTrigger = 0,
	DRV26x_mode_externalEdgeTrigger,
	DRV26x_mode_externalLevelTrigger,
	DRV26x_mode_PWMandAnalog,
	DRV26x_mode_audioToVibe,
	DRV26x_mode_realtimePlayback,
	DRV26x_mode_diagnostics,
	DRV26x_mode_autoCalibration
} DRV26x_Mode_t;

//! \brief REALTIME PLAYBACK INPUT REGISTER (0x02) (DRV26x_reg_realTimePlaybackInput)
//!
//! Insert RTP Data directly into the data field at this address
//! Open Loop (ERM Only), BiDir_Input = N/A, SignedUnsigned_RTP = 0
//! 0x7F, Full scale positive (forward rotation)
//! 0x00, No signal
//! 0x81, Full scale negative (reverse rotation or braking)
//!
//! Open Loop (ERM Only), BiDir_Input = N/A, SignedUnsigned_RTP = 1
//! 0xFF, Full scale positive (forward rotation)
//! 0x80, No signal
//! 0x00, Full scale negative (reverse rotation or braking)
//!
//! Closed Loop, BiDir_Input = 1, SignedUnsigned_RTP = 0
//! 0x7F, Full scale drive
//! Less than 0x00 interpreted as 0x81 (full-scale brake)
//!
//! Closed Loop, BiDir_Input = 1, SignedUnsigned_RTP = 1
//! 0xFF, Full scale drive
//! Less than 0x7F interpreted as 0x00 (full-scale brake)
//!
//! Closed Loop, BiDir_Input = 0, SignedUnsigned_RTP = N/A
//! 0xFF, Full scale drive
//! 0x80, Half scale drive
//! 0x00, No signal, braking happens when input is less than back-EMF
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bRTPInput            :	8;  /* Realtime Playback Input */
    } b;
} DRV26x_RealtimePlaybackInputReg_t;

//! \brief LIBRARY SELECTION REGISTER (0x03) (DRV26x_reg_librarySelection)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bLibrarySelection    :	3;  /* Library Selection */
      __R  uint8_t bRESERVED0           :	1;  /* Reserved */
      __RW uint8_t bHighZ               :   1;  /* High-Z */
      __R  uint8_t bRESERVED1           :	3;  /* Reserved */
    } b;
} DRV26x_LibSelectReg_t;

//! \brief Library field options.
//!
typedef enum
{
	DRV26x_lib_RAM = 0,
	DRV26x_lib_ROM_A,
	DRV26x_lib_ROM_B,
	DRV26x_lib_ROM_C,
	DRV26x_lib_ROM_D,
	DRV26x_lib_ROM_E,
	DRV26x_lib_ROM_LRA,
	DRV26x_lib_ROM_F
} DRV26x_Library_t;

//! \brief WAVEFORM SEQUENCE REGISTERS  (0x04 to 0x0B) (eWaveformSequenceXReg)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bParam              :	7;  /* Effect or Delay */
      __RW uint8_t bDelay              :	1;  /* Set as Delay */
    } b;
} DRV26x_WaveformSeqReg_t;

//! \brief GO REGISTER (0x0C) (DRV26x_reg_go)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bGo                 :	1;  /* Go Bit */
      __R  uint8_t bRESERVED0          :	7;  /* Reserved */
    } b;
} DRV26x_GoReg_t;

//! \brief OVERDRIVE TIME OFFSET REGISTER (0x0D) (DRV26x_reg_overdriveTimeOffset)
//!        OverDrive Time Offset (ms) = ODT[7:0] * 5
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bOvdrTimeOffset     :	8;  /* Overdrive Time Offset */
    } b;
} DRV26x_OvdrTimeOffsetReg_t;

//! \brief SUSTAIN TIME OFFSET POSITIVE REGISTER (0x0E) 
//!        (DRV26x_reg_sustainTimePositiveOffset)
//!        Sustain Time Positive Offset (ms) = SPT[7:0] * 5
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bSustTimePosOffset  :	8;  /* Sustain Time Pos Offset */
    } b;
} DRV26x_SustTimePosOffsetReg_t;

//! \brief SUSTAIN TIME OFFSET NEGATIVE REGISTER (0x0F) 
//!        (DRV26x_reg_sustainTimeNegativeOffset)
//!        Sustain Time Negative Offset (ms) = SNT[7:0] * 5
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bSustTimeNegOffset  :	8;  /* Sustain Time Neg Offset */
    } b;
} DRV26x_SustTimeNegOffsetReg_t;

//! \brief BRAKE TIME OFFSET REGISTER (0x10) (DRV26x_reg_brakeTimeOffset)
//!        Braking Time Offset (ms) = BRT[7:0] * 5
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bBrakeTimeOffset    :	8;  /* Brake Time Offset */
    } b;
} DRV26x_BrakeTimeOffsetReg_t;

//! \brief RATED VOLTAGE REGISTER (0x16) (DRV26x_reg_ratedVoltage)
//!        Rated Voltage (V) = RatedVoltage[7:0] / 255 * 5.3 V
//!        User should write this value before performing autocal
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bRatedVoltage       :	8;  /* Rated Voltage */
    } b;
} DRV26x_RatedVoltageReg_t;

//! \brief OVERDRIVE CLAMP VOLTAGE REGISTER (0x17) (DRV26x_reg_overdriveClampVoltage)
//!        Overdrive Clamp Voltage (V) = ODClamp[7:0] / 255 * 5.6 V
//!        User should write this value before performing autocal
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bOvdrClampVoltage   :	8;  /* Overdrive Clamp Voltage */
    } b;
} DRV26x_OvdrClampVoltageReg_t;

//! \brief AUTOCAL COMPENSATION RESULT REGISTER (0x18)
//!        (DRV26x_reg_autoCalibrationCompResult)
//!        Calibration Compensation Coefficient = 1 + ACalComp[7:0] / 255
//!        The AutoCal routine will populate this register
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bAutocalCompResult  :	8;  /* Auto-cal Compensation */
    } b;
} DRV26x_AutocalCompResultReg_t;

//! \brief AUTOCAL BACK EMF RESULT REGISTER (0x19)
//!        (DRV26x_reg_autoCalibrationBackEMFResult)
//!        Auto Calibration Back-EMF Coefficient = (Value/255)*2.44/BEMFGain
//!        The AutoCal routine will populate this register
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bAutocalBackEMFResult :	8;  /* Auto-cal Back EMF */
    } b;
} DRV26x_AutocalBackEMFResultReg_t;

//! \brief FEEDBACK CONTROL REGISTER (0x1A) (DRV26x_reg_feedbackControl)
//! 
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bBackEMFGain        :	2;  /* Back EMF Gain */
      __RW uint8_t bLoopGain           :	2;  /* Loop Gain */
      __RW uint8_t bFBBrakeFactor      :	3;  /* Feedback Brake Factor */
      __RW uint8_t bActuatorType       :	1;  /* ERM or LRA */
    } b;
} DRV26x_FeedbackCtrlReg_t;

//! \brief Back EMF options
//!
typedef enum
{
	DRV26x_backEMFgain_0 = 0,
	DRV26x_backEMFgain_1,
	DRV26x_backEMFgain_2,
	DRV26x_backEMFgain_3
} DRV26x_BackEMFGain_t;

//! \brief Loop Gain options
//!
typedef enum
{
	DRV26x_loopGain_slow = 0,
	DRV26x_loopGain_medium,
	DRV26x_loopGain_fast,
	DRV26x_loopGain_veryfast
} DRV26x_LoopGain_t;

//! \brief Feedback Brake Factor options
//!
typedef enum
{
	DRV26x_feedbackBrakeFactor_1x = 0,
	DRV26x_feedbackBrakeFactor_2x,
	DRV26x_feedbackBrakeFactor_3x,
	DRV26x_feedbackBrakeFactor_4x,
	DRV26x_feedbackBrakeFactor_6x,
	DRV26x_feedbackBrakeFactor_8x,
	DRV26x_feedbackBrakeFactor_16x,
	DRV26x_feedbackBrakeFactor_disabled
} DRV26x_FeedbackBrakeFactor_t;

//! \brief Actuator options
//!
typedef enum
{
	DRV26x_actuator_ERM = 0,
	DRV26x_actuator_LRA
} DRV26x_Actuator;

//! \brief CONTROL REGISTER 1 (0x1B) (DRV26x_reg_control1)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bDriveTime          :	5;  /* Drive Time */
      __RW uint8_t bACCoupling         :	1;  /* AC Coupling */
      __RW uint8_t bRESERVED0          :	1;  /* Reserved */
      __RW uint8_t bStartupBoost       :	1;  /* Startup Boost */
    } b;
} DRV26x_Ctrl1Reg_t;

//! \brief CONTROL REGISTER 2 (0x1C) (DRV26x_reg_control2)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bCurrDissTimeLSB    :	2;  /* I Dissapation Time */
      __RW uint8_t bBlankingTimeLSB    :	2;  /* Blanking Time */
      __RW uint8_t bSamplingTime         :	2;  /* Sample Time */
      __RW uint8_t bBrakeStabilizer    :	1;  /* Brake Stabilizer */
      __RW uint8_t bBidirectional      :	1;  /* Bidirectional Input */
    } b;
} DRV26x_Ctrl2Reg_t;

//! \brief Sampling Time options
//!
typedef enum
{
	DRV26x_LRASamplingTime_150us = 0,
	DRV26x_LRASamplingTime_200us,
	DRV26x_LRASamplingTime_250us,
	DRV26x_LRASamplingTime_300us
} DRV26x_LRASampleTime_t;

//! \brief CONTROL REGISTER 3 (0x1D) (DRV26x_reg_control3)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bLRAOpenLoop        :	1;  /* Open Loop LRA */
      __RW uint8_t bInputTriggerMode   :	1;  /* PWM or Analog Input Mode */
      __RW uint8_t bLRADriveMode       :	1;  /* LRA Drive Mode */
      __RW uint8_t bRTPDataFormat      :	1;  /* Realtime Playback Format */
      __RW uint8_t bDisableSupplyComp  :	1;  /* Supply Compensation */
      __RW uint8_t bERMOpenLoop        :	1;  /* Open Loop ERM */
      __RW uint8_t bNoiseGateThresh    :	2;  /* Noise Gate Thresh */
    } b;
} DRV26x_Ctrl3Reg_t;

//! \brief Input Trigger options
//!
typedef enum
{
	DRV26x_inputTriggerMode_PWM = 0,
	DRV26x_inputTriggerMode_analog
} DRV26x_InputTriggerMode_t;

//! \brief LRA Drive Mode options
//!
typedef enum
{
	DRV26x_LRADriveMode_oncePerCycle = 0,
	DRV26x_LRADriveMode_twicePerCycle
} DRV26x_LRADriveMode_t;

//! \brief RTP Data Format options
//!
typedef enum
{
	DRV26x_RTPDataFormat_signed = 0,
	DRV26x_RTPDataFormat_unsigned
} DRV26x_RTPDataFormat_t;

//! \brief Noise Gate Threshold Options
//!
typedef enum
{
	DRV26x_noiseGateThresh_off = 0,
	DRV26x_noiseGateThresh_2percent,
	DRV26x_noiseGateThresh_4percent,
	DRV26x_noiseGateThresh_8percent
} DRV26x_NoiseGateThresh_t;

//! \brief CONTROL REGISTER 4 (0x1E) (DRV26x_reg_control4)
//!        DRV26X__OTP_PROGRAM:
//!          Warning:  Only light this bit if you are happy with the settings
//!          VDD must be 4.2V +/- 5%
//!          This process can only be executed once
//!          Non-Volatile Memory will be burned for Registers 0x16 through 0x1A
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bOTPProgram         :	1;  /* Launch OTP Programming */
      __RW uint8_t bRESERVED0          :	1;  /* Reserved */
      __R  uint8_t bOTPStatus          :	1;  /* OTP Programming Status */
      __RW uint8_t bRESERVED1          :	1;  /* Reserved */
      __RW uint8_t bAutoCalTime        :	2;  /* Autocalibration Time */
      __RW uint8_t bZeroCrossTime      :	2;  /* Zero Cross Detect Time */
    } b;
} DRV26x_Ctrl4Reg_t;

//! \brief Auto Cal Time options
//!
typedef enum
{
	DRV26x_autoCalTime_150ms = 0,
	DRV26x_autoCalTime_250ms,
	DRV26x_autoCalTime_500ms,
	DRV26x_autoCalTime_1000ms
} DRV26x_AutoCalTime_t;

//! \brief Zero Crossing Time options
//!
typedef enum
{
	DRV26x_zeroCrossTime_100us = 0,
	DRV26x_zeroCrossTime_200us,
	DRV26x_zeroCrossTime_300us,
	DRV26x_zeroCrossTime_390us
} DRV26x_ZeroCrossTime_t;

//! \brief CONTROL REGISTER 5 (0x1F) (DRV26x_reg_control5)
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bCurrDissTimeMSB    :	2;  /* I Dissipation Time MSB*/
      __RW uint8_t bBlankingTimeMSB    :	2;  /* Blanking Time MSB */
      __RW uint8_t bPlaybackInterval   :	1;  /* Memory Playback Interval */
      __RW uint8_t bLRAAutoOpenLoop    :	1;  /* LRA Auto Open Loop */
      __RW uint8_t bAutoOpenLoopCnt    :	2;  /* Auto Open-Loop Attempts */
    } b;
} DRV26x_Ctrl5Reg_t;

//! \brief Playback Interval options
//!
typedef enum
{
	DRV26x_playbackInterval_5ms = 0,
	DRV26x_playbackInterval_1ms,
} DRV26x_PlaybackInterval_t;

//! \brief Auto Open Loop Attempt options
//!
typedef enum
{
	DRV26x_autoOpenLoopAttempts_3 = 0,
	DRV26x_autoOpenLoopAttempts_4,
	DRV26x_autoOpenLoopAttempts_5,
	DRV26x_autoOpenLoopAttempts_6
} DRV26x_AutoOpenLoopCnt_t;

//! \brief LRA OPEN LOOP PERIOD REGISTER (0x20) (DRV26x_reg_LRAOpenLoopPeriodReg)
//!        Period (us) = OL_LRA_Period[7:0] * 98.46
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bLRAOpenLoopPeriod  :	8;  /* LRA Open Loop Period*/
    } b;
} DRV26x_LRAOpenLoopReg_t;

//! \brief LRA Open Loop Period options
//!
typedef enum
{
	DRV26x_autoOpenLoopPeriod_240Hz = 42,
	DRV26x_autoOpenLoopPeriod_230Hz = 44,
	DRV26x_autoOpenLoopPeriod_220Hz = 46,
	DRV26x_autoOpenLoopPeriod_210Hz = 48,
	DRV26x_autoOpenLoopPeriod_205Hz = 50,
	DRV26x_autoOpenLoopPeriod_175Hz = 58
} DRV26x_LRAOpenLoopPeriod_t;

//! \brief VBAT VOLTAGE MONITOR REGISTER (0x21) (DRV26x_reg_voltageMonitorReg)
//!        VDD (V) = VBAT[7:0] * 5.6V / 255	--Read Only
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bVBatVoltage        :	8;  /* VBat Voltage */
    } b;
} DRV26x_VBatMonitorReg_t;

//! \brief LRA RESONANCE PERIOD REGISTER (0x22) (DRV26x_reg_LRAOpenLoopPeriodReg)
//!        LRA Period (us) = LRA_Period[7:0] * 196.2 us
//!
typedef union {
    __RW uint8_t r;                             /* Complete Register */
    struct {                                    /* Register Bits */
      __RW uint8_t bLRAResonancePeriod :	8;  /* LRA Resonance Period */
    } b;
} DRV26x_LRAResonancePeriodReg_t;

#endif /* DRV26X_REGISTERFILE_H_ */
