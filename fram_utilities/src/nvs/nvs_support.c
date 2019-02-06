/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <msp430.h>

#include <stdint.h>

/*
 * Calculate a 16-bit CRC over a storage buffer in bytes.
 */
uint16_t nvs_crc(void *data, uint16_t size)
{
    uint16_t res;
    uint16_t temp;
    uint8_t *ptrData;

    // Save CRC result register and reset input
    temp = CRCINIRES;
    CRCINIRES = 0xFFFF;

    // Set local pointer and calculate CRC
    ptrData = (uint8_t *)data;
    while (size--) {
        CRCDI_L = *ptrData++;
    }

    // Save result and restore CRC result register
    res = CRCINIRES;
    CRCINIRES = temp;

    // Return CRC result
    return res;
}

#if defined(__MSP430FR2XX_4XX_FAMILY__)

#if !defined(FRWPPW)
#define FRWPPW      0
#endif

#if !defined(DFWP)
#define DFWP        0
#endif

#endif

/*
 * Clear the FRAM program write protection bit and return the original status
 * of the bit. The return of this function can be used to restore the previous
 * FRAM protection state with nvs_restoreFRAM() (FR2XX and FR4XX only).
 */
uint16_t nvs_unlockFRAM(void)
{
    uint16_t state = 0;

    /* unlock FRAM write protect */
#if defined(__MSP430FR2XX_4XX_FAMILY__)

        /* Global disable interrupts while FRAM is not write protected */
        uint16_t interrupt_state = __get_interrupt_state() & GIE;
        __disable_interrupt();

        uint16_t fram_state = SYSCFG0 & (PFWP+DFWP);
        SYSCFG0 = FRWPPW;

        state = (fram_state << 8) | interrupt_state ;

#elif defined(__MSP430FR57XX_FAMILY__) || defined(__MSP430FR5XX_6XX_FAMILY__)

        /* Global disable interrupts while FRAM is not write protected */
        uint16_t interrupt_state = __get_interrupt_state() & GIE;
        __disable_interrupt();

        uint16_t fram_state = MPUSAM & (MPUSEGIWE);

        // write enable INFO
        MPUCTL0_H  = MPUPW_H ;         // unlock MPU
        MPUSAM    |= MPUSEGIWE ;       // enable WR to INFO SEGMENT
        MPUCTL0_H  = 0;                // lock MPU

        state = fram_state | interrupt_state ;
#endif

    return state;
}

/*
 * Restore the previous FRAM protection state with the state returned from
 * nvs_unlockFRAM() (FR2XX and FR4XX only).
 */
void nvs_lockFRAM(uint16_t state)
{
#if defined(__MSP430FR2XX_4XX_FAMILY__)

    uint16_t fram_state      = (state & 0xFF00) >> 8;
    uint16_t interrupt_state = (state & 0x00FF);

    SYSCFG0 = FRWPPW | fram_state;

    /* Restore interrupt state. */
    __set_interrupt_state(interrupt_state);

#elif defined(__MSP430FR57XX_FAMILY__) || defined(__MSP430FR5XX_6XX_FAMILY__)

    uint16_t fram_state      = state & 0xFF00;
    uint16_t interrupt_state = state & 0x00FF;

    // restore write enable for INFO
    MPUCTL0_H  = MPUPW_H ;                    // unlock MPU
    MPUSAM    &= (fram_state | ~MPUSEGIWE) ;  // restore WR to INFO SEGMENT
    MPUCTL0_H  = 0;                           // lock MPU

    /* Restore interrupt state. */
    __set_interrupt_state(interrupt_state);

#endif
}

