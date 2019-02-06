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

 * 0 Hz - 800 Hz
   gain = 0
   desired attenuation = -60 dB
   actual attenuation = n/a

 * 1000 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsHighPass1kHz[FIR_TAPS] = {
    -2368,
    1274,
    910,
    605,
    353,
    151,
    -5,
    -122,
    -204,
    -253,
    -274,
    -270,
    -245,
    -201,
    -143,
    -76,
    -4,
    68,
    135,
    193,
    236,
    262,
    267,
    249,
    209,
    149,
    74,
    -12,
    -102,
    -191,
    -269,
    -330,
    -365,
    -370,
    -344,
    -287,
    -197,
    -78,
    57,
    200,
    347,
    473,
    576,
    638,
    650,
    605,
    496,
    321,
    84,
    -209,
    -549,
    -922,
    -1312,
    -1700,
    -2070,
    -2402,
    -2680,
    -2889,
    -3019,
    29705,
    -3019,
    -2889,
    -2680,
    -2402,
    -2070,
    -1700,
    -1312,
    -922,
    -549,
    -209,
    84,
    321,
    496,
    605,
    650,
    638,
    576,
    473,
    347,
    200,
    57,
    -78,
    -197,
    -287,
    -344,
    -370,
    -365,
    -330,
    -269,
    -191,
    -102,
    -12,
    74,
    149,
    209,
    249,
    267,
    262,
    236,
    193,
    135,
    68,
    -4,
    -76,
    -143,
    -201,
    -245,
    -270,
    -274,
    -253,
    -204,
    -122,
    -5,
    151,
    353,
    605,
    910,
    1274,
    -2368,
    0
};
