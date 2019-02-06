<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   hal_adc.h
//!
//! \brief  Hardware Abstraction Layer for Sigma-Delta 24 (SD24)
//!
//!  Group:          MSP Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include "sd24.h"
#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
// defines
//*****************************************************************************

//! \brief Invalid Index for Converter Number
#define HAL_ADC_CONV_INVALID_INDEX  0xFF
//! \brief Maximum modulation frequency is 2MHz
#define HAL_ADC_MAX_MOD_FREQ        2097152L
//! \brief Size of voltage array (it must be a multiple of 2)
#define HAL_ADC_V_ARRAY_LENGTH      4
//! \brief Size of voltage array (it must be a multiple of 2)
#define HAL_ADC_I_ARRAY_LENGTH      4

//******************************************************************************
// DO NOT MODIFY THESE DEFINES
//******************************************************************************
//! Voltage Mask used for circular buffer
#define HAL_ADC_V_MASK              (HAL_ADC_V_ARRAY_LENGTH-1)
//! Current Mask used for circular buffer
#define HAL_ADC_I_MASK              (HAL_ADC_I_ARRAY_LENGTH-1)

//*****************************************************************************
//
// The following are values are used as masks for the chReadStatus global
// variable to keep track of what channels have been read.
//
//*****************************************************************************
#define HAL_ADC_CH0_READ            (BIT0)
#define HAL_ADC_CH1_READ            (BIT1)
#define HAL_ADC_CH2_READ            (BIT2)
#define HAL_ADC_CH3_READ            (BIT3)

//*****************************************************************************
//
// The following are values that can be passed to the sampleFreq parameter
// for functions: HAL_ADC_init()
//
//*****************************************************************************
#define HAL_ADC_FREQ_4000_HZ        (4000)
#define HAL_ADC_FREQ_8000_HZ        (8000)
#define HAL_ADC_FREQ_16000_HZ       (16000)

//*****************************************************************************
//
// The following are values are used as mask for the phaseDataReady global
// variable to trigger the foreground process.
//
//*****************************************************************************
#define HAL_ADC_PHASE_NO_DATA_READY         0x00
#define HAL_ADC_PHASE_A_DATA_READY          0x01
#define HAL_ADC_PHASE_B_DATA_READY          0x02
#define HAL_ADC_PHASE_C_DATA_READY          0x04
#define HAL_ADC_PHASE_NEUTRAL_DATA_READY    0x40
#define HAL_ADC_PHASE_TOTAL_DATA_READY	    0x80

//*****************************************************************************
//
// The following are values are oversample values used to calculate the
// preload initial value.
//
//*****************************************************************************
#define HAL_ADC_OSR_32                      (32-1)
#define HAL_ADC_OSR_64                      (64-1)
#define HAL_ADC_OSR_128                     (128-1)
#define HAL_ADC_OSR_256                     (256-1)

//*****************************************************************************
// typedefs
//*****************************************************************************

//! \brief The following enumeration contain all possible HAL_ADC driver return
//!         message codes.
//!
typedef enum _HAL_ADC_message_code_
{
    HAL_ADC_message_code_no_error=0,
    //!< This message code is returned when no error was detected while
    //!< invoking the API.
    HAL_ADC_message_code_invalid_freq_error=1,
    //!< This message code is returned when the input frequency is not supported
} HAL_ADC_message_code;

//! \brief Contains the SD24 Channel Configuration
//!
typedef struct _HAL_ADC_SD24_Channel_
{
    uint8_t gain;
    //!< Gain of the Sigma Delta 24 channel
    //! Valid values are:
    //! - \b SD24_GAIN_1
    //! - \b SD24_GAIN_2
    //! - \b SD24_GAIN_4
    //! - \b SD24_GAIN_8
    //! - \b SD24_GAIN_16
    uint8_t preload;
    //!< Preload of the Sigma Delta 24 Channel- Valid 0x00 to SD24_MAX_PRELOAD
    int32_t *samplePtr;
    //!< Channel enabled used to initialize the SD24 channel
    uint8_t converter;
    //! Selects the converter that will be configured. Check datasheet
    //! for available converters on device.
    //! \n Valid values are:
    //! - \b SD24_CONVERTER_0
    //! - \b SD24_CONVERTER_1
    //! - \b SD24_CONVERTER_2
    //! - \b SD24_CONVERTER_3
} HAL_ADC_SD24_Channel;

//! \brief Contains the SD24 Configuration
//!
typedef struct _HAL_ADC_SD24_Configuration_
{
    uint16_t overSampleRatio;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b SD24_OVERSAMPLE_32
    //! - \b SD24_OVERSAMPLE_64
    //! - \b SD24_OVERSAMPLE_128
    //! - \b SD24_OVERSAMPLE_256
    uint16_t sampleFreq;
    //!< Sample Frequency for each converter
    //! \n Valid values are:
    //! - \b HAL_ADC_sample_frequency_2000_Hz
    //! - \b HAL_ADC_sample_frequency_4000_Hz
    //! - \b HAL_ADC_sample_frequency_8000_Hz
    uint8_t referenceSelect;
    //! Selects the reference source for the SD24 core
    //! \n Valid values are:
    //! - \b SD24_REF_EXTERNAL
    //! - \b SD24_REF_INTERNAL
    uint8_t masterConverter;
    //! The highest converter used to start / stop conversions
    HAL_ADC_SD24_Channel *adcChannelPtr;
    //! Pointer to the location where the channel configuration is stored
    uint8_t adcChannelLength;
    //! Length of the adcChannelPtr array
} HAL_ADC_SD24_Configuration;

//*****************************************************************************
// the function prototypes
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup hal_adc_api_sd24
//! @{
//
//******************************************************************************
//! \brief The following API configures SD24 module based on user configuration
//! files "emSwLib_userConfig.c" and "emSwLib_userConfig.h".
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return HAL_ADC_message_code
//!
extern HAL_ADC_message_code HAL_ADC_init(HAL_ADC_SD24_Configuration *config);

//! \brief The following API starts the enabled converters.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return none
//!
extern void HAL_ADC_startConversion(HAL_ADC_SD24_Configuration *config);

//! \brief The following API stops the enabled converters.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return none
//!
extern void HAL_ADC_stopConversion(HAL_ADC_SD24_Configuration *config);

//! \brief The following API enables the interrupt for the master converter.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return none
//!
extern void HAL_ADC_enableInterrupt(HAL_ADC_SD24_Configuration *config);

//! \brief The following API disables the interrupt for the master converter
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return none
//!
extern void HAL_ADC_disableInterrupt(HAL_ADC_SD24_Configuration *config);

//! \brief The following API sends the stored results to the GUI
//!
//! \param[in] config  is a pointer to the HAL ADC SD24 configuration
//!
//! \return none
//!
extern void HAL_ADC_printResult(HAL_ADC_SD24_Configuration *config);

//! \brief The following API clears all the global variables used in the ISR.)
//!
//! \param[in] phase is the phase index.
//!
//! \return none
//!
extern void HAL_ADC_clearVariables(uint8_t phase);

//*****************************************************************************
// globals
//*****************************************************************************

extern volatile uint8_t phaseDataReady;

extern HAL_ADC_SD24_Configuration halAdcConfig;

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#endif
