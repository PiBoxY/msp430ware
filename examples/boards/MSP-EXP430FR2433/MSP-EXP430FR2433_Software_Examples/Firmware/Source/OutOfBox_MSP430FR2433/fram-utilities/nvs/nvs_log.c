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

nvs_log_handle nvs_log_init(uint8_t *storage, uint16_t size, uint16_t length)
{
    uint8_t *dataStorage;
    uint16_t crc;
    uint16_t index;
    uint16_t *crcTable;
    uint16_t framState;
    nvs_log_header *header;

    // Initialize local variables
    index = 0xffff;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_log_header *)storage;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_log_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*length);

    // Check status of log container
    if ((header->token == NVS_LOG_TOKEN) && (header->size == size) && (header->length == length)) {
        // Check index of last entry
        index = header->index;
        if ((index != 0xffff) && (index < header->length)) {
            // Check CRC and return if entry is valid
            crc = nvs_crc(dataStorage+(size*(index)), size);
            if ((crcTable[index] == crc) && ((crcTable[index+1]  == 0) || (index == header->length-1))) {
                return (nvs_log_handle)header;
            }
        }

        // Reset index and try to recover last entry using CRC
        index = 0;
        while ((index < length) && (nvs_crc(dataStorage, size) == crcTable[index])) {
            dataStorage += header->size;
            index++;
        }
        index -= 1;
    }

    // Unlock FRAM
    framState = nvs_unlockFRAM();

    // Initialize NVS log header
    header->token = NVS_LOG_TOKEN;
    header->index = index;
    header->size = size;
    header->length = length;

    // Clear all CRC checksum from empty log entries
    while (++index < length) {
        crcTable[index] = 0;
    }

    // Lock FRAM
    nvs_lockFRAM(framState);

    // Return NVS log handle
    return (nvs_log_handle)header;
}

nvs_status nvs_log_reset(nvs_log_handle handle)
{
    uint16_t *crcTable;
    uint16_t framState;
    nvs_status status;
    nvs_log_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_log_header *)handle;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_log_header));

    // Check the header is valid using the token
    if (header->token == NVS_LOG_TOKEN) {
        // Unlock FRAM
        framState = nvs_unlockFRAM();

        // Clear all CRC checksum for all log entries
        nvs_fill(crcTable, 0, header->length*sizeof(crcTable[0]));

        // Reset index
        header->index = 0xffff;

        // Lock FRAM
        nvs_lockFRAM(framState);

        // Set return status
        status = NVS_OK;

    }

    // Return status
    return status;
}

nvs_status nvs_log_add(nvs_log_handle handle, void *data)
{
    uint8_t *dataStorage;
    uint16_t next;
    uint16_t length;
    uint16_t *crcTable;
    uint16_t framState;
    nvs_status status;
    nvs_log_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_log_header *)handle;
    length = header->length;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_log_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*length);

    // Check the header is valid using the token
    if (header->token == NVS_LOG_TOKEN) {
        // Calculate the next index and check if the log has space
        next = header->index + 1;
        if (next < length) {
            // Unlock FRAM
            framState = nvs_unlockFRAM();

            // Increment data storage pointer, copy data and increment index
            dataStorage += header->size * next;
            nvs_copy(data, dataStorage, header->size);
            crcTable[next] = nvs_crc(data, header->size);
            header->index = next;

            // Lock FRAM
            nvs_lockFRAM(framState);

            // Set return status
            status = NVS_OK;
        }
        else {
            // Log is full, set return status
            status = NVS_FULL;
        }
    }

    // Return status
    return status;
}

nvs_status nvs_log_retrieve(nvs_log_handle handle, void *data, uint16_t index)
{
    uint8_t *dataStorage;
    uint16_t crc;
    uint16_t *crcTable;
    nvs_status status;
    nvs_log_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_log_header *)handle;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_log_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*header->length);

    // Check the header is valid using the token
    if (header->token == NVS_LOG_TOKEN) {
        // Check the index is within range
        if ((header->index != 0xffff) && (index <= header->index)) {
            // Increment data storage pointer, retrieve data and check CRC
            dataStorage += header->size * index;
            nvs_copy(dataStorage, data, header->size);
            crc = nvs_crc(data, header->size);
            status = (crc == crcTable[index]) ? NVS_OK : NVS_CRC_ERROR;
        }
        else {
            // Index out of range, set return status
            status = NVS_INDEX_OUT_OF_BOUND;
        }
    }

    // Return status
    return status;
}

bool nvs_log_full(nvs_log_handle handle)
{
    nvs_status status;
    nvs_log_header *header;

    // Calculate pointer to header
    header = (nvs_log_header *)handle;

    // Check if the log is full
    status = (header->index+1 >= header->length) ? NVS_FULL : NVS_OK;

    // Return status
    return status;
}

uint16_t nvs_log_max(nvs_log_handle handle)
{
    nvs_log_header *header;

    // Calculate pointer to header
    header = (nvs_log_header *)handle;

    // Return length of log
    return header->length;
}

uint16_t nvs_log_entries(nvs_log_handle handle)
{
    nvs_log_header *header;

    // Calculate pointer to header
    header = (nvs_log_header *)handle;

    // Return number of entries
    return (header->index + 1);
}
