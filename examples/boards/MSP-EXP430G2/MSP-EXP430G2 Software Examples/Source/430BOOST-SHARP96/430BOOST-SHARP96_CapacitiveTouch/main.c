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
 
//*****************************************************************************
//
//! main.c
//
//! 430BOOST-SHARP96 Capacitive Touch Slider Example Project
//! This project demonstrates the use of the two capacitive touch
//! slider sensors on the 430BOOST-SHARP96 booster pack.
//
//! Capacitive Touch Slider Implementation:
//!  - Two vertical sliders
//!  - Three elements each
//!  - End element is split between the top and the bottom on each slider,
//!    enabling four electrodes on the PCB to be driven with three pins
//!    and 3 software elements.  As such, each slider actually looks like
//!    a wheel sensor to software (since the ends are electrically connected,
//!    they appear as one element).  The MSP430 Capacitive Touch Library is
//!    used to measure the electrodes and calculate a wheel position, and the
//!    Sliders_update function in this file is used to derive a slider position
//!    from that.
//
//! Build Requirements:
//!  - GCC Extentions
//
//! \author Texas Instruments
//! \author MSP430 Strategic Applications
//
//! \version 1.0 - W. Schnoor, 1/7/14: Initial Release
//
//*****************************************************************************

//
// Include Standard Libraries
//
#include <msp430.h>
#include <stdbool.h>

//
// Include the MSP430 Capacitive Touch Library
//
#include "CTS_Layer.h"

//
// Include Touch Pro GUI Tools
//
#include "TouchProGUI.h"

//
//! \def TOGGLE_LEDR is a macro for toggling the MSP-EXP430G2
//! LaunchPad red LED (LED1)
//! \def TOGGLE_LEDG is a macro for toggling the MSP-EXP430G2
//! LaunchPad green LED (LED2)
//
#define LP_LEDPORT		(P1OUT)
#define LP_LEDPORTDIR 	(P1DIR)
#define LP_LEDR			(BIT0)
#define LP_LEDG			(BIT6)
#define TOGGLE_LEDR		(P1OUT ^= LP_LEDR)
#define TOGGLE_LEDG		(P1OUT ^= LP_LEDG)

//
//! \def RESPONSE_TIME_MILLISECONDS specifies the response time of the
//! panel in ms; i.e. how often the LCD image is re-drawn with the
//! updated slider position.
//
//! \def RESPONSE_TIME_CYCLES specifies the response time of the
//! panel in VLO clock cycles.
//
#define RESPONSE_TIME_MILLISECONDS	50
#define RESPONSE_TIME_CYCLES	(RESPONSE_TIME_MILLISECONDS * 12)

//
//! \def LEFT_SLIDER specifies the index of the left slider in the
//! slider pointer array and the position of the slider output in the
//! measurement array (g_ui16MeasurementArray).
//
//! \def RIGHT_SLIDER specifies the index of the right slider in the
//! slider pointer array and the position of the slider output in the
//! measurement array (g_ui16MeasurementArray).
//
//! \def NUM_OF_SLIDERS specifies the number of sliders to process.
//
#define LEFT_SLIDER		(0)
#define RIGHT_SLIDER	(1)
#define NUM_OF_SLIDERS	(2)

//
//! \var g_pSlider contains pointers to the sliders to process.
//
//! \var g_ui16LowerTrim contains the lower trim point for the two sliders.
//
//! \var g_ui16UpperTrim contains the upper trim point for the two sliders.
//
const struct Sensor* g_pSlider[NUM_OF_SLIDERS] = { &leftSlider, &rightSlider };
const uint16_t g_ui16LowerTrim[NUM_OF_SLIDERS] = { 2, 2 };
const uint16_t g_ui16UpperTrim[NUM_OF_SLIDERS] = { 62, 62 };

//
// \var g_bUpdatePanel indicates to the background loop that the
// panel should be scanned and the g_ui16MeasurementArray updated.
// TouchProGUI is also sent an update at this time.
//
bool g_bUpdatePanel = false;

