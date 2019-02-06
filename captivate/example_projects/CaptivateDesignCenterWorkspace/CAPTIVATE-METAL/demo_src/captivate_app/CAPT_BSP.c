/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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

#include <msp430.h>
#include <stdint.h>

#include "CAPT_BSP.h"

//*****************************************************************************
//
//! CAPT_BSP.c
//
//! CAPT_BSP is the board support package for the Capacitive Touch MCU 
//! Development Kit.  It defines the BSP_configureMCU() function, which 
//! configures the MCU clock system (CS) and port muxing (Digital IO).
//
//! \author Texas Instruments
//! \author MSP Microcontroller Products
//
//! \version Captivate Starter Project 1.00.08.11
//! Released on Fri, May 29, 2015  4:48:50 PM
//
//*****************************************************************************


#include <msp430.h>
#include <stdint.h>
//*****************************************************************************
//
//! This function is configures the MCU CS and Digital IO for operation.
//
//*****************************************************************************
void BSP_configureMCU(void)
{
    CS_initFLLParam fllParams = {0};
    uint8_t ui8LFOsc = CS_XT1CLK_SELECT;;

	//
	// Setup ports
	//

	// P1.0: OUTPUT LOW
	// P1.1: IRQ (OPEN DRAIN)
	// P1.2: UCB0 I2C SDA
	// P1.3: UCB0 I2C SCL
	// P1.4: UCA0 UART TXD
	// P1.5: UCA0 UART RXD
	// P1.6: LED2
	// P1.7: LED1
	P1OUT  = (0x00);
	P1DIR  = (BIT0 | BIT6 | BIT7);
	P1SEL0 = (BIT2 | BIT3 | BIT4 | BIT5);
	P1SEL1 = (0x00);

	// P2.0: XOUT
	// P2.1: XIN
	// P2.2: SYNC (SET OUTPUT LOW UNLESS USED)
	// P2.3: OUTPUT LOW
	// P2.4: OUTPUT LOW
	// P2.5: OUTPUT LOW
	// P2.6: OUTPUT LOW
	// P2.7: OUTPUT LOW
	P2OUT  =  (0x00);
	P2DIR  =  (BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	P2SEL0 =  (BIT0 | BIT1);
	P2SEL1 =  (0x00);

	// P3.0: OUTPUT LOW
	// P3.1: OUTPUT LOW
	// P3.2: OUTPUT LOW
	P3OUT  =  (0x00);
	P3DIR  =  (BIT0 | BIT1 | BIT2);
	P3SEL0 =  (0x00);
	P3SEL1 =  (0x00);

    //
    // Clear port lock
    //
    PM5CTL0 &= ~LOCKLPM5;

    //
    // Attempt to start the low frequency crystal oscillator
    //
    CS_setExternalClockSource(XT1_OSC_FREQ);
    if (CS_turnOnXT1LFWithTimeout(CS_XT1_DRIVE_0, XT1_OSC_TIMEOUT) == STATUS_FAIL)
    {
        //
        // If a crystal is not present or is failing, switch the LF
        // clock definition to the internal 32kHz reference oscillator.
        //
        ui8LFOsc = CS_REFOCLK_SELECT;
    }

    // Add wait states to run at 16 MHz
    FRAMCtl_configureWaitStateControl(FRAMCTL_ACCESS_TIME_CYCLES_1);

    //
    // Initialize Clock Signals
    //
    CS_initClockSignal(CS_FLLREF, ui8LFOsc, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, ui8LFOsc, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLKDIV_SELECT, CS_CLOCK_DIVIDER_8);

    //
    // Tune the DCO parameters
    //
    CS_initFLLCalculateTrim((MCLK_FREQ/1000), FLL_RATIO, &fllParams);
    CS_clearAllOscFlagsWithTimeout(1000);
}

//*****************************************************************************
//
//! This function disables the watchdog timer during boot, ensuring a WDT
//! reset does not occur during boot before main() is entered.
//
//*****************************************************************************
int _system_pre_init(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    return 1;
}
