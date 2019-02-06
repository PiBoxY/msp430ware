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
uint16_t pointer;                           // Global pointer to indicate what is the current value to
                                            // be written out to DAC
uint8_t buffer;                             // There are 2 memory buffers in this code.
                                            // The 2 buffers are used because the compiler limits each
                                            // array to only 16-bits in size.
                                            //    Buffer 1 - 0x10000 to 0x17FFF
                                            //    Buffer 2 - 0x18000 to 0x1FFFF
uint8_t playing = 0;

void runPlayback(void)
{
	pointer = 0;

	buffer = 0;

	P1OUT |= BIT1;

	// For safety, protect RMW Cpu instructions
	DMA_disableTransferDuringReadModifyWrite();

	// Reset the playback counter
	dacBufferPlaybackPointer = 0;

	// Initialize the audio playback config. This also reconfigures the
	// SPI configuration
	gPlaybackConfig.bufferSize = SAMPLES_LENGTH;
	gPlaybackConfig.sampleRate = SAMPLING_FREQUENCY;
	Audio_setupPlayback(&gPlaybackConfig);

	// Start the audio playback here for critical timing reasons
	// as the audio needs to always start from the beginning
	Audio_startPlayback(&gPlaybackConfig);

	//Control loop
	while(playing){
		__bis_SR_register(LPM0_bits + GIE);
	}
}

//******************************************************************************
// Functions
//******************************************************************************
/* Function that configures the external DAC and uses an internal timer to
 * periodically wake-up and write data out to the DAC.
 */
void Audio_setupPlayback(Playback_configParams * playbackConfig)
{

    // Enable the audio amplifier
    GPIO_setOutputLowOnPin(AUDIO_AMP_EN_PORT, AUDIO_AMP_EN_PIN);

    // 50us delay for DAC8311 to wake-up from low power mode
	__delay_cycles(__SYSTEM_FREQUENCY_MHZ__ * 50e-6);

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
}

void Audio_startPlayback(Playback_configParams * playbackConfig)
{
    // Start TA0 timer to begin audio data collection
    if (!playbackConfig->playbackStarted)
    {
        Timer_A_clear(TIMER_A2_BASE);
        Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
        // Indicate that the playback has started and no longer need to re-initialize
        playbackConfig->playbackStarted = true;

        //Indicates playback has begun and to remain in control loop until finished
        playing = 1;
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

    //Indicates when playback has finished to control loop
    playing = 0;

    // Disable the audio amplifier
    GPIO_setOutputHighOnPin(AUDIO_AMP_EN_PORT, AUDIO_AMP_EN_PIN);
}

// Timer2_A0 interrupt service routine
#pragma vector = TIMER2_A0_VECTOR
__interrupt void timer2A0Isr (void)
{
	uint16_t dacValue;

	if (buffer == 0)
	{
		dacValue = __data20_read_short(&dataRecorded1[pointer++]);
		if (pointer > SAMPLES_LENGTH)
		{
			// Switch playback to buffer 2
			buffer = 1;

			// Reset the pointer
			pointer = 0;
		}
	}
	else
	{
		dacValue = __data20_read_short(&dataRecorded2[pointer++]);
		if (pointer > SAMPLES_LENGTH)
		{
			// Switch playback to buffer 1
			buffer = 0;

		    // Stop the audio playback
		    Audio_stopPlayback(&gPlaybackConfig);

			// Set the DAC8311 to low power mode
			DAC8311_setLowPowerMode(DAC8311_OUTPUT_HIGHZ);

			// Disable LED2
			P1OUT &= ~(BIT1);

			__bic_SR_register_on_exit(LPM0_bits);
		}
	}

    DAC8311_updateDacOut(dacValue);
}
