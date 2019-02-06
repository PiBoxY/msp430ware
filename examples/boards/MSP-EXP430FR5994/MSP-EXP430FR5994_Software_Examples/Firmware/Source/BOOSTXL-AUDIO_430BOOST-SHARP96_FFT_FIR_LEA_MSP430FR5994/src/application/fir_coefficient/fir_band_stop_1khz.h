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

   fixed point precision: 15 bits

 * 0 Hz - 700 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 900 Hz - 1100 Hz
   gain = 0
   desired attenuation = -60 dB
   actual attenuation = n/a

 * 1400 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsBandStop1kHz[FIR_TAPS] = {
    2062,
    -918,
    -636,
    -384,
    -188,
    -64,
    -14,
    -33,
    -101,
    -195,
    -291,
    -367,
    -407,
    -405,
    -363,
    -291,
    -204,
    -118,
    -46,
    2,
    23,
    25,
    9,
    2,
    3,
    27,
    82,
    162,
    253,
    342,
    409,
    435,
    402,
    300,
    133,
    -87,
    -339,
    -593,
    -813,
    -968,
    -1027,
    -972,
    -798,
    -516,
    -149,
    264,
    680,
    1049,
    1324,
    1471,
    1465,
    1300,
    988,
    559,
    59,
    -459,
    -939,
    -1326,
    -1576,
    31105,
    -1576,
    -1326,
    -939,
    -459,
    59,
    559,
    988,
    1300,
    1465,
    1471,
    1324,
    1049,
    680,
    264,
    -149,
    -516,
    -798,
    -972,
    -1027,
    -968,
    -813,
    -593,
    -339,
    -87,
    133,
    300,
    402,
    435,
    409,
    342,
    253,
    162,
    82,
    27,
    3,
    2,
    9,
    25,
    23,
    2,
    -46,
    -118,
    -204,
    -291,
    -363,
    -405,
    -407,
    -367,
    -291,
    -195,
    -101,
    -33,
    -14,
    -64,
    -188,
    -384,
    -636,
    -918,
    2062,
    0
};
