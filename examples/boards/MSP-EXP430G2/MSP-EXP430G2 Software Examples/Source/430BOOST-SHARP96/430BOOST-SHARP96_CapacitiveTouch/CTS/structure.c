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

//******************************************************************************
//  430BOOST-SHARP96 example with the MSP430G2553 on a MSP-EXP430G2 LaunchPad
//******************************************************************************

#include "structure.h"

const struct Element leftSlider_Element0 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT2,
	.maxResponse = 70,
	.threshold = 5
};

const struct Element leftSlider_Element1 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT1,
	.maxResponse = 100,
	.threshold = 5
};

const struct Element leftSlider_Element2 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT0,
	.maxResponse = 95,
	.threshold = 5
};

const struct Element rightSlider_Element0 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT3,
	.maxResponse = 71,
	.threshold = 5
};

const struct Element rightSlider_Element1 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT4,
	.maxResponse = 103,
	.threshold = 5
};

const struct Element rightSlider_Element2 =
{
	.inputPxselRegister = (unsigned char *)&P2SEL,
	.inputPxsel2Register = (unsigned char *)&P2SEL2,
	.inputBits = BIT5,
	.maxResponse = 105,
	.threshold = 5
};

const struct Sensor leftSlider =
{
	.halDefinition = RO_PINOSC_TA0_WDTp,
	.numElements = 3,
	.points = 64,
	.sensorThreshold = 72,
	.baseOffset = 0,
	// Pointer to elements
	.arrayPtr[0] = &leftSlider_Element0,
	.arrayPtr[1] = &leftSlider_Element1,
	.arrayPtr[2] = &leftSlider_Element2,
	// Timer Information
	.measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
	.accumulationCycles= WDTp_GATE_512             //512
};

const struct Sensor rightSlider =
{
	.halDefinition = RO_PINOSC_TA0_WDTp,
	.numElements = 3,
	.points = 64,
	.sensorThreshold = 76,
	.baseOffset = 3,
	// Pointer to elements
	.arrayPtr[0] = &rightSlider_Element0,
	.arrayPtr[1] = &rightSlider_Element1,
	.arrayPtr[2] = &rightSlider_Element2,
	// Timer Information
	.measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
	.accumulationCycles= WDTp_GATE_512             //512
};
