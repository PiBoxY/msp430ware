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
#include "audio_collect.h"
#include "global.h"
#include "dac8311.h"

#pragma LOCATION(dataRecorded1, 0x10000);
#pragma PERSISTENT(dataRecorded1);
uint16_t dataRecorded1[SAMPLES_LENGTH] = {0};

#pragma LOCATION(dataRecorded2, 0x20000);
#pragma PERSISTENT(dataRecorded2);
uint16_t dataRecorded2[SAMPLES_LENGTH] = {0};

//******************************************************************************
// Globals
//******************************************************************************
static DMA_initParam dma0Config;
static DMA_initParam dma1Config;

void runRecord(void)
{
	uint16_t recording = 0;

	P1OUT |= BIT0;

	// Initialize the microphone for recording
	gAudioConfig.bufferSize = SAMPLES_LENGTH;
	gAudioConfig.sampleRate = SAMPLING_FREQUENCY;
	Audio_setupCollect(&gAudioConfig);

	// Start the recording by enabling the timer
	Audio_startCollect(&gAudioConfig);

	recording = 1;

	while (recording){
		__bis_SR_register(LPM4_bits + GIE);

		recording = 0;
	}
}

//******************************************************************************
// Functions
//******************************************************************************
/* Function that powers up the external microphone and starts sampling
 * the microphone output.
 * The ADC is triggered to sample using the Timer module
 * Then the data is moved via DMA. The device would only wake-up once
 * the DMA is done. */
void Audio_setupCollect(Audio_configParams * audioConfig)
{
    Timer_A_initUpModeParam upConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        (__SYSTEM_FREQUENCY_MHZ__ / audioConfig->sampleRate) - 1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_DO_CLEAR,
        false
    };

    Timer_A_initCompareModeParam compareConfig =
    {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_TOGGLE_RESET,
        ((__SYSTEM_FREQUENCY_MHZ__ / audioConfig->sampleRate) / 2) - 1
    };

    // Turn on mic power full drive strength and enable mic input pin to ADC
    MIC_POWER_PORT_OUT |= MIC_POWER_PIN;
    MIC_POWER_PORT_DIR |= MIC_POWER_PIN;

    AUDIO_PORT_SEL0 |= MIC_INPUT_PIN;
    AUDIO_PORT_SEL1 |= MIC_INPUT_PIN;

    // For safety, protect RMW Cpu instructions
    DMA_disableTransferDuringReadModifyWrite();

    // Initialize the DMA. Using DMA channel 1.
    dma0Config.channelSelect = DMA_CHANNEL_1;
    dma0Config.transferModeSelect = DMA_TRANSFER_SINGLE;
    dma0Config.transferSize = SAMPLES_LENGTH;
    dma0Config.triggerSourceSelect = DMA_TRIGGERSOURCE_26;
    dma0Config.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    dma0Config.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;


    // Initialize the DMA. Using DMA channel 2.
    dma1Config.channelSelect = DMA_CHANNEL_2;
    dma1Config.transferModeSelect = DMA_TRANSFER_SINGLE;
    dma1Config.transferSize = SAMPLES_LENGTH;
    dma1Config.triggerSourceSelect = DMA_TRIGGERSOURCE_26;
    dma1Config.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    dma1Config.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;

	DMA_init(&dma0Config);
    DMA_init(&dma1Config);

    DMA_setSrcAddress(DMA_CHANNEL_1,
                      (uint32_t) &ADC12MEM0,
                      DMA_DIRECTION_UNCHANGED);

    DMA_setDstAddress(DMA_CHANNEL_1,
                      (uint32_t) (&dataRecorded1),
                      DMA_DIRECTION_INCREMENT);

    DMA_setSrcAddress(DMA_CHANNEL_2,
                      (uint32_t) &ADC12MEM0,
                      DMA_DIRECTION_UNCHANGED);

    DMA_setDstAddress(DMA_CHANNEL_2,
                      (uint32_t) (&dataRecorded2),
                      DMA_DIRECTION_INCREMENT);

    // Configure ADC
    ADC12CTL0 &= ~ADC12ENC;           // Disable conversions to configure ADC12
    // Turn on ADC, sample 32 clock cycles =~ 2us
    ADC12CTL0 = ADC12ON + ADC12SHT0_3;

    // Use sample timer, rpt single chan 0, use MODOSC, TA0 timer channel 1
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SHS_1;

    // Mic input to ADC, (AVCC/AVSS ref), sequence end bit set
    ADC12MCTL0 = MIC_INPUT_CHAN | ADC12EOS;

    // Initialize Timer_A channel 1 to be used as ADC12 trigger
    // Initialize TACCR0 (period register) 8MHz/20kHz = 400
    // Simple counter with no interrupt. 0...400 = 400 counts/sample
    Timer_A_initUpMode(TIMER_A0_BASE, &upConfig);

    // Initialize TA0CCR1 to generate trigger clock output, reset/set mode
    Timer_A_initCompareMode(TIMER_A0_BASE, &compareConfig);

    // Enable ADC to convert when a TA0 edge is generated
    ADC12CTL0 |= ADC12ENC;

    // Delay for the microphone to settle
    __delay_cycles(10000);
}

/*--------------------------------------------------------------------------*/
/* Start collecting audio samples in ping-pong buffers */
void Audio_startCollect(Audio_configParams * audioConfig)
{
    // Enable DMA channel 1 interrupt
    DMA_enableInterrupt(DMA_CHANNEL_1);

    // Enable the DMA0 to start receiving triggers when ADC sample available
    DMA_enableTransfers(DMA_CHANNEL_1);

    // Start TA0 timer to begin audio data collection
    Timer_A_clear(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

/*--------------------------------------------------------------------------*/
/* Stop collecting audio samples in buffers */
void Audio_stopCollect(Audio_configParams * audioConfig)
{
    Timer_A_stop(TIMER_A0_BASE);

    ADC12_B_disableConversions(ADC12_B_BASE, ADC12_B_COMPLETECONVERSION);

    // Disable DMA channel and interrupt
    DMA_disableTransfers(DMA_CHANNEL_1);
    DMA_disableInterrupt(DMA_CHANNEL_1);

    // Disable DMA channel and interrupt
    DMA_disableTransfers(DMA_CHANNEL_2);
    DMA_disableInterrupt(DMA_CHANNEL_2);
}

/*--------------------------------------------------------------------------*/
/* Shut down the audio collection peripherals*/
void Audio_shutdownCollect(Audio_configParams * audioConfig)
{
    // Turn off preamp power
    MIC_POWER_PORT_OUT &= ~MIC_POWER_PIN;

    // Disable the ADC
    ADC12_B_disable(ADC12_B_BASE);
}
