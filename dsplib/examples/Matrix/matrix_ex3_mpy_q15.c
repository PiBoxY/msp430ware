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
// Multiplication of two real matrices.
//
//! \example matrix_ex3_mpy_q15.c
//! This example demonstrates how to use the msp_matrix_mpy_q15 API to perform
//! multiplication of two real source matrices.
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
#define SIGNAL_ROWS1        8
#define SIGNAL_COLS1        10
#define SIGNAL_FREQUENCY1   200
#define SIGNAL_AMPLITUDE1   0.6
#define SIGNAL_ROWS2        10
#define SIGNAL_COLS2        12
#define SIGNAL_FREQUENCY2   2100
#define SIGNAL_AMPLITUDE2   0.15

/* Constants */
#define PI                  3.1415926536

/* Input matrix A */
DSPLIB_DATA(inputA,4)
_q15 inputA[SIGNAL_ROWS1][SIGNAL_COLS1];

/* Input matrix B */
DSPLIB_DATA(inputB,4)
_q15 inputB[SIGNAL_ROWS2][SIGNAL_COLS2];

/* Result of the matrix add */
DSPLIB_DATA(result,4)
_q15 result[SIGNAL_ROWS1][SIGNAL_COLS2];

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

/* Function prototypes */
extern void initSignals(void);

void main(void)
{
    msp_status status;
    msp_matrix_mpy_q15_params mpyParams;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

#ifdef __MSP430_HAS_PMM__
    /* Disable GPIO power-on default high-impedance mode for FRAM devices */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Initialize input signals */
    initSignals();
    
    /* Initialize the parameter structure. */
    mpyParams.srcARows = SIGNAL_ROWS1;
    mpyParams.srcACols = SIGNAL_COLS1;
    mpyParams.srcBRows = SIGNAL_ROWS2;
    mpyParams.srcBCols = SIGNAL_COLS2;
    
    /* Invoke the msp_matrix_mpy_q15 API. */
    msp_benchmarkStart(MSP_BENCHMARK_BASE, 1);
    status = msp_matrix_mpy_q15(&mpyParams, *inputA, *inputB, *result);
    cycleCount = msp_benchmarkStop(MSP_BENCHMARK_BASE);
    msp_checkStatus(status);
    
    /* End of program. */
    __no_operation();
}

void initSignals(void)
{
    uint16_t i;
    msp_status status;
    msp_sinusoid_q15_params sinParams;

    /* Generate Q15 input signal 1 */
    for (i = 0; i < SIGNAL_ROWS1; i++) {
        sinParams.length = SIGNAL_COLS1;
        sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE1);
        sinParams.cosOmega = _Q15(cosf((float)i*2*PI*SIGNAL_FREQUENCY1/FS/(float)SIGNAL_ROWS1));
        sinParams.sinOmega = _Q15(sinf((float)i*2*PI*SIGNAL_FREQUENCY1/FS/(float)SIGNAL_ROWS1));
        status = msp_sinusoid_q15(&sinParams, inputA[i]);
        msp_checkStatus(status);
    }

    /* Generate Q15 input signal 2 */
    for (i = 0; i < SIGNAL_ROWS2; i++) {
        sinParams.length = SIGNAL_COLS2;
        sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE2);
        sinParams.cosOmega = _Q15(cosf((float)i*2*PI*SIGNAL_FREQUENCY2/FS/(float)SIGNAL_ROWS2));
        sinParams.sinOmega = _Q15(sinf((float)i*2*PI*SIGNAL_FREQUENCY2/FS/(float)SIGNAL_ROWS2));
        status = msp_sinusoid_q15(&sinParams, inputB[i]);
        msp_checkStatus(status);
    }
}
