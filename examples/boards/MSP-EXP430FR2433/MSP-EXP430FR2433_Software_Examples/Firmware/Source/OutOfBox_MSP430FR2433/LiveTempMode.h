/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
/*******************************************************************************
 *
 * LiveTempMode.h
 *
 * Continuously sample and convert internal ADC Temperature Sensor values and
 * transmit values through USB Backchannel UART.
 *
 * October 2017
 * E. Chen
 *
 ******************************************************************************/

#ifndef LIVETEMPMODE_H_
#define LIVETEMPMODE_H_

#include <stdio.h>
#include <string.h>
#include <driverlib.h>
#include "QmathLib.h"

#define LIVE_TEMP_MODE 1

// Appliation mode
extern char mode;

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-85 C

#define MAX_STRBUF_SIZE      64

extern _q8 threshold;
extern bool thresholdChanged;

void liveTemp(void);
extern void transmitString(char *);
extern void initAdc(void);
extern void initEusci(void);
void initTimerPWM(void);

#endif /* LIVETEMPMODE_H_ */
