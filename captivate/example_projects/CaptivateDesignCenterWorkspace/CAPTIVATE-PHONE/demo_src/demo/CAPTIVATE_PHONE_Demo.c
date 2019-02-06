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
//! \file   CAPTIVATE_PHONE_Demo.c
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
#include "CAPT_BSP.h"
#include "I2CMaster.h"
#include "DRV26x.h"
#include "CAPTIVATE_PHONE_Demo.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief Element ID of the speaker button in the mode button sensor
//!
#define DEMO_SPEAKER_BTN	(3)

//! \brief Element ID of the mute button in the mode button sensor
//!
#define DEMO_MUTE_BTN		(2)

//! \brief Element ID of the headset button in the mode button sensor
//!
#define DEMO_HEADSET_BTN	(1)

//! \brief Element ID of the end call button in the mode button sensor
//!
#define DEMO_ENDCALL_BTN	(0)

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief Flag 
//!
volatile bool Demo_guardMaskActive = false;

//*****************************************************************************
// Function Prototypes
//*****************************************************************************

//! \brief The event handler for the numeric keypad sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_numericKeypadHandler(tSensor* pSensor);

//! \brief The event handler for the mode keypad sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_modeKeypadHandler(tSensor* pSensor);

//! \brief The event handler for the wheel button sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_wheelButtonHandler(tSensor* pSensor);

//! \brief The event handler for position (slider/wheel) sensors.
//! \param pSensor is a pointer to the calling sensor.
void Demo_positionSensorHandler(tSensor* pSensor);

//! \brief The event handler for the proximity and guard sensor.
//! \param pSensor is a pointer to the calling sensor.
void Demo_guardChannelHandler(tSensor* pSensor);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void Demo_init(void)
{
	//
	// Open the I2C Master driver, which the DRV26x driver will use to
	// communicate with the DRV2605L haptic driver IC via I2C.
	// Enable the haptic driver by setting P1.0, which is connected
	// to the DRV2605L ENABLE pin.  Then, load the configuration for the
	// actuator, run an auto-calibration routine, set up for internal trigger
	// mode, and select the linear resonant actuator (LRA) effect library.
	//
	I2CMaster_open();
	P1OUT |= BIT0;
	DRV26x_reset();
	DRV26x_exitStandby();
	DRV26x_loadActuatorConfig(&DRV26x_actuator_DMJBRN1030);
	DRV26x_runAutoCalibration();
	DRV26x_setMode(DRV26x_mode_internalTrigger);
	DRV26x_selectEffectLibrary(DRV26x_lib_ROM_LRA);

	//
	// Associate the capacitive touch callback handlers with sensors.
	//
	MAP_CAPT_registerCallback(
	        &numericKeypadSensor, 
		    &Demo_numericKeypadHandler
		);
	MAP_CAPT_registerCallback(
	        &modeKeypadSensor, 
			&Demo_modeKeypadHandler
		);
	MAP_CAPT_registerCallback(
	        &wheelButtonSensor, 
			&Demo_wheelButtonHandler
		);
	MAP_CAPT_registerCallback(
	        &scrollWheelSensor, 
			&Demo_positionSensorHandler
		);
	MAP_CAPT_registerCallback(
	        &volumeSliderSensor, 
			&Demo_positionSensorHandler
		);
	MAP_CAPT_registerCallback(
	        &generalPurposeSliderSensor, 
			&Demo_positionSensorHandler
		);
	MAP_CAPT_registerCallback(
		    &proxAndGuardSensor, 
		    &Demo_guardChannelHandler
		);
}

void Demo_numericKeypadHandler(tSensor* pSensor)
{
	//
	// If the guard mask is activated, abort here and do not process
	// any events.
	//
	if (Demo_guardMaskActive == true)
	{
		return;
	}

	//
	// If the sensor has a new touch, fire a "strong click" effect.
	//
	if ((pSensor->bSensorTouch == true) 
	        && (pSensor->bSensorPrevTouch == false))
	{
		DRV26x_fireROMLibraryEffect(
				DRV26x_effect_strongClick_100P,
				true
			);
	}
}

