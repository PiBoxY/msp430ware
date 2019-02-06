/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
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

#include "msp430.h"
#include "stdint.h"

// Header files for driverlib
#include "driverlib.h"

// Header files for IEC
#include "IEC60730_cpu_test.h"
#include "IEC60730_pc_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include "IEC60730_clock_fail_test.h"
#include "IEC60730_march_test.h"
#include "IEC60730_crc_test.h"
#include "IEC60730_adc_test.h"
#include "IEC60730_gpio_test.h"



#define TEST_ADC10_INTERRUPT 			0x00000001
#define TEST_COMP_D_INTERRUPT 			0x00000002
#define TEST_DMA_INTERRUPT 				0x00000004
#define TEST_PORT1_INTERRUPT 			0x00000008
#define TEST_PORT2_INTERRUPT 			0x00000010
#define TEST_PORT3_INTERRUPT 			0x00000020
#define TEST_PORT4_INTERRUPT 			0x00000040
#define TEST_RTC_INTERRUPT 				0x00000080
#define TEST_TIMER0_A0_INTERRUPT 		0x00000100
#define TEST_TIMER0_A1_INTERRUPT 		0x00000200
#define TEST_TIMER0_B0_INTERRUPT 		0x00000400
#define TEST_TIMER0_B1_INTERRUPT 		0x00000800
#define TEST_TIMER1_A0_INTERRUPT 		0x00001000
#define TEST_TIMER1_A1_INTERRUPT		0x00002000
#define TEST_TIMER1_B0_INTERRUPT		0x00004000
#define TEST_TIMER1_B1_INTERRUPT		0x00008000
#define TEST_TIMER2_B0_INTERRUPT		0x00010000
#define TEST_TIMER2_B1_INTERRUPT		0x00020000
#define TEST_USCI_A0_INTERRUPT			0x00040000
#define TEST_USCI_A1_INTERRUPT			0x00080000
#define TEST_USCI_B0_INTERRUPT			0x00100000
#define TEST_WDT_INTERRUPT				0x00200000


// Defines for to determine failure
#define CPU_TEST_FAILURE_BLINK			1
#define PC_TEST_FAILURE_BLINK			2
#define OSCILLATOR_TEST_FAILURE_BLINK	3
#define MARCH_TEST_FAILURE_BLINK		4
#define CRC_TEST_FAILURE_BLINK			5
#define INTERRUPT_TEST_FAILURE_BLINK	6
#define ADC_TEST_FAILURE_BLINK			7
#define GPIO_INPUT_TEST_FAILURE_BLINK	8
#define GPIO_OUTPUT_TEST_FAILURE_BLINK	9


// Define to handle failure of an IEC60730 Class B test
void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks);

uint16_t IEC60730_INTERRUPT_TEST_testInterrupt(uint32_t interruptTestRegister);


void configureWDT(void);
void initGPIO(void);
void initCLK(void);
void initADC(void);
void initREF(void);
void runPOST(void);
void runPST(void);

uint32_t gInterruptCounter=0;

void main(void){

	configureWDT();
	initGPIO();
	initCLK();
	initADC();
	initREF();


	runPOST();


	for(;;){

		runPST();

	}
}

void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks){
	uint8_t i=0;
	GPIO_setOutputLowOnPin(GPIO_PORT_P3,
							GPIO_PIN6);
	for(i=blinks;i>0;i--){
		GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);
		__delay_cycles(60000);
		GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
		__delay_cycles(5000000);
		WDT_A_resetTimer(WDT_A_BASE);
	}

	for(;;);

}

