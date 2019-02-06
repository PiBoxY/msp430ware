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
#include "global.h"
#include "fir.h"
#include "fir_coefficients.h"
#include "application.h"
#include "audio_playback.h"
#include "grlib.h"
#include "Sharp96x96.h"

//******************************************************************************
// Globals
//******************************************************************************
// Allocate DSPLib parameter structures
static msp_fir_q15_params firParams;

// Allocating DMA1 to copy data from ADC buffer to LEA
static DMA_initParam dma1Config;

static _q15 * prevPtrInput = (leaMemoryStartAdd.firDataParam.inputData) +
                             ((FIR_SAMPLES - FIR_TAPS));

static void copyFirCoeff(uint32_t * firCoeffsConst);

void initFir(void)
{
    uint16_t ii;

    // Reconfigure the LEA for FIR parameters
    firParams.length = FIR_SAMPLES;
    firParams.tapLength = FIR_TAPS;
    firParams.coeffs = leaMemoryStartAdd.firDataParam.firCoeffs;
    firParams.enableCircularBuffer = false;

    // For safety, protect RMW Cpu instructions
    DMA_disableTransferDuringReadModifyWrite();

    // Prep the previous input data to 0 since this is the first time we're
    // starting
    for(ii = 0; ii < FIR_TAPS; ii++)
    {
        leaMemoryStartAdd.firDataParam.prevInputData[ii] = 0;
    }

    // Initialize the microphone for recording
    gAudioConfig.audioBuffer1 = (int16_t *) adcBuffer0;
    gAudioConfig.audioBuffer2 = (int16_t *) adcBuffer1;
    gAudioConfig.bufferSize = FIR_SAMPLES;
    gAudioConfig.sampleRate = FIR_SAMPLING_FREQUENCY;
    gAudioConfig.twosComplement = false;
    Audio_setupCollect(&gAudioConfig);

    // Reset the playback counter
    dacBufferPlaybackPointer = 0;

    // Initialize the audio playback config. This also reconfigures the
    // SPI configuration
    gPlaybackConfig.playbackBuffer1 = (int16_t *) dacBuffer0;
    gPlaybackConfig.playbackBuffer2 = (int16_t *) dacBuffer1;
    gPlaybackConfig.bufferSize = FIR_SAMPLES;
    gPlaybackConfig.sampleRate = FIR_SAMPLING_FREQUENCY;
    Audio_setupPlayback(&gPlaybackConfig);

    // Start the recording by enabling the timer
    Audio_startCollect(&gAudioConfig);
}

static void copyFirCoeff(uint32_t * firCoeffsConst)
{
    // Using DMA1 to copy the FIR Coefficients to LEA memory first
    dma1Config.channelSelect = DMA_CHANNEL_1;
    dma1Config.transferModeSelect = DMA_TRANSFER_REPEATED_BURSTBLOCK;
    dma1Config.transferSize = FIR_TAPS;
    dma1Config.triggerSourceSelect = (DMA1TSEL__DMAREQ >> 8);
    dma1Config.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    dma1Config.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;

    // Initialize DMA1 with the config
    DMA_init(&dma1Config);

    // Set source address to the FIR coefficient constants in memory
    DMA_setSrcAddress(DMA_CHANNEL_1,
                      (uint32_t) firCoeffsConst,
                      DMA_DIRECTION_INCREMENT);

    // Set the destination to LEA memory
    DMA_setDstAddress(DMA_CHANNEL_1,
                      (uint32_t) leaMemoryStartAdd.firDataParam.firCoeffs,
                      DMA_DIRECTION_INCREMENT);

    // Enable DMA channel 1 interrupt
    DMA_enableInterrupt(DMA_CHANNEL_1);

    // Re-enable the DMA transfer
    DMA_enableTransfers(DMA_CHANNEL_1);

    // Start the data transfer
    DMA_startTransfer(DMA_CHANNEL_1);

    // Enter LPM and wait for transfer to complete
    __bis_SR_register(LPM0_bits + GIE);
}

