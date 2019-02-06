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
 * FRAMLogMode.h
 *
 * Wakes up every 5 seconds from LPM3 to measure and store its
 * internal temperature sensor & battery monitor data to FRAM
 *
 * October 2017
 * E. Chen
 *
 ******************************************************************************/

#ifndef FRAMLOGMODE_H_
#define FRAMLOGMODE_H_

#include <stdio.h>
#include <string.h>
#include <driverlib.h>
#include <ctpl.h>
#include <nvs.h>
#include "QmathLib.h"

#define FRAM_LOG_MODE 0

// Appliation mode
extern char mode;

// Size of NVS ring storage
#define NVS_RING_SIZE      100

// Define structure to hold timestamp and ADC samples
typedef struct adc_data_t {
    int16_t value;
} adc_data_t;

// ADC data
extern adc_data_t adc_data;

// NVS ring handle
extern nvs_ring_handle nvsHandle;

//extern uint8_t nvsStorage[NVS_RING_STORAGE_SIZE(sizeof(adc_data_t), NVS_RING_SIZE)];

extern bool buttonS1Pressed;
extern bool buttonS2Pressed;
extern bool rtcWakeup;

#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-85 C

#define MAX_STRBUF_SIZE      64

void framLog(void);
extern void transmitString(char *);
extern void initAdc(void);
extern void initEusci(void);

#endif /* FRAMLOGMODE_H_ */