void runPST(void){
	if(IEC60730_PC_TEST_testPcRegister()==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
			IEC60730_FAIL_SAFE_failSafe(PC_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_OSCILLATOR_TEST_testOsc()==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(OSCILLATOR_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_MARCH_TEST_testRam((uint16_t*) RAM_START_ADDRESS , (uint16_t*)RAM_END_ADDRESS)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(MARCH_TEST_FAILURE_BLINK);
#endif
	}
}


void runPOST(void){
	IEC60730_ADC_TEST_adcTest_Handle adcTestHandle;

	if(IEC60730_CPU_TEST_testCpuRegisters()==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
			IEC60730_FAIL_SAFE_failSafe(CPU_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_PC_TEST_testPcRegister()==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
			IEC60730_FAIL_SAFE_failSafe(PC_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_OSCILLATOR_TEST_testOsc()==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(OSCILLATOR_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_MARCH_TEST_testRam((uint16_t*) RAM_START_ADDRESS , (uint16_t*)RAM_END_ADDRESS)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(MARCH_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_INTERRUPT_TEST_testInterrupt(TEST_ADC10_INTERRUPT 		|
											 TEST_COMP_D_INTERRUPT 		|
											 TEST_DMA_INTERRUPT 		|
											 TEST_PORT1_INTERRUPT 		|
											 TEST_PORT2_INTERRUPT 		|
											 TEST_PORT3_INTERRUPT 		|
											 TEST_PORT4_INTERRUPT 		|
											 TEST_RTC_INTERRUPT 		|
											 TEST_TIMER0_A0_INTERRUPT	|
											 TEST_TIMER0_A1_INTERRUPT	|
											 TEST_TIMER0_B0_INTERRUPT	|
											 TEST_TIMER0_B1_INTERRUPT	|
											 TEST_TIMER1_A0_INTERRUPT	|
											 TEST_TIMER1_A1_INTERRUPT	|
											 TEST_TIMER1_B0_INTERRUPT	|
											 TEST_TIMER1_B1_INTERRUPT	|
											 TEST_TIMER2_B0_INTERRUPT	|
											 TEST_TIMER2_B1_INTERRUPT	|
											 TEST_USCI_A0_INTERRUPT		|
											 TEST_USCI_A1_INTERRUPT		|
											 TEST_USCI_B0_INTERRUPT		|
											 TEST_WDT_INTERRUPT)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(INTERRUPT_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_CRC_TEST_testNvMemory((uint16_t*) 0xC200, 0x3D80 ,(uint16_t*) CRC_CHECKSUM_LOCATION)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
	}

	ADC10_B_configureMemory(ADC10_B_BASE,
					ADC10_B_INPUT_BATTERYMONITOR,
					ADC10_B_VREFPOS_INT,
					ADC10_B_VREFNEG_AVSS);

	adcTestHandle.muxChannel=11;
	adcTestHandle.pinCount=0x3FF;
	adcTestHandle.useInternalInput=INT_REF_1_5_V;


	if(IEC60730_ADC_TEST_testAdcInput(&adcTestHandle)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
			IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_GPIO_TEST_testGpioOutput(PORT_3, PIN4 | PIN5| PIN6 | PIN7)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
			IEC60730_FAIL_SAFE_failSafe(GPIO_OUTPUT_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_GPIO_TEST_testGpioInput(PORT_3, PIN0 | PIN1 | PIN2 | PIN3, PIN0_LOW | PIN1_LOW | PIN2_LOW | PIN3_LOW )==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
	IEC60730_FAIL_SAFE_failSafe(GPIO_INPUT_TEST_FAILURE_BLINK);
#endif
	}
}

void configureWDT(void){

#if ENABLED_WDT
	// Start Watchdog timer on interval mode using ACLK/32K to generate a 1 sec Watchdog interval.
	WDT_A_initWatchdogTimer(WDT_A_BASE,WDT_A_CLOCKSOURCE_ACLK,WDT_A_CLOCKDIVIDER_32K);
	WDT_A_start(WDT_A_BASE);
#else
	WDT_A_hold(WDT_A_BASE);
#endif
}

void initGPIO(void){


	GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
	GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);

	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);

	// XT1 Setup
	//Set PJ.4 and PJ.5 as Primary Module Function Input.
	/*
	  * Select Port J
	  * Set Pin 4, 5 to input Primary Module Function, XT1.
	*/
	GPIO_setAsPeripheralModuleFunctionInputPin(
		GPIO_PORT_PJ,
		GPIO_PIN4 + GPIO_PIN5,
		GPIO_PRIMARY_MODULE_FUNCTION
	);
}

void initCLK(){
    //Initializes the XT1 crystal oscillator with no timeout
    //In case of failure, code hangs here.
    //For time-out instead of code hang use UCS_LFXT1StartWithTimeout()
    CS_turnOnXT1(CS_XT1_DRIVE_0);


    CS_initClockSignal(CS_ACLK,CS_XT1CLK_SELECT,CS_CLOCK_DIVIDER_1);

#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_3);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);
#elif defined(MAIN_CLOCK_FREQUENCY_8MHz)
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_3);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
#elif defined(MAIN_CLOCK_FREQUENCY_12MHz)
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_3);
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_2);
#endif
}

void initADC(void){

    //Initialize the ADC10_B Module
    /*
     * Base Address of ADC10_B Module
     * Use internal ADC10_B bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC10_B_init(ADC10_B_BASE,
        ADC10_B_SAMPLEHOLDSOURCE_SC,
        ADC10_B_CLOCKSOURCE_ADC10OSC,
        ADC10_B_CLOCKDIVIDER_1);

    ADC10_B_enable(ADC10_B_BASE);

    /*
     * Base Address of ADC10_B Module
     * For memory buffers 0-7 sample/hold for 256 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC10_B_setupSamplingTimer(ADC10_B_BASE,
        ADC10_B_CYCLEHOLD_32_CYCLES,
        ADC10_B_MULTIPLESAMPLESDISABLE);
}

void initREF(){
    Ref_setReferenceVoltage(REF_BASE,REF_VREF1_5V);
    Ref_enableReferenceVoltage(REF_BASE);
}


uint16_t IEC60730_INTERRUPT_TEST_testInterrupt(uint32_t interruptTestRegister){

	__bis_SR_register(GIE);

	if(interruptTestRegister&TEST_ADC10_INTERRUPT){
		// Generate ADC Interrupt in SW
		ADC10_B_enableInterrupt(ADC10_B_BASE,ADC10IE0);
		ADC10IFG |= ADC10IFG0;
	}

	if(interruptTestRegister&TEST_COMP_D_INTERRUPT){
		// Generate COMP_D Interrupt in SW
		CDINT |= (CDIE + CDIFG);
	}


	if(interruptTestRegister&TEST_DMA_INTERRUPT){
		// Generate DMA Interrupt in SW
		DMA0CTL |= (DMAIE + DMAIFG);
	}

	if(interruptTestRegister&TEST_PORT1_INTERRUPT){
		// Generate PORT1 Interrupt in SW
		GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN7);
		P1IFG |=  BIT7;
	}

	if(interruptTestRegister&TEST_PORT2_INTERRUPT){
		// Generate PORT1 Interrupt in SW
		GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN7);
		P2IFG |=  BIT7;
	}

	if(interruptTestRegister&TEST_PORT3_INTERRUPT){
		// Generate PORT1 Interrupt in SW
		GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN7);
		P3IFG |=  BIT7;
	}

	if(interruptTestRegister&TEST_PORT4_INTERRUPT){
		// Generate PORT1 Interrupt in SW
		GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
		P4IFG |=  BIT0;
	}

	if(interruptTestRegister&TEST_RTC_INTERRUPT){
		// Generate RTC Interrupt in SW
		RTCCTL0 |= (RTCTEVIE + RTCTEVIFG);
	}

	if(interruptTestRegister&TEST_TIMER0_A0_INTERRUPT){
		// Generate T0A0 Interrupt in SW
		TA0CCTL0 |= CCIE +CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER0_A1_INTERRUPT){
		// Generate T0A1 Interrupt in SW
		TA0CCTL1 |= CCIE +CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER0_B0_INTERRUPT){
		// Generate T0B0 Interrupt in SW
		TB0CCTL0 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER0_B1_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TB0CCTL1 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER1_A0_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TA1CCTL0 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER1_A1_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TA1CCTL1 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER1_B0_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TB1CCTL0 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER1_B1_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TB1CCTL1 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER2_B0_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TB2CCTL0 |= CCIE + CCIFG;
	}

	if(interruptTestRegister&TEST_TIMER2_B1_INTERRUPT){
		// Generate T0B1 Interrupt in SW
		TB2CCTL1 |= CCIE + CCIFG;
	}


	if(interruptTestRegister&TEST_USCI_A0_INTERRUPT){
		// Initialize eUSCI state machine
		EUSCI_A_SPI_enable(EUSCI_A0_BASE);
		// Enable eUSCI_A1 TX interrupt
		EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE,
								EUSCI_A_SPI_TRANSMIT_INTERRUPT);
	}

	if(interruptTestRegister&TEST_USCI_A1_INTERRUPT){
		// Initialize eUSCI state machine
		EUSCI_A_SPI_enable(EUSCI_A1_BASE);
		// Enable eUSCI_A1 TX interrupt
		EUSCI_A_SPI_enableInterrupt(EUSCI_A1_BASE,
									 EUSCI_A_SPI_TRANSMIT_INTERRUPT);
	}

	if(interruptTestRegister&TEST_USCI_B0_INTERRUPT){
		// Initialize eUSCI state machine
		EUSCI_B_SPI_enable(EUSCI_B0_BASE);
		// Enable eUSCI_A1 TX interrupt
		EUSCI_B_SPI_enableInterrupt(EUSCI_B0_BASE,
									 EUSCI_B_SPI_TRANSMIT_INTERRUPT);
	}


	if(interruptTestRegister&TEST_WDT_INTERRUPT){
		WDT_A_initIntervalTimer( WDT_A_BASE,
								 WDT_A_CLOCKSOURCE_ACLK,
								 WDT_A_CLOCKDIVIDER_32K);
		// Enable WDY interrupt.
		SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
		// Generate Software WDT interrupt.
		SFRIFG1 |= WDTIFG;
		// No operation required to allow WDT interrupt to be triggered
		__no_operation();
		configureWDT();
	}


	// Disable Global interrupts after interrupt test completion
	__bic_SR_register(GIE);

	if(gInterruptCounter!= interruptTestRegister){
			return TEST_FAILURE;
	}

	return SIG_INTERRUPT_TEST;
}


#pragma vector = ADC10_VECTOR
__interrupt void ADC10_Isr(void)
{
	gInterruptCounter |= TEST_ADC10_INTERRUPT;
	ADC10_B_disableInterrupt(ADC10_B_BASE,ADC10IE0);
	ADC10_B_clearInterrupt(ADC10_B_BASE,ADC10IFG0);
}


#pragma vector = COMP_D_VECTOR
__interrupt void COMP_D_Isr(void)
{
	gInterruptCounter |= TEST_COMP_D_INTERRUPT;
	Comp_D_disableInterrupt(COMP_D_BASE, COMP_D_INTERRUPT);
	Comp_D_clearInterrupt(COMP_D_BASE, COMP_D_INTERRUPT );
}


#pragma vector = DMA_VECTOR
__interrupt void DMA_Isr(void)
{
	gInterruptCounter |= TEST_DMA_INTERRUPT;
	DMA_clearInterrupt(DMA_CHANNEL_0);
	DMA_disableInterrupt(DMA_CHANNEL_0);
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_Isr(void)
{
	gInterruptCounter |= TEST_PORT1_INTERRUPT;
	GPIO_disableInterrupt(GPIO_PORT_P1, GPIO_PIN7);
	GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN7);
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_Isr(void)
{
	gInterruptCounter |= TEST_PORT2_INTERRUPT;
	GPIO_disableInterrupt(GPIO_PORT_P2, GPIO_PIN7);
	GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7);
}

#pragma vector = PORT3_VECTOR
__interrupt void PORT3_Isr(void)
{
	gInterruptCounter |= TEST_PORT3_INTERRUPT;
	GPIO_disableInterrupt(GPIO_PORT_P3, GPIO_PIN7);
	GPIO_clearInterrupt(GPIO_PORT_P3, GPIO_PIN7);
}

#pragma vector = PORT4_VECTOR
__interrupt void PORT4_Isr(void)
{
	gInterruptCounter |= TEST_PORT4_INTERRUPT;
	GPIO_disableInterrupt(GPIO_PORT_P4, GPIO_PIN0);
	GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN0);
}

#pragma vector = RTC_VECTOR
__interrupt void RTC_Isr(void)
{
	gInterruptCounter |= TEST_RTC_INTERRUPT;
	RTC_B_disableInterrupt(RTC_B_BASE, RTC_B_TIME_EVENT_INTERRUPT);
	RTC_B_clearInterrupt(RTC_B_BASE, RTC_B_TIME_EVENT_INTERRUPT);
}

#pragma vector = SYSNMI_VECTOR
__interrupt void SYSNMI_Isr(void)
{
	return;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_Isr(void)
{
	gInterruptCounter |= TEST_TIMER0_A0_INTERRUPT;
	Timer_A_disableCaptureCompareInterrupt(TIMER_A0_BASE,
										   TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_Isr(void)
{
	gInterruptCounter |= TEST_TIMER0_A1_INTERRUPT;
	Timer_A_disableCaptureCompareInterrupt(TIMER_A0_BASE,
										   TIMER_A_CAPTURECOMPARE_REGISTER_1);
	Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
											   TIMER_A_CAPTURECOMPARE_REGISTER_1);
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_Isr(void)
{
	gInterruptCounter |= TEST_TIMER0_B0_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B0_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_0);
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_Isr(void)
{
	gInterruptCounter |= TEST_TIMER0_B1_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B0_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_1);
	Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE,
											   TIMER_B_CAPTURECOMPARE_REGISTER_1);
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_Isr(void)
{
	gInterruptCounter |= TEST_TIMER1_A0_INTERRUPT;
	Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,
										   TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_Isr(void)
{
	gInterruptCounter |= TEST_TIMER1_A1_INTERRUPT;
	Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,
										   TIMER_A_CAPTURECOMPARE_REGISTER_1);
}

#pragma vector = TIMER1_B0_VECTOR
__interrupt void TIMER1_B0_Isr(void)
{
	gInterruptCounter |= TEST_TIMER1_B0_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B1_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_0);
}

#pragma vector = TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_Isr(void)
{
	gInterruptCounter |= TEST_TIMER1_B1_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B1_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_1);
	Timer_B_clearCaptureCompareInterrupt(TIMER_B0_BASE,
												   TIMER_B_CAPTURECOMPARE_REGISTER_1);
}

#pragma vector = TIMER2_B0_VECTOR
__interrupt void TIMER2_B0_Isr(void)
{
	gInterruptCounter |= TEST_TIMER2_B0_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B2_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_0);
}

#pragma vector = TIMER2_B1_VECTOR
__interrupt void TIMER2_B1_Isr(void)
{
	gInterruptCounter |= TEST_TIMER2_B1_INTERRUPT;
	Timer_B_disableCaptureCompareInterrupt(TIMER_B2_BASE,
										   TIMER_B_CAPTURECOMPARE_REGISTER_1);
	Timer_B_clearCaptureCompareInterrupt(TIMER_B2_BASE,
													   TIMER_B_CAPTURECOMPARE_REGISTER_1);
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_Isr(void)
{
	gInterruptCounter |= TEST_USCI_A0_INTERRUPT;
	EUSCI_A_SPI_disableInterrupt(EUSCI_A0_BASE,
								  EUSCI_A_SPI_TRANSMIT_INTERRUPT);
}

#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_Isr(void)
{
	gInterruptCounter |= TEST_USCI_A1_INTERRUPT;
	EUSCI_A_SPI_disableInterrupt(EUSCI_A1_BASE,
								  EUSCI_A_SPI_TRANSMIT_INTERRUPT);
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_Isr(void)
{
	gInterruptCounter |= TEST_USCI_B0_INTERRUPT;
	EUSCI_B_SPI_disableInterrupt(EUSCI_B0_BASE,
								  EUSCI_B_SPI_TRANSMIT_INTERRUPT);
}

#pragma vector = WDT_VECTOR
__interrupt void WDT_Isr(void)
{
	gInterruptCounter |= TEST_WDT_INTERRUPT;
	SFR_disableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
}


#pragma vector = UNMI_VECTOR
__interrupt void UNMI_Isr(void)
{
	return;
}



