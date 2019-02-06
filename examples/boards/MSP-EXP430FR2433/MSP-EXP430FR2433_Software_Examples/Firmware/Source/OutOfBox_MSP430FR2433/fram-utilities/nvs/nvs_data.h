/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef NVS_DATA_H_
#define NVS_DATA_H_

//******************************************************************************
//
//! \addtogroup nvs_api_data
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
#define NVS_DATA_TOKEN          0xA531

//******************************************************************************
//
//! macros to calculate NVS container size
//
//******************************************************************************
#define NVS_DATA_STORAGE_SIZE(size)    \
    (sizeof(nvs_data_header)+2*(sizeof(uint16_t)+size))

//******************************************************************************
//
//! NVS data container handle.
//
//******************************************************************************
typedef void *nvs_data_handle;

//******************************************************************************
//
//! NVS data status flags.
//
//******************************************************************************
typedef enum {
    //! Successful operation.
    NVS_DATA_INIT = 0,
    //! Storage 1 contains the latest data.
    NVS_DATA_1 = 1,
    //! Storage 2 contains the latest data.
    NVS_DATA_2 = 2
} nvs_data_status;

//******************************************************************************
//
//! NVS header for a non-volatile data storage container.
//
//******************************************************************************
typedef struct nvs_data_header {
    //! Identifier token.
    uint16_t token;
    //! Storage status.
    nvs_data_status    status;
    //! Size of data entry in byte.
    uint16_t size;
    //! CRC for storage 1.
    uint16_t crc1;
    //! CRC for storage 2.
    //!
    uint16_t crc2;
} nvs_data_header;

//******************************************************************************
//
//! \brief  Initialize non-volatile data storage container
//!
//! This function checks for an existing non-volatile data container at the
//! given location. If it finds an existing container, it will match the
//! properties of the container and verify the constancy of the container.
//! A CRC check of the data is performed and if everything is match the
//! function will return without any modification to the storage container.
//! In case of a failing CRC an incomplete storage commit is assumed and the
//! alternate storage buffer is checked. If the CRC check is OK, the status is
//! updated and will point to the alternate storage buffer.
//! Only when no container is found, or the properties of the container have
//! changed, or no matching CRC was found, then the container will be
//! initialized.
//!
//! Example non-volatile-storage space and function call:
//! - unit8_t nvs_data_container[NVS_DATA_STORAGE_SIZE(sizeof(DATA))];
//! - nvs_data_init(nvs_data_container, sizeof(DATA));
//!
//! \param  storage        Pointer to NVS data storage with size calculated using
//!                     NVS_DATA_STORAGE_SIZE.
//! \param  size        Size of data structure element that is being stored.
//! \return             NVS data container handle.
//
//******************************************************************************
extern nvs_data_handle nvs_data_init(uint8_t *storage, uint16_t size);

//******************************************************************************
//
//! \brief  Restore a data entry from the non-volatile data storage container
//!
//! This function does restore a data entry by copying the most recent data
//! from the storage container to the data location. The size of the data
//! is defined during the nvs_data_init function call and is fixed for every
//! nvs_data_container.
//! After the data has been copied, the CRC of the data is calculated and
//! compared against the stored CRC value. The result is reflected in the
//! return value.
//!
//! \param  handle    NVS data container handle.
//! \param  data    Pointer to a data structure that will hold the data after
//!                 the restore operation.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_data_restore(nvs_data_handle handle, void *data);

//******************************************************************************
//
//! \brief  Commit a data entry to the non-volatile storage container
//!
//! This function copies the data to the storage container. For integrity checks
//! the CRC of the data is calculated and stored in the container as well.
//!
//! \param  handle    NVS data container handle.
//! \param  data    Pointer to a data structure that holds the data to be added
//!                    to the storage container.
//! \return         Status of the NVS operation.
//
//******************************************************************************
extern nvs_status nvs_data_commit(nvs_data_handle handle, void *data);

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

#endif /* NVS_DATA_H_ */
