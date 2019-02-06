<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   EM_userConfig.c
//!
//! \brief  This file contains EM SW Library configuration
//!
//
//!  Group:          MSP Embedded Software Development
<EMDC_DEVICE_NAME>
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: MSP430_ENERGY_MEASUREMENT 
// Release Date: 10Jul2018
//#############################################################################
#include <msp430.h>
#include "hal.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "emSwLib.h"
#include "EM_userConfig.h"
#include "IQmathLib.h"
#include "gpio.h"
#include "flashctl.h"
#include "emReactivePowerIIRCoeff.h"

HAL_ADC_SD24_B_Channel adcChannelArray[EM_CHANNEL_LENGTH] =
{
<EMDC_HAL_ADC_CHANNEL>
};

SD24_B_initParam sd24Param =
{
    .clockSourceSelect = SD24_B_CLOCKSOURCE_SMCLK,
    .referenceSelect  = SD24_B_REF_INTERNAL
};

HAL_ADC_SD24_B_Configuration halAdcConfig =
{
    .SMCLKFrequency = EM_SMCLK_FREQ_IN_HZ,
    .sampleFreq = EM_SAMPLING_FREQ_HZ,
    .sd24Param = &sd24Param,
    .adcChannelPtr = adcChannelArray,
    .adcChannelLength = EM_CHANNEL_LENGTH,
<EMDC_OVERSAMPLE_RATE>
};

uint16_t HAL_ADC_maxFreqCount = EM_MAX_FREQ_COUNT;
uint16_t HAL_ADC_minFreqCount = EM_MIN_FREQ_COUNT;

uint16_t *EMSWLIB_EM_HAL_ADC_maxFreqCount =  &HAL_ADC_maxFreqCount;
uint16_t *EMSWLIB_EM_HAL_ADC_minFreqCount =  &HAL_ADC_minFreqCount;

EM_Pulse_Settings emPulseSetting[EM_PULSE_ARRAY_LENGTH] =
{
<EMDC_PULSE_CONFIG>
};

EM_System_Configuration emSystemConfig =
{
    .pulseArray = emPulseSetting,
    .pulseArrayLength = EM_PULSE_ARRAY_LENGTH,
    .indPhaseMeterConstant = EM_PHASE_CONSTANT,
    .indPhasePulseDuration = EM_PHASE_PULSE_DUR,
    .totalPhaseMeterConstant = EM_TOTAL_PHASE_CONSTANT,
    .totalPhasePulseDuration = EM_TOTAL_PHASE_PULSE_DUR,
<EMDC_PULSE_GENERATION_FLAG>
};

#if defined (__TI_COMPILER_VERSION__)
    #pragma LOCATION(g_savedEmPhaseCalibration, SYSTEM_NONV_CALIB_ADDR);
#endif
const EM_Phase_Calibration g_savedEmPhaseCalibration[EM_PHASE_LENGTH];

#pragma location=SYSTEM_NONV_CHECKSUM_ADDR
const uint16_t g_calibrationChecksum;

EM_Phase_Calibration g_emPhaseCalibration[EM_PHASE_LENGTH] =
{
<EMDC_CALIBRATION_FACTORS>
};

EM_Phase_Configuration emPhaseConfiguration[EM_PHASE_LENGTH] =
{
<EMDC_PHASE_CONFIG>
};

EM_Algorithm_Configuration emAlgorithmConfig =
{
    .residualPowerCutoff = EM_PHASE_START_UP_POWER_mW,
    .totalResidualPowerCutoff = EM_TOTAL_START_UP_POWER_mW,
    .mainNomFreq = EM_MAIN_NOMINAL_FREQ_HZ,
	.freqSampling = EM_SAMPLING_FREQ_HZ,
	.reactPowIIRLength = IIR_COEFF_TABLE_LEN,
	.reactPowIIRMinFreq = IIR_COEFF_MIN_FREQ,
	.reactPowIIRShift = IIR_COEFF_SHIFT,
	.reactPowIIRTable = (_iq30 *) G_CONSTANT
};

EM_Meter_Configuration emMeterConfig =
{
    .phaseConfigPtr = emPhaseConfiguration,
    .phaseLength = EM_PHASE_LENGTH,
<EMDC_METER_NAME>
<EMDC_LIBRARY_VERSION>
};

EM_SW_Lib_Config_Handle gEmSWConfig =
{
	.systemConfig = &emSystemConfig,
	.algorithmConfig = &emAlgorithmConfig,
	.meterConfig = &emMeterConfig,
};

EM_Total_Results emTotalResult = {0};

EM_Phase_Results emPhaseResult[EM_PHASE_LENGTH] =
{
<EMDC_PHASE_RESULT_ID>
};

EM_Metrology emMetrologyPing[EM_PHASE_LENGTH] = {0};

EM_Metrology emMetrologyPong[EM_PHASE_LENGTH] = {0};

