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
//*****************************************************************************
// CAPT_UserConfig.c
//
// Example User Configuration file for the CAPTIVATE-PHONE demonstration.
//
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//
//*****************************************************************************

#include "CAPT_UserConfig.h"

//*****************************************************************************
//
//! Captivate Element Definitions
//! All elements in this application are defined below.
//! Each element has 3 components:
//!  1) a raw count array (One index per freq. scanned) (uint16_t)
//!  2) a tuning array (One index per freq. scanned) (tCaptivateElementTuning)
//!  3) a element structure (tElement)
//
//*****************************************************************************
// Sensor: generalPurposeSliderSensor, Element: E00
uint16_t generalPurposeSliderSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning generalPurposeSliderSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement generalPurposeSliderSensor_E00 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 10,
    .pRawCount = generalPurposeSliderSensor_E00_RawCnts,
    .pTuning = generalPurposeSliderSensor_E00_Tuning,
};

// Sensor: generalPurposeSliderSensor, Element: E01
uint16_t generalPurposeSliderSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning generalPurposeSliderSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement generalPurposeSliderSensor_E01 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 11,
    .pRawCount = generalPurposeSliderSensor_E01_RawCnts,
    .pTuning = generalPurposeSliderSensor_E01_Tuning,
};

// Sensor: generalPurposeSliderSensor, Element: E02
uint16_t generalPurposeSliderSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning generalPurposeSliderSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement generalPurposeSliderSensor_E02 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 11,
    .pRawCount = generalPurposeSliderSensor_E02_RawCnts,
    .pTuning = generalPurposeSliderSensor_E02_Tuning,
};

// Sensor: generalPurposeSliderSensor, Element: E03
uint16_t generalPurposeSliderSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning generalPurposeSliderSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement generalPurposeSliderSensor_E03 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 11,
    .pRawCount = generalPurposeSliderSensor_E03_RawCnts,
    .pTuning = generalPurposeSliderSensor_E03_Tuning,
};

// Sensor: modeKeypadSensor, Element: E00
uint16_t modeKeypadSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E00 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 17,
    .pRawCount = modeKeypadSensor_E00_RawCnts,
    .pTuning = modeKeypadSensor_E00_Tuning,
};

// Sensor: modeKeypadSensor, Element: E01
uint16_t modeKeypadSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E01 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 19,
    .pRawCount = modeKeypadSensor_E01_RawCnts,
    .pTuning = modeKeypadSensor_E01_Tuning,
};

// Sensor: modeKeypadSensor, Element: E02
uint16_t modeKeypadSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E02 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 10,
    .pRawCount = modeKeypadSensor_E02_RawCnts,
    .pTuning = modeKeypadSensor_E02_Tuning,
};

// Sensor: modeKeypadSensor, Element: E03
uint16_t modeKeypadSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning modeKeypadSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement modeKeypadSensor_E03 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 2,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 15,
    .pRawCount = modeKeypadSensor_E03_RawCnts,
    .pTuning = modeKeypadSensor_E03_Tuning,
};

// Sensor: numericKeypadSensor, Element: E00
uint16_t numericKeypadSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E00 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 9,
    .pRawCount = numericKeypadSensor_E00_RawCnts,
    .pTuning = numericKeypadSensor_E00_Tuning,
};

// Sensor: numericKeypadSensor, Element: E01
uint16_t numericKeypadSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E01 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 17,
    .pRawCount = numericKeypadSensor_E01_RawCnts,
    .pTuning = numericKeypadSensor_E01_Tuning,
};

// Sensor: numericKeypadSensor, Element: E02
uint16_t numericKeypadSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E02 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 22,
    .pRawCount = numericKeypadSensor_E02_RawCnts,
    .pTuning = numericKeypadSensor_E02_Tuning,
};

// Sensor: numericKeypadSensor, Element: E03
uint16_t numericKeypadSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E03 =
{
    .ui8TxPin = 1,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 16,
    .pRawCount = numericKeypadSensor_E03_RawCnts,
    .pTuning = numericKeypadSensor_E03_Tuning,
};

// Sensor: numericKeypadSensor, Element: E04
uint16_t numericKeypadSensor_E04_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E04_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E04 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 15,
    .pRawCount = numericKeypadSensor_E04_RawCnts,
    .pTuning = numericKeypadSensor_E04_Tuning,
};

// Sensor: numericKeypadSensor, Element: E05
uint16_t numericKeypadSensor_E05_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E05_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E05 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 15,
    .pRawCount = numericKeypadSensor_E05_RawCnts,
    .pTuning = numericKeypadSensor_E05_Tuning,
};

