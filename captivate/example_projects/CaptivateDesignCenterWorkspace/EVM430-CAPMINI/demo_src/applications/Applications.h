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
//#############################################################################
//
//! \file   Applications.h
//
//  Group:          MSP
//  Target Devices: MSP430FR2512
//
//  (C) Copyright 2018, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Definitions
//*****************************************************************************

#ifndef APPLICATIONS_APPLICATIONS_H_
#define APPLICATIONS_APPLICATIONS_H_

// define keys value
#define Key1    0x0001
#define Key2    0x0002
#define Key3    0x0004
#define Key4    0x0008

// define led on/off
#define LED1_ON (P2OUT |= BIT6)
#define LED1_OFF (P2OUT &= ~BIT6)
#define LED2_ON (P2OUT |= BIT5)
#define LED2_OFF (P2OUT &= ~BIT5)
#define LED3_ON (P2OUT |= BIT4)
#define LED3_OFF (P2OUT &= ~BIT4)
#define LED4_ON (P2OUT |= BIT3)
#define LED4_OFF (P2OUT &= ~BIT3)

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

extern void Panel_Init();

#endif /* APPLICATIONS_APPLICATIONS_H_ */
