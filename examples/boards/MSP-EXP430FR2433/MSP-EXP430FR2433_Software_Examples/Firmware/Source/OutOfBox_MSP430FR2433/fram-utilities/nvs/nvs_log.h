/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef NVS_LOG_H_
#define NVS_LOG_H_

//******************************************************************************
//
//! \addtogroup nvs_api_log
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
//! NVS token to identify the corresponding container.
//
//******************************************************************************
#define NVS_LOG_TOKEN                   0xA532

//******************************************************************************
//
//! Calculate the NVS log storage size from structure size and number of
//! elements.
//
//******************************************************************************
#define NVS_LOG_STORAGE_SIZE(size, num)    \
    (sizeof(nvs_log_header)+num*(sizeof(uint16_t)+size))

//******************************************************************************
//
//! NVS log container handle.
//
//******************************************************************************
typedef void *nvs_log_handle;

//******************************************************************************
//
//! NVS type definition for a non volatile LOG storage container.
//
//******************************************************************************
typedef struct nvs_log_header {
    //! Identifier token.
    uint16_t token;
    //! Index of last log entry.
    uint16_t index;
    //! Size of data entry in bytes.
    uint16_t size;
    //! Maximum number of entries in log storage.
    //!
    uint16_t length;
} nvs_log_header;

//******************************************************************************
//
//! \brief  Initialize non-volatile data LOG storage container
//!
//! This function checks for an existing non-volatile log container at the
//! given location. If it finds an existing container, it will match the
//! properties of the container and verify the constancy of the container.
//! A CRC check of the most recent data is performed and if everything is match
//! the function will return without any modification to the storage container.
//! In case of a failing CRC an incomplete storage operation is assumed and the
//! log container is verified for consistency from the beginning. The last good
//! known data/CRC match is used as the new end of log index.
//!
//! Only when no container is found, or the properties of the container have
//! changed, or no matching CRC was found, then the container will be
//! initialized.
//!
//! \param  storage        Pointer to NVS data storage with size calculated using
//!                     NVS_LOG_STORAGE_SIZE.
//! \param  size        Size of data structure element that is being stored.
//! \param  length        Maximum number of entries in the log storage.
//! \return             NVS log container handle
//
//******************************************************************************
extern nvs_log_handle nvs_log_init(uint8_t *storage, uint16_t size, uint16_t length);

//******************************************************************************
//
//! \brief  Reset (clear) non-volatile data LOG storage container.
//!
//! This function will reset/clear the non-volatile log container. All CRC
//! values will be invalidated, so no data can be recovered.
//!
//! \param  handle    NVS log container handle.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_log_reset(nvs_log_handle handle);

//******************************************************************************
//
//! \brief  Adds a data entry to the non-volatile LOG storage container.
//!
//! This function copies the data to the storage container. For integrity checks
//! the CRC of the data is calculated and stored in the container as well.
//!
//! \param  handle    NVS log container handle.
//! \param  data    Pointer to data structure to add to the storage container.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_log_add(nvs_log_handle handle, void *data);

//******************************************************************************
//
//! \brief  Retrieve a specific data entry from the non-volatile log storage container
//!
//! This function does retrieve a specific data entry by copying the data
//! with the given index from the storage container to the data location. The
//! size of the data is defined during the nvs_log_init function call and is
//! fixed for every nvs_log_container entry.
//! After the data has been copied, the CRC of the data is calculated and
//! compared against the stored CRC value. The result is reflected in the
//! return value.
//!
//! \param  handle    NVS log container handle.
//! \param  data    Pointer to data structure to populate with retrieved data.
//! \param  index     Index of data to retrieve.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_log_retrieve(nvs_log_handle handle, void *data, uint16_t index);

//******************************************************************************
//
//! \brief  Check whether the log storage container is full.
//!
//! This function does check whether the log storage container is full.
//!
//! \param  handle    NVS log container handle.
//! \return         True if NVS log is full.
//
//******************************************************************************
extern bool nvs_log_full(nvs_log_handle handle);

//******************************************************************************
//
//! \brief  Return the max number of entries for this log container.
//!
//! This function will return the maximum number of allowed entries for the
//! given log container.
//!
//! \param  handle    NVS log container handle.
//! \return         Maximum number of allowed entries in log container.
//
//******************************************************************************
extern uint16_t nvs_log_max(nvs_log_handle handle);

//******************************************************************************
//
//! \brief  Return the number of valid entries in the log container.
//!
//! This function will return the number of valid entries of the log container.
//!
//! \param  handle    NVS log container handle.
//! \return         Number of valid entries.
//
//******************************************************************************
extern uint16_t nvs_log_entries(nvs_log_handle handle);

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

#endif /* NVS_LOG_H_ */