// Sensor: numericKeypadSensor, Element: E06
uint16_t numericKeypadSensor_E06_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E06_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E06 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 19,
    .pRawCount = numericKeypadSensor_E06_RawCnts,
    .pTuning = numericKeypadSensor_E06_Tuning,
};

// Sensor: numericKeypadSensor, Element: E07
uint16_t numericKeypadSensor_E07_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E07_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E07 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 15,
    .pRawCount = numericKeypadSensor_E07_RawCnts,
    .pTuning = numericKeypadSensor_E07_Tuning,
};

// Sensor: numericKeypadSensor, Element: E08
uint16_t numericKeypadSensor_E08_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E08_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E08 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 13,
    .pRawCount = numericKeypadSensor_E08_RawCnts,
    .pTuning = numericKeypadSensor_E08_Tuning,
};

// Sensor: numericKeypadSensor, Element: E09
uint16_t numericKeypadSensor_E09_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E09_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E09 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 8,
    .pRawCount = numericKeypadSensor_E09_RawCnts,
    .pTuning = numericKeypadSensor_E09_Tuning,
};

// Sensor: numericKeypadSensor, Element: E10
uint16_t numericKeypadSensor_E10_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E10_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E10 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 18,
    .pRawCount = numericKeypadSensor_E10_RawCnts,
    .pTuning = numericKeypadSensor_E10_Tuning,
};

// Sensor: numericKeypadSensor, Element: E11
uint16_t numericKeypadSensor_E11_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning numericKeypadSensor_E11_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement numericKeypadSensor_E11 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 0,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 15,
    .pRawCount = numericKeypadSensor_E11_RawCnts,
    .pTuning = numericKeypadSensor_E11_Tuning,
};

// Sensor: proxAndGuardSensor, Element: E00
uint16_t proxAndGuardSensor_E00_RawCnts[CAPT_SELF_FREQ_CNT];
tCaptivateElementTuning proxAndGuardSensor_E00_Tuning[CAPT_SELF_FREQ_CNT];
tElement proxAndGuardSensor_E00 =
{
    .ui8RxPin = 3,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 5,
    .pRawCount = proxAndGuardSensor_E00_RawCnts,
    .pTuning = proxAndGuardSensor_E00_Tuning,
};

// Sensor: scrollWheelSensor, Element: E00
uint16_t scrollWheelSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning scrollWheelSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement scrollWheelSensor_E00 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 10,
    .pRawCount = scrollWheelSensor_E00_RawCnts,
    .pTuning = scrollWheelSensor_E00_Tuning,
};

// Sensor: scrollWheelSensor, Element: E01
uint16_t scrollWheelSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning scrollWheelSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement scrollWheelSensor_E01 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 9,
    .pRawCount = scrollWheelSensor_E01_RawCnts,
    .pTuning = scrollWheelSensor_E01_Tuning,
};

// Sensor: scrollWheelSensor, Element: E02
uint16_t scrollWheelSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning scrollWheelSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement scrollWheelSensor_E02 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 9,
    .pRawCount = scrollWheelSensor_E02_RawCnts,
    .pTuning = scrollWheelSensor_E02_Tuning,
};

// Sensor: volumeSliderSensor, Element: E00
uint16_t volumeSliderSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning volumeSliderSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement volumeSliderSensor_E00 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 0,
    .ui8TouchThreshold = 12,
    .pRawCount = volumeSliderSensor_E00_RawCnts,
    .pTuning = volumeSliderSensor_E00_Tuning,
};

// Sensor: volumeSliderSensor, Element: E01
uint16_t volumeSliderSensor_E01_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning volumeSliderSensor_E01_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement volumeSliderSensor_E01 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 1,
    .ui8TouchThreshold = 13,
    .pRawCount = volumeSliderSensor_E01_RawCnts,
    .pTuning = volumeSliderSensor_E01_Tuning,
};

// Sensor: volumeSliderSensor, Element: E02
uint16_t volumeSliderSensor_E02_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning volumeSliderSensor_E02_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement volumeSliderSensor_E02 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 13,
    .pRawCount = volumeSliderSensor_E02_RawCnts,
    .pTuning = volumeSliderSensor_E02_Tuning,
};

// Sensor: volumeSliderSensor, Element: E03
uint16_t volumeSliderSensor_E03_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning volumeSliderSensor_E03_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement volumeSliderSensor_E03 =
{
    .ui8TxPin = 3,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 3,
    .ui8TouchThreshold = 14,
    .pRawCount = volumeSliderSensor_E03_RawCnts,
    .pTuning = volumeSliderSensor_E03_Tuning,
};

