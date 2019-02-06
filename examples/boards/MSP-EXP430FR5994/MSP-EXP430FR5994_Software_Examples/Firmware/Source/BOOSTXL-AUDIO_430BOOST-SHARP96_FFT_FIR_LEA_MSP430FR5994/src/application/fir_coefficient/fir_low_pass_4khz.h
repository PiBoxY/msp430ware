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

 * 0 Hz - 4000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 4400 Hz - 10000 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 */

const _q15 firCoeffsLowPass4kHz[FIR_TAPS] = {
    55,
    251,
    562,
    806,
    718,
    260,
    -255,
    -401,
    -99,
    265,
    262,
    -75,
    -291,
    -102,
    228,
    236,
    -96,
    -301,
    -73,
    279,
    239,
    -163,
    -353,
    -27,
    371,
    245,
    -266,
    -424,
    46,
    496,
    237,
    -413,
    -503,
    168,
    660,
    197,
    -625,
    -585,
    361,
    871,
    99,
    -933,
    -660,
    683,
    1165,
    -105,
    -1425,
    -724,
    1268,
    1639,
    -562,
    -2396,
    -770,
    2677,
    2791,
    -2031,
    -5839,
    -794,
    12949,
    25161,
    25161,
    12949,
    -794,
    -5839,
    -2031,
    2791,
    2677,
    -770,
    -2396,
    -562,
    1639,
    1268,
    -724,
    -1425,
    -105,
    1165,
    683,
    -660,
    -933,
    99,
    871,
    361,
    -585,
    -625,
    197,
    660,
    168,
    -503,
    -413,
    237,
    496,
    46,
    -424,
    -266,
    245,
    371,
    -27,
    -353,
    -163,
    239,
    279,
    -73,
    -301,
    -96,
    236,
    228,
    -102,
    -291,
    -75,
    262,
    265,
    -99,
    -401,
    -255,
    260,
    718,
    806,
    562,
    251,
    55
};
