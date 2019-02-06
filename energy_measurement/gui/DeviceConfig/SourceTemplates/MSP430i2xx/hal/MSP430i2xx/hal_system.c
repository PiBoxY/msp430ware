<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   hal_system.c
//!
//! \brief  Hardware Abstraction Layer for MSP system including clocks,
//!         watchdog, and GPIOs
//!
//!  Group:          MSP Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "hal.h"

//
// Local function prototypes
//
static void hal_system_GPIOInit(void);

//
// Function declarations
//
void hal_system_Init(void)
{
    uint8_t i;

    // Configure all the phase GPIOs
    for(i=0;i<gEmSWConfig.systemConfig->pulseArrayLength;i++)
    {
        GPIO_setAsOutputPin(
                gEmSWConfig.systemConfig->pulseArray[i].activePulseGpioPort,
                gEmSWConfig.systemConfig->pulseArray[i].activePulseGpioPin
        );

        GPIO_setAsOutputPin(
                gEmSWConfig.systemConfig->pulseArray[i].reactivePulseGpioPort,
                gEmSWConfig.systemConfig->pulseArray[i].reactivePulseGpioPin
        );

        GPIO_setOutputHighOnPin(gEmSWConfig.systemConfig->pulseArray[i].activePulseGpioPort,
                                gEmSWConfig.systemConfig->pulseArray[i].activePulseGpioPin);

        GPIO_setOutputHighOnPin(gEmSWConfig.systemConfig->pulseArray[i].reactivePulseGpioPort,
                                gEmSWConfig.systemConfig->pulseArray[i].reactivePulseGpioPin);
    }

    hal_system_GPIOInit();
}


//
// Local Function declarations
//
//*****************************************************************************
//
//! Initializes GPIOs
//!
//! \return none
//
// *****************************************************************************
static void hal_system_GPIOInit(void)
{
<EMDC_HAL_SYSTEM_GPIO>
}
