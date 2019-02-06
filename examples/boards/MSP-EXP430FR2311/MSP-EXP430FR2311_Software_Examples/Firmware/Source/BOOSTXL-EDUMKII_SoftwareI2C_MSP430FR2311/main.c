/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/*******************************************************************************
 *
 * main.c
 *
 * Software I2C Example for the MSP-EXP430FR2311 using the BOOSTXL-SENSORS
 * BoosterPack. Dimming of LED2 on P2.0 using the output of the OPT3001.
 * Main loop, initialization, sensor read and conversion.
 *
 * August 2015
 * J. Collins
 *
 ******************************************************************************/

#include <msp430.h> 
#include "opt3001.h"
#include <stdbool.h>
#include <stdint.h>
#include "MSP430_SWI2C_Master.h"
#include "driverlib.h"

// OPT3001
uint16_t rawData;
float   convertedLux;

void clockInit(void);
void initTimerB(int);
void initGPIO(void);

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	clockInit();

	initGPIO();

	MSP430_SWI2CMST_init();

	//Initialize opt3001 sensor
	sensorOpt3001Init();

	// Enable opt3001
    sensorOpt3001Enable(true);

	while(1)
	{
		// Read/convert opt3001 data
		sensorOpt3001Read(&rawData);
		sensorOpt3001Convert(rawData, &convertedLux);
		initTimerB(((int)convertedLux/4.0));
	}
}

void clockInit(void){

	// Configure one FRAM waitstate as required by the device datasheet for MCLK
	    // operation beyond 8MHz _before_ configuring the clock system.
	    FRCTL0 = FRCTLPW | NWAITS_1;

	    __bis_SR_register(SCG0);                           // disable FLL
	    CSCTL3 |= SELREF__REFOCLK;                         // Set REFO as FLL reference source
	    CSCTL0 = 0;                                        // clear DCO and MOD registers
	    CSCTL1 &= ~(DCORSEL_7);                            // Clear DCO frequency select bits first
	    CSCTL1 |= DCORSEL_5;                               // Set DCO = 16MHz
	    CSCTL2 = FLLD_0 + 487;                             // DCOCLKDIV = 16MHz
	    __delay_cycles(3);
	    __bic_SR_register(SCG0);                           // enable FLL
	    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));         // FLL locked

	    CSCTL4 |= SELMS__DCOCLKDIV | SELA__REFOCLK;        // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
	                                                       // default DCOCLKDIV as MCLK and SMCLK source
}

void initGPIO(void){
     // Select P2.0 as Timer B output function
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN0,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();
}

void initTimerB(int ccr1){
    //Start timer
    Timer_B_initUpModeParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_SMCLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 1023;
    param.startTimer = true;
    Timer_B_initUpMode(TIMER_B1_BASE, &param);

    //Initialize compare mode to generate PWM1
    Timer_B_initCompareModeParam param1 = {0};
    param1.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
    param1.compareOutputMode = TIMER_B_OUTPUTMODE_RESET_SET;
    param1.compareValue = ccr1;
    Timer_B_initCompareMode(TIMER_B1_BASE, &param1);
}
