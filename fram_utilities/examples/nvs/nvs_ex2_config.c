/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  nvs_ex2_config - Store application configuration
//
//  This example demonstrates how to store a complex structure in the non
//  volatile memory. The library function will assure to always retrieve a
//  complete set of variables regardless of asynchronous reset or power cycle
//  events. Even a compile and program update does not disrupt the configuration
//  as long as the NVS container stays the same.
//
//  Michael Zwerg, Brent Peterson
//  Texas Instruments Inc.
//  December 2016
//******************************************************************************
#include <msp430.h>

#include <stdint.h>
#include <string.h>

#include "nvs.h"

// Define enumerated type for application mode
typedef enum {
    MODE_0,
    MODE_1,
    MODE_2
} app_mode_t;

// Define structure to hold application configuration
typedef struct app_config_t {
    app_mode_t mode;
    char buffer[20];
} app_config_t;

// Initial application configuration
const app_config_t initConfig = {
    .mode = MODE_0,
    .buffer = "Hello World!"
};

// Application configuration
app_config_t config;

// NVS data handle
nvs_data_handle nvsHandle;

// Allocate NVS container inside INFO memory to store application configuration
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(nvsStorage, ".infoA")
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma location="INFOA"
__no_init
#endif
uint8_t nvsStorage[NVS_DATA_STORAGE_SIZE(sizeof(app_config_t))];

int main(void)
{
    uint16_t status;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Check integrity of NVS container and initialize if required
    nvsHandle = nvs_data_init(nvsStorage, sizeof(app_config_t));

    // Retrieve application configuration
    status = nvs_data_restore(nvsHandle, &config);

    switch (status) {
    case NVS_OK: break;
    case NVS_EMPTY:
        // Initialize local application configuration.
        memcpy(&config, &initConfig, sizeof(config));

        // Update NVS container with initial application configuration.
        status = nvs_data_commit(nvsHandle, &config);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         */
        if (status != NVS_OK) {
            while (1);
        }
        break;
    default:
        /*
         * Status should never be NVS_NOK or NVS_CRC_ERROR, potential reasons:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         */
        while (1);
    }

    // Update application configuration
    switch (config.mode) {
    case MODE_0:
        config.mode = MODE_1;
        break;
    case MODE_1:
        config.mode = MODE_2;
        break;
    case MODE_2:
    default:
        config.mode = MODE_0;
        break;
    }

    /*
     * Update NVS container with application configuration. In case of a reset
     * the application will resume with this state.
     */
    status = nvs_data_commit(nvsHandle, &config);

    /*
     * Status should never be not NVS_OK but if it happens trap execution.
     * Potential reason for NVS_NOK:
     *     1. nvsStorage not initialized
     *     2. nvsStorage got corrupted by other task (buffer overflow?)
     */
    if (status != NVS_OK) {
        while (1);
    }

    return 0;
}


