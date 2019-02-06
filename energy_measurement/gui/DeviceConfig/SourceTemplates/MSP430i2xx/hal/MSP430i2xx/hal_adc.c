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

void HAL_ADC_initPreload(HAL_ADC_SD24_Configuration *config)
{
<EMDC_HAL_ADC_INIT_PRELOAD>
}

void HAL_ADC_initWholeSampleAlignment(HAL_ADC_SD24_Configuration *config)
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

void HAL_ADC_findConverterMinMax(HAL_ADC_SD24_Configuration *config, uint8_t *minPtr, uint8_t *maxPtr)
{
    uint8_t i;

    // Set max and min to first item in the adcChannelArray
    *maxPtr = config->adcChannelPtr[0].converter;
    *minPtr = config->adcChannelPtr[0].converter;

    // Find the max and the min
    for(i=1;i<config->adcChannelLength;i++)
    {
        if(config->adcChannelPtr[i].converter > *maxPtr)
        {
            *maxPtr = config->adcChannelPtr[i].converter;
        }
        else if(config->adcChannelPtr[i].converter < *minPtr)
        {
            *minPtr = config->adcChannelPtr[i].converter;
        }
        else
        {
            // Do nothing
        }
    }
}

uint8_t HAL_ADC_findConverterIndex(HAL_ADC_SD24_Configuration *config, uint8_t converterNum)
{
    uint8_t i;
    uint8_t convIndex=HAL_ADC_CONV_INVALID_INDEX;

    for(i=0;i<config->adcChannelLength;i++)
    {
        // Check if the current index matches the converter number
        if(config->adcChannelPtr[i].converter == converterNum)
        {
            convIndex = i;
            break;
        }
    }

    return convIndex;
}

void HAL_ADC_initializeGroup(HAL_ADC_SD24_Configuration *config, uint8_t minConv, uint8_t maxConv)
{
    uint8_t i, convIndex;

    for(i=minConv;i<=maxConv;i++)
    {
        // Find the convIndex of the current converter
        convIndex = HAL_ADC_findConverterIndex(config,i);

        // Write the preload
        // Calculating address
        uint16_t address = SD24_BASE + (OFS_SD24PRE0 + i);
        SD24_initConverterAdvancedParam param = {0};

        if(convIndex == HAL_ADC_CONV_INVALID_INDEX)
        {
            //Clear Interrupt Flags
            HWREG16(address) = 0;
            param.gain = SD24_GAIN_1;
        }
        else
        {
            //Clear Interrupt Flags
            HWREG16(address) = config->adcChannelPtr[convIndex].preload;
            param.gain = config->adcChannelPtr[convIndex].gain;
        }

        param.converter = i;
        param.conversionMode = SD24_CONTINUOUS_MODE;
        param.inputChannel = SD24_INPUT_CH_ANALOG;
        param.dataFormat = SD24_DATA_FORMAT_2COMPLEMENT;
        param.interruptDelay = SD24_FOURTH_SAMPLE_INTERRUPT;
        switch(param.oversampleRatio)
        {
            case HAL_ADC_OSR_256:
                param.oversampleRatio = SD24_OVERSAMPLE_256;
                break;
            case HAL_ADC_OSR_128:
                param.oversampleRatio = SD24_OVERSAMPLE_128;
                break;
            case HAL_ADC_OSR_64:
                param.oversampleRatio = SD24_OVERSAMPLE_64;
                break;
            case HAL_ADC_OSR_32:
                param.oversampleRatio = SD24_OVERSAMPLE_32;
                break;
        }

        // Check if the existing ith converter is the last one
        if(i==maxConv)
        {
            param.groupEnable = SD24_NOT_GROUPED;
            SD24_initConverterAdvanced(SD24_BASE, &param);
        }
        else
        {
            param.groupEnable = SD24_GROUPED;
            SD24_initConverterAdvanced(SD24_BASE, &param);
        }

        if(convIndex != HAL_ADC_CONV_INVALID_INDEX)
        {
            SD24_enableInterrupt(SD24_BASE, i, SD24_CONVERTER_INTERRUPT);
        }
    }

}

