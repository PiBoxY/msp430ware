<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   hal_adc.h
//!
//! \brief  Hardware Abstraction Layer for Sigma-Delta 24 (SD24_B)
//!
//!  Group:          MSP Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include "sd24_b.h"
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
#define HAL_ADC_CH4_READ            (BIT4)
#define HAL_ADC_CH5_READ            (BIT5)
#define HAL_ADC_CH6_READ            (BIT6)
#define HAL_ADC_CH7_READ            (BIT7)

//*****************************************************************************
//
// The following are values are used by HAL_ADC_setClockDividers() function
// to set the clockPreDivider and clockDivider. The formula
// SMCLK / (OSR * sampleFreq) should include one of the values below.
//
//*****************************************************************************
#define HAL_ADC_CLOCK_MULT_4        4
#define HAL_ADC_CLOCK_MULT_8        8
#define HAL_ADC_CLOCK_MULT_12       12
#define HAL_ADC_CLOCK_MULT_16       16
#define HAL_ADC_CLOCK_MULT_24       24
#define HAL_ADC_CLOCK_MULT_32       32
#define HAL_ADC_CLOCK_MULT_64       64
#define HAL_ADC_CLOCK_MULT_96       96
#define HAL_ADC_CLOCK_MULT_128      128
#define HAL_ADC_CLOCK_MULT_192      192
#define HAL_ADC_CLOCK_MULT_384      384

//*****************************************************************************
//
// The following are values that can be passed to the clockPreDivider parameter
// for functions: SD24_B_init();
//
//*****************************************************************************
#define HAL_ADC_SD24PDIV_MULT_4     (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_8     (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_12    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_16    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_24    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_32    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_48    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_64    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_96    (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_128   (SD24_B_PRECLOCKDIVIDER_4)
#define HAL_ADC_SD24PDIV_MULT_192   (SD24_B_PRECLOCKDIVIDER_8)
#define HAL_ADC_SD24PDIV_MULT_384   (SD24_B_PRECLOCKDIVIDER_128)

//*****************************************************************************
//
// The following are values that can be passed to the clockDivider parameter
// for functions: SD24_B_init();
//
//*****************************************************************************
#define HAL_ADC_SD24DIV_MULT_4      (SD24_B_CLOCKDIVIDER_1)
#define HAL_ADC_SD24DIV_MULT_8      (SD24_B_CLOCKDIVIDER_2)
#define HAL_ADC_SD24DIV_MULT_12     (SD24_B_CLOCKDIVIDER_3)
#define HAL_ADC_SD24DIV_MULT_16     (SD24_B_CLOCKDIVIDER_4)
#define HAL_ADC_SD24DIV_MULT_24     (SD24_B_CLOCKDIVIDER_6)
#define HAL_ADC_SD24DIV_MULT_32     (SD24_B_CLOCKDIVIDER_8)
#define HAL_ADC_SD24DIV_MULT_48     (SD24_B_CLOCKDIVIDER_12)
#define HAL_ADC_SD24DIV_MULT_64     (SD24_B_CLOCKDIVIDER_16)
#define HAL_ADC_SD24DIV_MULT_96     (SD24_B_CLOCKDIVIDER_24)
#define HAL_ADC_SD24DIV_MULT_128    (SD24_B_CLOCKDIVIDER_32)
#define HAL_ADC_SD24DIV_MULT_192    (SD24_B_CLOCKDIVIDER_24)
#define HAL_ADC_SD24DIV_MULT_384    (SD24_B_CLOCKDIVIDER_3)

//*****************************************************************************
//
// The following are values that can be passed to the sampleFreq parameter
// for functions: HAL_ADC_init()
//
//*****************************************************************************
#define HAL_ADC_FREQ_2048_HZ        2048
#define HAL_ADC_FREQ_4096_HZ        4096
#define HAL_ADC_FREQ_8192_HZ        8192
#define HAL_ADC_FREQ_16384_HZ       16384

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
#define HAL_ADC_PHASE_D_DATA_READY          0x08
#define HAL_ADC_PHASE_E_DATA_READY          0x10
#define HAL_ADC_PHASE_F_DATA_READY          0x20
#define HAL_ADC_PHASE_NEUTRAL_DATA_READY    0x40
#define HAL_ADC_PHASE_TOTAL_DATA_READY	    0x80

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
    HAL_ADC_message_code_invalid_mod_freq_error=1,
    //!< This message code is returned when the modulation frequency (OSR*sampFreq)
    //!< is greater than 2Mhz
    HAL_ADC_message_code_invalid_clock_divider_error=2,
    //!< This message code is returned when the clock divider multiplier
    //!< (SMCLK / (OSR * sampleFreq) is not supported.
} HAL_ADC_message_code;

