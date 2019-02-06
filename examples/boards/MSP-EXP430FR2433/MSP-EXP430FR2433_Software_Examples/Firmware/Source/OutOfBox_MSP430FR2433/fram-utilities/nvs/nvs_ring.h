/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef NVS_RING_H_
#define NVS_RING_H_

//******************************************************************************
//
//! \addtogroup nvs_api_ring
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

//******************************************************************************
//
//! NVS token to identify the corresponding container.
//
//******************************************************************************
#define NVS_RING_TOKEN  0xA533

//******************************************************************************
//
//! Calculate the NVS ring storage size from structure size and number of
//! elements.
//
//******************************************************************************
#define NVS_RING_STORAGE_SIZE(size, num)    \
    (sizeof(nvs_ring_header)+num*(sizeof(uint16_t)+size))

//******************************************************************************
//
//! NVS ring container handle.
//
//******************************************************************************
typedef void *nvs_ring_handle;

//******************************************************************************
//
//! NVS type definition for a non volatile RING storage container.
//
//******************************************************************************
typedef struct nvs_ring_header {
    //! Identifier token.
    uint16_t token;
    //! Index of first ring entry.
    uint16_t first;
    //! Index of last ring entry.
    uint16_t last;
    //! Size of data entry in bytes.
    uint16_t size;
    //! Maximum number of entries in ring storage.
    //!
    uint16_t length;
} nvs_ring_header;

//******************************************************************************
//
//! \brief  Initialize non-volatile data RING storage container
//!
//! This function checks for an existing non-volatile ring container at the
//! given location. If it finds an existing container, it will match the
//! properties of the container and verify the constancy of the container.
//! A CRC check of the most recent data is performed and if everything is match
//! the function will return without any modification to the storage container.
//! In case of a failing CRC an incomplete storage operation is assumed and the
//! ring container is completely analyzed to identify the oldest and most recent
//! data entry. The ring container is then updated to contain the updated
//! first/last information.
//!
//! Only when no container is found, or the properties of the container have
//! changed, or no matching CRC was found, then the container will be
//! initialized with an empty ring buffer.
//!
//! \param  storage        Pointer to NVS data storage with size calculated using
//!                     NVS_RING_STORAGE_SIZE.
//! \param  size        Size of data structure element that is being stored.
//! \param  length        Maximum number of entries in the ring storage.
//! \return             NVS ring container handle
//
//******************************************************************************
extern nvs_ring_handle nvs_ring_init(uint8_t *storage, uint16_t size, uint16_t length);

//******************************************************************************
//
//! \brief  Reset (clear) non-volatile data RING storage container.
//!
//! This function will reset/clear the non-volatile ring container. All CRC values
//! will be invalidated, so no data can be recovered.
//!
//! \param  handle    NVS ring container handle.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_ring_reset(nvs_ring_handle handle);

//******************************************************************************
//
//! \brief  Adds a data entry to the non-volatile RING storage container
//!
//! This function copies the data to the storage container. For integrity checks
//! the CRC of the data is calculated and stored in the container as well.
//!
//! \param  handle    NVS ring container handle.
//! \param  data    Pointer to data structure to add to the storage container.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_ring_add(nvs_ring_handle handle, void *data);

//******************************************************************************
//
//! \brief  Retrieve a specific data entry from the non-volatile ring storage
//!         container.
//!
//! This function does retrieve a specific data entry by copying the data
//! with the given index from the storage container to the data location.
//! An index of 1 does point to the oldest entry and every increment will
//! return one younger data entry.
//! The size of the data is defined during the nvs_log_init function call
//! and is fixed for every nvs_log_container entry.
//! After the data has been copied, the CRC of the data is calculated and
//! compared against the stored CRC value. The result is reflected in the
//! return value.
//!
//! \param  handle    NVS ring container handle.
//! \param  data    Pointer to data structure to populate with retrieved data.
//! \param  index     Index of data to retrieve.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_ring_retrieve(nvs_ring_handle handle, void *data, uint16_t index);

//******************************************************************************
//
//! \brief  Check whether the ring storage container is full.
//!
//! This function does check whether the ring storage container is full.
//!
//! \param  handle    NVS ring container handle.
//! \return         True if NVS ring is full.
//
//******************************************************************************
extern bool nvs_ring_full(nvs_ring_handle handle);

//******************************************************************************
//
//! \brief  Return the max number of entries for this ring container.
//!
//! This function will return the maximum number of allowed entries for the
//! given ring container.
//!
//! \param  handle    NVS ring container handle.
//! \return         Maximum number of allowed entries in ring container.
//
//******************************************************************************
extern uint16_t nvs_ring_max(nvs_ring_handle handle);

//******************************************************************************
//
//! \brief  Return the number of valid entries in the ring container.
//!
//! This function will return the number of valid entries of the ring container.
//!
//! \param  handle    NVS ring container handle.
//! \return         Number of valid entries.
//
//******************************************************************************
extern uint16_t nvs_ring_entries(nvs_log_handle handle);

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

#endif /* NVS_RING_H_ */
