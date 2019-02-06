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
//******************************************************************************
//!  TI-Design Signal Processing with LEA on MSP430FR5994
//!
//!  William Goh
//!  Texas Instruments Inc.
//!  April 2016
//******************************************************************************

#include <driverlib.h>
#include <stdint.h>
#include <grlib.h>
#include <stdio.h>
#include <math.h>
#include "DSPLib.h"
#include "FFT.h"
#include "FFT_430.h"
#include "benchmark.h"
#include "audio_collect.h"
#include "global.h"
#include "application.h"

// Rectangular to clear my FFT image after every round
const Graphics_Rectangle clearSpace = { 0, 10, 96, 83 };
const Graphics_Rectangle clearSpaceTopText = { 0, 0, 96, 7 };
const Graphics_Rectangle clearSpaceCycleText = { 10, 85, 65, 96 };

#if defined(__IAR_SYSTEMS_ICC__)
// Real FFT values that can be used by user application
__persistent int16_t FFT_data[VECTOR_SIZE] = {0};

// FFT buffer that contains real and imaginary values after being calculated
__persistent int16_t FBuff[VECTOR_SIZE * 2] = {0};

// raw data buffer for processing by the FFT algorithm
__persistent int16_t voice_data[VECTOR_SIZE * 2] = {0};
#elif defined(__TI_COMPILER_VERSION__)
// Real FFT values that can be used by user application
#pragma PERSISTENT(FFT_data)
int16_t FFT_data[VECTOR_SIZE] = {0};

// FFT buffer that contains real and imaginary values after being calculated
#pragma PERSISTENT(FBuff)
int16_t FBuff[VECTOR_SIZE * 2] = {0};

// raw data buffer for processing by the FFT algorithm
#pragma PERSISTENT(voice_data)
int16_t voice_data[VECTOR_SIZE * 2] = {0};
#endif

const int bit_rev_index[] =
{0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50,
 0xD0, 0x30, 0xB0, 0x70, 0xF0, 0x08, 0x88, 0x48,
 0xC8, 0x28, 0xA8,
 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8,
 0x78, 0xF8, 0x04,
 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14,
 0x94, 0x54, 0xD4,
 0x34, 0xB4, 0x74, 0xF4, 0x0C, 0x8C, 0x4C, 0xCC,
 0x2C, 0xAC, 0x6C,
 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C,
 0xFC, 0x02, 0x82,
 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92,
 0x52, 0xD2, 0x32,
 0xB2, 0x72, 0xF2, 0x0A, 0x8A, 0x4A, 0xCA, 0x2A,
 0xAA, 0x6A, 0xEA,
 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
 0x06, 0x86, 0x46,
 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56,
 0xD6, 0x36, 0xB6,
 0x76, 0xF6, 0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE,
 0x6E, 0xEE, 0x1E,
 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE, 0x01,
 0x81, 0x41, 0xC1,
 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1,
 0x31, 0xB1, 0x71,
 0xF1, 0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69,
 0xE9, 0x19, 0x99,
 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 0x05, 0x85,
 0x45, 0xC5, 0x25,
 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35,
 0xB5, 0x75, 0xF5,
 0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
 0x1D, 0x9D, 0x5D,
 0xDD, 0x3D, 0xBD, 0x7D, 0xFD, 0x03, 0x83, 0x43,
 0xC3, 0x23, 0xA3,
 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3,
 0x73, 0xF3, 0x0B,
 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B,
 0x9B, 0x5B, 0xDB,
 0x3B, 0xBB, 0x7B, 0xFB, 0x07, 0x87, 0x47, 0xC7,
 0x27, 0xA7, 0x67,
 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77,
 0xF7, 0x0F, 0x8F,
 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F,
 0x5F, 0xDF, 0x3F,
 0xBF, 0x7F, 0xFF};

uint16_t i, index;
int16_t imag, real, real_abs, imag_abs, mag, max, min;
int16_t scale;

msp_cmplx_fft_q15_params complexFftParams;

static uint8_t refreshDisp = 0;
static int8_t str[20];