//! \brief Contains the SD24 Channel Configuration
//!
typedef struct _HAL_ADC_SD24_B_Channel_
{
    uint8_t gain;
    //!< Gain of the Sigma Delta 24 channel
    //! Valid values are:
    //! - \b SD24_GAIN_1
    //! - \b SD24_GAIN_2
    //! - \b SD24_GAIN_4
    //! - \b SD24_GAIN_8
    //! - \b SD24_GAIN_16
    uint16_t preload;
    //!< Preload of the Sigma Delta 24 Channel
    uint8_t converter;
    //! Selects the converter that will be configured. Check datasheet
    //! for available converters on device.
    //! \n Valid values are:
    //! - \b SD24_B_CONVERTER_0
    //! - \b SD24_B_CONVERTER_1
    //! - \b SD24_B_CONVERTER_2
    //! - \b SD24_B_CONVERTER_3
    uint8_t alignment;
    //! Selects how the data will be aligned in result
    //! \n Valid values are:
    //! - \b SD24_B_ALIGN_RIGHT [Default]
    //! - \b SD24_B_ALIGN_LEFT
} HAL_ADC_SD24_B_Channel;

//! \brief Contains the SD24 Configuration
//!
typedef struct _HAL_ADC_SD24_B_Configuration_
{
    uint32_t SMCLKFrequency;
    //!< Sub-System Clock (SMCLK) Frequency in Hz
    uint16_t overSampleRatio;
    //! Selects oversampling ratio for the converter
    //! \n Valid values are:
    //! - \b SD24_B_OVERSAMPLE_32
    //! - \b SD24_B_OVERSAMPLE_64
    //! - \b SD24_B_OVERSAMPLE_128
    //! - \b SD24_B_OVERSAMPLE_256
    //! - \b SD24_B_OVERSAMPLE_1024
    uint16_t sampleFreq;
    //!< Sample Frequency for each converter
    //! \n Valid values are:
    //! - \b HAL_ADC_FREQ_2048_HZ
    //! - \b HAL_ADC_FREQ_4096_HZ
    //! - \b HAL_ADC_FREQ_8192_HZ
    //! - \b HAL_ADC_FREQ_16384_HZ
    SD24_B_initParam *sd24Param;
    //! Pointer to struct for SD24_B initialization
    uint8_t masterConverter;
    //! The highest converter used to start / stop conversions
    HAL_ADC_SD24_B_Channel *adcChannelPtr;
    //! Pointer to the location where the channel configuration is stored
    uint8_t adcChannelLength;
    //! Length of the adcChannelPtr array
} HAL_ADC_SD24_B_Configuration;

//*****************************************************************************
// the function prototypes
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup hal_adc_api_sd24b
//! @{
//
//******************************************************************************
//! \brief The following API configures SD24_B module based on user configuration
//! files "emSwLib_userConfig.c" and "emSwLib_userConfig.h".
//!
//! \param[in] config  is a pointer to the HAL ADC SD24_B configuration
//!
//! \return HAL_ADC_message_code
//!
extern HAL_ADC_message_code HAL_ADC_init(HAL_ADC_SD24_B_Configuration *config);

//! \brief The following API starts the enabled converters.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24_B configuration
//!
//! \return none
//!
extern void HAL_ADC_startConversion(HAL_ADC_SD24_B_Configuration *config);

//! \brief The following API stops the enabled converters.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24_B configuration
//!
//! \return none
//!
extern void HAL_ADC_stopConversion(HAL_ADC_SD24_B_Configuration *config);

//! \brief The following API enables the interrupt for the master converter.
//!
//! \param[in] config  is a pointer to the HAL ADC SD24_B configuration
//!
//! \return none
//!
extern void HAL_ADC_enableInterrupts(HAL_ADC_SD24_B_Configuration *config);

//! \brief The following API disables the interrupt for all the channels
//! initialized in adcChannelArray (located in the "emSwLib_userConfig.c")
//!
//! \param[in] config  is a pointer to the HAL ADC SD24_B configuration
//!
//! \return none
//!
extern void HAL_ADC_disableInterrupts(HAL_ADC_SD24_B_Configuration *config);

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

extern HAL_ADC_SD24_B_Configuration halAdcConfig;

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#endif