//
//! \var g_ui16MeasurementArray contains the outputs of the capacitive touch
//! algorithms.  There are two sliders with three elements each for a total of
//! 6 raw channels and 2 position channels (8 total channels).
//
//! The array is used as follows:
//!  -[0] = Left Slider Position
//!  -[1] = Right Slider Position
//!  -[2] = Left Slider Element 0 Delta
//!  -[3] = Left Slider Element 1 Delta
//!  -[4] = Left Slider Element 2 Delta
//!  -[5] = Right Slider Element 0 Delta
//!  -[6] = Right Slider Element 1 Delta
//!  -[7] = Right Slider Element 2 Delta
//
uint16_t g_ui16MeasurementArray[8] = {0};

//
// Local Function Prototypes
//
bool Sliders_update(void);
void errorTrap(void);

//*****************************************************************************
//
//! main()
//! This is the point of entry into the program,
//! and houses the background loop.
//
//*****************************************************************************
void
main(void)
{
	bool bPanelTouched;

	//
	// Hold the watchdog timer.
	// NOTE: The watchdog timer is utilized in this program by
	// the capacitive touch library (in interval mode).
	// As such, it is unavailable to the application to be used in
	// watchdog mode (though it can be shared with the touch library
	// via time multiplexing.
	//
	WDTCTL = WDTPW | WDTHOLD;

	//
	// Configure Port 1 Pins 0 and 6 to be Output Low.
	// These pins are the LED pins on the MSP-EXP430G2 LaunchPad.
	//
	LP_LEDPORT = 0x00;
	LP_LEDPORTDIR = LP_LEDR | LP_LEDG;

	//
	// Configure the G2xx Basic Clock Module (BCM+)
	//  - MCLK  = 12MHz
	//  - SMCLK = 6MHz
	//  - ACLK  = ~12kHz
	//
	if ((CALBC1_1MHZ == 0xFF) || (CALBC1_8MHZ==0xFF))
	{
		//
		// If DCO calibrations are erased, trap CPU
		//
		errorTrap();
	}
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL =  CALDCO_12MHZ;
	BCSCTL2 = DIVS_1;
	BCSCTL3 |= LFXT1S_2;

	//
	// Initialize the TouchPro GUI USCI UART interface.
	//
	TouchProGUI_init();

	//
	// Initialize the baseline measurement for each element
	// in the left slider and the right slider.
	//
	TI_CAPT_Update_Tracking_Rate(TRIDOI_MED | TRADOI_SLOW);
	TI_CAPT_Init_Baseline(&leftSlider);
	TI_CAPT_Init_Baseline(&rightSlider);

	//
	// Perform baseline averaging to ensure the starting
	// baseline value is accurate.
	//
	TI_CAPT_Update_Baseline(&leftSlider, 5);
	TI_CAPT_Update_Baseline(&rightSlider, 5);

	//
	// Start the periodic interval wakeup timer (Timer A1 is used in this example).
	//
	TA1CCTL0 = CCIE;
	TA1CCR0 = RESPONSE_TIME_CYCLES;
	TA1CTL = TASSEL_1 | MC_1;

	//
	// Enable CPU interrupts
	//
	__enable_interrupt();

	//
	// Background Loop
	//
	while (1)
	{
		if (g_bUpdatePanel == true)
		{
			g_bUpdatePanel = false;
			TOGGLE_LEDR;
			bPanelTouched = Sliders_update();
			TouchProGUI_sendData(g_ui16MeasurementArray, 8);
			if (bPanelTouched == true)
			{
				LP_LEDPORT |= LP_LEDG;
			}
			else
			{
				LP_LEDPORT &= ~LP_LEDG;
			}

		}
		LPM0;
	}
}

