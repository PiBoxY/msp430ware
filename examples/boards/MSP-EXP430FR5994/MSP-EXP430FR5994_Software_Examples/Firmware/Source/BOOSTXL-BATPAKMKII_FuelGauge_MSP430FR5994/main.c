/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
//****************************************************************************
//
// main.c - MSP-EXP430FR5994 + Battery Boosterpack MkII - Fuel Guage demo
//
//          Initializes BQ27441 Fuel Gauge configurations suitable for the
//          included battery and periodically prints information to serial
//          backchannel uart.
//
//****************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <driverlib.h>
#include "HAL_I2C.h"
#include "HAL_UART.h"
#include "HAL_BQ27441.h"

// Global function declarations
void CS_init(void);
void GPIO_init(void);
char *itoa(int, char*, int);


/*
 * main.c
 */
int main(void) {
	// Halt the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    /* Initialization */
    GPIO_init();
    CS_init();

	/* Initialize I2C */
	I2C_initGPIO();
	I2C_init();

	/* Initialize UART */
	UART_initGPIO();
	UART_init();
	
	__delay_cycles(800000);

	// Initialize BQ27441 Config
	if (!BQ27441_initConfig())
	{
		UART_transmitString("Error initializing BQ27441 Config\r\n");
		UART_transmitString("Make sure BOOSTXL-BATPAKMKII is connected and switch is flipped to \"CONNECTED\"\r\n");
	}

	// Clear Battery Interrupt Enable inside Operation Configuration
	while (!BQ27441_initOpConfig())
	{
		__delay_cycles(800000);
		UART_transmitString("Clearing BIE in Operation Configuration\r\n");
	}

	// Indicate battery insertion from host MCU
	BQ27441_control(BAT_INSERT, 1000);
	__delay_cycles(800000);

	/* Display Battery information */
	while(1)
	{
		short result16 = 0;
		char str[64];
		UART_transmitString("\r\n");
		UART_transmitString("\r\n");
		UART_transmitString("*************************************\r\n");
		UART_transmitString("Battery Information\r\n");
		UART_transmitString("*************************************\r\n");

		/* Read Design Capacity */
		if(!BQ27441_read16(DESIGN_CAPACITY, &result16, 1000))
			UART_transmitString("Error Reading Design Capacity \r\n");
		else
		{
			sprintf(str, "Design Capacity: %dmAh\r\n", result16);
			UART_transmitString(str);
		}

		/* Read Remaining Capacity */
		if(!BQ27441_read16(REMAINING_CAPACITY, &result16, 1000))
			UART_transmitString("Error Reading Remaining Capacity \r\n");
		else
		{
			sprintf(str, "Remaining Capacity: %dmAh\r\n", result16);
			UART_transmitString(str);
		}

		/* Read State Of Charge */
		if(!BQ27441_read16(STATE_OF_CHARGE, &result16, 1000))
			UART_transmitString("Error Reading State Of Charge \r\n");
		else
		{
			sprintf(str, "State of Charge: %d%%\r\n", (unsigned short)result16);
			UART_transmitString(str);
		}

		/* Read Temperature */
		if(!BQ27441_read16(TEMPERATURE, &result16, 1000))
			UART_transmitString("Error Reading Temperature \r\n");
		else
		{
			sprintf(str, "Temperature: %dC\r\n", result16/10 - 273);
			UART_transmitString(str);
		}

		/* Read Voltage */
		if(!BQ27441_read16(VOLTAGE, &result16, 1000))
			UART_transmitString("Error Reading Voltage \r\n");
		else
		{
			sprintf(str, "Voltage: %dmV\r\n", result16);
			UART_transmitString(str);
		}

		/* Read Average Current */
		if(!BQ27441_read16(AVERAGE_CURRENT, &result16, 1000))
			UART_transmitString("Error Reading Average Current \r\n");
		else
		{
			sprintf(str, "Average Current: %dmA\r\n", result16);
			UART_transmitString(str);
			if (result16 > 0) {
				UART_transmitString("Status: charging\r\n");
			} else {
				UART_transmitString("Status: discharging\r\n");
			}
		}

		__delay_cycles(16000000);
	}
}


/* Initializes Clock System */
void CS_init()
{
    // LFXT Setup
    // Set PJ.4 and PJ.5 as Primary Module Function Input.
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    CS_setExternalClockSource(32768, 0);
    // Intializes the XT1 crystal oscillator
    CS_turnOnLFXT(CS_LFXT_DRIVE_3);

    // Set DCO frequency to 16MHz
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);

    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK,CS_LFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
}


/* Initializes GPIO */
void GPIO_init()
{
	/* Terminate all GPIO pins to Output LOW to minimize power consumption */
    GPIO_setAsOutputPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PE, GPIO_PIN_ALL16);
    GPIO_setAsOutputPin(GPIO_PORT_PF, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PA, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PC, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PD, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PE, GPIO_PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PF, GPIO_PIN_ALL16);

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();
}
