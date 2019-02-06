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

//******************************************************************************
// Includes
//******************************************************************************
#include <stdlib.h>
#include <stdint.h>
#include <driverlib.h>
#include "audio_playback.h"
#include "dac8311.h"
#include "global.h"

//******************************************************************************
// Globals
//******************************************************************************
//int16_t * storeOutput = (int16_t *) 0x10000;

//******************************************************************************
// Functions
//******************************************************************************
/* Function that configures the external DAC and uses an internal timer to
 * periodically wake-up and write data out to the DAC.
 */
void Audio_setupPlayback(Playback_configParams * playbackConfig)
{
    // Initializes the DAC
    DAC8311_init();

    // Enable UART data dump back
    playbackConfig->enableUartDump = true;

    // Check if we need to initialize the UART to 1Mbps
    if(playbackConfig->enableUartDump)
    {
        GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P6,
            GPIO_PIN0,
            GPIO_PRIMARY_MODULE_FUNCTION
            );

        // Configure UART
        EUSCI_A_UART_initParam param = {0};
        param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
        param.clockPrescalar = 8;
        param.firstModReg = 0;
        param.secondModReg = 0;
        param.parity = EUSCI_A_UART_NO_PARITY;
        param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
        param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
        param.uartMode = EUSCI_A_UART_MODE;
        param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

        EUSCI_A_UART_init(EUSCI_A3_BASE, &param);
        EUSCI_A_UART_enable(EUSCI_A3_BASE);
    }

    // Enable the audio amplifier
    GPIO_setOutputLowOnPin(AUDIO_AMP_EN_PORT, AUDIO_AMP_EN_PIN);

    Timer_A_initUpModeParam upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        (__SYSTEM_FREQUENCY_MHZ__ / playbackConfig->sampleRate) - 1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_DO_CLEAR,
        false
    };

    // Initialize Timer_A2 channel 1 to wake-up periodically
    Timer_A_initUpMode(TIMER_A2_BASE, &upConfig);

    // Initialize the flag indicating that the playback has not started
    // and start with audio playback buffer 1
    playbackConfig->playbackStarted = false;
    playbackConfig->playingBuffer1 = true;
    playbackConfig->bufferActive = false;
    // Set the current DAC buffer to point to the first playback buffer
    playbackConfig->currentPlaybackBuffer = playbackConfig->playbackBuffer1;
}

void Audio_startPlayback(Playback_configParams * playbackConfig)
{
    // Start TA0 timer to begin audio data collection
    if(!playbackConfig->playbackStarted)
    {
        Timer_A_clear(TIMER_A2_BASE);
        Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
        // Indicate that the playback has started and no longer need to re-initialize
        playbackConfig->playbackStarted = true;
    }
}

void Audio_stopPlayback(Playback_configParams * playbackConfig)
{
    // Stop the timer
    Timer_A_stop(TIMER_A2_BASE);

    // Set the external DAC to be output high-Z
    DAC8311_setLowPowerMode(DAC8311_OUTPUT_HIGHZ);

    // Indicates that the playback has stopped
    playbackConfig->playbackStarted = false;

    // Disable the audio amplifier
    GPIO_setOutputHighOnPin(AUDIO_AMP_EN_PORT, AUDIO_AMP_EN_PIN);
}

/*--------------------------------------------------------------------------*/
/* Switch buffers collecting audio samples in ping-pong buffers */
void Audio_switchPlaybackBuffer(Playback_configParams * playbackConfig)
{
    // Check if it is using only single or dual buffer
    if(playbackConfig->playbackBuffer2 != 0)
    {
        if(playbackConfig->playingBuffer1)
        {
            playbackConfig->currentPlaybackBuffer =
                playbackConfig->playbackBuffer2;

            playbackConfig->playingBuffer1 = false;
        }
        else
        {
            playbackConfig->currentPlaybackBuffer =
                playbackConfig->playbackBuffer1;

            playbackConfig->playingBuffer1 = true;
        }
    }

    // Most likely an overflow condition has occurred
    if(playbackConfig->bufferActive)
    {
        playbackConfig->overflow = true;
    }
}

/*--------------------------------------------------------------------------*/
/* Get pointer to active buffer with valid data ready for processing */
int16_t * Audio_getEmptyPlaybackBuffer(Playback_configParams * playbackConfig)
{
    // Check if it is using only single or dual buffer
    if(playbackConfig->playbackBuffer2 != 0)
    {
        if(playbackConfig->playingBuffer1)
        {
            return(playbackConfig->playbackBuffer2);
        }
        else
        {
            return(playbackConfig->playbackBuffer1);
        }
    }
    else
    {
        return(playbackConfig->playbackBuffer1);
    }
}

// Timer2_A0 interrupt service routine
#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer2A0Isr(void)
{
    int16_t dacValue =
        gPlaybackConfig.currentPlaybackBuffer[dacBufferPlaybackPointer++];

    dacValue = dacValue + 550;

//    if (storeOutput < (int16_t *) 0x43FFF)
//    {
//        *storeOutput = dacValue;
//        storeOutput++;
//    }
//    else
//    {
//        __no_operation();
//    }

    if(dacValue < 0)
    {
        dacValue = 0;
    }

    DAC8311_updateDacOut(dacValue, gPlaybackConfig.enableUartDump);

    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);
}
