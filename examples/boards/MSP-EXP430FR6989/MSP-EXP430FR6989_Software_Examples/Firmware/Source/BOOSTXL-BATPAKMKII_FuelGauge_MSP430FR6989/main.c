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
// main.c - MSP-EXP430FR6959 + Battery Boosterpack MkII - Fuel Guage demo
//
//          Initializes BQ27441 Fuel Gauge configurations suitable for the
//          included battery and periodically prints information to serial
//          backchannel uart.
//
//          LCD displays fuel gauge status and battery icon indicator.
//          Button S1 and S2 cycles through different fuel gauge status
//
//****************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <driverlib.h>
#include "HAL_I2C.h"
#include "HAL_UART.h"
#include "HAL_BQ27441.h"
#include "HAL_LCD.h"

// Global Variables

// Push button variables
volatile unsigned char S1buttonDebounce = 0;
volatile unsigned char S2buttonDebounce = 0;
int status = 0;

// Fuel Gauge register variables
short designCapacity = 0;
short remainCapacity = 0;
short soc = 0;
short temperature = 0;
short voltage = 0;
short avgCurrent = 0;

// TimerA0 UpMode Configuration Parameter
Timer_A_initUpModeParam initUpParam_A0 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/4 = 2MHz
        30000,                                  // 15ms debounce period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR,                       // Clear value
        true                                    // Start Timer
};

// Global function declarations
void CS_init(void);
void GPIO_init(void);
void TIMER_init(void);
void updateBatteryIcon(void);
void updateLCD(void);


/*
 * main.c
 */
int main(void) {
	// Halt the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    /* Initialization */
    GPIO_init();
    CS_init();
    LCD_init();

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

	// Initialize Timer
	TIMER_init();

	/* Display Battery information */
	while(1)
	{
		__bis_SR_register(LPM3_bits | GIE);

		char str[64];
		UART_transmitString("\r\n");
		UART_transmitString("\r\n");
		UART_transmitString("*************************************\r\n");
		UART_transmitString("Battery Information\r\n");
		UART_transmitString("*************************************\r\n");

		/* Read Design Capacity */
		if(!BQ27441_read16(DESIGN_CAPACITY, &designCapacity, 1000))
			UART_transmitString("Error Reading Design Capacity \r\n");
		else
		{
			sprintf(str, "Design Capacity: %dmAh\r\n", designCapacity);
			UART_transmitString(str);
			updateLCD();
		}

		/* Read Remaining Capacity */
		if(!BQ27441_read16(REMAINING_CAPACITY, &remainCapacity, 1000))
			UART_transmitString("Error Reading Remaining Capacity \r\n");
		else
		{
			sprintf(str, "Remaining Capacity: %dmAh\r\n", remainCapacity);
			UART_transmitString(str);
		}

		/* Read State Of Charge */
		if(!BQ27441_read16(STATE_OF_CHARGE, &soc, 1000))
			UART_transmitString("Error Reading State Of Charge \r\n");
		else
		{
			sprintf(str, "State of Charge: %d%%\r\n", (unsigned short)soc);
			UART_transmitString(str);
			updateBatteryIcon();
		}

		/* Read Temperature */
		if(!BQ27441_read16(TEMPERATURE, &temperature, 1000))
			UART_transmitString("Error Reading Temperature \r\n");
		else
		{
			sprintf(str, "Temperature: %.1fC\r\n", temperature/10.0 - 273.0);
			UART_transmitString(str);
		}

		/* Read Voltage */
		if(!BQ27441_read16(VOLTAGE, &voltage, 1000))
			UART_transmitString("Error Reading Voltage \r\n");
		else
		{
			sprintf(str, "Voltage: %dmV\r\n", voltage);
			UART_transmitString(str);
		}

		/* Read Average Current */
		if(!BQ27441_read16(AVERAGE_CURRENT, &avgCurrent, 1000))
			UART_transmitString("Error Reading Average Current \r\n");
		else
		{
			sprintf(str, "Average Current: %dmA\r\n", avgCurrent);
			UART_transmitString(str);
			if (avgCurrent> 0) {
				UART_transmitString("Status: charging\r\n");
			} else {
				UART_transmitString("Status: discharging\r\n");
			}
		}
	}
}


/* Initializes Clock System */
void CS_init()
{
    // Set PJ.4 and PJ.5 as Secondary Module Function Input, LFXT.
    GPIO_setAsPeripheralModuleFunctionInputPin(
           GPIO_PORT_PJ,
           GPIO_PIN4 + GPIO_PIN5,
           GPIO_PRIMARY_MODULE_FUNCTION
           );

    CS_setExternalClockSource(32768, 0);
    // Intializes the XT1 crystal oscillator
    CS_turnOnLFXT(CS_LFXT_DRIVE_3);

    /* Initializes Clock System DCO = 8MHz */
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
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

    // Configure button S1 (P1.1) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Configure button S2 (P1.2) interrupt
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PMM_unlockLPM5();
}


/* Initializes TIMER */
void TIMER_init()
{
    //Start timer in continuous mode sourced by ACLK
    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);

    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 32768;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &param);
}


