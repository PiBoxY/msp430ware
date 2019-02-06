/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
 * FRAMLogMode.c
 *
 * MSP430FR5969 wakes up every 5 seconds from LPM3.5 to measure and store its
 * internal temperature sensor & battery monitor data to FRAM
 *
 * June 2014
 * E. Chen
 *
 ******************************************************************************/

#ifndef OUTOFBOX_FR5969_NEWD_FRAMLOGMODE_H_
#define OUTOFBOX_FR5969_NEWD_FRAMLOGMODE_H_

#include <stdint.h>

#define FRAM_LOG_MODE      '2'
#define TRANSMIT_DATA_MODE '3'

#define FRAM_TEMP_INDEX          0xF000          // FRAM location for Temp data index
#define FRAM_TEMP_RESULTS        0x9000          // FRAM location for Temp Sensor results
#define FRAM_BAT_RESULTS         0xC000          // FRAM location for Battery Monitor results
//#define FRAM_TEMP_RESULTS_END    0xFF7F          // Address following the storage area
#define FRAM_TIME_STAMP          0xF002          // FRAM location for Temp data index

extern uint16_t dataArray[];
extern uint8_t timeStamp[];

void framLog(void);
void transmitFRAMData(void);
void storeTimeStamp(void);

#endif /* OUTOFBOX_FR5969_NEWD_FRAMLOGMODE_H_ */
