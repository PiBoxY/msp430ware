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

 * 0 Hz - 1000 Hz
   gain = 1
   desired ripple = 1 dB
   actual ripple = n/a

 * 1300 Hz - 10000 Hz
   gain = 0
   desired attenuation = -80 dB
   actual attenuation = n/a

 */

const _q15 firCoeffsLowPass1kHz[FIR_TAPS] = {
    -22,
    -10,
    -6,
    5,
    26,
    60,
    108,
    171,
    250,
    342,
    444,
    550,
    654,
    747,
    819,
    863,
    870,
    835,
    755,
    632,
    469,
    278,
    69,
    -140,
    -333,
    -492,
    -601,
    -648,
    -626,
    -533,
    -377,
    -169,
    70,
    318,
    546,
    729,
    840,
    863,
    785,
    605,
    333,
    -10,
    -393,
    -777,
    -1118,
    -1372,
    -1497,
    -1456,
    -1226,
    -797,
    -174,
    622,
    1554,
    2572,
    3615,
    4618,
    5517,
    6252,
    6772,
    7041,
    7041,
    6772,
    6252,
    5517,
    4618,
    3615,
    2572,
    1554,
    622,
    -174,
    -797,
    -1226,
    -1456,
    -1497,
    -1372,
    -1118,
    -777,
    -393,
    -10,
    333,
    605,
    785,
    863,
    840,
    729,
    546,
    318,
    70,
    -169,
    -377,
    -533,
    -626,
    -648,
    -601,
    -492,
    -333,
    -140,
    69,
    278,
    469,
    632,
    755,
    835,
    870,
    863,
    819,
    747,
    654,
    550,
    444,
    342,
    250,
    171,
    108,
    60,
    26,
    5,
    -6,
    -10,
    -22
};
