/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  nvs_ex4_blackbox - Black box recorder
//
//  This example demonstrates how to utilize the NVS ring storage container
//  to create a black box recorder with the most recent samples. The main
//  program will log a specified number of entries and then read back and print
//  the latest data.
//
//  Michael Zwerg, Brent Peterson
//  Texas Instruments Inc.
//  December 2016
//******************************************************************************
#include <msp430.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "nvs.h"

// Number of simulated samples
#define SIMULATED_SAMPLES   20

// Size of NVS ring storage
#define NVS_RING_SIZE       8

// Simulated timestamp start
#define TIMESTAMP_START     1483228800

// Precision of simulated ADC data
#define ADC_BITS            12

// Define structure to hold timestamp and ADC samples
typedef struct adc_data_t {
    uint32_t timestamp;
    uint16_t value;
} adc_data_t;

// NVS ring handle
nvs_ring_handle nvsHandle;

// FRAM storage for ADC samples using NVS ring storage
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(nvsStorage)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
uint8_t nvsStorage[NVS_RING_STORAGE_SIZE(sizeof(adc_data_t), NVS_RING_SIZE)] = {0};

int main(void)
{
    uint16_t i;
    uint16_t status;
    uint32_t timestamp;
    adc_data_t adc_data;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Set and print the initial timestamp
    timestamp = TIMESTAMP_START;
    printf ("Initial timestamp: %lu\n", timestamp);

    // Check integrity of NVS container and initialize if required;
    nvsHandle = nvs_ring_init(nvsStorage, sizeof(adc_data_t), NVS_RING_SIZE);

    // Simulate samples and write to ring storage
    for (i = 0; i < SIMULATED_SAMPLES; i++) {
        // Populate ADC data with simulated timestamp and ADC sample
        adc_data.timestamp = timestamp;
        adc_data.value = rand() % (1 << ADC_BITS);

        // Add adc_data to ring storage
        status = nvs_ring_add(nvsHandle, &adc_data);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         */
        if (status != NVS_OK) {
            while (1);
        }

        // Delay ~1 second and increment timestamp
        __delay_cycles(1000000);
        timestamp++;
    }

    // Print the current timestamp to check only the latest entries are printed.
    printf ("Current timestamp: %lu\n", timestamp);

    // Retrieve ring samples and print simulated timestamp and ADC sample
    for (i = 0; i < NVS_RING_SIZE; i++) {
        // Retrieve ring sample
        status = nvs_ring_retrieve(nvsHandle, &adc_data, i);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK, NVS_CRC_ERROR or NVS_INDEX_OUT_OF BOUND:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         *     3. index outside of NVS_RING_SIZE
         */
        if (status != NVS_OK) {
            while (1);
        }

        // Print timestamp and ADC sample
        printf ("ADC sample %u: 0x%04x, %lu\n", i, adc_data.value, adc_data.timestamp);
    }

    // Reset ring storage
    status = nvs_ring_reset(nvsHandle);

    /*
     * Status should never be not NVS_OK but if it happens trap execution.
     * Potential reason for NVS_NOK:
     *     1. nvsStorage not initialized
     *     2. nvsStorage got corrupted by other task (buffer overflow?)
     */
    if (status == NVS_NOK) {
        while (1);
    }

    return 0;
}
