/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef CTPL_LOW_LEVEL_H_
#define CTPL_LOW_LEVEL_H_

//******************************************************************************
//
//! \addtogroup ctpl_low_level_api
//! @{
//
//******************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef CTPL_STACK_SIZE
#error CTPL: You must define the stack size being used (--define=CTPL_STACK_SIZE=160)
#endif

//******************************************************************************
//
//! Bits that define no CTPL mode.
//
//******************************************************************************
#define CTPL_MODE_NONE              0x0000

//******************************************************************************
//
//! Bits that define the LPM3.5 CTPL mode.
//
//******************************************************************************
#define CTPL_MODE_LPM35             0x0001

//******************************************************************************
//
//! Bits that define the LPM4.5 mode.
//
//******************************************************************************
#define CTPL_MODE_LPM45             0x0002

//******************************************************************************
//
//! Bits that define the shutdown CTPL mode.
//
//******************************************************************************
#define CTPL_MODE_SHUTDOWN          0x0003

//******************************************************************************
//
//! Bit mask for CTPL mode.
//
//******************************************************************************
#define CTPL_MODE_BITS              0x0003

//******************************************************************************
//
//! Bits that define the optional restoreOnReset flag.
//
//******************************************************************************
#define CTPL_MODE_RESTORE_RESET     0x0010

//******************************************************************************
//
//! Bits that define the LPM3.5 and LPM4.5 wakeup flags.
//
//******************************************************************************
#define CTPL_MODE_LPMX5_WAKEUP      0x0100

//******************************************************************************
//
//! Size of RAM contents to save to FRAM. By default this is set to the entire
//! RAM contents for FR2xx and FR4xx devices and disabled for all other
//! devices. This setting can be overriden by manually defining in the compiler
//! options (--define=CTPL_RAM_SIZE=864).
//
//******************************************************************************
#if !defined(CTPL_RAM_SIZE)
#if defined(__MSP430FR2032__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2033__)
#define CTPL_RAM_SIZE                (0x0800 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2110__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2111__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2310__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2311__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2433__)
#define CTPL_RAM_SIZE                (0x1000 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2532__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2533__)
#define CTPL_RAM_SIZE                (0x0800 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2632__)
#define CTPL_RAM_SIZE                (0x0800 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR2633__)
#define CTPL_RAM_SIZE                (0x1000 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR4131__)
#define CTPL_RAM_SIZE                (0x0200 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR4132__)
#define CTPL_RAM_SIZE                (0x0400 - CTPL_STACK_SIZE)
#elif defined(__MSP430FR4133__)
#define CTPL_RAM_SIZE                (0x0800 - CTPL_STACK_SIZE)
#endif
#endif

#ifndef __IAR_SYSTEMS_ASM__

#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
//
//! \brief Low level assembly function used to save the state and enter LPM.
//!
//! This assembly function saves the CPU state and stack into non-volatile FRAM
//! before setting the state as valid and entering into the low-power mode
//! defined by ctpl_mode. On device reset with a valid state ctpl_init will jump
//! back to this function which restores the CPU state and stack from the FRAM
//! copy. After restoring the state the function returns to the higher-level
//! CTPL function that was invoked by the main application.
//!
//! This function is only intended to be called from within the library code,
//! the user does not need to invoke this function manually.
//!
//! \param  mode     CTPL modes and flags.
//!                 Valid flags are:
//!                     - \b CTPL_MODE_NONE
//!                     - \b CTPL_MODE_LPM35
//!                     - \b CTPL_MODE_LPM45
//!                     - \b CTPL_MODE_SHUTDOWN
//!                     - \b CTPL_MODE_RESTORE_RESET
//!                     - \b CTPL_MODE_LPMX5_WAKEUP
//!
//! \param  timeout Configurable timeout for a reset if device does not enter
//!                 BOR.
//!                 Valid values are:
//!                     - \b CTPL_POWERLOSS_TIMEOUT_1_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_2_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_4_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_8_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_16_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_32_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_64_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_128_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_256_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_512_MS
//!                     - \b CTPL_POWERLOSS_TIMEOUT_1024_MS
//!
//! \return mode    CTPL mode and flags.
//
//******************************************************************************
extern uint16_t ctpl_saveCpuStackEnterLpm(uint16_t mode, uint16_t timeout);

#endif /* __IAR_SYSTEMS_ASM__ */

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#endif /* CTPL_LOW_LEVEL_H_ */