EM_Phase_BG_Results emPhaseBackground[EM_PHASE_LENGTH] = {0};

<EMDC_PING_PONG_INIT>

EM_SW_Lib_Result_Handle gEmSWResult =
{
    .emTotalResult = &emTotalResult,
    .phaseResults = &emPhaseResult[0],
	.phaseBGResults = &emPhaseBackground[0],
    .phaseMetrologyPing = emMetrologyPingArray,
	.phaseMetrologyPong = emMetrologyPongArray,
    .phaseResultsSupportedLen = EM_PHASE_LENGTH,
};

void EM_perSampleProc(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_BG_Results *bgData)
{
<EMDC_PER_SAMPLE_PROCESSING>
}

void EM_processForegroundData(uint8_t phaseIdx)
{
<EMDC_APP_PROCESS_FOREGROUND>
}

void EM_foregroundProc(uint8_t *resultStatus)
{
	*resultStatus = HAL_ADC_PHASE_NO_DATA_READY;
<EMDC_TOTAL_CALC_DEF>
	
	// Check if the background data is ready to process
    while(phaseDataReady !=  HAL_ADC_PHASE_NO_DATA_READY)
    {
<EMDC_APP_PHASE_READY>
<EMDC_APP_TOTAL_POWER_CALC>
    }
}

void EM_genPulseIndPhase(EM_System_Configuration *config,
                         EM_Pulse_Settings *pulse,
                         EM_Phase_Results *result)
{
    // Check if the selected phase is enabled
    if((result->phaseID & config->pulseGeneration) == 0x00)
    {
        return;
    }

    // Active Power Pulse Generation
    if(result->activePower != 0)
    {
        if(result->activePower > 0)
        {
            // Add Active Energy to existing sum
            pulse->activeEnergyCounter += result->activePower;
        }
        else
        {
            // Add Active Energy to existing sum
            pulse->activeEnergyCounter += llabs(result->activePower);
        }

        // Check active energy threshold
        if (pulse->activeEnergyCounter > config->indPhaseMeterConstant)
        {
            pulse->activeEnergyCounter -= config->indPhaseMeterConstant;

            // Set Active GPIO Pin
            GPIO_setOutputLowOnPin(pulse->activePulseGpioPort,pulse->activePulseGpioPin);

            // Set Remaining Cycles
            pulse->activeEnergyPulseRemaining = config->indPhasePulseDuration;
        }
    }

    // Check if there are remaining pulses for Active Energy
    if(pulse->activeEnergyPulseRemaining > 0 &&
            --pulse->activeEnergyPulseRemaining == 0)
    {
        // Clear Active GPIO Pin
        GPIO_setOutputHighOnPin(pulse->activePulseGpioPort,pulse->activePulseGpioPin);
    }

    // Reactive Power Pulse Generation
    if(result->reactivePower != 0)
    {
        if(result->reactivePower > 0)
        {
            // Add Reactive Energy to existing sum
            pulse->reactiveEnergyCounter += result->reactivePower;
        }
        else
        {
            // Add Reactive Energy to existing sum
            pulse->reactiveEnergyCounter += llabs(result->reactivePower);
        }

        // Check reactive energy threshold
        if (pulse->reactiveEnergyCounter > config->indPhaseMeterConstant)
        {
            pulse->reactiveEnergyCounter -= config->indPhaseMeterConstant;

            // Set Reactive GPIO Pin
            GPIO_setOutputLowOnPin(pulse->reactivePulseGpioPort,pulse->reactivePulseGpioPin);

            // Set Remaining Cycles
            pulse->reactiveEnergyPulseRemaining = config->indPhasePulseDuration;
        }
    }

    // Check if there are remaining pulses for Reactive Energy
    if(pulse->reactiveEnergyPulseRemaining > 0 &&
            --pulse->reactiveEnergyPulseRemaining == 0)
    {
        // Clear Reactive GPIO Pin
        GPIO_setOutputHighOnPin(pulse->reactivePulseGpioPort,pulse->reactivePulseGpioPin);
    }
}

