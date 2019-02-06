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
static void hal_system_ClockInit(uint32_t mclkFreq);
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

    hal_system_ClockInit(EM_SMCLK_FREQ_IN_HZ);
    hal_system_GPIOInit();
}

//*****************************************************************************
//
//! Initializes the system clocks
//!
//! This function initializes crystals, DCO and ACLK,SMCLK,MCLK
//!
//! \return none
//
// *****************************************************************************
static void hal_system_ClockInit(uint32_t mclkFreq)
{
    PMM_setVCore(PMM_CORE_LEVEL_3);

    UCS_turnOnLFXT1(UCS_XT1_DRIVE_0, UCS_XCAP_0);

    UCS_initClockSignal(
       UCS_FLLREF,
       UCS_XT1CLK_SELECT,
       UCS_CLOCK_DIVIDER_1);

    UCS_initClockSignal(
       UCS_ACLK,
       UCS_XT1CLK_SELECT,
       UCS_CLOCK_DIVIDER_1);

    UCS_initFLLSettle(
        mclkFreq/1000,
        mclkFreq/32768);
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