void Demo_modeKeypadHandler(tSensor* pSensor)
{
	//
	// If the guard mask is activated, abort here and do not process
	// any events.
	//
	if (Demo_guardMaskActive == true)
	{
		return;
	}

	//
	// If the sensor has a new touch, process it based on the key
	//
	if ((pSensor->bSensorTouch == true) 
	        && (pSensor->bSensorPrevTouch == false))
	{
		//
		// Identify the dominant element in the sensor.
		// Of the four buttons, only one is allowed to be touched at a time,
		// and the button with the strongest response is selected.
		//
		switch (CAPT_getDominantButton(pSensor))
		{
			//
			// If the mute button was pressed, fire a double click.
			//
			case DEMO_MUTE_BTN:
				DRV26x_fireROMLibraryEffect(
				        DRV26x_effect_doubleClick_100P,
						true
					);
				break;

			//
			// If the speaker button was pressed, fire a strong click.
			//	
			case DEMO_SPEAKER_BTN:
				DRV26x_fireROMLibraryEffect(
						DRV26x_effect_strongClick_100P,
						true
					);
				break;
				
			//
			// If the headset button was pressed, fire a strong click.
			//	
			case DEMO_HEADSET_BTN:
				DRV26x_fireROMLibraryEffect(
						DRV26x_effect_strongClick_100P,
						true
					);
				break;
				
			//
			// If the end call button was pressed, fire a triple click.
			//	
			case DEMO_ENDCALL_BTN:
				DRV26x_fireROMLibraryEffect(
				        DRV26x_effect_tripleClick_100P,
						true
					);
				break;
			default:
				break;
		}
	}
}

void Demo_wheelButtonHandler(tSensor* pSensor)
{
	//
	// If the guard mask is activated, abort here and do not process
	// any events.
	//
	if (Demo_guardMaskActive == true)
	{
		return;
	}

	//
	// If the sensor has a new touch, fire a "sharp pulsing" effect.
	//
	if ((pSensor->bSensorTouch == true) && (pSensor->bSensorPrevTouch == false))
	{
		DRV26x_fireROMLibraryEffect(
				DRV26x_effect_pulsingSharp1_100P,
				true
			);
	}
}

void Demo_positionSensorHandler(tSensor* pSensor)
{
	static uint16_t g_ui16PrevNumber = 0xFF;
	uint16_t ui16SensorPosition = 0;
	uint8_t ui8Number;

	//
	// If the guard mask is activated, abort here and do not process
	// any events.
	//	
	if (Demo_guardMaskActive == true)
	{
		return;
	}

	if (pSensor->bSensorTouch == true)
	{
		if (pSensor->bSensorPrevTouch == false)
		{
			DRV26x_fireROMLibraryEffect(
					DRV26x_effect_buzz1_100P,
					true
				);
		}
		else
		{
			for (ui8Number = 0; ui8Number < 10; ui8Number++)
			{
				ui16SensorPosition += 10;
				if (CAPT_getSensorPosition(pSensor) < ui16SensorPosition)
				{
					if (ui8Number != g_ui16PrevNumber)
					{
						DRV26x_fireROMLibraryEffect(
						        DRV26x_effect_softBump_100P, 
							    false
							);
						g_ui16PrevNumber = ui8Number;
					}
					break;
				}
			}
		}
	}
}

void Demo_guardChannelHandler(tSensor *pSensor)
{
	//
	// If the guard channel is detecting a touch,
	// set the guard mask active flag to mask all other
	// touch processing.
	//
	if (pSensor->bSensorTouch == true)
	{
		Demo_guardMaskActive = true;
		if (pSensor-> bSensorPrevTouch == false)
		{
			DRV26x_fireROMLibraryEffect(
					DRV26x_effect_smoothHum3_30P,
					false
				);
		}
	}
	
	//
	// If the guard channel is not detecting a touch,
	// clear the guard mask active flag to allow standard
	// touch processing.
	//
	else
	{
		Demo_guardMaskActive = false;
	}
}

void Demo_checkForValidTouch(void)
{
	uint8_t ui8Sensor;
	bool bTouch = false;

	//
	// If the guard mask is activated, clear LED1
	// and do not continue checking for a valid touch detection.
	// Indicate that the guard channel is active by illuminating LED2.
	//	
	if (Demo_guardMaskActive == true)
	{
		LED1_OFF;
		LED2_ON;
		return;
	}

	//
	// If the guard mask is inactive, clear the guard LED.
	// Then, search the application's sensors for a valid touch detection.
	// If one is found, set the touch status flag.
	//
	LED2_OFF;
	for (ui8Sensor=0; ui8Sensor<g_uiApp.ui8NrOfSensors; ui8Sensor++)
	{
		if ((g_uiApp.pSensorList[ui8Sensor]->bSensorTouch == true) &&
				(g_uiApp.pSensorList[ui8Sensor] != &proxAndGuardSensor))
		{
			bTouch = true;
			break;
		}
	}

	//
	// If the touch status flag is set, also set LED1.
	//
	if (bTouch==true)
	{
		LED1_ON;
	}
	else
	{
		LED1_OFF;
	}
}