/* Display battery state of charge using the battery bar icon on the LCD */
void updateBatteryIcon()
{
    LCD_showSymbol(LCD_UPDATE, LCD_BRACKETS, 0);
    LCD_showSymbol(LCD_UPDATE, LCD_BATT, 0);
    LCD_showSymbol(LCD_UPDATE, LCD_BRACKETS, 1);
    LCD_showSymbol(LCD_UPDATE, LCD_BATT, 1);

    // Calculate number of bars based on state of charge %
    int bar = soc * 6 / 100;
    int i;

    for (i = 0; i < 6; i++)
    {
    	// Clear battery indicator
        LCD_showSymbol(LCD_CLEAR, LCD_B1+i, 0);
        LCD_showSymbol(LCD_CLEAR, LCD_B1+i, 1);

        // Draw bars corresponding to state of charge
        if (i < bar)
        {
            LCD_showSymbol(LCD_UPDATE, LCD_B1+i, 0);
            LCD_showSymbol(LCD_UPDATE, LCD_B1+i, 1);
        }

        // Blink extra bar if charging
        if (bar < 5 && avgCurrent > 0)
    		LCD_showSymbol(LCD_UPDATE, LCD_B1+bar, 1);
    }
}


/* Display the selected fuel gauge status */
void updateLCD()
{
    switch (status) {
    case 0:
        LCD_showChar('D', 1, 0);
        LCD_showChar('E', 2, 0);
        LCD_showChar('S', 3, 0);
        LCD_showChar('I', 4, 0);
        LCD_showChar('G', 5, 0);
        LCD_showChar('N', 6, 0);
        LCD_displayNumber(designCapacity, 1);
        break;
    case 1:
        LCD_showChar('R', 1, 0);
        LCD_showChar('E', 2, 0);
        LCD_showChar('M', 3, 0);
        LCD_showChar('A', 4, 0);
        LCD_showChar('I', 5, 0);
        LCD_showChar('N', 6, 0);
        LCD_displayNumber(remainCapacity, 1);
        break;
    case 2:
        LCD_showChar('S', 1, 0);
        LCD_showChar('O', 2, 0);
        LCD_showChar('C', 3, 0);
        LCD_showChar(' ', 4, 0);
        LCD_showChar(' ', 5, 0);
        LCD_showChar(' ', 6, 0);
        LCD_displayNumber(soc, 1);
        break;
    case 3:
        LCD_showChar('T', 1, 0);
        LCD_showChar('E', 2, 0);
        LCD_showChar('M', 3, 0);
        LCD_showChar('P', 4, 0);
        LCD_showChar(' ', 5, 0);
        LCD_showChar(' ', 6, 0);
        LCD_displayNumber(temperature/10.0 - 273.0, 1);
        break;
    case 4:
        LCD_showChar('V', 1, 0);
        LCD_showChar('O', 2, 0);
        LCD_showChar('L', 3, 0);
        LCD_showChar('T', 4, 0);
        LCD_showChar(' ', 5, 0);
        LCD_showChar(' ', 6, 0);
        LCD_displayNumber(voltage, 1);
        break;
    case 5:
        LCD_showChar('A', 1, 0);
        LCD_showChar('V', 2, 0);
        LCD_showChar('G', 3, 0);
        LCD_showChar('C', 4, 0);
        LCD_showChar('U', 5, 0);
        LCD_showChar('R', 6, 0);
        LCD_displayNumber(avgCurrent, 1);
        break;
	}
}


//******************************************************************************
//
//This is the TIMER1_A3 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR(void)
{
    // Toggle between LCD Normal/Blink memory
    LCDCMEMCTL ^= LCDDISP;
	__bic_SR_register_on_exit(LPM3_bits);
}


/*
 * PORT1 Interrupt Service Routine
 * Handles S1 and S2 button press interrupts
 */
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    switch(__even_in_range(P1IV, P1IV_P1IFG7))
    {
        case P1IV_NONE : break;
        case P1IV_P1IFG0 : break;
        case P1IV_P1IFG1 :    // Button S1 pressed
            P1OUT |= BIT0;    // Turn LED1 On
            if ((S1buttonDebounce) == 0)
            {
                // Set debounce flag on first high to low transition
                S1buttonDebounce = 1;
                status++;
                if (status > 5)
                	status = 0;

                // Switch to LCD memory
                LCDCMEMCTL &= ~LCDDISP;
                updateLCD();

                // Start debounce timer
                Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
            }
            break;
        case P1IV_P1IFG2 :    // Button S2 pressed
            P9OUT |= BIT7;    // Turn LED2 On
            if ((S2buttonDebounce) == 0)
            {
                // Set debounce flag on first high to low transition
                S2buttonDebounce = 1;
                status--;
                if (status < 0)
                	status = 5;

                // Switch to LCD memory
                LCDCMEMCTL &= ~LCDDISP;
                updateLCD();

                // Start debounce timer
                Timer_A_initUpMode(TIMER_A0_BASE, &initUpParam_A0);
            }
            break;
        case P1IV_P1IFG3 : break;
        case P1IV_P1IFG4 : break;
        case P1IV_P1IFG5 : break;
        case P1IV_P1IFG6 : break;
        case P1IV_P1IFG7 : break;
    }
}


/*
 * Timer A0 Interrupt Service Routine
 * Used as button debounce timer
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    // Button S1 released
    if (P1IN & BIT1)
    {
        S1buttonDebounce = 0;                                   // Clear button debounce
        P1OUT &= ~BIT0;
    }

    // Button S2 released
    if (P1IN & BIT2)
    {
        S2buttonDebounce = 0;                                   // Clear button debounce
        P9OUT &= ~BIT7;
    }

    // Both button S1 & S2 released
    if ((P1IN & BIT1) && (P1IN & BIT2))
    {
        // Stop timer A0
        Timer_A_stop(TIMER_A0_BASE);
    }
}
