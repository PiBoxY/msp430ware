/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef CTPL_PORT_INT_H_
#define CTPL_PORT_INT_H_

//******************************************************************************
//
//! \addtogroup ctpl_port_int_api
//! Compute Through Power Loss Interruptible PORT Peripheral Definition
//! @{
//
//******************************************************************************

#include <stdint.h>
#include <stdlib.h>

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

//******************************************************************************
//
//! Length of FRAM storage for the interruptible PORT peripheral registers in 16-bit words.
//
//******************************************************************************
#define CTPL_PORT_INT_STORAGE_LENGTH    7

//******************************************************************************
//
//! Save function for the interruptible PORT peripheral.
//!
//! \param  baseAddress Peripheral base address.
//! \param  storage     FRAM storage to save registers to.
//! \param  mode        CTPL mode used.
//!
//! \return none
//
//******************************************************************************
void ctpl_PORT_INT_save(uint16_t baseAddress, uint16_t *storage, uint16_t mode);

//******************************************************************************
//
//! Restore function for the interruptible PORT peripheral.
//!
//! \param  baseAddress Peripheral base address.
//! \param  storage     FRAM storage to save registers to.
//! \param  mode        CTPL mode used.
//!
//! \return none
//
//******************************************************************************
void ctpl_PORT_INT_restore(uint16_t baseAddress, uint16_t *storage, uint16_t mode);

//******************************************************************************
//
//! Epilogue function for the interruptible PORT peripheral.
//!
//! \param  baseAddress Peripheral base address.
//! \param  storage     FRAM storage to save registers to.
//! \param  mode        CTPL mode used.
//!
//! \return none
//
//******************************************************************************
void ctpl_PORT_INT_epilogue(uint16_t baseAddress, uint16_t *storage, uint16_t mode);

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

#endif /* CTPL_PORT_INT_H_ */
