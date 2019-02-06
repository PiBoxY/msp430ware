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

#ifndef AUDIO_COLLECT_H_
#define AUDIO_COLLECT_H_

/*----------------------------------------------------------------------------
 * Includes
 * -------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

#define MIC_POWER_PORT_DIR  P6DIR
#define MIC_POWER_PORT_OUT  P6OUT
#define MIC_POWER_PIN       BIT2

#define AUDIO_PORT_SEL0     P1SEL0
#define AUDIO_PORT_SEL1     P1SEL1
#define MIC_INPUT_PIN       BIT3

#define MIC_INPUT_CHAN      ADC12INCH_3

/*----------------------------------------------------------------------------
 * Defines and Typedefs
 * -------------------------------------------------------------------------*/
typedef struct Audio_Struct *Audio_Handle;

/* The Audio object structure, containing the Audio instance information */
typedef struct Audio_configParams
{
    /* Size of both audio buffers */
    uint16_t bufferSize;

    /* audio sample rate in Hz */
    uint16_t sampleRate;

} Audio_configParams;

//extern Audio_configParams AudioGlobal;

/*----------------------------------------------------------------------------
 * Functions
 * --------------------------------------------------------------------------*/
/* Set up the device to collect audio samples in ping-pong buffers */
//Audio_Handle Audio_setupCollect(uint32_t *dmaControlTable,
//    int_least16_t *audioBuffer1, int_least16_t *audioBuffer2,
//    uint_least32_t bufferSize, uint_least32_t sampleRate);
extern void runRecord(void);

void Audio_setupCollect(Audio_configParams * audioConfig);

/* Start collecting audio samples in ping-pong buffers */
void Audio_startCollect(Audio_configParams * audioConfig);

/* Stop collecting audio samples in buffers */
void Audio_stopCollect(Audio_configParams * audioConfig);

/* Shut down the audio collection peripherals */
void Audio_shutdownCollect(Audio_configParams * audioConfig);

#endif /* AUDIO_COLLECT_H_ */
