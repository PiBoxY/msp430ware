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
#ifndef __HW_MEMMAP__
#define __HW_MEMMAP__

#define __DRIVERLIB_MSP430I2XX_FAMILY__

//*****************************************************************************
//
// Include device specific header file
//
//*****************************************************************************
#if defined(__IAR_SYSTEMS_ICC__)
#if defined(__MSP430i2020__)
#include "../deprecated/IAR/msp430i2020.h"
#elif defined(__MSP430i2021__)
#include "../deprecated/IAR/msp430i2021.h"
#elif defined(__MSP430i2030__)
#include "../deprecated/IAR/msp430i2030.h"
#elif defined(__MSP430i2031__)
#include "../deprecated/IAR/msp430i2031.h"
#elif defined(__MSP430i2040__)
#include "../deprecated/IAR/msp430i2040.h"
#elif defined(__MSP430i2041__)
#include "../deprecated/IAR/msp430i2041.h"
#else
#include <msp430.h>
#endif
#include "../deprecated/IAR/msp430i2xxgeneric.h"

#elif defined(__TI_COMPILER_VERSION__)
#if defined(__MSP430i2020__)
#include "../deprecated/CCS/msp430i2020.h"
#elif defined(__MSP430i2021__)
#include "../deprecated/CCS/msp430i2021.h"
#elif defined(__MSP430i2030__)
#include "../deprecated/CCS/msp430i2030.h"
#elif defined(__MSP430i2031__)
#include "../deprecated/CCS/msp430i2031.h"
#elif defined(__MSP430i2040__)
#include "../deprecated/CCS/msp430i2040.h"
#elif defined(__MSP430i2041__)
#include "../deprecated/CCS/msp430i2041.h"
#else
#include <msp430.h>
#endif
#include "../deprecated/CCS/msp430i2xxgeneric.h"

#elif defined(__GNUC__)
#if defined(__MSP430i2020__)
#include "../deprecated/GCC/msp430i2020.h"
#elif defined(__MSP430i2021__)
#include "../deprecated/GCC/msp430i2021.h"
#elif defined(__MSP430i2030__)
#include "../deprecated/GCC/msp430i2030.h"
#elif defined(__MSP430i2031__)
#include "../deprecated/GCC/msp430i2031.h"
#elif defined(__MSP430i2040__)
#include "../deprecated/GCC/msp430i2040.h"
#elif defined(__MSP430i2041__)
#include "../deprecated/GCC/msp430i2041.h"
#else
#include <msp430.h>
#endif
#include "../deprecated/GCC/msp430i2xxgeneric.h"

#else
#include <msp430.h>
#include "msp430i2xxgeneric.h"
#endif

#include <stdbool.h>
#include <stdint.h>

//*****************************************************************************
//
// SUCCESS and FAILURE for API return value
//
//*****************************************************************************
#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00

#endif // #ifndef __HW_MEMMAP__
