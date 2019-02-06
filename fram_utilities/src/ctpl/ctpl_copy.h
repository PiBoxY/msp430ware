/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef CTPL_COPY_H_
#define CTPL_COPY_H_

//******************************************************************************
//
//! \addtogroup ctpl_copy_api
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

#include <msp430.h>

#include <stdint.h>
#include <string.h>

//******************************************************************************
//
//! \brief  Perform copy using the CPU or DMA channel zero if available.
//!
//! \param  src Copy source address
//! \param  dst Copy destination address
//! \param  len Word length of copy
//!
//! \return none
//
//******************************************************************************
static inline void ctpl_copy(uint16_t *src, uint16_t *dst, size_t len)
{
#if (defined(__MSP430_HAS_DMAX_3__) || defined(__MSP430_HAS_DMAX_6__))
    /* Perform copy using DMA channel zero. */
    DMA0CTL = 0;
    __data16_write_addr((uint16_t)((uint32_t)&DMA0SA & 0x0FFFF),(uintptr_t)src);
    __data16_write_addr((uint16_t)((uint32_t)&DMA0DA & 0x0FFFF),(uintptr_t)dst);
    DMA0SZ = len;
    DMA0CTL = DMASWDW+DMADT_1+DMASRCINCR_3+DMADSTINCR_3+DMAEN+DMAREQ;
#else
    /* Perform copy using CPU */
    while (len-- > 0) {
        *dst++ = *src++;
    }
#endif

    return;
}

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

#endif /* CTPL_COPY_H_ */
