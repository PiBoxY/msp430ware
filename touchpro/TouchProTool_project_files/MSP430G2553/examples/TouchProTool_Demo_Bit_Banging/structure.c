/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
/********************************************************************************************
 * structure.c
 *
 * Descritption:  User configurable file
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************************/
/************************************************************************************************
 * @file   Structure.c
 *
 * @brief
 *
 * @par    Project:
 *             TouchPro Demo Using Bit Banging Method
 *
 * @par    Developed using:
 *             CCS Version : 5.2.1.00018, w/support for GCC extensions (--gcc)
 *
 *
 * @version     1.0.0 Initial Release
 *
 *
 ***********************************************************************************************/
//******************************************************************************
//  RO_PINOSC_TA_WDTp example with the MSP430G2553
//  threshold and maxResponse values must be updated for electrode design,
//  system clock settings, selection of gate measurement source, and 
//  accumulation cycles
//******************************************************************************

#include "structure.h"

const struct Element wheelElement1 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
                       .inputPxsel2Register = (unsigned char *)&P2SEL2,
                       .inputBits = BIT1,
                       .maxResponse = 245,
                       .threshold = 25
};

const struct Element wheelElement2 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,  
               .inputPxsel2Register = (unsigned char *)&P2SEL2,
               .inputBits = BIT2,
               .maxResponse = 235,
               .threshold = 25
};

const struct Element wheelElement3 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,
              .inputPxsel2Register = (unsigned char *)&P2SEL2,
              .inputBits = BIT3,
              .maxResponse = 290,
              .threshold = 25
};

const struct Element wheelElement4 = {

              .inputPxselRegister = (unsigned char *)&P2SEL,
              .inputPxsel2Register = (unsigned char *)&P2SEL2,
              .inputBits = BIT4,
              .maxResponse = 206,
              .threshold = 25
};

//*** Sensor   *******************************************************/
// This defines the grouping of sensors, the method to measure change in
// capacitance, and the function of the group


const struct Sensor wheel =
{
  .halDefinition = RO_PINOSC_TA0_WDTp,
  .numElements = 4,     //total number of elements that make up the wheel
  .baseOffset = 0,      //is a cumulative count of the number of elements defined
  	  	  	  	  	    //in this application.  Button1 has baseOffset = 0, Button2 has baseOffset = 1
  // Pointer to elements
  .arrayPtr[0] = &wheelElement1,  // point to first element
  .arrayPtr[1] = &wheelElement2,  // point to first element
  .arrayPtr[2] = &wheelElement3,  // point to first element
  .arrayPtr[3] = &wheelElement4,  // point to first element
  // Timer Information
  .measGateSource= GATE_WDT_SMCLK,     //  0->SMCLK, 1-> ACLK
  //.accumulationCycles= WDTp_GATE_32768             //32768
  //.accumulationCycles= WDTp_GATE_8192,               // 8192
  .accumulationCycles= WDTp_GATE_512,             //512
  //  .accumulationCycles= WDTp_GATE_64,             //64

  .points = 24,
  .sensorThreshold = 1
};