void initFft(void)
{
    resetBenchmark();

    // Disable the audio playback for lowest power configuration
    Audio_stopPlayback(&gPlaybackConfig);

    // Initializes the FFT parameters
    complexFftParams.length = VECTOR_SIZE;
    complexFftParams.bitReverse = 1;
    complexFftParams.twiddleTable = msp_cmplx_twiddle_table_256_q15;

    // Initialize the microphone for recording
    gAudioConfig.audioBuffer1 = (int16_t *) adcBuffer0;
    gAudioConfig.audioBuffer2 = (int16_t *) adcBuffer1;
    gAudioConfig.bufferSize = VECTOR_SIZE;
    gAudioConfig.sampleRate = FFT_SAMPLING_FREQUENCY;
    gAudioConfig.twosComplement = true;
    Audio_setupCollect(&gAudioConfig);

    // Start the recording by enabling the timer
    Audio_startCollect(&gAudioConfig);
}

/**********************************************************************//**
 * @brief  Runs the FFT. Performs the FFT and puts it in FFT_data buffer.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void runFftWithoutLea(void)
{
    int16_t * currentAdcBuffer;

    // Initialize the number of times to refresh the display
    refreshDisp = 2;

    // Clear the screen first
    Graphics_clearDisplay(&g_sContext);

    // Draw the basic structure
    GrStringDraw(&g_sContext, "0",
                 AUTO_STRING_LENGTH, 1, 86,
                 TRANSPARENT_TEXT);
    GrStringDraw(&g_sContext, "Fs 2",
                 AUTO_STRING_LENGTH, 70, 86,
                 TRANSPARENT_TEXT);
    GrLineDrawH(&g_sContext, 0, 96, 84);

    // Draw the title once
    Graphics_drawString(&g_sContext, "FFT w/o LEA",
                        AUTO_STRING_LENGTH, 0, 0,
                        GRAPHICS_OPAQUE_TEXT);

    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);

    keepAppRunning = true;

    while(keepAppRunning)
    {
        __bis_SR_register(LPM3_bits + GIE);

        // Copy recorded ADC data to LEA RAM buffer for FFT processing
        currentAdcBuffer = Audio_getActiveBuffer(&gAudioConfig);

        for(i = 0; i < (VECTOR_SIZE); i++)
        {
            ((int32_t *)voice_data)[i] = currentAdcBuffer[i];
        }

        resetBenchmark();
        startBenchmark();

        // Perform FFT and store
        FFT_prog();

        stopBenchmark();

        // Calculate magnitude for the positive frequency domain
        for(i = 0; i < (VECTOR_SIZE / 2); i++)
        {
            index = bit_rev_index[i];
            real = FBuff[2 * index];
            imag = FBuff[2 * index + 1];
            if(real < 0)
            {
                real_abs = ~real + 1;
            }
            else
            {
                real_abs = real;
            }
            if(imag < 0)
            {
                imag_abs = ~imag + 1;
            }
            else
            {
                imag_abs = imag;
            }
            if(real_abs >= imag_abs)
            {
                max = real_abs;
                min = imag_abs;
            }
            else
            {
                max = imag_abs;
                min = real_abs;
            }
            mag = max + 3 * (min >> 3);
            FFT_data[i] = mag;
        }

#ifndef PWR_BENCHMARK
        if(refreshDisp)
        {
            sprintf((char *) str, "Cyc:%lu", cycleCounts);
        }

        // Clear the lines by drawing a box
        Graphics_setForegroundColor(&g_sContext, ClrWhite);
        Graphics_fillRectangle(&g_sContext, &clearSpace);
        if(refreshDisp)
        {
            Graphics_fillRectangle(&g_sContext, &clearSpaceCycleText);
        }
        Graphics_setForegroundColor(&g_sContext, ClrBlack);
        if(refreshDisp)
        {
            Graphics_drawString(&g_sContext, str,
                                AUTO_STRING_LENGTH, 10, 86,
                                GRAPHICS_OPAQUE_TEXT);
            refreshDisp--;
        }

        // Draw the FFT magnitudes but restricting it to only 96 pixel screen
        for(i = 0; i < 96; i++)
        {
            // Restricting it to only 96 pixel screen
            if(i < 96)
            {
                // scale the magnitude down and restrict it to only 83 pixels
                scale = (FFT_data[i] >> 6);

                // Check if is out of bounds.
                if(scale < 0)
                {
                    scale = 12;
                }
                else if(scale > 72)
                {
                    scale = 71;
                }

                if(scale != 0)
                {
                    // Draw each line only if scale the mag is 0 pixels
                    Graphics_drawLineV(&g_sContext, i, 83, 83 - scale);
                }
            }
        }

        // Flush this content to display
        Graphics_flushBuffer(&g_sContext);
#endif
    }

    Audio_stopCollect(&gAudioConfig);
    Audio_shutdownCollect(&gAudioConfig);
}

void runFftWithLea(void)
{
    msp_status status;
    uint16_t i;
    int16_t * currentAdcBuffer;

    // Initialize the number of times to refresh the display
    refreshDisp = 2;

    // Clear the screen first
    Graphics_clearDisplay(&g_sContext);

    // Draw the basic structure
    GrStringDraw(&g_sContext, "0",
                 AUTO_STRING_LENGTH, 1, 86,
                 TRANSPARENT_TEXT);
    GrStringDraw(&g_sContext, "Fs 2",
                 AUTO_STRING_LENGTH, 70, 86,
                 TRANSPARENT_TEXT);
    GrLineDrawH(&g_sContext, 0, 96, 84);

    // Draw the title once
    GrStringDraw(&g_sContext, "FFT w/ LEA",
                 AUTO_STRING_LENGTH, 0, 0,
                 GRAPHICS_OPAQUE_TEXT);

    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);

    // Initialize LEA module
    initFft();

    keepAppRunning = true;

    while(keepAppRunning)
    {
        __bis_SR_register(LPM3_bits + GIE);

        // Get the latest buffer pointer to be copied
        currentAdcBuffer = Audio_getActiveBuffer(&gAudioConfig);

        // Copy recorded ADC data to LEA RAM buffer for FFT processing
        for(i = 0; i < (VECTOR_SIZE); i++)
        {
            ((int32_t *)leaMemoryStartAdd.fftDataParam.fftInputOutput)[i] =
                currentAdcBuffer[i] & 0x0000FFFF;
        }

        resetBenchmark();
        startBenchmark();

        // Call fixed scaling fft function.
        status = msp_cmplx_fft_fixed_q15(&complexFftParams,
                                         leaMemoryStartAdd.fftDataParam.fftInputOutput);

        // Check status flag.
        if(status != MSP_SUCCESS)
        {
            // ERROR!
            __no_operation();
        }

        stopBenchmark();

        // Calculate magnitude for the positive frequency domain
        for(i = 0; i < (VECTOR_SIZE / 2); i++)
        {
            real = leaMemoryStartAdd.fftDataParam.fftInputOutput[2 * i];
            imag = leaMemoryStartAdd.fftDataParam.fftInputOutput[2 * i + 1];
            if(real < 0)
            {
                real_abs = ~real + 1;
            }
            else
            {
                real_abs = real;
            }
            if(imag < 0)
            {
                imag_abs = ~imag + 1;
            }
            else
            {
                imag_abs = imag;
            }
            if(real_abs >= imag_abs)
            {
                max = real_abs;
                min = imag_abs;
            }
            else
            {
                max = imag_abs;
                min = real_abs;
            }
            mag = max + 3 * (min >> 3);
            FFT_data[i] = mag;
        }

#ifndef PWR_BENCHMARK
        // Refresh the display
        if(refreshDisp)
        {
            sprintf((char *) str, "Cyc:%lu", cycleCounts);
        }

        // Clear the lines by drawing a box
        Graphics_setForegroundColor(&g_sContext, ClrWhite);
        if(refreshDisp)
        {
            Graphics_fillRectangle(&g_sContext, &clearSpaceCycleText);
        }
        Graphics_fillRectangle(&g_sContext, &clearSpace);
        Graphics_setForegroundColor(&g_sContext, ClrBlack);
        if(refreshDisp)
        {
            Graphics_drawString(&g_sContext, str,
                                AUTO_STRING_LENGTH, 10, 86,
                                GRAPHICS_OPAQUE_TEXT);
            refreshDisp--;
        }

        // Draw the FFT magnitudes but restricting it to only 96 pixel screen
        for(i = 0; i < 96; i++)
        {
            // set the scale and restrict it to only 83 pixels
            scale = ((FFT_data[i]) >> 6);

            // Check if is out of bounds.
            if(scale < 0)
            {
                scale = 12;
            }
            else if(scale > 72)
            {
                scale = 71;
            }

            if(scale != 0)
            {
                // Draw each line only if scale the mag is 0 pixels
                Graphics_drawLineV(&g_sContext, i, 83, 83 - scale);
            }
        }

        // Flush this content to display
        Graphics_flushBuffer(&g_sContext);
#endif
    }
}
