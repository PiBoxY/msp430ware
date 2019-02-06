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
//!  main.c
//!
//!  Description: Record/playback demo application on MSP-EXP430FR5994
//!               using BOOSTXL-AUDIO.
//!
//!               Press the S1 switch to begin recording (3 seconds),
//!               Red LED1 indicating active recording.
//!
//!               Press the S2 switch to playback what was just recorded.
//!               Green LED2 indicates active playback.
//!
//!               NOTE: This demo requires the BOOSTXL-AUDIO boosterpack
//!
//!                  MSP430FR5994               Audio Boosterpack
//!               -----------------  		    -----------------
//!              |                 |           |                 |
//!              |    P5.0/UCB1SIMO|---------> |SPI_SI           |
//!              |                 |           |                 |
//!          /|\ |     P5.2/UCB1CLK|---------> |SPI_CLK          |
//!           |  |                 |           |                 |
//!            --|RST          P8.2|---------> |SYNC             |
//!              |                 |           |                 |
//!              |             P1.3|<--------- | MIC OUT         |
//!              |                 |           |                 |
//!              |             P6.2|---------> | MIC PWR         |
//!              |-----------------|           |-----------------|
//!
//******************************************************************************

#include <driverlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "audio_collect.h"
#include "audio_playback.h"
#include "application.h"
#include "dac8311.h"

void initClock(void);
void initGpio(void);

// Global audio config parameter
Audio_configParams gAudioConfig;

// Global audio playback config parameter
Playback_configParams gPlaybackConfig;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    initClock();
    initGpio();

    PM5CTL0 &= ~LOCKLPM5;           // Clear lock bit

    // Enable Switch interrupt
    GPIO_clearInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);
    GPIO_enableInterrupt(PUSHBUTTON1_PORT, PUSHBUTTON1_PIN);
    GPIO_clearInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);
    GPIO_enableInterrupt(PUSHBUTTON2_PORT, PUSHBUTTON2_PIN);

    // Initialize DAC
	DAC8311_init();

	// Set the DAC to low power mode with output high-Z
	DAC8311_setLowPowerMode(DAC8311_OUTPUT_HIGHZ);

	__bis_SR_register(GIE);

    // Starts the application. It is a function that never returns.
    runApplication();

    return 1;
}

// Initializes the 32kHz crystal and MCLK to 8MHz
void initClock(void)
{
    PJSEL0 |= BIT4 | BIT5;                  // For XT1

    // XT1 Setup
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // set all dividers
    CSCTL4 &= ~LFXTOFF;

    do
    {
        CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);              // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers
}

void initGpio(void)
{
    P1OUT = 0x00;
    P1DIR = 0xFF;

    P2OUT = 0x00;
    P2DIR = 0xFF;

    P3OUT = 0x00;
    P3DIR = 0xFF;

    P4OUT = 0x00;
    P4DIR = 0xFF;

    P5OUT = 0x00;
    P5DIR = 0xFF;

    P6OUT = 0x00;
    P6DIR = 0xFF;

    P7OUT = 0x00;
    P7DIR = 0xFF;

    P8OUT = 0x04;
    P8DIR = 0xFF;

    PJOUT = 0x00;
    PJDIR = 0xFF;

    // Configure Push button switch with high to low transition
    GPIO_setAsInputPinWithPullUpResistor(PUSHBUTTON1_PORT,
                                         PUSHBUTTON1_PIN);

    GPIO_selectInterruptEdge(PUSHBUTTON1_PORT,
                             PUSHBUTTON1_PIN,
                             GPIO_HIGH_TO_LOW_TRANSITION);

    GPIO_setAsInputPinWithPullUpResistor(PUSHBUTTON2_PORT,
                                         PUSHBUTTON2_PIN);

    GPIO_selectInterruptEdge(PUSHBUTTON2_PORT,
                             PUSHBUTTON2_PIN,
                             GPIO_HIGH_TO_LOW_TRANSITION);
}
