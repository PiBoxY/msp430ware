/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * ======== hal.c ========
 *
 */
#include "msp430.h"

#include "driverlib/MSP430F5xx_6xx/gpio.h"
#include "driverlib/MSP430F5xx_6xx/wdt_a.h"
#include "driverlib/MSP430F5xx_6xx/ucs.h"
#include "driverlib/MSP430F5xx_6xx/pmm.h"

#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_config/descriptors.h"

#include "hal.h"

/*
 * This functions drives all the outputs low which may not be optimal for a
 * different board. This function needs to be modified as per the connections
 * to the GPIO pins.
 */
void initPorts(void)
{
    P1OUT = 0x00;
    P1DIR = 0xFF;
    P2OUT = 0x00;
    P2DIR = 0xFF;
    P3OUT = 0x00;
    P3DIR = 0xFF;
    P4OUT = 0x00;
    P4DIR = 0xFF;
    P5OUT = 0x00;
    P5DIR = 0xFF;
    P6OUT = 0x00;
    P6DIR = 0xFF;
    P7OUT = 0x00;
    P7DIR = 0xFF;
    P8OUT = 0x00;
    P8DIR = 0xFF;
}

void initClocks(DWORD mclkFreq)
{
    // Assign the REFO as the FLL reference clock
	UCS_initClockSignal(
	   UCS_FLLREF,
	   UCS_REFOCLK_SELECT,
	   UCS_CLOCK_DIVIDER_1);

	// Assign the REFO as the source for ACLK
	UCS_initClockSignal(
	   UCS_ACLK,
	   UCS_REFOCLK_SELECT,
	   UCS_CLOCK_DIVIDER_1);

    UCS_initFLLSettle(
        mclkFreq/1000,
        mclkFreq/32768);
        //use REFO for FLL and ACLK
        UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
        UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);

}



