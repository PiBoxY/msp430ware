/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <msp430.h>

#include <ctpl.h>

/*
 * This function will be called before main and initialization of variables.
 * The ctpl_init() function must be called at the start to enable the compute
 * through power loss library. If your application already declares this
 * function you can remove this file and add the call to ctpl_init() at the
 * start of your function.
 */
#if defined(__TI_COMPILER_VERSION__)
int _system_pre_init(void)
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init(void)
#endif
{
    /* Initialize ctpl library */
    ctpl_init();
    
    /* Insert application pre-init code here. */

    return 1;
}
