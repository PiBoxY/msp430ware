/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef XXHASH_H_
#define XXHASH_H_

//******************************************************************************
//
//! \addtogroup xxhash_api
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
//! \brief Compute xxhash checksum for a block of data.
//!
//! Used to compute and verify checksums for the LZ4 frame and block format.
//! This function has been optimized for MSP430 but it not as efficient as
//! hardware CRC16 or CRC32 calculation.
//!
//! \param src Pointer to the data block.
//! \param length Length of data block.
//! \param seed Initialization value.
//! \return The computed xxhash checksum.
//
//******************************************************************************
extern uint32_t xxhash_compute(const void *src, uint32_t length, uint32_t seed);

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

#endif /* XXHASH_H_ */
