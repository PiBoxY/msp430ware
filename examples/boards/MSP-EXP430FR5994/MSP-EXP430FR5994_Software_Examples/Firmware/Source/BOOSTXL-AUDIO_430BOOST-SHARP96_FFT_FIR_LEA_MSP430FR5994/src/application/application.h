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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "global.h"
#include "DSPLib.h"
#include "FFT.h"
#include "fir.h"

// Uncomment this to perform power benchmark without the audio boosterpack and
// graphics LCd boosterpack
//#define PWR_BENCHMARK

#define MAXIMUM_ARRAY_SIZE              512
#define FFT_SAMPLING_FREQUENCY          8000
#define FIR_SAMPLING_FREQUENCY          20000

#define PUSHBUTTON1_PORT                GPIO_PORT_P5
#define PUSHBUTTON1_PIN                 GPIO_PIN6
#define PUSHBUTTON2_PORT                GPIO_PORT_P5
#define PUSHBUTTON2_PIN                 GPIO_PIN5

typedef enum
{
    FFT_WITH_LEA = 0,
    FFT_WITHOUT_LEA,
    RECORD_AND_FIR
} applicationMode_t;

typedef enum
{
    FIR_LOW_PASS_1000 = 0,
    FIR_LOW_PASS_2000,
    FIR_LOW_PASS_3000,
    FIR_LOW_PASS_4000,
    FIR_LOW_PASS_5000,
    FIR_HIGH_PASS_1000,
    FIR_HIGH_PASS_2000,
    FIR_HIGH_PASS_3000,
    FIR_HIGH_PASS_4000,
    FIR_BAND_STOP_1000,
    FIR_BAND_STOP_4000
} firApplicationMode_t;

extern firApplicationMode_t firAppMode;

typedef union
{
    struct FftData_s
    {
        _q15 fftInputOutput[VECTOR_SIZE * 2];
    } fftDataParam;
    struct FirData_s
    {
        _q15 reserved[2];
        _q15 prevInputData[FIR_TAPS];
        _q15 inputData[FIR_SAMPLES];
        _q15 result[FIR_SAMPLES];
        _q15 firCoeffs[FIR_TAPS];
    } firDataParam;
} LeaMemoryStartAdd;

extern void runApplication(void);
extern LeaMemoryStartAdd leaMemoryStartAdd;
extern int16_t adcBuffer0[MAXIMUM_ARRAY_SIZE];
extern int16_t adcBuffer1[MAXIMUM_ARRAY_SIZE];
extern int16_t dacBuffer0[MAXIMUM_ARRAY_SIZE];
extern int16_t dacBuffer1[MAXIMUM_ARRAY_SIZE];
extern uint16_t dacBufferPlaybackPointer;

#endif /* APPLICATION_H_ */
