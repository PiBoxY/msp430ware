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
//! \file   Backchannel_UART_demo.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2018, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "demo.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//*****************************************************************************
// Global Variables
//*****************************************************************************

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//*****************************************************************************
// Function Implementations
//*****************************************************************************

//callback function for keypad buttons
void keypad_callback(tSensor* pSensor){

    //trigger once when the button is touched
    if(((pSensor->bSensorTouch)) && (!(pSensor->bSensorPrevTouch))){

        //decode the dominant button number
        uint8_t myButtonNumber = CAPT_getDominantButton(pSensor);
        uint8_t loopCounter = 0;
        uint8_t cycleCounter = 0;
        uint8_t elementCounter = 0;
        uint8_t myCycle;
        uint8_t myElement;

        //go through each cycle pointer, and within it each element pointer to find element that corresponds to the dominant button
        for(cycleCounter = 0; cycleCounter < pSensor->ui8NrOfCycles; ++cycleCounter){
            for(elementCounter = 0; elementCounter < pSensor->pCycle[cycleCounter]->ui8NrOfElements; ++elementCounter){
                //get the cycle and element numbers from the dominant button
                if(loopCounter == myButtonNumber){
                    myCycle = cycleCounter;
                    myElement = elementCounter;
                }
                loopCounter++;
            }

        }

        //placeholder for output data, must be static to ensure that it is available & unchanged across function calls
        static uint8_t str[100];

        //access the element's raw count
        //CAPT_getDominantButton() returns 0x00 for button #1, 0x01 for button #2... etc
        uint16_t rawCount = pSensor->pCycle[myCycle]->pElements[myElement]->pRawCount[0];

        //extract the filtered count, NOTE this truncates to the nearest natural number
        uint16_t filteredCount = pSensor->pCycle[myCycle]->pElements[myElement]->filterCount.ui16Natural ;

        //get the length of the output string, insert int data into output string
        uint8_t str_length = sprintf((char*)str, "Button Pressed: %d\tRaw Count: %d\tFiltered Count: %d\n\r", myButtonNumber + 1, rawCount, filteredCount);

        //transmit data
        UART_transmitBuffer(str, str_length);
    }
}



//Sets all the parameters for UART transmission
const tUARTPort UARTPort =
{
     //does not receive messages, so no callback needed
    .pbReceiveCallback = NULL,

    //does not receive messages, so no callback needed
    .pbErrorCallback = 0,

    //choose clock source
    .peripheralParameters.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,

    //clockPrescalar is UCBRx = int(N / 16), where N = clock_frequency / baud_rate
    //int(N / 16) = int(2000000 / 9600 / 16) = int(13.02) = 13;
    .peripheralParameters.clockPrescalar = SMCLK_FREQ_MHZ * 1000000 / 9600 / 16,

    //controls UCBRF bits for oversampling mode
    //UCBRF = int(((N/16) - int(N/16)) * 16) = int(((208.3333/16) - int(208.3333/16)) * 16) = int(0.3333) = 0
    .peripheralParameters.firstModReg = 0,

    //consult table 22-4 in the user's guide to find the register value corresponding to UCBRF number
    //fraction portion of N = 0.3333
    //in the table, 0.3333 <---> 0x49
    .peripheralParameters.secondModReg = 0x49,

    //controls parity bit - NOTE the eZ-FET does not support a parity bit
    .peripheralParameters.parity = EUSCI_A_UART_NO_PARITY,

    //least or most significant bit first
    .peripheralParameters.msborLsbFirst = EUSCI_A_UART_LSB_FIRST,

    //select either one or two stop bits
    .peripheralParameters.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT,

    //select whether to use multiprocessor/autobaud modes
    .peripheralParameters.uartMode = EUSCI_A_UART_MODE,

    //selects oversampling vs low-freq baud rate modes
    .peripheralParameters.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION

};
