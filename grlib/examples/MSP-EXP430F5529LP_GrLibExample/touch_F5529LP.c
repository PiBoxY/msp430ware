/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
#include <stdbool.h>
#include <stdint.h>
#include <msp430f5529.h>
#include "driverlib.h"
#include "grlib.h"
#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "touch_F5529LP.h"

/*
 * Graphic library context
 */
extern Graphics_Context g_sContext;

static bool touch_detectedTouch(void);
static uint16_t touch_sampleX(void);
static uint16_t touch_sampleY(void);

#if defined(__TI_COMPILER_VERSION__)
#pragma LOCATION(touch_calibrationData, 0x1980);
#pragma NOINIT(touch_calibrationData);
static touch_calibration touch_calibrationData;
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma location=0x1980
__no_init static touch_calibration touch_calibrationData;
#else
#error "Compiler not supported!"
#endif

void touch_initInterface(void)
{
    /* Initialize analog interface. */
    /* Initialize the ADC12_A Module. */
    ADC12_A_init(ADC12_A_BASE,
                 ADC12_A_SAMPLEHOLDSOURCE_SC,
                 ADC12_A_CLOCKSOURCE_ADC12OSC,
                 ADC12_A_CLOCKDIVIDER_1);
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
                               ADC12_A_CYCLEHOLD_64_CYCLES,
                               ADC12_A_CYCLEHOLD_64_CYCLES,
                               ADC12_A_MULTIPLESAMPLESDISABLE);
    ADC12_A_enable(ADC12_A_BASE);

    ADC12_A_configureMemoryParam adc12MemConfigParamsY =
    {
        TOUCH_Y_PLUS_MEMORY,
        TOUCH_Y_PLUS_INPUT,
        ADC12_A_VREFPOS_AVCC,
        ADC12_A_VREFNEG_AVSS,
        ADC12_A_NOTENDOFSEQUENCE
    };

    ADC12_A_configureMemoryParam adc12MemConfigParamsX =
    {
        TOUCH_X_PLUS_MEMORY,
        TOUCH_X_PLUS_INPUT,
        ADC12_A_VREFPOS_AVCC,
        ADC12_A_VREFNEG_AVSS,
        ADC12_A_NOTENDOFSEQUENCE
    };

    /* Configure Y+ input to memory buffer 0. */
    ADC12_A_configureMemory(ADC12_A_BASE,
                            &adc12MemConfigParamsY);

    /* Configure X+ input to memory buffer 1. */
    ADC12_A_configureMemory(ADC12_A_BASE,
                            &adc12MemConfigParamsX);

    /* Configure SW1 for input. */
    GPIO_setAsInputPinWithPullUpResistor(TOUCH_SW1_PORT, TOUCH_SW1_PIN);

    /* Check if the screen has already been calibrated. */
    if(touch_calibrationData.key == TOUCH_CALIBRATION_KEY)
    {
        /* Return if the user is not manually requesting calibration. */
        if(GPIO_getInputPinValue(TOUCH_SW1_PORT, TOUCH_SW1_PIN))
        {
            return;
        }
    }

    /* Wait for SW1 to be released. */
    while(!GPIO_getInputPinValue(TOUCH_SW1_PORT, TOUCH_SW1_PIN))
    {
        ;
    }

    /* Calibrate the touch screen. */
    touch_calibrate();
}

void touch_updateCurrentTouch(touch_context *data)
{
    ;
    uint16_t adcResult;
    float q12Ratio;

    /* Sample new X and Y coordinates if touch was detected. */
    if(touch_detectedTouch())
    {
        data->touch = true;

        /* Sample the X analog axis. */
        adcResult = touch_sampleX();

        /* Map the analog reading to the display coordinates. */
        /* Map the ADC reading to the display coordinates. */
        q12Ratio =
            (float)(((float)(adcResult -
                             touch_calibrationData.xMin)) /
                    (float)(touch_calibrationData.xMax -
                            touch_calibrationData.xMin));
        if(q12Ratio >= 1)
        {
            q12Ratio = 1;
        }
        if(q12Ratio < 0)
        {
            q12Ratio = 0;
        }
        data->x = (uint16_t)(q12Ratio * LCD_HORIZONTAL_MAX);

        /* Sample the Y analog axis. */
        adcResult = touch_sampleY();

        /* Map the ADC reading to the display coordinates. */
        q12Ratio =
            (float)(((float)(adcResult -
                             touch_calibrationData.yMin)) /
                    ((float)(touch_calibrationData.yMax -
                             touch_calibrationData.yMin)));
        if(q12Ratio >= 1)
        {
            q12Ratio = 1;
        }
        if(q12Ratio < 0)
        {
            q12Ratio = 0;
        }

        data->y = (uint16_t)(q12Ratio * LCD_VERTICAL_MAX);
    }
    else
    {
        data->touch = false;
        data->x = 0xffff;
        data->y = 0xffff;
    }
}

