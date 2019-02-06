/* --COPYRIGHT--,BSD
 * Copyright (c) 2018, Texas Instruments Incorporated
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
//
//  functiongenerator.h
//
//  Uses SAC3's DAC to generate a sinusoidal, square, or triangle waveform with 
//  variable frequency and amplitude.
//  
//  SAC1 is configured as a Programmable Gain Amplifier and can be used to manipulate
//  the generated waveform or external signals.
//
//  The final output signal of SAC1 is measured by the internal ADC and transmitted
//  to through the backchannel UART to the computer.
//
//  E. Chen
//  Texas Instruments Inc.
//  May 2018
//******************************************************************************

#ifndef FUNCTIONGENERATOR_H_
#define FUNCTIONGENERATOR_H_

#define FUNCTIONGENERATOR_MODE 1

#define PI                  3.1415926536
#define MAX_SAMPLE_LEN      8192

// Appliation mode
extern char mode;

extern char txString[];

typedef enum {
    SINE_WAVE,
    SQUARE_WAVE,
    SAWTOOTH_WAVE
} SignalType;

extern int signal[];

/* DAC Signal parameters */
extern char inputSignalSource;
extern int signalType;
extern int dacFreq;
extern int sampleLen;
extern int signalFreq;
extern int signalAmp;
extern int opAmpMode;
extern int opAmpGain;
extern int adcFreq;

/* ADC Result */
extern unsigned int functiongenerator_ADC_Result;

void functiongenerator(void);
void functiongenerator_init_GPIO(void);

void init_DAC_Signal(void);
void init_DAC(void);
void init_PGA(void);
void init_ADC(void);

void functiongenerator_enable();
void functiongenerator_disable(void);

extern void transmitString(char *);

#endif /* FUNCTIONGENERATOR_H_ */