void runFirFilter(void)
{
    msp_status status;
    uint16_t ii;
    int16_t * currentAdcBuffer;

    keepAppRunning = true;

    // Clear the display first
    Graphics_clearDisplay(&g_sContext);

    // Create text for display
    Graphics_drawStringCentered(&g_sContext, "Real-Time FIR",
                                AUTO_STRING_LENGTH, 48, 30,
                                GRAPHICS_OPAQUE_TEXT);

    Graphics_drawStringCentered(&g_sContext, "Filtering",
                                AUTO_STRING_LENGTH, 48, 40,
                                GRAPHICS_OPAQUE_TEXT);

    switch(firAppMode)
    {
    case FIR_LOW_PASS_1000:
        Graphics_drawStringCentered(&g_sContext, "Low-Pass @ 1kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsLowPass1kHz);
        break;
    case FIR_LOW_PASS_2000:
        Graphics_drawStringCentered(&g_sContext, "Low-Pass @ 2kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsLowPass2kHz);
        break;
    case FIR_LOW_PASS_3000:
        Graphics_drawStringCentered(&g_sContext, "Low-Pass @ 3kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsLowPass3kHz);
        break;
    case FIR_LOW_PASS_4000:
        Graphics_drawStringCentered(&g_sContext, "Low-Pass @ 4kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsLowPass4kHz);
        break;
    case FIR_LOW_PASS_5000:
        Graphics_drawStringCentered(&g_sContext, "Low-Pass @ 5kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsLowPass5kHz);
        break;
    case FIR_HIGH_PASS_1000:
        Graphics_drawStringCentered(&g_sContext, "High-Pass @ 1kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsHighPass1kHz);
        break;
    case FIR_HIGH_PASS_2000:
        Graphics_drawStringCentered(&g_sContext, "High-Pass @ 2kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsHighPass2kHz);
        break;
    case FIR_HIGH_PASS_3000:
        Graphics_drawStringCentered(&g_sContext, "High-Pass @ 3kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsHighPass3kHz);
        break;
    case FIR_HIGH_PASS_4000:
        Graphics_drawStringCentered(&g_sContext, "High-Pass @ 4kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsHighPass4kHz);
        break;
    case FIR_BAND_STOP_1000:
        Graphics_drawStringCentered(&g_sContext, "Band-Stop @ 1kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsBandStop1kHz);
        break;
    case FIR_BAND_STOP_4000:
        Graphics_drawStringCentered(&g_sContext, "Band-Stop @ 4kHz",
                                    AUTO_STRING_LENGTH, 48, 50,
                                    GRAPHICS_OPAQUE_TEXT);
        copyFirCoeff((uint32_t *) firCoeffsBandStop4kHz);
        break;
    default: break;
    }

    // Flush this content to display
    Graphics_flushBuffer(&g_sContext);

    // Initialize FIR configuration
    initFir();

    while(keepAppRunning)
    {
        __bis_SR_register(LPM3_bits + GIE);

        // Reset the pointer to the top of the buffer
        dacBufferPlaybackPointer = 0;

        // Switch the audio playback buffer here
        Audio_switchPlaybackBuffer(&gPlaybackConfig);

        // Start the audio playback here for critical timing reasons
        // as the audio needs to always start from the beginning
        Audio_startPlayback(&gPlaybackConfig);

        // First get the active buffer
        currentAdcBuffer = Audio_getActiveBuffer(&gAudioConfig);

        // First copy the last x sized input samples as the previous history
        // buffer
        for(ii = 0; ii < FIR_TAPS; ii++)
        {
            leaMemoryStartAdd.firDataParam.prevInputData[ii] = prevPtrInput[ii];
        }

        // Now copy the new ADC data into the LEA input buffer
        for(ii = 0; ii < FIR_SAMPLES; ii++)
        {
            leaMemoryStartAdd.firDataParam.inputData[ii] = currentAdcBuffer[ii];
        }

        // Run through a FIR filter
        status = msp_fir_q15(&firParams,
                             leaMemoryStartAdd.firDataParam.prevInputData,
                             leaMemoryStartAdd.firDataParam.result);

        if(status != MSP_SUCCESS)
        {
            P1OUT |= BIT0;
        }

        // Now start copying the result buffer out from LEA memory to a
        // temporary buffer before being written out to the DAC
        int16_t * emptyBuffer = Audio_getEmptyPlaybackBuffer(&gPlaybackConfig);
        for(ii = 0; ii < FIR_SAMPLES; ii++)
        {
            emptyBuffer[ii] = leaMemoryStartAdd.firDataParam.result[ii];
        }
    }

    // Stop the audio playback
    Audio_stopPlayback(&gPlaybackConfig);

    Audio_stopCollect(&gAudioConfig);
    Audio_shutdownCollect(&gAudioConfig);

    // Re-initialize the LCD module
    Sharp96x96_initDisplay();
}
