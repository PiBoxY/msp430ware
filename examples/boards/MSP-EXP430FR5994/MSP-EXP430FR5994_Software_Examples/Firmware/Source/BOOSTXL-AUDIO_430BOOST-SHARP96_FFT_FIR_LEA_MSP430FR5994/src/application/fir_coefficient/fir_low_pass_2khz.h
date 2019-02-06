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

 * 0 Hz - 2000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 2300 Hz - 10000 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 */

const _q15 firCoeffsLowPass2kHz[FIR_TAPS] = {
    55,
    146,
    297,
    507,
    746,
    974,
    1127,
    1154,
    1019,
    729,
    329,
    -90,
    -432,
    -607,
    -575,
    -352,
    -23,
    301,
    502,
    509,
    318,
    2,
    -320,
    -518,
    -511,
    -293,
    56,
    400,
    593,
    549,
    268,
    -147,
    -533,
    -721,
    -616,
    -236,
    281,
    728,
    905,
    709,
    181,
    -489,
    -1028,
    -1186,
    -843,
    -75,
    848,
    1548,
    1677,
    1072,
    -157,
    -1611,
    -2706,
    -2852,
    -1656,
    916,
    4482,
    8319,
    11550,
    13396,
    13396,
    11550,
    8319,
    4482,
    916,
    -1656,
    -2852,
    -2706,
    -1611,
    -157,
    1072,
    1677,
    1548,
    848,
    -75,
    -843,
    -1186,
    -1028,
    -489,
    181,
    709,
    905,
    728,
    281,
    -236,
    -616,
    -721,
    -533,
    -147,
    268,
    549,
    593,
    400,
    56,
    -293,
    -511,
    -518,
    -320,
    2,
    318,
    509,
    502,
    301,
    -23,
    -352,
    -575,
    -607,
    -432,
    -90,
    329,
    729,
    1019,
    1154,
    1127,
    974,
    746,
    507,
    297,
    146,
    55
};
