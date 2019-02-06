/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  nvs_ex1_odometer - Continuous counter
//
//  This example demonstrates how to implement a continuous up counter that
//  never looses its value regardless of asynchronous reset or power cycle
//  events. Even a compile and program update does not disrupt the counter,
//  as long as the NVS container stays the same.
//
//  Michael Zwerg, Brent Peterson
//  Texas Instruments Inc.
//  December 2016
//******************************************************************************
#include <msp430.h>

#include <stdint.h>

#include "nvs.h"

// NVS data handle
nvs_data_handle nvsHandle;

// Allocate NVS container inside INFO memory to store 64-bit odometer value
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_SECTION(nvsStorage, ".infoA")
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma location="INFOA"
__no_init
#endif
uint8_t nvsStorage[NVS_DATA_STORAGE_SIZE(sizeof(uint64_t))];

int main(void)
{
    uint16_t status;
    uint64_t odometer;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Check integrity of NVS container and initialize if required
    nvsHandle = nvs_data_init(nvsStorage, sizeof(odometer));

    // Restore odometer value
    status = nvs_data_restore(nvsHandle, &odometer);

    switch (status) {
    case NVS_OK: break;
    case NVS_EMPTY:
        // Initialize odometer value.
        odometer = 0;
        break;
    default:
        /*
         * Status should never be NVS_NOK or NVS_CRC_ERROR, potential reasons:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         */
        while (1);
    }

    // Increment odometer value and store to NVS storage
    while (1) {
        // Increment odometer value
        odometer++;

        // Update NVS container with current odometer value.
        status = nvs_data_commit(nvsHandle, &odometer);

        /*
         * Status should never be not NVS_OK but if it happens trap execution.
         * Potential reason for NVS_NOK:
         *     1. nvsStorage not initialized
         *     2. nvsStorage got corrupted by other task (buffer overflow?)
         */
        if (status != NVS_OK) {
            while (1);
        }
    }
}
