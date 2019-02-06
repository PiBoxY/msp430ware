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
// Addition of two complex vectors.
//
//! \example vector_ex2_complex_add_q15.c
//! This example demonstrates how to use the msp_cmplx_add_q15 API to perform
//! addition of two complex source vectors.
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
#define SAMPLES             100
#define SIGNAL_FREQUENCY1   200
#define SIGNAL_AMPLITUDE1   0.6
#define SIGNAL_FREQUENCY2   700
#define SIGNAL_AMPLITUDE2   0.15

/* Constants */
#define PI                  3.1415926536

/* Input vector A */
DSPLIB_DATA(inputA,4)
_q15 inputA[SAMPLES*2];

/* Input vector B */
DSPLIB_DATA(inputB,4)
_q15 inputB[SAMPLES*2];

/* Complex real input */
DSPLIB_DATA(cmplxReal,4)
_q15 cmplxReal[SAMPLES];

/* Complex imaginary input */
DSPLIB_DATA(cmplxImag,4)
_q15 cmplxImag[SAMPLES];

/* Result vector */
DSPLIB_DATA(result,4)
_q15 result[SAMPLES*2];

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

/* Function prototypes */
extern void initSignal(void);

void main(void)
{
    msp_status status;
    msp_cmplx_add_q15_params addParams;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

#ifdef __MSP430_HAS_PMM__
    /* Disable GPIO power-on default high-impedance mode for FRAM devices */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Initialize input signal */
    initSignal();
    
    /* Initialize the parameter structure. */
    addParams.length = SAMPLES;
    
    /* Invoke the msp_cmplx_add_q15 API. */
    msp_benchmarkStart(MSP_BENCHMARK_BASE, 1);
    status = msp_cmplx_add_q15(&addParams, inputA, inputB, result);
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

    /* Generate Q15 input signal 1 real component. */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE1);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY1/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY1/FS));
    status = msp_sinusoid_q15(&sinParams, cmplxReal);
    msp_checkStatus(status);

    /* Generate Q15 input signal 1 imaginary component. */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE1);
    sinParams.cosOmega = _Q15(cosf(2*PI*1.5*SIGNAL_FREQUENCY1/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*1.5*SIGNAL_FREQUENCY1/FS));
    status = msp_sinusoid_q15(&sinParams, cmplxImag);
    msp_checkStatus(status);

    /* Combine signal 1 real and imaginary components. */
    cmplxParams.length = SAMPLES;
    msp_cmplx_q15(&cmplxParams, cmplxReal, cmplxImag, inputA);
    msp_checkStatus(status);

    /* Generate Q15 input signal 1 real component. */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE2);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY2/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY2/FS));
    status = msp_sinusoid_q15(&sinParams, cmplxReal);
    msp_checkStatus(status);

    /* Generate Q15 input signal 1 imaginary component. */
    sinParams.length = SAMPLES;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE2);
    sinParams.cosOmega = _Q15(cosf(2*PI*1.5*SIGNAL_FREQUENCY2/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*1.5*SIGNAL_FREQUENCY2/FS));
    status = msp_sinusoid_q15(&sinParams, cmplxImag);
    msp_checkStatus(status);

    /* Combine signal 2 real and imaginary components. */
    cmplxParams.length = SAMPLES;
    msp_cmplx_q15(&cmplxParams, cmplxReal, cmplxImag, inputB);
    msp_checkStatus(status);
}
