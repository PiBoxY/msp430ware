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
//! \file   CAPTIVATE_METAL_Demo.c
//
//! \brief  CAPTIVATE-METAL demo
//
//*****************************************************************************

//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************
#include <captivate.h>           // CapTIvate Touch Software Library
#include <CAPT_UserConfig.h>     // CapTIvate Auto-Generated User Configuration
#include <FourDigitDisplay.h>	 // 7Segment-LED
#include <I2CMaster.h>           // I2C Master driver
#include <TCA95xx.h>             // 7Segment-LED Display Driver
#include "CAPTIVATE_METAL_Demo.h"          // User experience

//*****************************************************************************
// Definitions
//*****************************************************************************
#define DIGIT_DISPLAY_ACTIVE_TIME   (200)

// Demo_1 shows the touch button and one to three bars representing a variable force
#define Demo_ForceTouch_1           (1)

// Demo_2 shows multi-touch by displaying number of buttons pressed and sum of button values
#define Demo_ForceTouch_2           (2)

//Demo_3 shows the touch button and force level
#define Demo_ForceTouch_3           (3)

//*****************************************************************************
// Private Variables
//*****************************************************************************
typedef enum
{
    eStandby = 0x00,
    eActive = 0x01,
}tSystemStatus;


typedef enum
{
    eMode1 = 1,
    eMode2,
    eMode3,

}tDemoMode;

#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(DemoMode)
#pragma LOCATION(DemoMode, 0x1800);
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
tDemoMode DemoMode = eMode3;

static bool bNegativeCalibrationTimeout = false;
static tSystemStatus SystemStatus = eActive;
static uint16_t ui16InactiveTimerCount = DIGIT_DISPLAY_ACTIVE_TIME;
static uint8_t CAPT_getNegativeDominantButton(tSensor* pSensor);
static void Calibration_Timer_Start(void);
static void Calibration_Timer_Stop(void);
static void Calibration_Timer_Init(void);


//*****************************************************************************
// Functions
//*****************************************************************************

/*
 * This function is the handler called each time the button group is scanned.
 */
void ButtonEventHandler(tSensor *pSensor)
{
	tElement* Element = NULL;
    tDemoMode temp;
	uint16_t ui16DominantButton = 0;
	uint16_t ui16NegativeButton = 0;
    uint16_t ui16Delta = 0;
    uint16_t ui16Threshold = 0;
	uint8_t ui8Element = 0;
	uint8_t ui8Cycle = 0;
	uint8_t ui8Index = 0;
	uint8_t ui8NrOfButtons = 0;
	uint8_t ui8Sum = 0;
	static bool bFirstNegativeTouch = false;

	/*
	 * First check for a negative touch
	 * A negative touch occurs when the PCB is powered up while a finger is pressing one
	 * or more buttons.  When the finger is removed, a negative touch is flagged.
	 * We use this feature to switch demo modes, save new mode to FRAM, then re-calibrate the sensors.
     * However, due to the response in the metal, it can take several sample periods before returning
     * to its resting position. During this time, additional negative touch detections are possible
     * after the initial re-calibration and the system will continue to re-calibrate until stable.
     * This time can vary and depends on the force that was applied to the button.
	 */
	if(pSensor->bSensorNegativeTouch == true)
	{
	    // If first negative touch, switch modes, then re-calibrate.
	    // else just re-calibrate
	    if(bFirstNegativeTouch == false)
	    {
	        bFirstNegativeTouch = true;

	        ui16NegativeButton = CAPT_getNegativeDominantButton(pSensor);

            switch (ui16NegativeButton)
            {
            case 0:
                temp = eMode1;
                break;
            case 1:
                temp = eMode2;
                break;
            case 2:
                temp = eMode3;
                break;
            default:
                break;
            }

            if((temp == eMode1) ||(temp == eMode2) || (temp == eMode3))
            {
                SYSCFG0 = 0xA501;   //enable FRAM write ability to data memory (INFO-A)
                DemoMode = temp;
                SYSCFG0 = 0xA503;
            }

	    }
	    // Show something on LEDs so user won't press any button while calibrating the buttons
	    // Here we use 2 * DIGIT_DISPLAY_ACTIVE_TIME to allow plenty of time for the metal to
	    // recover before re-calibrating
        FourDigitDisplay_writeNum(eDash, eLetter_C, eLetter_A, eLetter_L);

        // If system not already active, how it now active
        SystemStatus = eActive;

        // Set the time out value because calibration timer will disable inactivity when its done
        ui16InactiveTimerCount = 65535;

        // Start updating the LED
        DigitDisplayScan_Timer_Start();

        // Start the calibration delay timer
        Calibration_Timer_Start();

	    return;
	}

    // Next, check if the negative calibration was started and now is completed
	else if(bNegativeCalibrationTimeout == true)
    {
	    // Cause a system reset so we can go back through the startup process
        WDTCTL = 0;

    }


	// Else, If not a negative touch, then process normally
	else if(pSensor->bSensorProx == true)
	{
        if(SystemStatus == eStandby)
        {
            SystemStatus = eActive;
            ui16InactiveTimerCount = DIGIT_DISPLAY_ACTIVE_TIME;
            DigitDisplayScan_Timer_Start();
        }
        else
        {
            // Reset the timer count
            ui16InactiveTimerCount = DIGIT_DISPLAY_ACTIVE_TIME;
        }

		if (pSensor->bSensorTouch == true)
		{
		    ui16DominantButton = CAPT_getDominantButton(pSensor);

		    Element = CAPT_getDominantButtonAddr(pSensor);

		    ui16Delta = Element->LTA.ui16Natural - Element->filterCount.ui16Natural;

		    // Mode 1 - show button pressed and force using bars
            if(DemoMode == eMode1)
            {
                ui16Threshold = Element->LTA.ui16Natural * Element->ui8TouchThreshold / 128;

                if(ui16Delta > ui16Threshold)
                     FourDigitDisplay_writeNum(ui16DominantButton+1, eBlank, eBlank, eBar1);

                if(ui16Delta > (2 * ui16Threshold))
                    FourDigitDisplay_writeNum(ui16DominantButton+1, eBlank, eBar2, eBar1);

                if(ui16Delta > (3 * ui16Threshold))
                    FourDigitDisplay_writeNum(ui16DominantButton+1,eBar3, eBar2, eBar1);
            }
            // Mode 2 - show multiple buttons and sum of button values
            else if(DemoMode == eMode2)
            {
                for(ui8Cycle=0; ui8Cycle<pSensor->ui8NrOfCycles; ui8Cycle++)
                {
                    for(ui8Element=0; ui8Element<pSensor->pCycle[ui8Cycle]->ui8NrOfElements; ui8Element++)
                    {
                        ui8Index++;
                        if(pSensor->pCycle[ui8Cycle]->pElements[ui8Element]->bTouch == true)
                        {
                            ui8NrOfButtons++;
                            ui8Sum += ui8Index;
                        }
                    }
                    FourDigitDisplay_writeNum(ui8NrOfButtons,eBlank,ui8Sum/10,ui8Sum%10);
                }
            }
            // Mode 3 - show button and delta count
            else if(DemoMode == eMode3)
            {
               FourDigitDisplay_writeNum(ui16DominantButton+1,ui16Delta/100, ui16Delta/10-ui16Delta/100*10, ui16Delta%10);
            }
		}
		else
		    FourDigitDisplay_writeNum(eDash,eDash,eDash,eDash);
	}

}

