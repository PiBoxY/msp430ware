/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <stdint.h>

#include "nvs.h"
#include "nvs_support.h"

// Helper function for incrementing ring buffer index
static inline uint16_t __nvs_ring_increment(uint16_t index, uint16_t count, uint16_t length)
{
    uint16_t next;

    next = index + count;
    while (next >= length) {
        next -= length;
    }
    return next;
}

nvs_ring_handle nvs_ring_init(uint8_t *storage, uint16_t size, uint16_t length)
{
    uint8_t *dataStorage;
    uint8_t *dataPtr;
    uint16_t last;
    uint16_t next;
    uint16_t first;
    uint16_t crcLast;
    uint16_t crcFirst;
    uint16_t *crcTable;
    uint16_t framState;
    nvs_ring_header *header;

    // Initialize local variables
    first = 0xffff;
    last = 0xffff;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_ring_header *)storage;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_ring_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*length);

    // Check status of ring container
    if ((header->token == NVS_RING_TOKEN) && (header->size == size) && (header->length == length)) {
        // Check first and last index are within range
        first = header->first;
        last = header->last;
        if ((first < length) && (last < length)) {
            // Check first, last and next CRC and return if entries are valid
            next = __nvs_ring_increment(last, 1, length);
            crcLast = nvs_crc(dataStorage+(size*last), size);
            crcFirst = nvs_crc(dataStorage+(size*first), size);
            if ((crcTable[first] == crcFirst) && (crcTable[last] == crcLast) && ((next == first) || (crcTable[next] == 0))) {
                return (nvs_ring_handle)header;
            }
        }

        // Reset indices and try to recover last entry using CRC
        if (nvs_crc(dataStorage, size) == crcTable[0]) {
            // First CRC is valid, find last valid entry first
            last = 1;
            dataPtr = dataStorage + size;
            while ((last < length) && (nvs_crc(dataPtr, size) == crcTable[last])) {
                last++;
                dataPtr += size;
            }
            last--;

            // Find first valid entry
            first = length-1;
            dataPtr = dataStorage + size*(length-1);
            while ((first > last) && (nvs_crc(dataPtr, size) == crcTable[first])) {
                first--;
                dataPtr -= size;
            }
            first++;
        }
        else {
            // First CRC is not valid, find first valid entry
            first = 1;
            dataPtr = dataStorage + size;
            while ((first < length) && (nvs_crc(dataPtr, size) != crcTable[first])) {
                first++;
                dataPtr += size;
            }

            if (first != length) {
                // First valid entry found, find the last valid entry
                last = first + 1;
                dataPtr = dataStorage + size*last;
                while ((last < length) && (nvs_crc(dataPtr, size) == crcTable[last])) {
                    last++;
                    dataPtr += size;
                }
                last--;
            }
            else {
                // No valid entries found, reset first and last
                first = 0xffff;
                last = 0xffff;
            }
        }
    }

    // Unlock FRAM
    framState = nvs_unlockFRAM();

    // Initialize NVS ring header
    header->token = NVS_RING_TOKEN;
    header->first = first;
    header->last = last;
    header->size = size;
    header->length = length;

    // Clear CRC checksum
    if (first == 0xffff) {
        // Clear all CRC checksum for all ring entries
        nvs_fill(crcTable, 0, length*sizeof(crcTable[0]));
    }
    else {
        // Clear all CRC checksum from empty ring entries
        last = __nvs_ring_increment(last, 1, length);
        while (last != first) {
            crcTable[last] = 0;
            last = __nvs_ring_increment(last, 1, length);
        }
    }

    // Lock FRAM
    nvs_lockFRAM(framState);

    // Return NVS ring handle
    return (nvs_ring_handle)header;
}

nvs_status nvs_ring_reset(nvs_ring_handle handle)
{
    uint16_t length;
    uint16_t *crcTable;
    uint16_t framState;
    nvs_status status;
    nvs_ring_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_ring_header *)handle;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_ring_header));

    // Initialize local variables
    length = header->length;

    // Check the header is valid using the token
    if (header->token == NVS_RING_TOKEN) {
        // Unlock FRAM
        framState = nvs_unlockFRAM();

        // Clear all CRC checksum for all ring entries
        nvs_fill(crcTable, 0, length*sizeof(crcTable[0]));

        // Reset first and last indices
        header->first = 0xffff;
        header->last = 0xffff;

        // Lock FRAM
        nvs_lockFRAM(framState);

        // Set return status
        status = NVS_OK;

    }

    // Return status
    return status;
}

