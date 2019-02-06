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

#ifndef AUDIO_PLAYBACK_H_
#define AUDIO_PLAYBACK_H_

#define AUDIO_AMP_EN_PORT   GPIO_PORT_P8
#define AUDIO_AMP_EN_PIN    GPIO_PIN3

/* The Audio object structure, containing the Audio instance information */
typedef struct Playback_configParams
{
    /* Ping-pong audio playback buffers
     * To disable ping-pong bufferts, define playbackBuffer2 = 0
     */
    int16_t *playbackBuffer1;
    int16_t *playbackBuffer2;

    /* Pointer to either playbackBuffer1 or playbackBuffer2 */
    int16_t *currentPlaybackBuffer;

    /* Size of both audio playback buffers */
    uint16_t bufferSize;

    /* audio sample rate in Hz */
    uint16_t sampleRate;

    /* Flag indicating it is playbackBuffer1 is currently being played */
    bool playingBuffer1;

    /* Flag indicating it has a filled buffer, and is filling the other */
    bool bufferActive;

    /* Flag indicating the playback has started */
    bool playbackStarted;

    /* Flag indicating that the channel has not processed data in real time */
    bool overflow;

    /* Flag to enable UART data dump of playback data */
    bool enableUartDump;
} Playback_configParams;

void Audio_setupPlayback(Playback_configParams * playbackConfig);
void Audio_startPlayback(Playback_configParams * playbackConfig);
void Audio_stopPlayback(Playback_configParams * playbackConfig);
void Audio_switchPlaybackBuffer(Playback_configParams * playbackConfig);
int16_t * Audio_getEmptyPlaybackBuffer(Playback_configParams * playbackConfig);

#endif
