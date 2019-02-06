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

 * 0 Hz - 1700 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 * 2000 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsHighPass2kHz[FIR_TAPS] = {
    645,
    -1891,
    956,
    889,
    121,
    -416,
    -519,
    -308,
    8,
    252,
    339,
    269,
    96,
    -94,
    -229,
    -269,
    -206,
    -68,
    89,
    211,
    258,
    211,
    87,
    -71,
    -209,
    -278,
    -250,
    -129,
    47,
    217,
    319,
    313,
    192,
    -11,
    -227,
    -379,
    -405,
    -284,
    -45,
    238,
    466,
    546,
    431,
    138,
    -250,
    -602,
    -780,
    -691,
    -317,
    259,
    865,
    1282,
    1302,
    791,
    -264,
    -1735,
    -3374,
    -4868,
    -5913,
    26480,
    -5913,
    -4868,
    -3374,
    -1735,
    -264,
    791,
    1302,
    1282,
    865,
    259,
    -317,
    -691,
    -780,
    -602,
    -250,
    138,
    431,
    546,
    466,
    238,
    -45,
    -284,
    -405,
    -379,
    -227,
    -11,
    192,
    313,
    319,
    217,
    47,
    -129,
    -250,
    -278,
    -209,
    -71,
    87,
    211,
    258,
    211,
    89,
    -68,
    -206,
    -269,
    -229,
    -94,
    96,
    269,
    339,
    252,
    8,
    -308,
    -519,
    -416,
    121,
    889,
    956,
    -1891,
    645,
    0
};
