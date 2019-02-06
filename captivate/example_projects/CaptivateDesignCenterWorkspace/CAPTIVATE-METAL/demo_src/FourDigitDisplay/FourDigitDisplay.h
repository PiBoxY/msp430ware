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
//
//! \file   FourDigitDisplay.h
//
//! \brief  HAL for seven segment display interface
//
//*****************************************************************************

//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

#ifndef FOURDIGITDISPLAY_H_
#define FOURDIGITDISPLAY_H_

#include "TCA95xx.h"

//
// Digit Segments
//
#define SEG_A	(BIT6)
#define SEG_B	(BIT4)
#define SEG_C	(BIT0)
#define SEG_D	(BIT2)
#define SEG_E	(BIT3)
#define SEG_F	(BIT5)
#define SEG_G	(BIT7)
#define SEG_P	(BIT1)

// LED digits {0,1,2,3,...etc} are formed from combination of segments
// and located in lookup table (FourDigitDisplay.c).
// Accessing a particular combination requires an index into the the table.
// Numbers 0 - F have a one-one correspondence.
// Index for letters and symbols defined here

typedef enum {
    eLetter_A = 10,
    eLetter_B,
    eLetter_C,
    eLetter_D,
    eLetter_E,
    eLetter_F,
    eLetter_L,
    eLetter_d,
    eDP,
    eBlank,
    eDash,
    eBar1,
    eBar2,
    eBar3,
    eSegAll,
 }eSymbols;


 //*****************************************************************************
 //
 // \brief  FourDigitDisplay_init() Initialize the 4 LED display
 //
 // \param  none
 // \return none
 //
 //*****************************************************************************
void FourDigitDisplay_init(void);

//*****************************************************************************
//
// \brief  DigitDisplayUpdate() Update the 4 LED display
//
// \param  none
// \return none
//
//*****************************************************************************
void DigitDisplayUpdate(void);

//*****************************************************************************
//
// \brief  DigitDisplayScan_Timer_Init() Initialize the LED update timer
//
// \param  none
// \return none
//
//*****************************************************************************
void DigitDisplayScan_Timer_Init(void);

//*****************************************************************************
//
// \brief  DigitDisplayScan_Timer_Start() Start LED update timer
//
// \param  none
// \return none
//
//*****************************************************************************
void DigitDisplayScan_Timer_Start(void);

//*****************************************************************************
//
// \brief  DigitDisplayScan_Timer_Stop() Stop LED update timer
//
// \param  none
// \return none
//
//*****************************************************************************
void DigitDisplayScan_Timer_Stop(void);

//*****************************************************************************
//
// \brief  FourDigitDisplay_writeNum() Update display buffer
//
// \param  none
// \return none
//
//*****************************************************************************
void FourDigitDisplay_writeNum(uint8_t ui8Num1, uint8_t ui8Num2, uint8_t ui8Num3, uint8_t ui8Num4);


#endif /* FOURDIGITDISPLAY_H_ */
