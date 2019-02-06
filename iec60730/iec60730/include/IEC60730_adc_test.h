/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
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


#ifndef _IEC60730_ADC_TEST_H_
#define _IEC60730_ADC_TEST_H_


//*****************************************************************************
//
//The following include is added to use C99 data types
//
//*****************************************************************************
#include "stdint.h"


#ifdef __cplusplus
extern "C" {
#endif


//*****************************************************************************
//
//The following are values that can be assigned to useInternalInput in
//IEC60730_ADC_TEST_adcTest_Handle.
//
//*****************************************************************************
#define	EXTERNAL_REF	0
#define INT_REF_1_5_V	1
#define INT_REF_2_5_V	2
#define INT_REF_2_0_V   3



//*****************************************************************************
//
//The following structure is the argument for IEC60730_ADC_TEST_testAdcInput()
//
//*****************************************************************************

typedef struct IEC60730_ADC_TEST_adcTest_Handle
{
	// ADC count that is compared with ADC convertion value
    int16_t   pinCount;
    // Selection to use internal or external voltage references
    uint8_t   useInternalInput;
    // ADC channel to be sampled
    uint8_t   muxChannel;
} IEC60730_ADC_TEST_adcTest_Handle;


//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************

extern uint8_t IEC60730_ADC_TEST_testAdcInput(IEC60730_ADC_TEST_adcTest_Handle *adcTestHandle);

#ifdef __cplusplus
}
#endif // extern "C"



#endif
