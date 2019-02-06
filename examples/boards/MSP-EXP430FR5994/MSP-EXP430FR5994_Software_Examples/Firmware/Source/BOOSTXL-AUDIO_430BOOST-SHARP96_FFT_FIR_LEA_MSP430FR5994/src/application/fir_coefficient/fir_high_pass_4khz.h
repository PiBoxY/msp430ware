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

 * 0 Hz - 3700 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 * 4000 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsHighPass4kHz[FIR_TAPS] = {
    161,
    -803,
    1473,
    -1040,
    -283,
    655,
    207,
    -435,
    -267,
    256,
    318,
    -78,
    -313,
    -91,
    235,
    217,
    -94,
    -264,
    -72,
    214,
    210,
    -79,
    -266,
    -95,
    211,
    241,
    -58,
    -293,
    -140,
    217,
    298,
    -26,
    -342,
    -207,
    226,
    386,
    19,
    -411,
    -309,
    239,
    514,
    94,
    -512,
    -471,
    249,
    722,
    229,
    -684,
    -770,
    259,
    1139,
    532,
    -1067,
    -1544,
    264,
    2542,
    1820,
    -3274,
    -9832,
    19925,
    -9832,
    -3274,
    1820,
    2542,
    264,
    -1544,
    -1067,
    532,
    1139,
    259,
    -770,
    -684,
    229,
    722,
    249,
    -471,
    -512,
    94,
    514,
    239,
    -309,
    -411,
    19,
    386,
    226,
    -207,
    -342,
    -26,
    298,
    217,
    -140,
    -293,
    -58,
    241,
    211,
    -95,
    -266,
    -79,
    210,
    214,
    -72,
    -264,
    -94,
    217,
    235,
    -91,
    -313,
    -78,
    318,
    256,
    -267,
    -435,
    207,
    655,
    -283,
    -1040,
    1473,
    -803,
    161,
    0
};
