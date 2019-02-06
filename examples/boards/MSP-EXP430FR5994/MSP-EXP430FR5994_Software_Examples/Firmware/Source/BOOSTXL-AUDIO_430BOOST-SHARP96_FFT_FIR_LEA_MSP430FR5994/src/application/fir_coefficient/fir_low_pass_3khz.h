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

 * 0 Hz - 3000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 3400 Hz - 10000 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 */

const _q15 firCoeffsLowPass3kHz[FIR_TAPS] = {
    34,
    136,
    308,
    519,
    663,
    643,
    412,
    54,
    -259,
    -358,
    -200,
    90,
    290,
    249,
    2,
    -247,
    -287,
    -75,
    214,
    328,
    153,
    -175,
    -371,
    -244,
    119,
    407,
    347,
    -40,
    -431,
    -463,
    -68,
    434,
    587,
    211,
    -407,
    -717,
    -393,
    341,
    847,
    621,
    -222,
    -972,
    -908,
    32,
    1089,
    1276,
    266,
    -1192,
    -1780,
    -745,
    1277,
    2552,
    1605,
    -1340,
    -4063,
    -3649,
    1379,
    9634,
    17324,
    20453,
    17324,
    9634,
    1379,
    -3649,
    -4063,
    -1340,
    1605,
    2552,
    1277,
    -745,
    -1780,
    -1192,
    266,
    1276,
    1089,
    32,
    -908,
    -972,
    -222,
    621,
    847,
    341,
    -393,
    -717,
    -407,
    211,
    587,
    434,
    -68,
    -463,
    -431,
    -40,
    347,
    407,
    119,
    -244,
    -371,
    -175,
    153,
    328,
    214,
    -75,
    -287,
    -247,
    2,
    249,
    290,
    90,
    -200,
    -358,
    -259,
    54,
    412,
    643,
    663,
    519,
    308,
    136,
    34,
    0
};
