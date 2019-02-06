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

 * 0 Hz - 3600 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 3900 Hz - 4100 Hz
   gain = 0
   desired attenuation = -60 dB
   actual attenuation = n/a

 * 4400 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsBandStop4kHz[FIR_TAPS] = {
    -294,
    823,
    21,
    268,
    447,
    136,
    -273,
    -266,
    95,
    303,
    92,
    -230,
    -225,
    83,
    261,
    78,
    -193,
    -182,
    64,
    189,
    52,
    -114,
    -90,
    24,
    40,
    -1,
    37,
    77,
    -46,
    -204,
    -81,
    264,
    317,
    -142,
    -531,
    -187,
    550,
    612,
    -257,
    -911,
    -305,
    862,
    924,
    -376,
    -1291,
    -421,
    1158,
    1211,
    -482,
    -1617,
    -516,
    1391,
    1428,
    -558,
    -1836,
    -576,
    1526,
    1538,
    -591,
    30854,
    -591,
    1538,
    1526,
    -576,
    -1836,
    -558,
    1428,
    1391,
    -516,
    -1617,
    -482,
    1211,
    1158,
    -421,
    -1291,
    -376,
    924,
    862,
    -305,
    -911,
    -257,
    612,
    550,
    -187,
    -531,
    -142,
    317,
    264,
    -81,
    -204,
    -46,
    77,
    37,
    -1,
    40,
    24,
    -90,
    -114,
    52,
    189,
    64,
    -182,
    -193,
    78,
    261,
    83,
    -225,
    -230,
    92,
    303,
    95,
    -266,
    -273,
    136,
    447,
    268,
    21,
    823,
    -294
};
