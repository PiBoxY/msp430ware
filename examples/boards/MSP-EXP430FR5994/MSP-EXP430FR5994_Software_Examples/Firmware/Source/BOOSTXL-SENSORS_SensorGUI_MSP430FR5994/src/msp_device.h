/*
****************************************************************************
PROJECT : MSP430
FILE    : msp_device.h
============================================================================ 
DESCRIPTION
Device/compiler specific definitions and include files
============================================================================

Copyright (c) 2007-2012 Texas Instruments Incorporated.  All rights reserved.
Software License Agreement

Texas Instruments (TI) is supplying this software for use solely and
exclusively on TI's microcontroller products. The software is owned by
TI and/or its suppliers, and is protected under applicable copyright
laws. You may not combine this software with "viral" open-source
software in order to form a larger program.

THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, FOR ANY REASON WHATSOEVER.

****************************************************************************
*/

#ifndef __MSP_DEVICE_H__
#define __MSP_DEVICE_H__

#define COMP_CCS     1
#define COMP_IAR     2

#if defined (__IAR_SYSTEMS_ASM__)
    #define COMPILER COMP_IAR
#elif defined (__IAR_SYSTEMS_ICC__)
    #define COMPILER COMP_IAR
#else /*CCS */
    #define COMPILER COMP_CCS
#endif

//***** Definitions *****


#endif /* __MSP_DEVICE_H__ */
