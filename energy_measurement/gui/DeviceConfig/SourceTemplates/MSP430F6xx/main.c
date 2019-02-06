<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   main.c
//!
//! \brief  Energy Measurement demo
//!
//!  Group:          MSP Embedded Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: MSP430_ENERGY_MEASUREMENT 
// Release Date: 10Jul2018
//#############################################################################

#include <stdint.h>
#include "hal.h"
#include "hmi.h"
#include "EMLibGUIApp.h"
#include "EM_userConfig.h"

/*** FUNCTIONS ***/
static void MSP_GUI_System_setup(void);

//*****************************************************************************
//
//! In this software main will call a few initialization routines and then it
//!  will jump to the main loop for the Energy Measurement Library.
//!
//! \return none
//
// *****************************************************************************
void main(void)
{
	MSP_GUI_System_setup();
	
	// Initialize the EM Lib to default settings and start conversions
	EMLibGUIApp_Init();
	
	// This is the Design Centers Main Loop
	EMLibGUIApp_Engine();
}

//*****************************************************************************
//
//! This Function Initializes all the ports on the MCU 
//!
//! \return none
//
// *****************************************************************************
static void MSP_GUI_System_setup(void)
{
	// Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
	
    // Initializes the basic functionality of the system
    hal_system_Init();

    // Initialize interfaces to user including GUI
    HMI_Init();
}