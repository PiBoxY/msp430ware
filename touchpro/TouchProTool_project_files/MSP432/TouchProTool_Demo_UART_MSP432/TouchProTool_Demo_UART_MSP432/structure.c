//**************************************************************
// structure.c
//
//! \brief Contians definitions for element and sensor structures
//!        used in associated application.
//!
//!        Revised by a0224638 on 9/10/2015
//**************************************************************

#include "structure.h"
#include "TouchProTool_Demo_UART.h"    //Contains definitions that impact sensor configuration

//! \brief Struct containing definitions for Volume/Down
//!        element (connected to P6.4)
//!
const struct Element down = {

		// Select pin P6.4
		.inputBits = CAPTIOPOSEL_6 + CAPTIOPISEL_4,

// If Custom_Counts is not defined, .maxResponse and
// .threshold must be assigned to evaluate the position
// of the touch on the wheel.
#ifndef Custom_Counts

		//Set .maxResponse and .threshold according
		//to typical values for measurement method
		//and system frequency
#ifdef RO_CTIO_TA2_WDTA
        .maxResponse = 80+1400,
        .threshold = 80
#endif
#ifdef RO_CTIO_TA2_TA3
        .maxResponse = 50+750,
        .threshold = 50
#endif
#ifdef fRO_CTIO_TA2_TA3
#if (__SYSTEM_CLOCK == 24000000)
		.maxResponse = 50+1250,
		.threshold = 50
#elif (__SYSTEM_CLOCK == 48000000)
		.maxResponse = 100+3900,
		.threshold = 100
#else
		.maxResponse = 20+400,
		.threshold = 20
#endif
#endif

#endif
};

//! \brief Struct containing definitions for Forward/Right
//!        element (connected to P3.6)
//!
const struct Element right = {

		// Select Pin 3.6
		.inputBits = CAPTIOPOSEL_3 + CAPTIOPISEL_6,

// If Custom_Counts is not defined, .maxResponse and
// .threshold must be assigned to evaluate the position
// of the touch on the wheel.
#ifndef Custom_Counts

		//Set .maxResponse and .threshold according
		//to typical values for measurement method
		//and system frequency
#ifdef RO_CTIO_TA2_WDTA
        .maxResponse = 50+850,
        .threshold = 50
#endif
#ifdef RO_CTIO_TA2_TA3
		.maxResponse = 50+400,
		.threshold = 50
#endif
#ifdef fRO_CTIO_TA2_TA3
#if (__SYSTEM_CLOCK == 24000000)
		.maxResponse = 50+1350,
		.threshold = 50
#elif (__SYSTEM_CLOCK == 48000000)
		.maxResponse = 100+3900,
		.threshold = 100
#else
		.maxResponse = 20+430,
		.threshold = 20
#endif
#endif

#endif
};

//! \brief Struct containing definitions for Volume/Up
//!        element (connected to P5.2)
//!
const struct Element up = {

		// Select P5.2
		.inputBits = CAPTIOPOSEL_5 + CAPTIOPISEL_2,

// If Custom_Counts is not defined, .maxResponse and
// .threshold must be assigned to evaluate the position
// of the touch on the wheel.
#ifndef Custom_Counts

		//Set .maxResponse and .threshold according
		//to typical values for measurement method
		//and system frequency
#ifdef RO_CTIO_TA2_WDTA
        .maxResponse = 50+850,
        .threshold = 50
#endif
#ifdef RO_CTIO_TA2_TA3
		.maxResponse = 50+360,
		.threshold = 50
#endif
#ifdef fRO_CTIO_TA2_TA3
#if (__SYSTEM_CLOCK == 24000000)
		.maxResponse = 50+1350,
		.threshold = 50
#elif (__SYSTEM_CLOCK == 48000000)
		.maxResponse = 100+3900,
		.threshold = 100
#else
		.maxResponse = 20+430,
		.threshold = 20
#endif
#endif

#endif
};

//! \brief Struct containing definitions for Reverse/Left
//!        element (connected to P6.5)
//!
const struct Element left = {

		// Select P6.5
		.inputBits = CAPTIOPOSEL_6 + CAPTIOPISEL_5,

// If Custom_Counts is not defined, .maxResponse and
// .threshold must be assigned to evaluate the position
// of the touch on the wheel.
#ifndef Custom_Counts

		//Set .maxResponse and .threshold according
		//to typical values for measurement method
		//and system frequency
#ifdef RO_CTIO_TA2_WDTA
        .maxResponse = 80+1400,
        .threshold = 80
#endif
#ifdef RO_CTIO_TA2_TA3
		.maxResponse = 50+700,
		.threshold = 50
#endif
#ifdef fRO_CTIO_TA2_TA3
#if (__SYSTEM_CLOCK == 24000000)
		.maxResponse = 50+1350,
		.threshold = 50
#elif (__SYSTEM_CLOCK == 48000000)
		.maxResponse = 100+3900,
		.threshold = 100
#else
		.maxResponse = 20+430,
		.threshold = 20
#endif
#endif

#endif
};



//! \brief This defines the grouping of sensors, the method to measure change in
//!        capacitance, and the function of the group
//!
const struct Sensor wheel =	{

		//Set .halDefinition based on hardware configuration
		// selected in structure.h
#ifdef RO_CTIO_TA2_WDTA
		.halDefinition = RO_CTIO_TA2_WDTA,
#endif
#ifdef RO_CTIO_TA2_TA3
		.halDefinition = RO_CTIO_TA2_TA3,
#endif
#ifdef fRO_CTIO_TA2_TA3
		.halDefinition = fRO_CTIO_TA2_TA3,
#endif
		// CAPTIO0 must be used because of selected timers
		.inputCaptioctlRegister = (uint16_t *)&CAPTIO0CTL,
        .numElements = 4,

		//Set desired resolution along wheel according
		// to measurement capabilities
#if (defined (fRO_CTIO_TA2_TA3) && __SYSTEM_CLOCK == 48000000)
		//Increased resolution with fRO and high clock speed
        .points = 255,
#elif (defined (fRO_CTIO_TA2_TA3) && __SYSTEM_CLOCK == 24000000)
		.points = 128,
#else
		.points = 64,
#endif

		//Set .sensorThreshold to obtain best
		// performance for measurement method
#if (defined (RO_CTIO_TA2_TA3))
        .sensorThreshold = 70,
#else
		.sensorThreshold = 50,
#endif
        .baseOffset = 0,

        // Pointer to elements
        .arrayPtr[0] = &up,  // point to first element
        .arrayPtr[1] = &right,
		.arrayPtr[2] = &down,
        .arrayPtr[3] = &left,

        // Timer Information
#ifdef RO_CTIO_TA2_WDTA
      	.measGateSource = GATE_WDTA_VLO,     //  Gate Clock Source
		.accumulationCycles = WDTA_GATE_64
#endif
#ifdef RO_CTIO_TA2_TA3
		.measGateSource = TIMER_ACLK,     // Clock Source for the Gate Timer A3
		.sourceScale = TIMER_SOURCE_DIV_3,
		.accumulationCycles = 15        //CCRx for Timer A3*/
#endif
#ifdef fRO_CTIO_TA2_TA3
		.measGateSource = TIMER_SMCLK,     // Clock source for Timer A3
		.sourceScale = TIMER_SOURCE_DIV_0,
#if (__SYSTEM_CLOCK == 48000000)
		// Set lower number of accumulation cycles
		// to prevent overflowing counter
		// (Lowers scan time)
		.accumulationCycles = 4000
#else
		.accumulationCycles = 10000
#endif
#endif
};

