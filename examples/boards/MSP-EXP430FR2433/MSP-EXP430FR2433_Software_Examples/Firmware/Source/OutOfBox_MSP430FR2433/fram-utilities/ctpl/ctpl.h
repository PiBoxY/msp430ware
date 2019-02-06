/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef CTPL_H_
#define CTPL_H_

//******************************************************************************
//
//! \addtogroup ctpl_api
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

#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
//
//! Do not allow the CTPL utility to restore a saved state if the device is
//! reset or powered on from a cold start.
//
//******************************************************************************
#define CTPL_DISABLE_RESTORE_ON_RESET   false

//******************************************************************************
//
//! Allow the CTPL utility to restore a saved state if the device is reset or
//! powered on from a cold start.
//
//******************************************************************************
#define CTPL_ENABLE_RESTORE_ON_RESET    true

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 1 millisecond the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_1_MS      ((WDTIS__512 << 8)  | (DIVS__8 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_1_MS      ((WDTIS__512 << 8)  | (DIVS__16 | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 2 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_2_MS      ((WDTIS__8192 << 8) | (DIVS__1  | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_2_MS      ((WDTIS__8192 << 8) | (DIVS__2  | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 4 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_4_MS      ((WDTIS__8192 << 8) | (DIVS__2  | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_4_MS      ((WDTIS__8192 << 8) | (DIVS__4  | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 8 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_8_MS      ((WDTIS__8192 << 8) | (DIVS__4  | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_8_MS      ((WDTIS__8192 << 8) | (DIVS__8  | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 16 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_16_MS     ((WDTIS__8192 << 8) | (DIVS__8 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_16_MS     ((WDTIS__8192 << 8) | (DIVS__16 | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 32 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_32_MS     ((WDTIS__32K << 8) | (DIVS__4 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_32_MS     ((WDTIS__8192 << 8) | (DIVS__32 | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 64 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_64_MS     ((WDTIS__32K << 8)  | (DIVS__8 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_64_MS     ((WDTIS__32K << 8)  | (DIVS__16 | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 128 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_128_MS    ((WDTIS__512K << 8) | (DIVS__1 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_128_MS    ((WDTIS__32K << 8) | (DIVS__32 | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 256 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_256_MS    ((WDTIS__512K << 8) | (DIVS__2  | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_256_MS    ((WDTIS__512K << 8) | (DIVS__4  | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 512 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_512_MS    ((WDTIS__512K << 8) | (DIVS__4  | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_512_MS    ((WDTIS__512K << 8) | (DIVS__8  | DIVM__2))
#endif

//******************************************************************************
//
//! Timeout duration that can be passed to ctpl_enterShutdown(). If the device
//! does not enter BOR after 1024 milliseconds the watchdog timer will reset the
//! device and cause a restore of the saved state.
//
//******************************************************************************
#if defined(__MSP430FR2XX_4XX_FAMILY__)
#define CTPL_SHUTDOWN_TIMEOUT_1024_MS   ((WDTIS__512K << 8) | (DIVS__8 | DIVM__2))
#else
#define CTPL_SHUTDOWN_TIMEOUT_1024_MS   ((WDTIS__512K << 8) | (DIVS__16 | DIVM__2))
#endif

//******************************************************************************
//
//! \brief  Initialize the CTPL utility.
//!
//! This function initializes the utility and must be called at the start of
//! the _system_pre_init function for CCS or the __low_level_init function for
//! IAR. By default these functions are defined in ctpl_pre_init.c but some
//! applications might have their own version of the function. In this case the
//! ctpl_pre_init.c file can be omitted and the function called at the start of
//! the application's low level function.
//!
//! \return none
//
//******************************************************************************
extern void ctpl_init(void);

//******************************************************************************
//
//! \brief  Save state and enter into low power mode LPM3.5.
//!
//! LPM3.5 does not retain the settings of peripheral registers or RAM contents
//! so these settings and states must be saved to non-volatile FRAM. This
//! function will save the state of all the peripherals defined in the include
//! device file, the context of the CPU and the active stack to non-volatile
//! FRAM storage. After saving the state it is marked as valid so that it may be
//! restored after wakeup and the function will enter into LPM3.5. When the
//! device wakes up due to an interrupt or reset/power on event the ctpl_init()
//! function will check if the state is valid and if it should be restored. The
//! restoreOnReset argument determines if state context is restored on a device
//! reset or power on, passing true will always restore the saved state where as
//! passing false will only restore state on a LPM3.5 wakeup from interrupt
//! (returning to the start of main if the device was reset). The saved
//! peripheral states, CPU states and stack are restored from the FRAM storage
//! and the function returns back to the application from where it was called.
//! This function bypasses the need to check at device start up for a LPM3.5
//! wakeup and the application only needs to reinitialize peripherals that are
//! not saved by the utility.
//!
//! This API is functionally the same as ctpl_enterLpm45(). The actual low-power
//! mode used (LPM3.5 or LPM4.5) is determined by the state of the RTC
//! peripheral, LPM3.5 is used if the RTC is enabled and LPM4.5 is used if the
//! RTC is disabled. For more information on low power modes refer to the device
//! datasheet and user's guide.
//!
//! \param  restoreOnReset  Allow the CTPL utility to restore a saved state if
//!                         the device is reset or powered on from a cold start.
//!                         Valid values are:
//!                             - \b CTPL_DISABLE_RESTORE_ON_RESET
//!                             - \b CTPL_ENABLE_RESTORE_ON_RESET
//!
//! \return none
//
//******************************************************************************
extern void ctpl_enterLpm35(bool restoreOnReset);

//******************************************************************************
//
//! \brief  Save state and enter into low power mode LPM4.5.
//!
//! LPM4.5 does not retain the settings of peripheral registers or RAM contents
//! so these settings and states must be saved to non-volatile FRAM. This
//! function will save the state of all the peripherals defined in the include
//! device file, the context of the CPU and the active stack to non-volatile
//! FRAM storage. After saving the state it is marked as valid so that it may be
//! restored after wakeup and the function will enter into LPM4.5. When the
//! device wakes up due to an interrupt or reset/power on event the ctpl_init()
//! function will check if the state is valid and if it should be restored. The
//! restoreOnReset argument determines if state context is restored on a device
//! reset or power on, passing true will always restore the saved state where as
//! passing false will only restore state on a LPM4.5 wakeup from interrupt
//! (returning to the start of main if the device was reset). The saved
//! peripheral states, CPU states and stack are restored from the FRAM storage
//! and the function returns back to the application from where it was called.
//! This function bypasses the need to check at device start up for a LPM4.5
//! wakeup and the application only needs to reinitialize peripherals that are
//! not saved by the utility.
//!
//! This API is functionally the same as ctpl_enterLpm35(). The actual low-power
//! mode used (LPM3.5 or LPM4.5) is determined by the state of the RTC
//! peripheral, LPM3.5 is used if the RTC is enabled and LPM4.5 is used if the
//! RTC is disabled. For more information on low power modes refer to the device
//! datasheet and user's guide.
//!
//! \param  restoreOnReset  Allow the CTPL utility to restore a saved state if
//!                         the device is reset or powered on from a cold start.
//!                         Valid values are:
//!                             - \b CTPL_DISABLE_RESTORE_ON_RESET
//!                             - \b CTPL_ENABLE_RESTORE_ON_RESET
//!
//! \return none
//
//******************************************************************************
extern void ctpl_enterLpm45(bool restoreOnReset);

//******************************************************************************
//
//! \brief  Save the state when power is lost.
//!
//! Device shutdown does not retain the settings of peripheral registers or RAM
//! contents so these settings and states must be saved to non-volatile FRAM.
//! This function will save the state of all the peripherals defined in the
//! include device file, the context of the CPU and the active stack to
//! non-volatile FRAM storage. After saving the state it is marked as valid so
//! that it may be restored after a reset or powering the device back on. All
//! interrupt and wakeup sources are disabled and the device waits in active
//! mode for the SVS to put the device into BOR. MCLK is configured to 4MHz and
//! the SMCLK and WDT_A dividers are set based on the timeout parameter. In this
//! state the only source of a wakeup is a device reset, power up or a shutdown
//! timeout. In all three wakeup scenarios the state is restored and the
//! application resumes. The saved peripheral states, CPU states and stack are
//! restored from the FRAM storage and the function returns back to the
//! application from where it was called.
//!
//! When configuring the shutdown timeout parameter the device supply voltage
//! and ramp conditions should be considered to avoid scenarios where voltage
//! ramps down too slowly. If the timeout duration is not long enough the
//! timeout will trigger a restore before the device enters the BOR state. In
//! this scenario the restored image is no longer valid and the next power on
//! will cause a device reset to the beginning of the main application. To
//! prevent this a timeout duration should be selected so that sufficient time
//! is provided for the supply voltage to ramp down and the timeout only
//! triggers in the scenario where voltage ramps back up to operational levels.
//!
//! This API provides a method for application programmers to efficiently save
//! the application state and shutdown the CPU when a power loss is detected and
//! restore the applications state when the device regains power. The utility
//! includes two examples demonstrating methods for monitoring the device
//! voltage and detecting a power loss.
//!
//! This API only saves and restores RTC_B and RTC_C registers that are not
//! retained in LPMx.5 modes. In device shutdown the context of these other
//! registers must be reinitialized if using these peripherals. See the device
//! users guide for the complete list of RTC registers and details on which are
//! retained.
//!
//! \param  timeout Configurable timeout for a reset if device does not enter
//!                 BOR.
//!                 Valid values are:
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_1_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_2_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_4_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_8_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_16_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_32_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_64_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_128_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_256_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_512_MS
//!                     - \b CTPL_SHUTDOWN_TIMEOUT_1024_MS
//!
//! \return none
//
//******************************************************************************
extern void ctpl_enterShutdown(uint16_t timeout);

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

#endif /* CTPL_H_ */
