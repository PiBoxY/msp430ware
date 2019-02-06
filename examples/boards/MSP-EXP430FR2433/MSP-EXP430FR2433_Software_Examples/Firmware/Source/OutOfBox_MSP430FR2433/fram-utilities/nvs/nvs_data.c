/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <stdint.h>
#include <stdbool.h>

#include "nvs.h"
#include "nvs_support.h"

nvs_data_handle nvs_data_init(uint8_t *storage, uint16_t size)
{
    uint8_t *data1;
    uint8_t *data2;
    uint16_t crc;
    uint16_t init_crc1;
    uint16_t init_crc2;
    nvs_data_status init_status;
    nvs_data_header *header;

    // Initialize local variables
    init_status = NVS_DATA_INIT;
    init_crc1 = 0;
    init_crc2 = 0;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_data_header *)storage;
    data1 = (uint8_t *)header + sizeof(nvs_data_header);
    data2 = data1 + size;

    // check status of current container
    if ((header->token == NVS_DATA_TOKEN) && (header->size == size)) {
        switch (header->status) {
        // Storage 1 contains latest data
        case NVS_DATA_1:
            // Get CRC of storage 1 and check if it matches
            crc = nvs_crc(data1, header->size);
            if (crc == header->crc1) {
                // Matching checksum, return NVS data handle
                return (nvs_data_handle)header;
            }
            else {
                // Get CRC of storage 2 and check if it matches
                crc = nvs_crc(data2, header->size);
                if (crc == header->crc2) {
                    init_status = NVS_DATA_2;
                    init_crc2 = crc;
                }
            }
            break;
        // Storage 2 contains latest data
        case NVS_DATA_2:
            // Get CRC of storage 1 and check if it matches
            crc = nvs_crc (data2, header->size);
            if (crc == header->crc2) {
                // Matching checksum, return NVS data handle
                return (nvs_data_handle)header;
            }
            else {
                // Get CRC of storage 1 and check if it matches
                crc = nvs_crc (data1, header->size);
                if (crc == header->crc1) {
                    init_status = NVS_DATA_1;
                    init_crc1   = crc;
                }
            }
            break;
        default:
            // Try to recover status based on valid CRC signature
            // since last status is not known, recovering any valid
            // CRC is better than losing all information
            crc = nvs_crc(data1, header->size);
            if (crc == header->crc1) {
                init_status = NVS_DATA_1;
                init_crc1   = crc;
            }
            crc = nvs_crc (data2, header->size);
            if (crc == header->crc2) {
                init_status = NVS_DATA_2;
                init_crc2   = crc;
            }
            break;
        }
    }

    // Unlock FRAM
    uint16_t framState = nvs_unlockFRAM();

    // Initialize NVS data header
    header->token = NVS_DATA_TOKEN;
    header->status = init_status;
    header->size = size;
    header->crc1 = init_crc1;
    header->crc2 = init_crc2;

    // Lock FRAM
    nvs_lockFRAM(framState);

    // Return NVS data handle
    return (nvs_data_handle)header;
}

nvs_status nvs_data_restore(nvs_data_handle handle, void *data)
{
    uint16_t crc;
    uint8_t *data1;
    uint8_t *data2;
    nvs_status status;
    nvs_data_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_data_header *)handle;
    data1 = (uint8_t *)header + sizeof(nvs_data_header);
    data2 = data1 + header->size;

    // Process NVS header token
    if (header->token == NVS_DATA_TOKEN) {
        switch (header->status) {
        case NVS_DATA_INIT:
            // NVS storage is empty, zero data
            nvs_fill(data, 0, 2*header->size);
            status = NVS_EMPTY;
            break;
        case NVS_DATA_1:
            // Copy data and check CRC
            nvs_copy(data1, data, header->size);
            crc = nvs_crc(data, header->size);
            status = (crc == header->crc1) ? NVS_OK : NVS_CRC_ERROR;
            break;
        case NVS_DATA_2:
            // Copy data and check CRC
            nvs_copy(data2, data, header->size);
            crc = nvs_crc(data, header->size);
            status = (crc == header->crc2) ? NVS_OK : NVS_CRC_ERROR;
            break;
        default: break;
        }
    }

    // Return status
    return status;
}

nvs_status nvs_data_commit(nvs_data_handle handle, void *data)
{
    uint8_t *data1;
    uint8_t *data2;
    uint16_t framState;
    nvs_status status;
    nvs_data_header *header;

    // Initialize status and header pointer
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_data_header *)handle;
    data1 = (uint8_t *)header + sizeof(nvs_data_header);
    data2 = data1 + header->size;

    // Process NVS header token
    if (header->token == NVS_DATA_TOKEN) {
        // Unlock FRAM
        framState = nvs_unlockFRAM();

        switch (header->status) {
        case NVS_DATA_INIT:
        case NVS_DATA_2:
            // Commit to data1 and set CRC
            nvs_copy(data, data1, header->size);
            header->crc1 = nvs_crc(data, header->size);
            header->crc2 = 0;
            header->status = NVS_DATA_1;
            status = NVS_OK;
            break;
        case NVS_DATA_1:
            // Commit to data2 and set CRC
            nvs_copy(data, data2, header->size);
            header->crc2 = nvs_crc(data, header->size);
            header->crc1 = 0;
            header->status = NVS_DATA_2;
            status = NVS_OK;
        default: break;
        }

        // Lock FRAM
        nvs_lockFRAM(framState);
    }

    // Return status
    return status;
}