// Sensor: wheelButtonSensor, Element: E00
uint16_t wheelButtonSensor_E00_RawCnts[CAPT_MUTUAL_FREQ_CNT];
tCaptivateElementTuning wheelButtonSensor_E00_Tuning[CAPT_MUTUAL_FREQ_CNT];
tElement wheelButtonSensor_E00 =
{
    .ui8TxPin = 2,
    .ui8TxBlock = 1,
    .ui8RxPin = 0,
    .ui8RxBlock = 2,
    .ui8TouchThreshold = 14,
    .pRawCount = wheelButtonSensor_E00_RawCnts,
    .pTuning = wheelButtonSensor_E00_Tuning,
};


//*****************************************************************************
//
//! Captivate Time Cycle Definitions
//! All time cycles in this application are defined below.  Time cycles are
//! groups of elements that are measured together in parallel in one time slot.
//! Each cycle has 2 components:
//!  1) an element pointer array to the member elements (tElement*)
//!  2) a cycle structure (tCycle)
//
//*****************************************************************************
// Time Cycle: generalPurposeSliderSensor_C00
tElement* generalPurposeSliderSensor_C00_Elements[4] =
{
    &generalPurposeSliderSensor_E00,
    &generalPurposeSliderSensor_E01,
    &generalPurposeSliderSensor_E02,
    &generalPurposeSliderSensor_E03,
};
tCycle generalPurposeSliderSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = generalPurposeSliderSensor_C00_Elements,
};

// Time Cycle: modeKeypadSensor_C00
tElement* modeKeypadSensor_C00_Elements[4] =
{
    &modeKeypadSensor_E00,
    &modeKeypadSensor_E01,
    &modeKeypadSensor_E02,
    &modeKeypadSensor_E03,
};
tCycle modeKeypadSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = modeKeypadSensor_C00_Elements,
};

// Time Cycle: numericKeypadSensor_C00
tElement* numericKeypadSensor_C00_Elements[4] =
{
    &numericKeypadSensor_E00,
    &numericKeypadSensor_E01,
    &numericKeypadSensor_E02,
    &numericKeypadSensor_E03,
};
tCycle numericKeypadSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = numericKeypadSensor_C00_Elements,
};

// Time Cycle: numericKeypadSensor_C01
tElement* numericKeypadSensor_C01_Elements[4] =
{
    &numericKeypadSensor_E04,
    &numericKeypadSensor_E05,
    &numericKeypadSensor_E06,
    &numericKeypadSensor_E07,
};
tCycle numericKeypadSensor_C01 =
{
    .ui8NrOfElements = 4,
    .pElements = numericKeypadSensor_C01_Elements,
};

// Time Cycle: numericKeypadSensor_C02
tElement* numericKeypadSensor_C02_Elements[4] =
{
    &numericKeypadSensor_E08,
    &numericKeypadSensor_E09,
    &numericKeypadSensor_E10,
    &numericKeypadSensor_E11,
};
tCycle numericKeypadSensor_C02 =
{
    .ui8NrOfElements = 4,
    .pElements = numericKeypadSensor_C02_Elements,
};

// Time Cycle: proxAndGuardSensor_C00
tElement* proxAndGuardSensor_C00_Elements[1] =
{
    &proxAndGuardSensor_E00,
};
tCycle proxAndGuardSensor_C00 =
{
    .ui8NrOfElements = 1,
    .pElements = proxAndGuardSensor_C00_Elements,
};

// Time Cycle: scrollWheelSensor_C00
tElement* scrollWheelSensor_C00_Elements[3] =
{
    &scrollWheelSensor_E00,
    &scrollWheelSensor_E01,
    &scrollWheelSensor_E02,
};
tCycle scrollWheelSensor_C00 =
{
    .ui8NrOfElements = 3,
    .pElements = scrollWheelSensor_C00_Elements,
};

// Time Cycle: volumeSliderSensor_C00
tElement* volumeSliderSensor_C00_Elements[4] =
{
    &volumeSliderSensor_E00,
    &volumeSliderSensor_E01,
    &volumeSliderSensor_E02,
    &volumeSliderSensor_E03,
};
tCycle volumeSliderSensor_C00 =
{
    .ui8NrOfElements = 4,
    .pElements = volumeSliderSensor_C00_Elements,
};

