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

//******************************************************************************
// Globals
//******************************************************************************
static DMA_initParam dma0Config;

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

    // Start with primary (first) audio buffer transfer
    audioConfig->fillingBuffer1 = true;
    audioConfig->overflow = false;
    audioConfig->bufferActive = false;

    // Initialize the DMA. Using DMA channel 0.
    dma0Config.channelSelect = DMA_CHANNEL_0;
    dma0Config.transferModeSelect = DMA_TRANSFER_SINGLE;
    dma0Config.transferSize = audioConfig->bufferSize;
    dma0Config.triggerSourceSelect = DMA0TSEL__ADC12IFG;
    dma0Config.transferUnitSelect = DMA_SIZE_SRCWORD_DSTWORD;
    dma0Config.triggerTypeSelect = DMA_TRIGGER_RISINGEDGE;

    DMA_init(&dma0Config);

    // For safety, protect RMW Cpu instructions
    DMA_disableTransferDuringReadModifyWrite();

    DMA_setSrcAddress(DMA_CHANNEL_0,
                      (uint32_t) &ADC12MEM0,
                      DMA_DIRECTION_UNCHANGED);

    DMA_setDstAddress(DMA_CHANNEL_0,
                      (uint32_t) audioConfig->audioBuffer1,
                      DMA_DIRECTION_INCREMENT);

    // Configure ADC
    ADC12CTL0 &= ~ADC12ENC;           // Disable conversions to configure ADC12
    // Turn on ADC, sample 32 clock cycles =~ 2us
    ADC12CTL0 = ADC12ON | ADC12SHT0_3;

    // Use sample timer, rpt single chan 0, use MODOSC, TA0 timer channel 1
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_2 | ADC12SHS_1;

    // Use 2s-complement
    if(audioConfig->twosComplement)
    {
        ADC12CTL2 |= ADC12DF;
    }
    else
    {
        ADC12CTL2 &= ~ADC12DF;
    }

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
    // Start with primary (first) audio buffer transfer
//    audioConfig->fillingBuffer1 = true;
//    audioConfig->overflow = false;
//    audioConfig->bufferActive = false;

    // Enable DMA channel 0 interrupt
    DMA_enableInterrupt(DMA_CHANNEL_0);

    // Enable the DMA0 to start receiving triggers when ADC sample available
    DMA_enableTransfers(DMA_CHANNEL_0);

    // Start TA0 timer to begin audio data collection
    Timer_A_clear(TIMER_A0_BASE);
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

/*--------------------------------------------------------------------------*/
/* Switch buffers collecting audio samples in ping-pong buffers */
void Audio_switchBuffer(Audio_configParams * audioConfig)
{
    // Check if it is using only single or dual buffer
    if(audioConfig->audioBuffer2 != 0)
    {
        if(audioConfig->fillingBuffer1)
        {
            DMA_setDstAddress(DMA_CHANNEL_0,
                              (uint32_t) audioConfig->audioBuffer2,
                              DMA_DIRECTION_INCREMENT);
            audioConfig->fillingBuffer1 = false;
        }
        else
        {
            DMA_setDstAddress(DMA_CHANNEL_0,
                              (uint32_t) audioConfig->audioBuffer1,
                              DMA_DIRECTION_INCREMENT);
            audioConfig->fillingBuffer1 = true;
        }
    }

    // Most likely an overflow condition has occurred
    if(audioConfig->bufferActive)
    {
        audioConfig->overflow = true;
    }

    // Enable the DMA0 to start receiving triggers when ADC sample available
    DMA_enableTransfers(DMA_CHANNEL_0);
}

/*--------------------------------------------------------------------------*/
/* Stop collecting audio samples in buffers */
void Audio_stopCollect(Audio_configParams * audioConfig)
{
    Timer_A_stop(TIMER_A0_BASE);

    ADC12_B_disableConversions(ADC12_B_BASE, ADC12_B_COMPLETECONVERSION);

    // Disable DMA channel and interrupt
    DMA_disableTransfers(DMA_CHANNEL_0);
    DMA_disableInterrupt(DMA_CHANNEL_0);
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

/*--------------------------------------------------------------------------*/
/* Get pointer to active buffer with valid data ready for processing */
int16_t * Audio_getActiveBuffer(Audio_configParams * audioConfig)
{
    // Check if it is using only single or dual buffer
    if(audioConfig->audioBuffer2 != 0)
    {
        if(audioConfig->fillingBuffer1)
        {
            return(audioConfig->audioBuffer2);
        }
        else
        {
            return(audioConfig->audioBuffer1);
        }
    }
    else
    {
        return(audioConfig->audioBuffer1);
    }
}

/*--------------------------------------------------------------------------*/
/* Get pointer to the current buffer where data is currently written */
int16_t * Audio_getCurrentBuffer(Audio_configParams * audioConfig)
{
    // Check if it is using only single or dual buffer
    if(audioConfig->audioBuffer2 != 0)
    {
        if(audioConfig->fillingBuffer1)
        {
            return(audioConfig->audioBuffer1);
        }
        else
        {
            return(audioConfig->audioBuffer2);
        }
    }
    else
    {
        return(audioConfig->audioBuffer1);
    }
}

/*--------------------------------------------------------------------------*/
/* Indicate if an active buffer exists to be processed */
bool Audio_getActive(Audio_configParams * audioConfig)
{
    return(audioConfig->bufferActive);
}

/*--------------------------------------------------------------------------*/
/* Indicate done processing active buffer holding valid data */
void Audio_resetActive(Audio_configParams * audioConfig)
{
    audioConfig->bufferActive = false;
}

/*--------------------------------------------------------------------------*/
/* Get overflow status */
bool Audio_getOverflow(Audio_configParams * audioConfig)
{
    return(audioConfig->overflow);
}

/*--------------------------------------------------------------------------*/
/* Reset overflow status */
void Audio_resetOverflow(Audio_configParams * audioConfig)
{
    audioConfig->overflow = false;
}