nvs_status nvs_ring_add(nvs_ring_handle handle, void *data)
{
    uint8_t *dataStorage;
    uint16_t size;
    uint16_t last;
    uint16_t next;
    uint16_t first;
    uint16_t length;
    uint16_t *crcTable;
    uint16_t framState;
    nvs_status status;
    nvs_ring_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_ring_header *)handle;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_ring_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*header->length);

    // Initialize local variables
    first = header->first;
    last = header->last;
    size = header->size;
    length = header->length;

    // Check the header is valid using the token
    if (header->token == NVS_RING_TOKEN) {
        // Unlock FRAM
        framState = nvs_unlockFRAM();

        // Calculate the next index from last entry
        next = __nvs_ring_increment(last, 1, length);
        if (next == first) {
            // Ring storage is full, clear first CRC and increment index
            crcTable[next] = 0;
            header->first = __nvs_ring_increment(next, 1, length);
        }

        // Copy data and update last index
        dataStorage += size * next;
        nvs_copy(data, dataStorage, size);
        crcTable[next] = nvs_crc(data, size);
        header->last = next;
        
        // Check if this is the first entry
        if (first == 0xffff) {
            header->first = next;
        }

        // Lock FRAM
        nvs_lockFRAM(framState);

        // Set return status
        status = NVS_OK;
    }

    // Return status
    return status;
}

nvs_status nvs_ring_retrieve(nvs_ring_handle handle, void *data, uint16_t index)
{
    uint8_t *dataStorage;
    uint16_t crc;
    uint16_t size;
    uint16_t first;
    uint16_t length;
    uint16_t *crcTable;
    nvs_status status;
    nvs_ring_header *header;

    // Initialize status
    status  = NVS_NOK;

    // Calculate pointer to header and data storage inside the NVS container
    header = (nvs_ring_header *)handle;
    crcTable = (uint16_t *)((uintptr_t)header + sizeof(nvs_ring_header));
    dataStorage = (uint8_t *)((uintptr_t)crcTable + sizeof(uint16_t)*header->length);

    // Initialize local variables
    first = header->first;
    size = header->size;
    length = header->length;

    // Check the header is valid using the token
    if (header->token == NVS_RING_TOKEN) {
        // Check index is within range
        if (index < nvs_ring_entries((nvs_ring_handle)header)) {
            // Calculate actual index from header
            index = __nvs_ring_increment(first, index, length);

            // Increment data storage pointer, retrieve data and check CRC
            dataStorage += size * index;
            nvs_copy(dataStorage, data, size);
            crc = nvs_crc(data, size);
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

bool nvs_ring_full(nvs_ring_handle handle)
{
    uint16_t last;
    uint16_t first;
    uint16_t length;
    nvs_status status;
    nvs_ring_header *header;

    // Calculate pointer to header
    header = (nvs_ring_header *)handle;

    // Initialize local variables
    first = header->first;
    last = header->last;
    length = header->length;

    // Check if the ring is full
    status = (__nvs_ring_increment(last, 1, length) == first) ? NVS_FULL : NVS_OK;

    // Return status
    return status;
}

uint16_t nvs_ring_max(nvs_ring_handle handle)
{
    uint16_t length;
    nvs_ring_header *header;

    // Calculate pointer to header
    header = (nvs_ring_header *)handle;

    // Initialize local variables
    length = header->length;

    // Return length of ring
    return length;
}

uint16_t nvs_ring_entries(nvs_ring_handle handle)
{
    uint16_t last;
    uint16_t first;
    uint16_t length;
    nvs_ring_header *header;

    // Calculate pointer to header
    header = (nvs_ring_header *)handle;

    // Initialize local variables
    first = header->first;
    last = header->last;
    length = header->length;

    // Calculate and return number of entries
    if (first == 0xffff) {
        return 0;
    }
    else if (first > last) {
        return (last + length - first + 1);
    }
    else {
        return (last - first + 1);
    }
}
