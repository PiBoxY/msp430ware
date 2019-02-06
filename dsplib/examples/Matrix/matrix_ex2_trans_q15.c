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
// Transpose of a real matrix.
//
//! \example matrix_ex2_trans_q15.c
//! This example demonstrates how to use the msp_matrix_trans_q15 API to perform
//! transpose of a real matrix.
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
#define SIGNAL_ROWS         10
#define SIGNAL_COLS         12
#define SIGNAL_FREQUENCY    200
#define SIGNAL_AMPLITUDE    0.6

/* Constants */
#define PI                  3.1415926536

/* Input matrix A */
DSPLIB_DATA(inputA,4)
_q15 inputA[SIGNAL_ROWS][SIGNAL_COLS];

/* Result of the matrix transpose */
DSPLIB_DATA(result,4)
_q15 result[SIGNAL_COLS][SIGNAL_ROWS];

/* Benchmark cycle counts */
volatile uint32_t cycleCount;

/* Function prototypes */
extern void initSignal(void);

void main(void)
{
    msp_status status;
    msp_matrix_trans_q15_params transParams;
    
    /* Disable WDT. */
    WDTCTL = WDTPW + WDTHOLD;

#ifdef __MSP430_HAS_PMM__
    /* Disable GPIO power-on default high-impedance mode for FRAM devices */
    PM5CTL0 &= ~LOCKLPM5;
#endif

    /* Initialize input signal */
    initSignal();
    
    /* Initialize the parameter structure. */
    transParams.rows = SIGNAL_ROWS;
    transParams.cols = SIGNAL_COLS;
    
    /* Invoke the msp_matrix_trans_q15 API. */
    msp_benchmarkStart(MSP_BENCHMARK_BASE, 1);
    status = msp_matrix_trans_q15(&transParams, *inputA, *result);
    cycleCount = msp_benchmarkStop(MSP_BENCHMARK_BASE);
    msp_checkStatus(status);
    
    /* End of program. */
    __no_operation();
}

void initSignal(void)
{
    uint16_t i;
    msp_status status;
    msp_sinusoid_q15_params sinParams;

    /*
     * Generate a set of linearly increasing in frequency sine waves for each
     * row of matrices in Q15 format.
     */
    for (i = 0; i < SIGNAL_ROWS; i++) {
        sinParams.length = SIGNAL_COLS;
        sinParams.amplitude = _Q15(SIGNAL_AMPLITUDE);
        sinParams.cosOmega = _Q15(cosf((float)i*2*PI*SIGNAL_FREQUENCY/FS/(float)SIGNAL_ROWS));
        sinParams.sinOmega = _Q15(sinf((float)i*2*PI*SIGNAL_FREQUENCY/FS/(float)SIGNAL_ROWS));
        status = msp_sinusoid_q15(&sinParams, inputA[i]);
        msp_checkStatus(status);
    }
}