void EM_genPulseTotalPhases(EM_System_Configuration *config,
                            EM_Pulse_Settings *pulse,
                            EM_Total_Results *result)
{
    // Check if generating pulses for the aggregate total is enabled
    if((EM_PULSE_GEN_PHASE_AGGREGATE & config->pulseGeneration) == 0x00)
    {
        return;
    }

    // Active Power Pulse Generation
    if(result->activePower != 0)
    {
        if(result->activePower > 0)
        {
            // Add Active Energy to existing sum
            pulse->activeEnergyCounter += result->activePower;
        }
        else
        {
            // Add Active Energy to existing sum
            pulse->activeEnergyCounter += llabs(result->activePower);
        }

        // Check active energy threshold
        if (pulse->activeEnergyCounter > config->indPhaseMeterConstant)
        {
            pulse->activeEnergyCounter -= config->indPhaseMeterConstant;

            // Set Active GPIO Pin
            GPIO_setOutputLowOnPin(pulse->activePulseGpioPort,pulse->activePulseGpioPin);

            // Set Remaining Cycles
            pulse->activeEnergyPulseRemaining = config->indPhasePulseDuration;
        }
    }

    // Check if there are remaining pulses for Active Energy
    if(pulse->activeEnergyPulseRemaining > 0 &&
            --pulse->activeEnergyPulseRemaining == 0)
    {
        // Clear Active GPIO Pin
        GPIO_setOutputHighOnPin(pulse->activePulseGpioPort,pulse->activePulseGpioPin);
    }

    // Reactive Power Pulse Generation
    if(result->reactivePower != 0)
    {
        if(result->reactivePower > 0)
        {
            // Add Reactive Energy to existing sum
            pulse->reactiveEnergyCounter += result->reactivePower;
        }
        else
        {
            // Add Reactive Energy to existing sum
            pulse->reactiveEnergyCounter += llabs(result->reactivePower);
        }

        // Check reactive energy threshold
        if (pulse->reactiveEnergyCounter > config->indPhaseMeterConstant)
        {
            pulse->reactiveEnergyCounter -= config->indPhaseMeterConstant;

            // Set Reactive GPIO Pin
            GPIO_setOutputLowOnPin(pulse->reactivePulseGpioPort,pulse->reactivePulseGpioPin);

            // Set Remaining Cycles
            pulse->reactiveEnergyPulseRemaining = config->indPhasePulseDuration;
        }
    }

    // Check if there are remaining pulses for Reactive Energy
    if(pulse->reactiveEnergyPulseRemaining > 0 &&
            --pulse->reactiveEnergyPulseRemaining == 0)
    {
        // Clear Reactive GPIO Pin
        GPIO_setOutputHighOnPin(pulse->reactivePulseGpioPort,pulse->reactivePulseGpioPin);
    }
}

void EM_initalizeCalibrationValues(uint8_t *pWorkingCalibration,
                                   uint8_t *pFlashCalibration)
{
    bool bStatus;
    uint16_t ui16tempChecksum = 0;
    uint8_t ui8Idx;

    // Check if Flash is cleared
    bStatus = FlashCtl_performEraseCheck(
                        pFlashCalibration,
                        EM_PHASE_LENGTH*sizeof(EM_Phase_Calibration)
                   );


    if(bStatus == STATUS_FAIL)
    {
        // If bStatus == FAIL then pFlashCalibration, then we need to verify the checksum

        // Calculate the checksum
        for(ui8Idx=0;ui8Idx<EM_PHASE_LENGTH*sizeof(EM_Phase_Calibration);ui8Idx++)
        {
            ui16tempChecksum += (uint16_t) pFlashCalibration[ui8Idx];
        }

        // Check if the checksum is correct
        if(ui16tempChecksum == g_calibrationChecksum)
        {
            // Copy flash values to the calibration values used in RAM
            memcpy(pWorkingCalibration,
                   pFlashCalibration,
                   EM_PHASE_LENGTH*sizeof(EM_Phase_Calibration));
        }
    }
}

void EM_updateFlashCalibrationValues(uint8_t *pWorkingCalibration,
                                     uint8_t *pFlashCalibration)
{
    bool bStatus;
    uint16_t ui16tempChecksum = 0;
    uint8_t ui8Idx;
    uint16_t ui16GIE;

    // Mask global interrupts for safety
    ui16GIE = __get_SR_register() & GIE;
    __bic_SR_register(GIE);

    // Unlock Info A
    FlashCtl_unlockInfoA();

    // Erase segment
    do {
        FlashCtl_eraseSegment((uint8_t*)pFlashCalibration);
        bStatus = FlashCtl_performEraseCheck(
                    pFlashCalibration,
                    SYSTEM_NONV_SEGMENT_SIZE
                );
    } while (bStatus == STATUS_FAIL);

    // Write calibration values
    FlashCtl_write8(
                pWorkingCalibration,
                pFlashCalibration,
                EM_PHASE_LENGTH*sizeof(EM_Phase_Calibration)
            );

    // Calculate the checksum
    for(ui8Idx=0;ui8Idx<EM_PHASE_LENGTH*sizeof(EM_Phase_Calibration);ui8Idx++)
    {
        ui16tempChecksum += (uint16_t) pWorkingCalibration[ui8Idx];
    }

    // Store the checksum in flash
    FlashCtl_write16(&ui16tempChecksum, (uint16_t *)&g_calibrationChecksum, 1);

    // Lock Info A
    FlashCtl_lockInfoA();

    //
    // Restore global interrupt enable if it was previously enabled
    //
    __bis_SR_register(ui16GIE);
}
