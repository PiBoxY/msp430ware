<EMDC_COPYRIGHT>
#ifndef _EMSWLIB_USERCONFIG_H_
#define _EMSWLIB_USERCONFIG_H_

//#############################################################################
//
//! \file   EM_userConfig.h
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


//*****************************************************************************
// includes
//*****************************************************************************
//*****************************************************************************
// includes
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "emSwLib.h"
#include "IQmathLib.h"
#include "hal_adc.h"

//*****************************************************************************
//! \addtogroup emSwLib_userConfig 
//! @{
//*****************************************************************************

#ifdef __cplusplus

extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************

<EMDC_SMCLK_FREQ>

<EMDC_CHANNEL_LENGTH>
<EMDC_PHASE_LENGTH>
<EMDC_ADC_AGGREGATE>
#define EM_PULSE_ARRAY_LENGTH               (EM_PHASE_LENGTH+EM_AGGREGATE_ENABLE)

<EMDC_MAIN_NOM_FREQ_HZ>
<EMDC_SAMPLING_FREQ_HZ>
<EMDC_PHASE_IMP_PER_KWHR>
<EMDC_PHASE_PULSE_DUR>
<EMDC_TOTAL_PHASE_IMP_PER_KWHR>
<EMDC_TOTAL_PHASE_PULSE_DUR>
<EMDC_START_UP_CURRENT>

<EMDC_PHASE_IDXS>

//*****************************************************************************
// typedefs
//*****************************************************************************

//******************************************************************************
// DO NOT MODIFY THESE DEFINES
//******************************************************************************
#define EM_PHASE_CONSTANT               	(3600 * 1e9 * EM_SAMPLING_FREQ_HZ / EM_PHASE_IMP_PER_KWHR)
#define EM_TOTAL_PHASE_CONSTANT             (3600 * 1e9 * EM_SAMPLING_FREQ_HZ / EM_TOTAL_PHASE_IMP_PER_KWHR)
#define EM_MAX_FREQ_COUNT                   (EM_SAMPLING_FREQ_HZ*256L/40)
#define EM_MIN_FREQ_COUNT                   (EM_SAMPLING_FREQ_HZ*256L/70)

#define SYSTEM_NONV_CALIB_ADDR      (0x00001000)
#define SYSTEM_NONV_SEGMENT_SIZE    (1024)
#define SYSTEM_NONV_CHECKSUM_ADDR   (0x00001078)
#define SYSTEM_NONV_INFOA_TLV_ADDR  (0x000013C0)
#define SYSTEM_NONV_INFOA_TLV_SIZE  32
//*****************************************************************************
// globals
//*****************************************************************************

extern EM_SW_Lib_Config_Handle gEmSWConfig;

extern EM_SW_Lib_Result_Handle gEmSWResult;

extern EM_Phase_Configuration emPhaseConfiguration[EM_PHASE_LENGTH];

#if defined(__IAR_SYSTEMS_ICC__)
    #pragma location=SYSTEM_NONV_CALIB_ADDR
#endif
extern const EM_Phase_Calibration g_savedEmPhaseCalibration[EM_PHASE_LENGTH];

extern EM_Phase_Calibration g_emPhaseCalibration[EM_PHASE_LENGTH];

//*****************************************************************************
// the function prototypes
//*****************************************************************************

extern void EM_perSampleProc(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_BG_Results *bgData);

extern void EM_foregroundProc(uint8_t *resultStatus);
		
#ifdef __cplusplus
}
#endif // extern "C"
//@}  // ingroup

#endif // end of  _EMSWLIB_USERCONFIG_H_ definition
