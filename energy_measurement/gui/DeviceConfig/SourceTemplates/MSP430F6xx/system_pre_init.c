<EMDC_COPYRIGHT>
//#############################################################################
//
//! \file   system_pre_init.c
//!
//! \brief  Disables the watchdog timer.
//!
//!  Group:          MSP Software Development
<EMDC_DEVICE_NAME>
//!
//!  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
#include "msp430.h"

#ifdef __TI_COMPILER_VERSION__
int _system_pre_init(void)
#elif __IAR_SYSTEMS_ICC__
int __low_level_init(void)
#elif __GNUC__
extern int system_pre_init(void) __attribute__((constructor));
int system_pre_init(void)
#else
#error Compiler not supported!
#endif
{
	WDTCTL = WDTPW + WDTHOLD;     // Stop WDT

    /* Return value:
     *  1 - Perform data segment initialization.
     *  0 - Skip data segment initialization.
     */
    return 1;
}