HAL_ADC_message_code HAL_ADC_calculateOSR(HAL_ADC_SD24_Configuration *config)
{
    HAL_ADC_message_code status = HAL_ADC_message_code_no_error;

    // Check that the frequency is valid and calculate the oversSample ratio
    // Fm (Modulation Frequency) = 1MHz = Fs * OSR
    // Therefore Fs = 1MHz / OSR
    switch(config->sampleFreq)
    {
    case HAL_ADC_FREQ_4000_HZ:
        config->overSampleRatio = HAL_ADC_OSR_256;
        break;
    case HAL_ADC_FREQ_8000_HZ:
        config->overSampleRatio = HAL_ADC_OSR_128;
        break;
    case HAL_ADC_FREQ_16000_HZ:
        config->overSampleRatio = HAL_ADC_OSR_64;
        break;
    default:
        status = HAL_ADC_message_code_invalid_freq_error;
        break;
    }

    return status;
}

HAL_ADC_message_code HAL_ADC_init(HAL_ADC_SD24_Configuration *config)
{
    uint8_t maxConverter;
    uint8_t minConverter;
	HAL_ADC_message_code code = HAL_ADC_message_code_no_error;
	
    // Initialize global variables
    phaseDataReady = HAL_ADC_PHASE_NO_DATA_READY;
	chReadStatus=0;
<EMDC_ADC_INIT_GLOBAL>

    // Check that the OSR code is valid
    code = HAL_ADC_calculateOSR(config);
    if(code != HAL_ADC_message_code_no_error)
    {
        return code;
    }
	
    HAL_ADC_initPreload(config);
    HAL_ADC_initWholeSampleAlignment(config);

    // Find min and max values
    HAL_ADC_findConverterMinMax(config,&minConverter, &maxConverter);

    // Set the master converter
    config->masterConverter = maxConverter;

    // Initialize SD24
    SD24_init(SD24_BASE, config->referenceSelect);

    // Configure one group for the SD24
    HAL_ADC_initializeGroup(config,minConverter,maxConverter);

    return code;
}

void HAL_ADC_startConversion(HAL_ADC_SD24_Configuration *config)
{
    // Start conversion
    SD24_startConverterConversion(SD24_BASE, config->masterConverter);
}

void HAL_ADC_stopConversion(HAL_ADC_SD24_Configuration *config)
{
    // Stops the conversion for the enabled SD24 channels
    SD24_stopConverterConversion(SD24_BASE,config->masterConverter);
}

void HAL_ADC_enableInterrupt(HAL_ADC_SD24_Configuration *config)
{
    SD24_clearInterrupt(SD24_BASE, config->masterConverter, SD24_CONVERTER_INTERRUPT);
    SD24_enableInterrupt(SD24_BASE, config->masterConverter, SD24_CONVERTER_INTERRUPT);
}

void HAL_ADC_disableInterrupt(HAL_ADC_SD24_Configuration *config)
{
    SD24_disableInterrupt(SD24_BASE, config->masterConverter, SD24_CONVERTER_INTERRUPT);
}

typedef union
{
    int16_t by16[2];
    int32_t by32;
} sh_lo_t;

int32_t SD24_getResultsTwoComp(uint16_t baseAddress,
                               uint8_t converter)
{
    volatile uint16_t OSR;
    sh_lo_t value;

    // Calculate address of MEM results
    uint16_t address = baseAddress + (OFS_SD24MEM0 + (converter * 0x02));

    // Get high word result
    HWREG16(baseAddress +
            (OFS_SD24CCTL0 + (converter * 0x02))) &= ~(SD24LSBACC);
    value.by16[1] = (int16_t) HWREG16(address) >> 8;

    // Get low word result
    HWREG16(baseAddress + (OFS_SD24CCTL0 + (converter * 0x02))) |= SD24LSBACC;
    value.by16[0] = HWREG16(address);
    HWREG16(baseAddress +
            (OFS_SD24CCTL0 + (converter * 0x02))) &= ~(SD24LSBACC);

    // Determine the OSR and combine the high and low result words as appropriate
    OSR =
        HWREG16(baseAddress +
                (OFS_SD24CCTL0 + (converter * 0x02))) & (SD24OSR0 | SD24OSR1);

    if(OSR == SD24_OVERSAMPLE_256)
    {
        return(value.by32);
    }
    else if(OSR == SD24_OVERSAMPLE_128)
    {
        value.by16[0] = value.by16[0]  << 3;
        return(value.by32);
    }
    else if(OSR == SD24_OVERSAMPLE_64)
    {
        value.by16[0] = value.by16[0]  << 6;
        return(value.by32);
    }
    else        // OSR = SD24_OVERSAMPLE_32
    {
        value.by16[1] = 0;
        return(value.by32);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=SD24_VECTOR
__interrupt void SD24_ISR(void) {
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SD24_VECTOR))) SD24_ISR(void)
#else
#error Compiler not supported!
#endif
    int16_t tempV;
    int32_t tempI;
<EMDC_HAL_ADC_ISR>
}