// Time Cycle: wheelButtonSensor_C00
tElement* wheelButtonSensor_C00_Elements[1] =
{
    &wheelButtonSensor_E00,
};
tCycle wheelButtonSensor_C00 =
{
    .ui8NrOfElements = 1,
    .pElements = wheelButtonSensor_C00_Elements,
};


//*****************************************************************************
//
//! Captivate Sensor Definitions
//! All sensors in this application are defined below.  Sensors are
//! groups of time cycles that utilize raw measurement data to create an
//! abstract sensor type, such as a button, slider, wheel, or prox sensor.
//! Each sensor has 3 components:
//!  1) a cycle pointer array to the member time cycles (tCycle*)
//!  2) a sensor-specific parameter structure (tGenericSensorParams)
//!  3) a sensor structure (tSensor)
//
//*****************************************************************************
//Sensor: generalPurposeSliderSensor
const tCycle* generalPurposeSliderSensor_Cycles[1] =
{
    &generalPurposeSliderSensor_C00,
};

tSliderSensorParams generalPurposeSliderSensor_Params =
{
    .ui8TotalElements = 4,
    .ui16Resolution = 100,
    .SliderBeta = 120,
    .SliderLower = 3,
    .SliderUpper = 95,
    .SliderAlgorithm = eVectors,
    .SliderFilterEnable = true,
};

