/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  rng_ex1_generate_data - Generate random data.
//
//  This example demonstrates how to generate random 8-bit and 32-bit data. The
//  random bytes can be used for a variety of applications including
//  cryptography and tamper detection.
// 
//  This example generates data with length 64-bytes but data can be generated
//  in any multiple of RNG_KEYLEN (16) bytes.
//
//  Caleb Overbay, Brent Peterson
//  Texas Instruments Inc.
//  November 2016
//******************************************************************************
#include <msp430.h>

#include <stdint.h>

#include "rng.h"

// Length of random data to generate, multiple of RNG_KEYLEN
#define LENGTH              (4*RNG_KEYLEN)

// Storage for generated random 8-bit data
uint8_t data[LENGTH];

// Storage for generated random 32-bit data
uint32_t data32[LENGTH];

int main(void)
{
    uint16_t len;
    
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Configure MPU with INFO memory as RW
    MPUCTL0 = MPUPW;
    MPUSAM |= MPUSEGIRE | MPUSEGIWE;
    MPUCTL0 = MPUPW | MPUENA;

    // Generate random 8-bit data and check return length
    len = rng_generateBytes(data, LENGTH);
    if (len != LENGTH) {
        // Error generating bytes
        __no_operation();
    }
    
    // Generate random 32-bit data and check return length
    len = rng_generateBytes((uint8_t *)data32, sizeof(data32));
    if (len != sizeof(data32)) {
        // Error generating bytes
        __no_operation();
    }

    return(0);
}
