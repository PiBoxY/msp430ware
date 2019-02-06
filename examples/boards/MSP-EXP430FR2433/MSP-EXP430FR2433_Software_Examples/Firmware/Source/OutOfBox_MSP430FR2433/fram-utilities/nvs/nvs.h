/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef NVS_H_
#define NVS_H_

//******************************************************************************
//
//! \addtogroup nvs_api
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

//******************************************************************************
//
//! \brief NVS return code and status information.
//
//******************************************************************************
typedef enum {
    //! Successful operation
    NVS_OK = 0,
    //! NVS storage format is corrupted
    NVS_NOK = 1,
    //! Index is out of bounds
    NVS_INDEX_OUT_OF_BOUND = 2,
    //! Data checksum in incorrect
    NVS_CRC_ERROR = 3,
    //! NVS storage is empty
    NVS_EMPTY = 4,
    //! NVS storage is full
    NVS_FULL = 5
} nvs_status;

#include "nvs_data.h"
#include "nvs_log.h"
#include "nvs_ring.h"

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

#endif /* NVS_H_ */