tSensor generalPurposeSliderSensor =
{
    // Basic Properties
    .TypeOfSensor = eSlider,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = generalPurposeSliderSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&generalPurposeSliderSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 600,
    .ui16ConversionGain = 150,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 3960,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = false,
    .ui8CntBeta = 0,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: modeKeypadSensor
const tCycle* modeKeypadSensor_Cycles[1] =
{
    &modeKeypadSensor_C00,
};

tButtonSensorParams modeKeypadSensor_Params;
tSensor modeKeypadSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = modeKeypadSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&modeKeypadSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 150,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 1980,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 1,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: numericKeypadSensor
const tCycle* numericKeypadSensor_Cycles[3] =
{
    &numericKeypadSensor_C00,
    &numericKeypadSensor_C01,
    &numericKeypadSensor_C02,
};

tButtonSensorParams numericKeypadSensor_Params;
tSensor numericKeypadSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 3,
    .pCycle = numericKeypadSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&numericKeypadSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 150,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 7,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 1980,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 1,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: proxAndGuardSensor
const tCycle* proxAndGuardSensor_Cycles[1] =
{
    &proxAndGuardSensor_C00,
};

tProxSensorParams proxAndGuardSensor_Params =
{
    .pSensor = NULL,
    .ui8NumberOfSensors = 0,
};

tSensor proxAndGuardSensor =
{
    // Basic Properties
    .TypeOfSensor = eProx,
    .SensingMethod = eSelf,
    .DirectionOfInterest = eDOIDown,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = proxAndGuardSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&proxAndGuardSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 800,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 3,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 2000,
    .ui16TimeoutThreshold = 7920,
    .ProxDbThreshold.DbIn = 1,
    .ProxDbThreshold.DbOut = 3,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 15,
    .bCountFilterEnable = true,
    .ui8CntBeta = 2,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: scrollWheelSensor
const tCycle* scrollWheelSensor_Cycles[1] =
{
    &scrollWheelSensor_C00,
};

tWheelSensorParams scrollWheelSensor_Params =
{
    .ui8TotalElements = 3,
    .ui16Resolution = 100,
    .SliderBeta = 100,
    .SliderUpper = 100,
    .SliderLower = 0,
    .SliderAlgorithm = eVectors,
    .SliderFilterEnable = true,
};

tSensor scrollWheelSensor =
{
    // Basic Properties
    .TypeOfSensor = eWheel,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = scrollWheelSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&scrollWheelSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 600,
    .ui16ConversionGain = 150,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 3960,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = false,
    .ui8CntBeta = 0,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: volumeSliderSensor
const tCycle* volumeSliderSensor_Cycles[1] =
{
    &volumeSliderSensor_C00,
};

tSliderSensorParams volumeSliderSensor_Params =
{
    .ui8TotalElements = 4,
    .ui16Resolution = 100,
    .SliderBeta = 120,
    .SliderLower = 4,
    .SliderUpper = 95,
    .SliderAlgorithm = eVectors,
    .SliderFilterEnable = true,
};

tSensor volumeSliderSensor =
{
    // Basic Properties
    .TypeOfSensor = eSlider,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = volumeSliderSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&volumeSliderSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 600,
    .ui16ConversionGain = 150,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 10,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 3960,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 0,
    .TouchDbThreshold.DbOut = 0,
    .bCountFilterEnable = false,
    .ui8CntBeta = 0,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};

//Sensor: wheelButtonSensor
const tCycle* wheelButtonSensor_Cycles[1] =
{
    &wheelButtonSensor_C00,
};

tButtonSensorParams wheelButtonSensor_Params;
tSensor wheelButtonSensor =
{
    // Basic Properties
    .TypeOfSensor = eButtonGroup,
    .SensingMethod = eProjected,
    .DirectionOfInterest = eDOIUp,
    .pvCallback = NULL,
    .ui8NrOfCycles = 1,
    .pCycle = wheelButtonSensor_Cycles,
    .pSensorParams = (tGenericSensorParams*)&wheelButtonSensor_Params,
    // Conversion Control Parameters
    .ui16ConversionCount = 150,
    .ui16ConversionGain = 100,
    .ui8FreqDiv = 1,
    .ui8ChargeLength = 0,
    .ui8TransferLength = 0,
    .bModEnable = false,
    .ui8BiasControl = 3,
    .bCsDischarge = true,
    .bLpmControl = false,
    .ui8InputSyncControl = 0,
    .bTimerSyncControl = false,
    .bIdleState = true,
    // Tuning  Parameters
    .ui16ProxThreshold = 8,
    .ui16NegativeTouchThreshold = 20,
    .ui16ErrorThreshold = 1000,
    .ui16TimeoutThreshold = 1980,
    .ProxDbThreshold.DbIn = 2,
    .ProxDbThreshold.DbOut = 1,
    .TouchDbThreshold.DbIn = 2,
    .TouchDbThreshold.DbOut = 1,
    .bCountFilterEnable = true,
    .ui8CntBeta = 1,
    .bSensorHalt = false,
    .bPTSensorHalt = true,
    .bPTElementHalt = true,
    .ui8LTABeta = 7,
    .bReCalibrateEnable = true,
};


#if (CAPT_CONDUCTED_NOISE_IMMUNITY_ENABLE==true)
//*****************************************************************************
//
//! \var g_EMCConfig
//! This structure stores the EMC configuration for this application.
//
//*****************************************************************************
const tEMCConfig g_EMCConfig =
{
	// Conversion Style
	.selfModeConversionStyle = eMultiFrequency,
	.projModeConversionStyle = eMultiFrequencyWithOutlierRemoval,
	
	// Oversampling Style
	.selfModeOversamplingStyle = eNoOversampling,
	.projModeOversamplingStyle = eNoOversampling,
	
	// Jitter Filter Enable
	.bJitterFilterEnable = true,
	
	// Noise Thresholds and Calibration Noise Limits
	.ui8NoiseThreshold = 20,
	.ui16CalibrationNoiseLimit = 10,
	.ui8CalibrationTestSampleSize = 8,
		
	// Dynamic Threshold Adjustment Parameters
	.bEnableDynamicThresholdAdjustment = true,
	.ui8MaxRelThreshAdj = 76,
	.ui8NoiseLevelFilterEntryThresh = 40,
	.ui8NoiseLevelFilterExitThresh = 0,
	.ui8NoiseLevelFilterDown = 6,
	.ui8NoiseLevelFilterUp = 1,
	.coeffA = _IQ31(0.0065),
	.coeffB = _IQ31(0.050)
};
#endif

//*****************************************************************************
//
//! \var g_pCaptivateSensorArray
//! This array allows for indexed access to any
//! sensor in the configuration.
//
//*****************************************************************************
tSensor* g_pCaptivateSensorArray[CAPT_SENSOR_COUNT] =
{
    &generalPurposeSliderSensor,
    &modeKeypadSensor,
    &numericKeypadSensor,
    &proxAndGuardSensor,
    &scrollWheelSensor,
    &volumeSliderSensor,
    &wheelButtonSensor,
};


//*****************************************************************************
//
//! \var g_uiApp
//! This structure stores the global settings for this application.
//
//*****************************************************************************
tCaptivateApplication g_uiApp =
{
    .state = eUIActive,
	.pSensorList = &g_pCaptivateSensorArray[0],
    .ui8NrOfSensors = CAPT_SENSOR_COUNT,
    .ui8AppLPM = LPM0_bits,
    .bElementDataTxEnable = true,
    .bSensorDataTxEnable = true,
    .ui16ActiveModeScanPeriod = 33,
    .ui16WakeOnProxModeScanPeriod = 100,
    .ui16InactivityTimeout = 32,
    .ui8WakeupInterval = 5,
};
