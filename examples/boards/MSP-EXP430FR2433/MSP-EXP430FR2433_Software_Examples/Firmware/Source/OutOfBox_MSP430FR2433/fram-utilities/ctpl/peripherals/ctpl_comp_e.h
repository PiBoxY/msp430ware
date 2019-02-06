/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef CTPL_COMP_E_H_
#define CTPL_COMP_E_H_

//******************************************************************************
//
//! \addtogroup ctpl_comp_e_api
//! Compute Through Power Loss COMP-E Peripheral Definition
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
//! Length of FRAM storage for the COMP-E peripheral registers in 16-bit words.
//
//******************************************************************************
#define CTPL_COMP_E_STORAGE_LENGTH          5

//******************************************************************************
//
//! Save function for the COMP-E peripheral.
//!
//! \param  baseAddress Peripheral base address.
//! \param  storage     FRAM storage to save registers to.
//! \param  mode        CTPL mode used.
//!
//! \return none
//
//******************************************************************************
void ctpl_COMP_E_save(uint16_t baseAddress, uint16_t *storage, uint16_t mode);

//******************************************************************************
//
//! Restore function for the COMP-E peripheral.
//!
//! \param  baseAddress Peripheral base address.
//! \param  storage     FRAM storage to save registers to.
//! \param  mode        CTPL mode used.
//!
//! \return none
//
//******************************************************************************
void ctpl_COMP_E_restore(uint16_t baseAddress, uint16_t *storage, uint16_t mode);

//******************************************************************************
//
//! Save function for the COMP-E peripheral.
//
//******************************************************************************
#define ctpl_COMP_E_epilogue                NULL

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

#endif /* CTPL_COMP_E_H_ */