/*
 * This function initializes the LED, timer and callback
 */
void Demo_Init()
{
    // Initialize LED driver
	FourDigitDisplay_init();

	// Initialize the negative touch calibration timeout timer
	Calibration_Timer_Init();

	// Reset the LED inactivity counter
	ui16InactiveTimerCount = DIGIT_DISPLAY_ACTIVE_TIME;

	// Initialize the timer that performs the background LED update
	DigitDisplayScan_Timer_Init();

	// Start the LED scan timer
	DigitDisplayScan_Timer_Start();

	// Display the current mode
	FourDigitDisplay_writeNum(DemoMode,eLetter_d,eBlank,eBlank);

	// Register a CapTIvate callback for the button handler
	MAP_CAPT_registerCallback(&BUTTONS, &ButtonEventHandler);
}

/*
 * This function determines which button is in a negative touch state
 * and returns the button index.
 */
static uint8_t CAPT_getNegativeDominantButton(tSensor* pSensor)
{
    uint8_t ui8Cycle;
    uint8_t ui8Element;
    uint8_t ui8ID = 0;
    uint8_t ui8Button = 0;
    uint16_t ui16MinCount = 0;
    uint16_t ui16RawCount = 0;

    for (ui8Cycle=0; ui8Cycle < pSensor->ui8NrOfCycles; ui8Cycle++)
    {
        for (ui8Element=0; ui8Element < pSensor->pCycle[ui8Cycle]->ui8NrOfElements; ui8Element++)
        {
            ui16RawCount = pSensor->pCycle[ui8Cycle]->pElements[ui8Element]->pRawCount[0];
            if(ui16RawCount > ui16MinCount)
            {
                ui16MinCount = ui16RawCount;
                ui8Button = ui8ID;
            }
            ui8ID++;
        }
    }
    return (ui8Button);
}

static void Calibration_Timer_Init(void)
{
    TA1CCTL0 |= CCIE;                            // TACCR0 interrupt enabled
    TA1CCR0 = 65535;                             // 2 second timeout @ 32Khz ACLK
    TA1CTL = TASSEL__ACLK;                       // ACLK
}

static void Calibration_Timer_Start(void)
{
    TA1CTL |= MC__UP;                            // UP mode
}

static void Calibration_Timer_Stop(void)
{
    TA1CTL &= ~MC__UPDOWN;
}


/*
 * Timer A0 interrupt service routine
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{

    // Disable timer interrupt
    TA0CCTL0 &= ~CCIE;

    // Re-enable GLOBAL interrupt  because we want to use I2C interrupts
    // from within this ISR
    __bis_SR_register(GIE);


    // Update the display (I2C using interrupts)
    DigitDisplayUpdate();

    // Check if time to go into standby mode
    // No touch detect for some time now
    if(--ui16InactiveTimerCount == 0)
    {
        // Set system status to standby
        SystemStatus = eStandby;

        // Blank out the display
        TCA9534_SetOutput(TCA5934_ADDRS, 0x00);

        // Disable the timer to save power
        TA0CTL &= ~MC__UPDOWN;
    }

    // Re-enable clocks upon entry so we can exit sleep mode if needed
    LPM3_EXIT;

    // Re-enable timer interrupts
    TA0CCTL0 |= CCIE;
}

/*
 * Timer A0 interrupt service routine
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Calibration_Timer_A1 (void)
{

    // Disable timer interrupt
    TA1CCTL0 &= ~CCIE;

    bNegativeCalibrationTimeout = true;

    // Re-enable clocks upon entry so we can exit sleep mode if needed
    LPM3_EXIT;

    // Re-enable timer interrupts
    TA1CCTL0 |= CCIE;

}


