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
 * LiveTempMode.c
 *
 * MSP430FR5969 measures and transmits internal temperature sensor data over UART
 *
 * June 2014
 * E. Chen
 *
 ******************************************************************************/

#include "LiveTempMode.h"
#include "main.h"
#include "driverlib.h"

void liveTemp()
{
	//Initialize the ADC12B Module
    /*
     * Base address of ADC12B Module
     * Use internal ADC12B bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider/pre-divider of 1
     * Use Temperature Sensor and Battery Monitor internal channels
     */
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ACLK;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_TEMPSENSEMAP;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    // Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    // Sets up the sampling timer pulse mode
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_MULTIPLESAMPLESDISABLE);

    // Maps Temperature Sensor input channel to Memory 0 and select voltage references
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A1 to memory buffer 0
     * Vref+ = IntBuffer
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam configureMemoryParam = {0};
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_TCMAP;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    // Clear memory buffer 0 interrupt
    ADC12_B_clearInterrupt(ADC12_B_BASE,
                           0,
                           ADC12_B_IFG0
                           );

    // Enable memory buffer 0 interrupt
    ADC12_B_enableInterrupt(ADC12_B_BASE,
                            ADC12_B_IE0,
                            0,
                            0);

    // Configure internal reference
    while(Ref_A_isRefGenBusy(REF_A_BASE));              // If ref generator busy, WAIT
    Ref_A_enableTempSensor(REF_A_BASE);
    Ref_A_setReferenceVoltage(REF_A_BASE, REF_A_VREF2_0V);
    Ref_A_enableReferenceVoltage(REF_A_BASE);

    // Start timer
    Timer_A_initUpModeParam param = {0};
    param.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 13;
    param.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_A_DO_CLEAR;
    param.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &param);

    __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3. Delay for Ref to settle.

    // Change timer delay to 1/8 second
    Timer_A_setCompareValue(TIMER_A0_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            0x1000
                            );

    while(mode == LIVE_TEMP_MODE)
    {
    	__bis_SR_register(LPM3_bits | GIE);   // Enter LPM3, wait for ~1/8 sec timer

		//Enable/Start sampling and conversion
		/*
		 * Base address of ADC12B Module
		 * Start the conversion into memory buffer 0
		 * Use the single-channel, single-conversion mode
		 */
		ADC12_B_startConversion(ADC12_B_BASE,
								ADC12_B_MEMORY_0,
								ADC12_B_SINGLECHANNEL);

		__bis_SR_register(LPM3_bits | GIE);   // Wait for conversion to complete
		__bic_SR_register(GIE);

		// Select UART TXD on P2.0
		GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

		EUSCI_A_UART_transmitData(EUSCI_A0_BASE, ADC12MEM0_H);             // Send higher byte of temperature data
		EUSCI_A_UART_transmitData(EUSCI_A0_BASE, ADC12MEM0_L);             // Send higher byte of temperature data

		GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

        while(EUSCI_A_UART_queryStatusFlags(EUSCI_A0_BASE, EUSCI_A_UART_BUSY));
        __no_operation();                     // Set a breakpoint here to verify UART transmission
    }

    // Disable ADC12 and Timer_A0
    ADC12_B_disable(ADC12_B_BASE);
    Timer_A_stop(TIMER_A0_BASE);
}