/*
 * Returns true when a touch is detected.
 */
static bool touch_detectedTouch(void)
{
    uint16_t ui16ADCtemp;

    /* Set X- and Y- as output and Y+ as input (floating). */
    GPIO_setAsOutputPin(TOUCH_X_MINUS_PORT, TOUCH_X_MINUS_PIN);
    GPIO_setAsOutputPin(TOUCH_Y_MINUS_PORT, TOUCH_Y_MINUS_PIN);
    GPIO_setAsInputPin(TOUCH_Y_PLUS_PORT, TOUCH_Y_PLUS_PIN);

    /* Set X- high and Y- low to detect touch. */
    GPIO_setOutputHighOnPin(TOUCH_X_MINUS_PORT, TOUCH_X_MINUS_PIN);
    GPIO_setOutputLowOnPin(TOUCH_Y_MINUS_PORT, TOUCH_Y_MINUS_PIN);

    /* Sample the X+ ADC channel to check if there is currently a touch. */
    GPIO_setAsPeripheralModuleFunctionOutputPin(TOUCH_X_PLUS_PORT,
                                                TOUCH_X_PLUS_PIN);
    ADC12_A_clearInterrupt(ADC12_A_BASE, TOUCH_X_PLUS_IFG);
    ADC12_A_startConversion(ADC12_A_BASE, TOUCH_X_PLUS_MEMORY,
                            ADC12_A_SINGLECHANNEL);
    while(!ADC12_A_getInterruptStatus(ADC12_A_BASE, TOUCH_X_PLUS_IFG))
    {
        ;
    }
    ui16ADCtemp = ADC12_A_getResults(ADC12_A_BASE, TOUCH_X_PLUS_MEMORY);

    /* Check if the detected touch is below the threshold. */
    if(ui16ADCtemp < TOUCH_THRESHOLD)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

static uint16_t touch_sampleX(void)
{
    uint32_t average = 0;
    uint8_t i;
    /* Set X+ and X- as output and Y- as input (floating). */
    GPIO_setAsOutputPin(TOUCH_X_PLUS_PORT, TOUCH_X_PLUS_PIN);
    GPIO_setAsOutputPin(TOUCH_X_MINUS_PORT, TOUCH_X_MINUS_PIN);
    GPIO_setAsInputPin(TOUCH_Y_MINUS_PORT, TOUCH_Y_MINUS_PIN);

    /* Set X+ high and X- low. */
    GPIO_setOutputHighOnPin(TOUCH_X_PLUS_PORT, TOUCH_X_PLUS_PIN);
    GPIO_setOutputLowOnPin(TOUCH_X_MINUS_PORT, TOUCH_X_MINUS_PIN);

    /* Sample the Y+ ADC channel. */
    GPIO_setAsPeripheralModuleFunctionOutputPin(TOUCH_Y_PLUS_PORT,
                                                TOUCH_Y_PLUS_PIN);
    ADC12_A_clearInterrupt(ADC12_A_BASE, TOUCH_Y_PLUS_IFG);

    for(i = 0; i < TOUCH_OVERSAMPLE; i++)
    {
        ADC12_A_startConversion(ADC12_A_BASE, TOUCH_Y_PLUS_MEMORY,
                                ADC12_A_SINGLECHANNEL);
        while(!ADC12_A_getInterruptStatus(ADC12_A_BASE, TOUCH_Y_PLUS_IFG))
        {
            ;
        }
        average += ADC12_A_getResults(ADC12_A_BASE, TOUCH_Y_PLUS_MEMORY);
    }

    /* Return the analog result. */
    return(average >> TOUCH_AVERAGE_DIVISOR);
}

/*
 * Sample the Y analog axis.
 */
static uint16_t touch_sampleY(void)
{
    uint32_t average = 0;
    uint8_t i;
    /* Set Y+ and Y- as output and X- as input (floating). */
    GPIO_setAsOutputPin(TOUCH_Y_PLUS_PORT, TOUCH_Y_PLUS_PIN);
    GPIO_setAsOutputPin(TOUCH_Y_MINUS_PORT, TOUCH_Y_MINUS_PIN);
    GPIO_setAsInputPin(TOUCH_X_MINUS_PORT, TOUCH_X_MINUS_PIN);

    /* Set Y+ low and Y- high. */
    GPIO_setOutputLowOnPin(TOUCH_Y_PLUS_PORT, TOUCH_Y_PLUS_PIN);
    GPIO_setOutputHighOnPin(TOUCH_Y_MINUS_PORT, TOUCH_Y_MINUS_PIN);

    /* Sample the X+ ADC channel. */
    GPIO_setAsPeripheralModuleFunctionOutputPin(TOUCH_X_PLUS_PORT,
                                                TOUCH_X_PLUS_PIN);
    ADC12_A_clearInterrupt(ADC12_A_BASE, TOUCH_X_PLUS_IFG);

    for(i = 0; i < TOUCH_OVERSAMPLE; i++)
    {
        ADC12_A_startConversion(ADC12_A_BASE, TOUCH_X_PLUS_MEMORY,
                                ADC12_A_SINGLECHANNEL);
        while(!ADC12_A_getInterruptStatus(ADC12_A_BASE, TOUCH_X_PLUS_IFG))
        {
            ;
        }
        average += ADC12_A_getResults(ADC12_A_BASE, TOUCH_X_PLUS_MEMORY);
    }

    /* Return the anlog result. */
    return(average >> TOUCH_AVERAGE_DIVISOR);
}

static void touch_calibrateCircle(uint16_t *xSum,
                                  uint16_t *ySum,
                                  uint16_t xPos,
                                  uint16_t yPos)
{
    /* Draw a red circle for the user to touch. */
    Graphics_setForegroundColor(&g_sContext, ClrRed);
    Graphics_fillCircle(&g_sContext, xPos, yPos, TOUCH_CALIBRATION_RADIUS);

    /* Wait for a tocuh to be detected and wait ~4ms. */
    while(!touch_detectedTouch())
    {
        ;
    }
    __delay_cycles(100000);

    /* Sample the X and Y measurements of the touch screen. */
    *xSum += touch_sampleX();
    *ySum += touch_sampleY();

    /* Wait for the touch to stop and wait ~4ms. */
    while(touch_detectedTouch())
    {
        ;
    }
    __delay_cycles(100000);

    /* Clear the drawn circle. */
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_fillCircle(&g_sContext, xPos, yPos, TOUCH_CALIBRATION_RADIUS);

    return;
}

void touch_calibrate(void)
{
    touch_calibration calData;

    FlashCtl_unlockInfoA();
    FlashCtl_eraseSegment((uint8_t *)&touch_calibrationData);

    /* Zero out local copies of calibration data. */
    calData.xMin = 0;
    calData.xMax = 0;
    calData.yMin = 0;
    calData.yMax = 0;

    /* Display Instructions to the screen. */
    Graphics_setForegroundColor(&g_sContext, ClrWhite);
    Graphics_drawStringCentered(&g_sContext,
                                "TOUCH DOT TO CALIBRATE",
                                AUTO_STRING_LENGTH,
                                LCD_HORIZONTAL_MAX / 2,
                                LCD_VERTICAL_MAX / 2,
                                TRANSPARENT_TEXT);

    /* Top left corner. */
    touch_calibrateCircle(
        &calData.xMin,
        &calData.yMin,
        TOUCH_CALIBRATION_RADIUS,
        TOUCH_CALIBRATION_RADIUS);

    /* Top right corner. */
    touch_calibrateCircle(
        &calData.xMax,
        &calData.yMin,
        (LCD_HORIZONTAL_MAX - 1) - TOUCH_CALIBRATION_RADIUS,
        TOUCH_CALIBRATION_RADIUS);

    /* Bottom left corner. */
    touch_calibrateCircle(
        &calData.xMin,
        &calData.yMax,
        TOUCH_CALIBRATION_RADIUS,
        (LCD_VERTICAL_MAX - 1) - TOUCH_CALIBRATION_RADIUS);

    /* Bottom right corner. */
    touch_calibrateCircle(
        &calData.xMax,
        &calData.yMax,
        (LCD_HORIZONTAL_MAX - 1) - TOUCH_CALIBRATION_RADIUS,
        (LCD_VERTICAL_MAX - 1) - TOUCH_CALIBRATION_RADIUS);

    /* Compensate for the radius offset to caluculate final X calibration values. */
    calData.xMin = (calData.xMin >> 1) - TOUCH_CALIBRATION_RADIUS;
    calData.xMax = (calData.xMax >> 1) + TOUCH_CALIBRATION_RADIUS;

    /* Compensate for the radius offset to caluculate final Y calibration values. */
    calData.yMin = (calData.yMin >> 1) - TOUCH_CALIBRATION_RADIUS;
    calData.yMax = (calData.yMax >> 1) + TOUCH_CALIBRATION_RADIUS;

    /* Write the calibration key to signal the values have been saved. */
    calData.key = TOUCH_CALIBRATION_KEY;

    /* Save the calibration data. */
    FlashCtl_write32((uint32_t *)&calData, (uint32_t *)&touch_calibrationData,
                     sizeof(touch_calibrationData) / 4);
    FlashCtl_lockInfoA();

    Graphics_clearDisplay(&g_sContext);
}
