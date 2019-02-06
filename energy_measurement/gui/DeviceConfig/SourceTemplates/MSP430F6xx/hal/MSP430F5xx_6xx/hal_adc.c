<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   hal_adc.c
//!
//! \brief  Hardware Abstraction Layer for Sigma-Delta 24 (SD24)
//!
//!  Group:          MSP Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################

#include "hal.h"

// Status variable used to trigger background process of voltage and current samples
uint16_t chReadStatus;

// Variable used to trigger foreground process
volatile uint8_t phaseDataReady;

<EMDC_ADC_GLOBAL_DEF>

//
// Function declarations
//
int8_t HAL_ADC_getWholeSample(int16_t phaseCorrection)
{
    return (phaseCorrection >> 10);
}

uint16_t HAL_ADC_getPreload(int16_t phaseCorrection, uint16_t osr)
{
    return (phaseCorrection & osr);
}

void HAL_ADC_initPreload(HAL_ADC_SD24_B_Configuration *config)
{
<EMDC_HAL_ADC_INIT_PRELOAD>
}

void HAL_ADC_initWholeSampleAlignment(HAL_ADC_SD24_B_Configuration *config)
{
<EMDC_HAL_ADC_INIT_WHOLE_SAMPLE>
}

void HAL_ADC_clearVariables(uint8_t phase)
{
    switch(phase)
    {
<EMDC_ADC_CLEAR_VARIABLES>
    }
	chReadStatus = 0;
}

void HAL_ADC_initializeGroup(HAL_ADC_SD24_B_Configuration *config)
{
    uint8_t i, converter;

    for(i=0;i<config->adcChannelLength;i++)
    {
        // Find the convIndex of the current converter
        converter = config->adcChannelPtr[i].converter;
        // Create a param structure
        SD24_B_initConverterAdvancedParam param = {0};

        SD24_B_setConversionDelay(SD24_BASE,converter, config->adcChannelPtr[i].preload);
		param.gain = config->adcChannelPtr[i].gain;
        param.converter = converter;
        param.alignment = config->adcChannelPtr[i].alignment;
        param.startSelect = SD24_B_CONVERSION_SELECT_GROUP0;
        param.conversionMode = SD24_B_CONTINUOUS_MODE;
        param.dataFormat = SD24_B_DATA_FORMAT_2COMPLEMENT;
        param.sampleDelay = SD24_B_FOURTH_SAMPLE_INTERRUPT;
        param.oversampleRatio = config->overSampleRatio;

        SD24_B_initConverterAdvanced(SD24_BASE, &param);
        // Enable the interrupt for each channel
        SD24_B_enableInterrupt(SD24_BASE, converter, SD24_B_CONVERTER_INTERRUPT);
    }
}

HAL_ADC_message_code HAL_ADC_setClockDividers(HAL_ADC_SD24_B_Configuration *config)
{
    uint32_t modFreq;
    uint32_t tempDiv;
    HAL_ADC_message_code code = HAL_ADC_message_code_no_error;

    // Calculate the modulation frequency
    modFreq = (uint32_t) config->sampleFreq * (uint32_t) (config->overSampleRatio+1);

    // Calculate SMCLK / (OSR * sampleFreq)
    tempDiv = config->SMCLKFrequency / modFreq;

    // Set ClockPreDivider and ClockDivider
    switch(tempDiv)
    {
    case HAL_ADC_CLOCK_MULT_4:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_4;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_4;
        break;
    case HAL_ADC_CLOCK_MULT_8:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_8;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_8;
        break;
    case HAL_ADC_CLOCK_MULT_12:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_12;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_12;
        break;
    case HAL_ADC_CLOCK_MULT_16:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_16;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_16;
        break;
    case HAL_ADC_CLOCK_MULT_24:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_24;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_24;
        break;
    case HAL_ADC_CLOCK_MULT_32:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_32;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_32;
        break;
    case HAL_ADC_CLOCK_MULT_64:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_64;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_64;
        break;
    case HAL_ADC_CLOCK_MULT_96:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_96;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_96;
        break;
    case HAL_ADC_CLOCK_MULT_128:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_128;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_128;
        break;
    case HAL_ADC_CLOCK_MULT_192:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_192;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_192;
        break;
    case HAL_ADC_CLOCK_MULT_384:
        config->sd24Param->clockPreDivider = HAL_ADC_SD24PDIV_MULT_384;
        config->sd24Param->clockDivider = HAL_ADC_SD24DIV_MULT_384;
        break;
    default:
        code = HAL_ADC_message_code_invalid_clock_divider_error;
        break;
    }

    return code;
}

