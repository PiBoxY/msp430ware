/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
// Complex FFT with fixed scaling.
//
//! \example transform_ex2_cmplx_fft_auto_q15.c
//! This example demonstrates how to use the msp_fft_iq31 API to transform a
//! real input data array to the frequency domain using the Q15 fast fourier
//! transform (FFT) without scaling. The real FFT function is identical to
//! running a complex FFT with imaginary components set to zero. The result of
//! the real FFT is a complex vector with half the samples but identical byte
//! length.
//!
// Brent Peterson, Jeremy Friesenhahn
// Texas Instruments Inc.
// April 2016
//******************************************************************************
#include "msp430.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "DSPLib.h"

/* Input signal parameters */
#define FS                  8192
#define SAMPLES             256
#define SIGNAL_FREQUENCY1   200
#define SIGNAL_AMPLITUDE1   0.6
#define SIGNAL_FREQUENCY2   2100
#define SIGNAL_AMPLITUDE2   0.15

/* Hamming window parameters */
#define HAMMING_ALPHA       0.53836
#define HAMMING_BETA        0.46164

/* Constants */
#define PI                  3.1415926536

/* Generated Hamming window function */
DSPLIB_DATA(window,4)
_q15 window[SAMPLES];

/* Input signal and FFT result */
DSPLIB_DATA(input,MSP_ALIGN_CMPLX_FFT_Q15(SAMPLES))
_q15 input[SAMPLES*2];

/* Temporary data array for processing */
DSPLIB_DATA(temp,4)
_q15 temp[SAMPLES*2];

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

/* Function prototypes */
extern void initSignal(void);
extern void initHamming(void);

void main(void)
{
    uint16_t shift;
    msp_status status;
    msp_cmplx_fft_q15_params fftParams;
    msp_cmplx_mpy_real_q15_params mpyParams;

    /* Disable WDT */
    WDTCTL = WDTPW + WDTHOLD;

#ifdef __MSP430_HAS_PMM__
    /* Disable GPIO power-on default high-impedance mode for FRAM devices */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Initialize input signal and Hamming window */
    initSignal();
    initHamming();

    /* Multiply input signal by generated Hamming window */
    mpyParams.length = SAMPLES;
    status = msp_cmplx_mpy_real_q15(&mpyParams, input, window, input);
    msp_checkStatus(status);

    /* Initialize the fft parameter structure. */
    fftParams.length = SAMPLES;
    fftParams.bitReverse = 1;
    fftParams.twiddleTable = msp_cmplx_twiddle_table_256_q15;

    /* Perform complex FFT with auto scaling */
    msp_benchmarkStart(MSP_BENCHMARK_BASE, 16);
    status = msp_cmplx_fft_auto_q15(&fftParams, input, &shift);
    cycleCount = msp_benchmarkStop(MSP_BENCHMARK_BASE);
    msp_checkStatus(status);
    
    /* End of program. */
    __no_operation();
}

void initSignal(void)
{
    msp_status status;
    msp_cmplx_q15_params cmplxParams;
    msp_sinusoid_q15_params sinParams;

    /* Generate Q15 input signal 1 */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE1);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY1/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY1/FS));
    status = msp_sinusoid_q15(&sinParams, temp);
    msp_checkStatus(status);

    /* Generate Q15 input signal 2 to temporary array */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE2);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY2/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY2/FS));
    status = msp_sinusoid_q15(&sinParams, &temp[SAMPLES]);
    msp_checkStatus(status);

    /* Combine signal 1 and 2 as real and imaginary respectively. */
    cmplxParams.length = SAMPLES;
    msp_cmplx_q15(&cmplxParams, temp, &temp[SAMPLES], input);
    msp_checkStatus(status);
}

void initHamming(void)
{
    msp_status status;
    msp_sub_q15_params subParams;
    msp_copy_q15_params copyParams;
    msp_fill_q15_params fillParams;
    msp_sinusoid_q15_params sinParams;

    /* Generate sinusoid for cosine function */
    sinParams.length = 3*SAMPLES/2;
    sinParams.amplitude = _Q15(HAMMING_BETA);
    sinParams.cosOmega = _Q15(cosf(2*PI/(SAMPLES-1)));
    sinParams.sinOmega = _Q15(sinf(2*PI/(SAMPLES-1)));
    status = msp_sinusoid_q15(&sinParams, temp);
    msp_checkStatus(status);

    /* Shift sinusoid by pi/2 to create cosine function */
    copyParams.length = SAMPLES;
    status = msp_copy_q15(&copyParams, &temp[SAMPLES/4], &temp[0]);
    msp_checkStatus(status);

    /* Fill temporary array with alpha constant */
    fillParams.length = SAMPLES;
    fillParams.value = _Q15(HAMMING_ALPHA);
    status = msp_fill_q15(&fillParams, window);
    msp_checkStatus(status);

    /* Subtract generated cosine from alpha constant to generate final window */
    subParams.length = SAMPLES;
    status = msp_sub_q15(&subParams, window, temp, window);
    msp_checkStatus(status);
}
