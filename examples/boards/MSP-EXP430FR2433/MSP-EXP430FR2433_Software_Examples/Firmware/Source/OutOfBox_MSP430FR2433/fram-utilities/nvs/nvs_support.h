/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef NVS_SUPPORT_H_
#define NVS_SUPPORT_H_

//******************************************************************************
//
//! \addtogroup nvs_api_support
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
#include <string.h>

//******************************************************************************
//
//! \brief  Fill memory with a single value
//!
//! Fill memory with a single value. Use memset by default but DMA could be used
//! used for more efficient fill operations when a channel is available.
//!
//! \param  dst     Pointer to destination to fill.
//! \param  value   Value to fill destination with.
//! \param  length  Length of destination array.
//! \return         none
//
//******************************************************************************
static inline void nvs_fill(void *dst, uint8_t value, uint16_t length)
{
    memset(dst, value, length);
}

//******************************************************************************
//
//! \brief  Copy data between memory locations
//!
//! Copy data between memory locations. Use memcpy by default but DMA could be
//! used for more efficient copy operations when a channel is available.
//!
//! \param  src     Pointer to source to copy.
//! \param  dst     Pointer to destination to copy to.
//! \param  length  Length of source and destination arrays.
//! \return         none
//
//******************************************************************************
static inline void nvs_copy(const void *src, void *dst, uint16_t length)
{
    memcpy(dst, src, length);
}

//******************************************************************************
//
//! \brief  Calculate a 16-bit CRC over a storage buffer in bytes
//!
//! \param  data    Pointer to data to calculate CRC on.
//! \param  size    Length of data array.
//! \return         none
//
//******************************************************************************
uint16_t nvs_crc(void *data, uint16_t size);

//******************************************************************************
//
//! \brief  Unlock FRAM for writing
//!
//! Clear the FRAM program write protection bit and return the original status
//! of the bit. The return of this function can be used to restore the previous
//! FRAM protection state with nvs_lockFRAM().
//!
//! \return         FRAM state that can be passed into nvs_lockFRAM()
//
//******************************************************************************
inline uint16_t nvs_unlockFRAM(void);

//******************************************************************************
//
//! \brief  Lock FRAM after writing
//!
//! Restore the previous FRAM protection state with the state returned from
//! nvs_unlockFRAM().
//!
//! \param  state   FRAM state returned from nvs_unlockFRAM().
//! \return         none
//
//******************************************************************************
inline void nvs_lockFRAM(uint16_t state);

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

#endif /* NVS_SUPPORT_H_ */
