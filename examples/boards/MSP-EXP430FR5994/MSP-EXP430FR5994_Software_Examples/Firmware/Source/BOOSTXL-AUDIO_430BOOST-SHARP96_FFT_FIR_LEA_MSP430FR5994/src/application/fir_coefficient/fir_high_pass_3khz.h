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

 * 0 Hz - 2700 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 * 3000 Hz - 10000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 */

const _q15 firCoeffsHighPass3kHz[FIR_TAPS] = {
    349,
    -1361,
    1567,
    15,
    -770,
    -327,
    320,
    461,
    125,
    -256,
    -341,
    -117,
    176,
    291,
    155,
    -95,
    -256,
    -202,
    10,
    212,
    245,
    81,
    -149,
    -268,
    -175,
    63,
    261,
    260,
    49,
    -217,
    -324,
    -176,
    127,
    352,
    310,
    8,
    -330,
    -429,
    -185,
    239,
    514,
    395,
    -69,
    -538,
    -623,
    -195,
    469,
    850,
    573,
    -259,
    -1055,
    -1121,
    -200,
    1217,
    2061,
    1307,
    -1322,
    -5033,
    -8278,
    23204,
    -8278,
    -5033,
    -1322,
    1307,
    2061,
    1217,
    -200,
    -1121,
    -1055,
    -259,
    573,
    850,
    469,
    -195,
    -623,
    -538,
    -69,
    395,
    514,
    239,
    -185,
    -429,
    -330,
    8,
    310,
    352,
    127,
    -176,
    -324,
    -217,
    49,
    260,
    261,
    63,
    -175,
    -268,
    -149,
    81,
    245,
    212,
    10,
    -202,
    -256,
    -95,
    155,
    291,
    176,
    -117,
    -341,
    -256,
    125,
    461,
    320,
    -327,
    -770,
    15,
    1567,
    -1361,
    349,
    0
};