//*****************************************************************************
//
//! Sliders_update()
//! Measure the left and right slider sensors, storing new measurement info in
//! the global array g_ui16MeasurementArray.  This function calls into the
//! capacitive touch library to get measurents back, than handles the wheel
//! unwrapping, basic edge trim, and a single-level averaging filter.
//
//! /param none.
//! /return true if either of the two sliders are being touched, else false.
//
//*****************************************************************************
bool
Sliders_update(void)
{
	uint16_t ui16newPosition[NUM_OF_SLIDERS];
	uint8_t ui8currSlider;
	uint16_t ui16ZeroPoint;

	//
	// Get Delta Measurement.  This data will be used to display in TouchPro GUI.
	//
	TI_CAPT_Custom(&leftSlider, &g_ui16MeasurementArray[2]);
	TI_CAPT_Custom(&rightSlider, &g_ui16MeasurementArray[5]);

	//
	// Get Current Slider Positions.
	// The Capacitive Touch Library wheel abstraction is used to handle
	// the electrode geometry, and is unwrapped into a slider in the
	// processing code below.
	//
	for (ui8currSlider=0; ui8currSlider<NUM_OF_SLIDERS; ui8currSlider++)
	{
		ui16newPosition[ui8currSlider] = TI_CAPT_Wheel(g_pSlider[ui8currSlider]);

		//
		// If new position is valid, unwrap the wheel value back to a slider value
		//
		if (ui16newPosition[ui8currSlider] != ILLEGAL_SLIDER_WHEEL_POSITION)
		{
			//
			// Calculate Zero Point (bottom/top division point)
			//
			ui16ZeroPoint = (g_pSlider[ui8currSlider]->points / g_pSlider[ui8currSlider]->numElements) >> 1;

			//
			// If below zero point, the user is at the top of the slider
			//
			if(ui16newPosition[ui8currSlider] < ui16ZeroPoint)
            {
            	ui16newPosition[ui8currSlider] += g_pSlider[ui8currSlider]->points - ui16ZeroPoint;
            }

			//
			// Else if above the zero point, the user is at the bottom or middle of the slider
			//
            else
            {
            	ui16newPosition[ui8currSlider] -= ui16ZeroPoint;
            }

			//
			// Trim edges to prevent un-intentional wraparound
			//
			if (ui16newPosition[ui8currSlider] > g_ui16LowerTrim[ui8currSlider])
			{
				ui16newPosition[ui8currSlider] -= g_ui16LowerTrim[ui8currSlider];
			}
			else
			{
				ui16newPosition[ui8currSlider] = 0;
			}

			ui16newPosition[ui8currSlider] = (ui16newPosition[ui8currSlider] * g_pSlider[ui8currSlider]->points);
			ui16newPosition[ui8currSlider] /= g_ui16UpperTrim[ui8currSlider] - g_ui16LowerTrim[ui8currSlider];

			if (ui16newPosition[ui8currSlider] > g_pSlider[ui8currSlider]->points)
			{
				ui16newPosition[ui8currSlider] = g_pSlider[ui8currSlider]->points;
			}

			//
			// Average in new positions to the measurement array ([0] = Left Slider Position, [1] = Right Slider Position)
			//
			g_ui16MeasurementArray[ui8currSlider] += ui16newPosition[ui8currSlider];
			g_ui16MeasurementArray[ui8currSlider] >>= 1;

			//
			// Panel is being touched, return true
			//
			return true;
		}
	}

	//
	// Panel not touched, return false
	//
	return false;
}

//*****************************************************************************
//
//! errorTrap() will hold the CPU and light both LEDs if called.
//
//! \param none.
//! \return none.
//
//*****************************************************************************
void
errorTrap(void)
{
	LP_LEDPORT |= LP_LEDR | LP_LEDG;
	while(1)
	{
		__no_operation();
	};
}

//*****************************************************************************
//
//! IntervalWakeup provides periodic wake up from LPM3 to measure the
//! capacitive touch sliders.
//
//*****************************************************************************
#pragma vector=TIMER1_A0_VECTOR
__interrupt void IntervalWakeupISR (void)
{
	//
	// Set flag to update panel, then wake from LPM3 sleep
	//
	g_bUpdatePanel = true;
	LPM3_EXIT;
}

//*****************************************************************************
//
//! ISR Trap
//
//*****************************************************************************
#pragma vector= PORT2_VECTOR,PORT1_VECTOR,ADC10_VECTOR,NMI_VECTOR,          \
                USCIAB0RX_VECTOR,TIMER0_A1_VECTOR, COMPARATORA_VECTOR,      \
                TIMER1_A1_VECTOR,TIMER0_A0_VECTOR
__interrupt void ISR_trap(void)
{
	//
	// Trap the CPU and flag an error
	//
	errorTrap();
}