HAL_ADC_message_code HAL_ADC_checkModFreq(HAL_ADC_SD24_B_Configuration *config)
{
    uint32_t modFreq;
    HAL_ADC_message_code code;

    modFreq = (uint32_t) config->sampleFreq * (uint32_t) (config->overSampleRatio+1);

    if(modFreq > HAL_ADC_MAX_MOD_FREQ)
    {
        code = HAL_ADC_message_code_invalid_mod_freq_error;
    }
    else
    {
        code = HAL_ADC_message_code_no_error;
    }

    return code;
}

HAL_ADC_message_code HAL_ADC_init(HAL_ADC_SD24_B_Configuration *config)
{
    HAL_ADC_message_code code;

    // Initialize global variables
    phaseDataReady = HAL_ADC_PHASE_NO_DATA_READY;
	chReadStatus=0;
<EMDC_ADC_INIT_GLOBAL>
    HAL_ADC_initPreload(config);
    HAL_ADC_initWholeSampleAlignment(config);

    // Verify the modulation frequency is less than 2MHz
    code = HAL_ADC_checkModFreq(config);
    if(code != HAL_ADC_message_code_no_error)
    {
        return code;
    }

    // Set the clock dividers for the desired OSR and Fsamp
    code = HAL_ADC_setClockDividers(config);
    if(code != HAL_ADC_message_code_no_error)
    {
        return code;
    }

    // Initialize SD24
    SD24_B_init(SD24_BASE, config->sd24Param);

    // Configure one group for the SD24
    HAL_ADC_initializeGroup(config);

    return code;
}

void HAL_ADC_startConversion(HAL_ADC_SD24_B_Configuration *config)
{
    // Start conversion
    SD24_B_startGroupConversion(SD24_BASE, SD24_B_GROUP0);
}

void HAL_ADC_stopConversion(HAL_ADC_SD24_B_Configuration *config)
{
    // Stops the conversion for the enabled SD24 channels
    SD24_B_stopGroupConversion(SD24_BASE, SD24_B_GROUP0);
}

void HAL_ADC_enableInterrupts(HAL_ADC_SD24_B_Configuration *config)
{
    uint8_t i, converter;

    // Enable all interrupts
    for(i=0;i<config->adcChannelLength;i++)
    {
        // Find the convIndex of the current converter
        converter = config->adcChannelPtr[i].converter;

        SD24_B_enableInterrupt(SD24_BASE, converter, SD24_B_CONVERTER_INTERRUPT);
    }
}

void HAL_ADC_disableInterrupts(HAL_ADC_SD24_B_Configuration *config)
{
    uint8_t i, converter;

    // Disable all interrupts
    for(i=0;i<config->adcChannelLength;i++)
    {
        // Find the convIndex of the current converter
        converter = config->adcChannelPtr[i].converter;

        SD24_B_disableInterrupt(SD24_BASE, converter, SD24_B_CONVERTER_INTERRUPT);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24B_VECTOR
__interrupt void SD24BISR(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24_VECTOR))) SD24_ISR(void)
#else
#error Compiler not supported!
#endif
    int16_t tempV;
    int32_t tempI;
<EMDC_HAL_ADC_ISR>
}
