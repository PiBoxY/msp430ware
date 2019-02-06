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
// Real 32-bit FIR filter.
//
//! \example filter_ex2_fir_iq31.c
//! This example demonstrates how to use the msp_fir_iq31 API and circular
//! buffer feature to filter 32-bit input data. The input signal is composed of
//! two generated sinusoidal signals added together, one sinusoid with a
//! frequency that will pass though the filter and one with a frequency that
//! will be filtered out. The inputs are copied into a circular buffer with
//! twice the length of the filter and allows input history from previous filter
//! operations to be reused without allocating and copying additional samples to
//! the start of the input data. The generated input and result can be compared
//! to see the effect of the filter.
//!
// Brent Peterson, Jeremy Friesenhahn
// Texas Instruments Inc.
// April 2016
//******************************************************************************
#include "msp430.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "DSPLib.h"

/* Include header generated from DSPLib GUI here (optional). */
//#include "myCoeffs_ex2.h"

/* Input signal parameters */
#define FS                  8192
#define SIGNAL_LENGTH       256
#define SIGNAL_FREQUENCY1   200
#define SIGNAL_AMPLITUDE1   0.6
#define SIGNAL_FREQUENCY2   2100
#define SIGNAL_AMPLITUDE2   0.15

/* Constants */
#define PI                  3.1415926536

/* Define example coefficients if no DSPLib GUI header is provided. */
#ifndef FILTER_COEFFS_EX2
/* 41st-order low pass filter coefficients (Fs=8192, Fc=1200) */
const _iq31 FILTER_COEFFS_EX2[42] = {
    _IQ31(+0.000023), _IQ31(-0.001096), _IQ31(-0.001694), _IQ31(-0.000904),
    _IQ31(+0.001577), _IQ31(+0.004256), _IQ31(+0.004037), _IQ31(-0.001064),
    _IQ31(-0.008566), _IQ31(-0.011503), _IQ31(-0.003761), _IQ31(+0.012531),
    _IQ31(+0.024826), _IQ31(+0.017987), _IQ31(-0.011481), _IQ31(-0.046095),
    _IQ31(-0.053380), _IQ31(-0.006793), _IQ31(+0.091888), _IQ31(+0.206279),
    _IQ31(+0.282933), _IQ31(+0.282933), _IQ31(+0.206279), _IQ31(+0.091888),
    _IQ31(-0.006793), _IQ31(-0.053380), _IQ31(-0.046095), _IQ31(-0.011481),
    _IQ31(+0.017987), _IQ31(+0.024826), _IQ31(+0.012531), _IQ31(-0.003761),
    _IQ31(-0.011503), _IQ31(-0.008566), _IQ31(-0.001064), _IQ31(+0.004037),
    _IQ31(+0.004256), _IQ31(+0.001577), _IQ31(-0.000904), _IQ31(-0.001694),
    _IQ31(-0.001096), _IQ31(+0.000023)
};
#endif

/* Filter parameters */
#define FIR_LENGTH      64
#define COEFF_LENTH     sizeof(FILTER_COEFFS_EX2)/sizeof(FILTER_COEFFS_EX2[0])

/*
 * Circular buffer of size 2*FIR_LENGTH, aligned to 8*FIR_LENGTH in order to
 * use the circular buffer feature.
 */
DSPLIB_DATA(circularBuffer,MSP_ALIGN_FIR_IQ31(FIR_LENGTH))
_iq31 circularBuffer[2*FIR_LENGTH];

/* Filter coefficients */
DSPLIB_DATA(filterCoeffs,4)
_iq31 filterCoeffs[COEFF_LENTH];

/* Generated input signal */
DSPLIB_DATA(input,4)
_iq31 input[SIGNAL_LENGTH];

/* Temporary data array for processing */
DSPLIB_DATA(temp,4)
_q15 temp[SIGNAL_LENGTH];

/* Filter result */
DSPLIB_DATA(result,4)
_iq31 result[SIGNAL_LENGTH];

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

/* Function prototypes */
extern void initSignal(void);

void main(void)
{
    uint16_t samples;
    uint16_t copyindex;
    uint16_t filterIndex;
    msp_status status;
    msp_fir_iq31_params firParams;
    msp_fill_iq31_params fillParams;
    msp_copy_iq31_params copyParams;

    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

#ifdef __MSP430_HAS_PMM__
    /* Disable GPIO power-on default high-impedance mode for FRAM devices */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Initialize input signal */
    initSignal();
    
    /* Initialize filter coefficients. */
    memcpy(filterCoeffs, FILTER_COEFFS_EX2, sizeof(filterCoeffs));

    /* Zero initialize FIR input for use with circular buffer. */
    fillParams.length = FIR_LENGTH*2;
    fillParams.value = 0;
    status = msp_fill_iq31(&fillParams, circularBuffer);
    msp_checkStatus(status);

    /* Initialize the copy parameter structure. */
    copyParams.length = FIR_LENGTH;

    /* Initialize the FIR parameter structure. */
    firParams.length = FIR_LENGTH;
    firParams.tapLength = COEFF_LENTH;
    firParams.coeffs = filterCoeffs;
    firParams.enableCircularBuffer = true;

    /* Initialize counters. */
    samples = 0;
    copyindex = 0;
    filterIndex = 2*FIR_LENGTH - COEFF_LENTH;

    /* Run FIR filter with 128 sample circular buffer. */
    while (samples < SIGNAL_LENGTH) {
        /* Copy FIR_LENGTH samples to filter input. */
        status = msp_copy_iq31(&copyParams, &input[samples], &circularBuffer[copyindex]);
        msp_checkStatus(status);

        /* Invoke the msp_fir_iq31 function. */
        msp_benchmarkStart(MSP_BENCHMARK_BASE, 16);
        status = msp_fir_iq31(&firParams, &circularBuffer[filterIndex], &result[samples]);
        cycleCount += msp_benchmarkStop(MSP_BENCHMARK_BASE);
        msp_checkStatus(status);

        /* Update counters. */
        copyindex ^= FIR_LENGTH;
        filterIndex ^= FIR_LENGTH;
        samples += FIR_LENGTH;
    }

    /* End of program. */
    __no_operation();
 }

void initSignal(void)
{
    msp_status status;
    msp_add_q15_params addParams;
    msp_sinusoid_q15_params sinParams;
    msp_q15_to_iq31_params convertParams;

    /* Generate Q15 input signal 1 */
    sinParams.length = SIGNAL_LENGTH;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE1);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY1/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY1/FS));
    status = msp_sinusoid_q15(&sinParams, (_q15 *)input);
    msp_checkStatus(status);

    /* Generate Q15 input signal 2 to temporary array */
    sinParams.length = SIGNAL_LENGTH;
    sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE2);
    sinParams.cosOmega = _Q15(cosf(2*PI*SIGNAL_FREQUENCY2/FS));
    sinParams.sinOmega = _Q15(sinf(2*PI*SIGNAL_FREQUENCY2/FS));
    status = msp_sinusoid_q15(&sinParams, temp);
    msp_checkStatus(status);

    /* Add input signals */
    addParams.length = SIGNAL_LENGTH;
    status = msp_add_q15(&addParams, (_q15 *)input, temp, temp);
    msp_checkStatus(status);

    /* Convert q15 input to iq31 */
    convertParams.length = SIGNAL_LENGTH;
    msp_q15_to_iq31(&convertParams, temp, input);
    msp_checkStatus(status);
}
