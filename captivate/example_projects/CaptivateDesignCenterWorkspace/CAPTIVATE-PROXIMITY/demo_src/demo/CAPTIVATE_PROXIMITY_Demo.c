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
//! \file   CAPTIVATE_PROXIMITY_Demo.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2633
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <msp430.h>
#include "captivate.h"
#include "CAPTIVATE_PROXIMITY_Demo_LEDS.h"
#include "CAPTIVATE_PROXIMITY_Demo.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Short-cut to the left element in the proximity sensor.
//!
#define DEMO_LEFT_ELEMENT   (proximityGroupSensor_E00)

//! \brief Short-cut to the center element in the proximity sensor.
//!
#define DEMO_CENTER_ELEMENT (proximityGroupSensor_E02)

//! \brief Short-cut to the right element in the proximity sensor.
//!
#define DEMO_RIGHT_ELEMENT  (proximityGroupSensor_E03)

//! \brief Short-cut to the ring element in the proximity sensor.
//!
#define DEMO_SHIELD_ELEMENT (proximityGroupSensor_E01)

//! \brief Bit mask of the left element
//!
#define DEMO_LEFT_MASK (BIT0)

//! \brief Bit mask of the center element
//!
#define DEMO_CNTR_MASK (BIT1)

//! \brief Bit mask of the right element
//!
#define DEMO_RIGHT_MASK (BIT2)

//*****************************************************************************
// Enumerations
//*****************************************************************************

//! \brief Enumeration of the swipe status options
//!
enum
{
    Demo_leftSwipe,
    Demo_rightSwipe,
    Demo_noSwipe
};

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Element 0 (Left Zone Element)
//!
extern tElement proximityGroupSensor_E00;

//! \brief Element 1 (Shield Element)
//!
extern tElement proximityGroupSensor_E01;

//! \brief Element 2 (Center Zone Element)
//!
extern tElement proximityGroupSensor_E02;

//! \brief Element 3 (Right Zone Element)
//!
extern tElement proximityGroupSensor_E03;

//! \brief Status variable that indicates the current swipe status.
//!
uint8_t Demo_swipeStatus = Demo_noSwipe;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief Handle events for the proximity group sensor.
//! \param pSensor is a pointer to the sensor structure
static void Demo_proximityGroupEventHandler(tSensor* pSensor);

//! \brief Update the 4 proximity status LEDs (amber) based on the proximity
//!        status of each element in the proximity group.
static void Demo_updateProximityLEDs(void);

//! \brief Update the 2 swipe status LEDs (amber) based on the global
//!        swipe status.
static void Demo_updateSwipeLEDs(uint8_t swipeStatus);

//! \brief Update the 4 proximity status LEDs (amber) based on the proximity
//!        status of each element in the proximity group.
static uint8_t Demo_getZoneStatus(void);

//! \brief Update the swipe state machine based on a new zone state.
//! \param zoneState is the new zone state, where bits 0-3 represent
//!        the left, center, and right elements, respectively.
static uint8_t Demo_getSwipeState(uint8_t zoneState);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Demo_init(void)
{
    //
    // Turn off all CAPTIVATE-PROXIMITY PCB LEDs
    //
    LED_LEFT__OFF;
    LED_CENTER__OFF;
    LED_RIGHT__OFF;
    LED_RING__OFF;
    LED_SWIPELEFT__OFF;
    LED_SWIPERIGHT__OFF;

    //
    // Associate the proximity group event handler with the proximity
    // group sensor.
    //
    MAP_CAPT_registerCallback(
            &proximityGroupSensor,
            &Demo_proximityGroupEventHandler
        );
}

void Demo_proximityGroupEventHandler(tSensor* pSensor)
{
    uint8_t zoneStatus;

    //
    // If a negative touch occurred,
    // re-calibrate the sensor.
    //
    if (pSensor->bSensorNegativeTouch)
    {
        CAPT_calibrateSensor(pSensor);
        LED_LEFT__OFF;
        LED_CENTER__OFF;
        LED_RIGHT__OFF;
        LED_RING__OFF;
        LED_SWIPELEFT__OFF;
        LED_SWIPERIGHT__OFF;
        return;
    }

    //
    // Update the proximity LEDs, then get the zone
    // status.  The zone detection status is based on
    // the touch status flags, which are re-purposed
    // for a closer level of proximity in this app.
    // Plug the new zone status into the swipe state
    // function, which will update the swipe state machine.
    // Then update the swipe LEDs accordinly.
    //
    Demo_updateProximityLEDs();
    zoneStatus = Demo_getZoneStatus();
    Demo_swipeStatus = Demo_getSwipeState(zoneStatus);
    Demo_updateSwipeLEDs(Demo_swipeStatus);
}

void Demo_updateProximityLEDs(void)
{
    //
    // Update the left element LED if the proximity
    // threshold has been exceeded
    //
    if (DEMO_LEFT_ELEMENT.bProx == true)
    {
        LED_LEFT__ON;
    }
    else
    {
        LED_LEFT__OFF;
    }

    //
    // Update the center element LED if the proximity
    // threshold has been exceeded
    //
    if (DEMO_CENTER_ELEMENT.bProx == true)
    {
        LED_CENTER__ON;
    }
    else
    {
        LED_CENTER__OFF;
    }

    //
    // Update the right element LED if the proximity
    // threshold has been exceeded
    //
    if (DEMO_RIGHT_ELEMENT.bProx == true)
    {
        LED_RIGHT__ON;
    }
    else
    {
        LED_RIGHT__OFF;
    }

    //
    // Update the ring element LED if the proximity
    // threshold has been exceeded
    //
    if (DEMO_SHIELD_ELEMENT.bProx == true)
    {
        LED_RING__ON;
    }
    else
    {
        LED_RING__OFF;
    }
}

