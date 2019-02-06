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

#include "fir.h"

/*

   FIR filter designed with
   http://t-filter.appspot.com

   sampling frequency: 20000 Hz

   fixed point precision: 16 bits

 * 0 Hz - 5000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 5300 Hz - 10000 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 */

const _q15 firCoeffsLowPass5kHz[FIR_TAPS] = {
    41,
    -183,
    -1062,
    -2258,
    -2476,
    -1011,
    813,
    1015,
    -241,
    -840,
    17,
    699,
    70,
    -608,
    -103,
    552,
    110,
    -524,
    -105,
    513,
    93,
    -515,
    -75,
    530,
    54,
    -553,
    -31,
    584,
    5,
    -621,
    25,
    665,
    -62,
    -716,
    105,
    776,
    -155,
    -845,
    215,
    923,
    -289,
    -1017,
    382,
    1133,
    -495,
    -1274,
    641,
    1452,
    -841,
    -1695,
    1120,
    2045,
    -1542,
    -2593,
    2271,
    3621,
    -3813,
    -6256,
    9448,
    29874,
    29874,
    9448,
    -6256,
    -3813,
    3621,
    2271,
    -2593,
    -1542,
    2045,
    1120,
    -1695,
    -841,
    1452,
    641,
    -1274,
    -495,
    1133,
    382,
    -1017,
    -289,
    923,
    215,
    -845,
    -155,
    776,
    105,
    -716,
    -62,
    665,
    25,
    -621,
    5,
    584,
    -31,
    -553,
    54,
    530,
    -75,
    -515,
    93,
    513,
    -105,
    -524,
    110,
    552,
    -103,
    -608,
    70,
    699,
    17,
    -840,
    -241,
    1015,
    813,
    -1011,
    -2476,
    -2258,
    -1062,
    -183,
    41
};
