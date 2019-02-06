<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   hal_system.c
//!
//! \brief  Hardware Abstraction Layer for MSP system including clocks,
//!         watchdog, and GPIOs
//!
//!
//!  Group:          MSP Embedded Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################

#ifndef HAL_SYSTEM_H_
#define HAL_SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>

/* Function prototypes */
//*****************************************************************************
//
//! Initialize the System
//! Includes initialization of clocks, watchdog and GPIOs
//!
//! \return none
//
// *****************************************************************************
extern void hal_system_Init(void);


#endif /* HAL_SYSTEM_H_ */