void Demo_updateSwipeLEDs(uint8_t swipeStatus)
{
    //
    // Set the appropriate LED if a swipe left
    // or right occurred, else clear the LEDs.
    //
    if (swipeStatus == Demo_leftSwipe)
    {
        LED_SWIPELEFT__ON;
    }
    else if (swipeStatus == Demo_rightSwipe)
    {
        LED_SWIPERIGHT__ON;
    }
    else
    {
        LED_SWIPELEFT__OFF;
        LED_SWIPERIGHT__OFF;
    }
}

uint8_t Demo_getZoneStatus(void)
{
    uint8_t zoneBitField = 0x00;

    //
    // Return a set of bit masks that is representative of
    // which zones are in detect.  Note that the touch status
    // flag is re-purposed for close proximity in this application.
    //
    if (DEMO_LEFT_ELEMENT.bTouch == true)
    {
        zoneBitField |= DEMO_LEFT_MASK;
    }
    if (DEMO_CENTER_ELEMENT.bTouch == true)
    {
        zoneBitField |= DEMO_CNTR_MASK;
    }
    if (DEMO_RIGHT_ELEMENT.bTouch == true)
    {
        zoneBitField |= DEMO_RIGHT_MASK;
    }

    return zoneBitField;
}

uint8_t Demo_getSwipeState(uint8_t zoneState)
{
    static uint8_t inProgressSwipeState = 0;
    static uint8_t inProgressSwipeDirection = Demo_noSwipe;
    static uint8_t ui8Result = Demo_noSwipe;
    static const uint8_t swipeStateTable[2][6] =
    {
        //
        // Left Swipe Pattern
        //
        {
            0x00,                                              // 0
            DEMO_RIGHT_MASK,                                   // 1
            DEMO_RIGHT_MASK | DEMO_CNTR_MASK,                  // 2
            DEMO_RIGHT_MASK | DEMO_CNTR_MASK | DEMO_LEFT_MASK, // 3
            DEMO_CNTR_MASK  | DEMO_LEFT_MASK,                  // 4
            DEMO_LEFT_MASK                                     // 5
        },
        
        //
        // Right Swipe Pattern
        //
        {
            0x00,                                              // 0
            DEMO_LEFT_MASK,                                    // 1
            DEMO_LEFT_MASK | DEMO_CNTR_MASK,                   // 2
            DEMO_LEFT_MASK | DEMO_CNTR_MASK | DEMO_RIGHT_MASK, // 3
            DEMO_CNTR_MASK | DEMO_RIGHT_MASK,                  // 4
            DEMO_RIGHT_MASK                                    // 5
        },
    };

    switch (inProgressSwipeState)
    {
        //
        // If no swipe was in progress, enter here.  Determine
        // if a new swipe is being started, and if so, whether it
        // is a left or right swipe.
        //
        case 0:
            if (zoneState == swipeStateTable[Demo_leftSwipe][1])
            {
                inProgressSwipeDirection = Demo_leftSwipe;
            }
            else if (zoneState == swipeStateTable[Demo_rightSwipe][1])
            {
                inProgressSwipeDirection = Demo_rightSwipe;
            }
            else
            {
                break;
            }
            inProgressSwipeState = 1;
            ui8Result = Demo_noSwipe;
            break;

        //
        // Cases 1-4 are progressions through the detection pattern for a swipe.
        //
        case 1:
            if (zoneState == swipeStateTable[inProgressSwipeDirection][2])
            {
                inProgressSwipeState = 2;
            }
            else if (zoneState != swipeStateTable[inProgressSwipeDirection][1])
            {
                inProgressSwipeState = 0;
            }
            break;

        case 2:
            if (zoneState == swipeStateTable[inProgressSwipeDirection][3])
            {
                inProgressSwipeState = 3;
            }
            else if (zoneState != swipeStateTable[inProgressSwipeDirection][2])
            {
                inProgressSwipeState = 0;
            }
            break;

        case 3:
            if (zoneState == swipeStateTable[inProgressSwipeDirection][4])
            {
                inProgressSwipeState = 4;
            }
            else if (zoneState != swipeStateTable[inProgressSwipeDirection][3])
            {
                inProgressSwipeState = 0;
            }
            break;

        case 4:
            if (zoneState == swipeStateTable[inProgressSwipeDirection][5])
            {
                inProgressSwipeState = 5;
            }
            else if (zoneState != swipeStateTable[inProgressSwipeDirection][4])
            {
                inProgressSwipeState = 0;
            }
            break;

        //
        // If this step is reached, when the user clears the proximity field the
        // swipe will be reported.
        //
        case 5:
            if (zoneState == swipeStateTable[inProgressSwipeDirection][0])
            {
                ui8Result = inProgressSwipeDirection;
                inProgressSwipeState = 0;
            }
            else if (zoneState != swipeStateTable[inProgressSwipeDirection][5])
            {
                inProgressSwipeState = 0;
            }
            break;

    }

    //
    // Return the swipe result.
    //
    return ui8Result;
}
