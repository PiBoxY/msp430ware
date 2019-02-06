/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  nvs_ex3_datalog - Log structure to FRAM
//
//  This example demonstrates how to utilize the data logger functionality
//  of the NVS library to log a structure containing timestamp and ADC
//  measurement. The main program will log until NVS storage is full and then
//  read back and print logged data.
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

// Maximum number of entries in data log
#define MAX_LOG_ENTRY       8

// Simulated timestamp start
#define TIMESTAMP_START     1483228800

// Precision of simulated ADC data
#define ADC_BITS            12

// Define structure to hold timestamp and ADC samples
typedef struct adc_data_t {
    uint32_t timestamp;
    uint16_t value;
} adc_data_t;

// NVS log handle
nvs_log_handle nvsHandle;

// FRAM storage for logging ADC samples using NVS
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(nvsStorage)
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent
#endif
uint8_t nvsStorage[NVS_LOG_STORAGE_SIZE(sizeof(adc_data_t), MAX_LOG_ENTRY)] = {0};

int main(void)
{
    uint16_t i;
    uint16_t status;
    uint32_t timestamp;
    adc_data_t adc_data;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Set initial timestamp
    timestamp = TIMESTAMP_START;

    // Check integrity of NVS container and initialize if required;
    nvsHandle = nvs_log_init(nvsStorage, sizeof(adc_data_t), MAX_LOG_ENTRY);

    // Simulate logging data to FRAM storage
    while (!nvs_log_full(nvsHandle)) {
        // Populate ADC data with simulated timestamp and ADC sample
        adc_data.timestamp = timestamp;
        adc_data.value = rand() % (1 << ADC_BITS);

        // Add adc_data to FRAM storage
        status = nvs_log_add(nvsHandle, &adc_data);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK or NVS_FULL:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         *     3. no NVS_LOG_FULL check ahead of NVS_LOG_ADD
         */
        if (status != NVS_OK) {
            while (1);
        }

        // Delay ~1 second and increment timestamp
        __delay_cycles(1000000);
        timestamp++;
    }

    // Retrieve log sample and print simulated timestamp and ADC sample
    for (i = 0; i < nvs_log_entries(nvsHandle); i++) {
        // Retrieve log sample
        status = nvs_log_retrieve(nvsHandle, &adc_data, i);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK, NVS_CRC_ERROR or NVS_INDEX_OUT_OF BOUND:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         *     3. index outside MAX_LOG_ENTRY
         */
        if (status != NVS_OK) {
            while (1);
        }

        // Print timestamp and ADC sample
        printf ("ADC sample %u: 0x%04x, %lu\n", i, adc_data.value, adc_data.timestamp);
    }

    // Reset data logging storage
    status = nvs_log_reset(nvsHandle);

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


