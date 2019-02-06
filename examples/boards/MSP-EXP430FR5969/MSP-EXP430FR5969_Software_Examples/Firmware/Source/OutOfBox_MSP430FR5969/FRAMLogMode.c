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
 * FRAMLogMode.c
 *
 * MSP430FR5969 wakes up every 5 seconds from LPM3.5 to measure and store its
 * internal temperature sensor & battery monitor data to FRAM
 *
 * June 2014
 * E. Chen
 *
 ******************************************************************************/

#include "FRAMLogMode.h"
#include "main.h"
#include "driverlib.h"

/*
 * FRAM Array reserved to store data memory for Temperature and Voltage
 * Stores up to 6,144 Temp + 6,144 Voltage + 1 index data
 * 5 second / sample ~ 8.5 hours of data
 */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = 0x9000
__no_init uint16_t dataArray[12289];
#elif defined(__TI_COMPILER_VERSION__)
#pragma location = 0x9000
#pragma NOINIT(dataArray)
uint16_t dataArray[12289];
#endif

// FRAM Array reserved to store FRAM Log Mode starting Time Stamp
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = 0xF002
__no_init uint8_t timeStamp[6];
#elif defined(__TI_COMPILER_VERSION__)
#pragma location = 0xF002
#pragma NOINIT(timeStamp)
uint8_t timeStamp[6];                      // 6 bytes Time Stamp received from PC
#endif

unsigned int *FRAM_index_ptr = (unsigned int*)(FRAM_TEMP_INDEX);  // Pointer to address that stores index
unsigned int *FRAM_TEMP_write_ptr;
unsigned int *FRAM_BAT_write_ptr;

unsigned int storageDone;

void framLog()
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
    initParam.internalChannelMap = ADC12_B_TEMPSENSEMAP | ADC12_B_BATTMAP;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    //Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    // Sets up the sampling timer pulse mode
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_CYCLEHOLD_128_CYCLES,
                               ADC12_B_MULTIPLESAMPLESENABLE);

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

    // Maps Battery Monitor input channel to Memory 0 and select voltage references
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A1 to memory buffer 0
     * Vref+ = IntBuffer
     * Vref- = AVss
     * Memory buffer 0 is not the end of a sequence
     */
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_1;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_BATMAP;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_ENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    // Clear memory buffer 0 interrupt
    ADC12_B_clearInterrupt(ADC12_B_BASE,
                           0,
                           ADC12_B_IFG1
                           );

    // Enable memory buffer 0 interrupt
    ADC12_B_enableInterrupt(ADC12_B_BASE,
                            ADC12_B_IE1,
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

    Timer_A_stop(TIMER_A0_BASE);

    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    //Enable/Start sampling and conversion
    /*
     * Base address of ADC12B Module
     * Start the conversion into memory buffer 0
     * Use the single-channel, single-conversion mode
     */
    ADC12_B_startConversion(ADC12_B_BASE,
                            ADC12_B_MEMORY_0,
                            ADC12_B_SEQOFCHANNELS);

    __bis_SR_register(LPM3_bits | GIE);   // Wait for conversion to complete
    __bic_SR_register(GIE);

    ADC12_B_disable(ADC12_B_BASE);

    // Log Temperature ADC conversion results to FRAM
    FRAM_TEMP_write_ptr = (unsigned int*)(FRAM_TEMP_RESULTS + *FRAM_index_ptr);
    *FRAM_TEMP_write_ptr = ADC12MEM0;

    // Log Bat (SuperCap) Voltage ADC conversion results to FRAM
    FRAM_BAT_write_ptr = (unsigned int*)(FRAM_BAT_RESULTS + *FRAM_index_ptr);
    *FRAM_BAT_write_ptr = ADC12MEM1;

    *FRAM_index_ptr += 2;

    // Index has reached the end of allocated FRAM
    if (((*FRAM_index_ptr)/2)+1 > 0x1800)
    {
        mode = '0';
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN6);
        __delay_cycles(5000);
    }

    P1OUT &= ~BIT0;

    if (mode == FRAM_LOG_MODE)
    {
        GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

        // Request the disabling of the core voltage regulator when device enters
        // LPM3 (or LPM4) so that we can effectively enter LPM3.5 (or LPM4.5).
        PMM_turnOffRegulator();

        //Enter LPM3 mode with interrupts enabled
        __bis_SR_register(LPM4_bits + GIE);
        __no_operation();
    }
}

void transmitFRAMData()
{
	FRAM_index_ptr = (unsigned int*)FRAM_TEMP_INDEX;

	// Select UART TXD on P2.0
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0, GPIO_SECONDARY_MODULE_FUNCTION);

    __delay_cycles(900000);

    // Send FRAM Index
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(*FRAM_index_ptr>>8));
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, (uint8_t)(*FRAM_index_ptr));

    __delay_cycles(900000);

    uint16_t i;

    for (i=0;i<*FRAM_index_ptr;i+=2)
    {
    	// Send logged Temperature Sensor ata
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, *(uint8_t *)(FRAM_TEMP_RESULTS+i+1));
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, *(uint8_t *)(FRAM_TEMP_RESULTS+i));

        __delay_cycles(900000);
    }

    for (i=0;i<*FRAM_index_ptr;i+=2)
	{
    	// Send logged Battery Monitor data
    	EUSCI_A_UART_transmitData(EUSCI_A0_BASE, *(uint8_t *)(FRAM_BAT_RESULTS+i+1));
		EUSCI_A_UART_transmitData(EUSCI_A0_BASE, *(uint8_t *)(FRAM_BAT_RESULTS+i));

		__delay_cycles(900000);
	}

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    mode = 0;
}

void storeTimeStamp()
{
	storageDone = 0;

    while(storageDone < 6)
    {
        __bis_SR_register(LPM3_bits | GIE);       // Enter LPM3. Delay for Ref to settle.
        __no_operation();

        timeStamp[storageDone] = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
    	storageDone++;
    }
}
