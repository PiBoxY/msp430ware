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


#include "TI_DRV_IEC60730.h"

uint16_t gInterruptCounter=BITF;

static void BoardConfig(void);
static void blinkStatusLED(uint8_t blinks);
static void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks);
static void runOscillatorTest(void);
static void runVolatileMemoryTest(void);
static void runCPUTest(void);
static void runPCTest(void);
static void runADCTest(void);
static void runNonVolatileMemoryTest(void);
static void runInterruptSweepTest(void);
static void runGPIOInputTest(void);
static void runGPIOOutputTest(void);
static void runSPITest(void);



#ifdef INJECTION_OF_ERROR_ENABLED
extern const IEC_ERROR_INJECTION iecError={
		FALSE,			//cpuError
		FALSE,			//pcError
		FALSE,          //ramError
		TRUE           //adcError
};
#endif /*INJECTION_OF_ERROR_ENABLED*/




uint16_t IEC60730_INTERRUPT_interruptSweep(IEC_ISR_TEST_VECTOR *iecTestVector);

//******************************************************************************
// The following function runs all POST test required by IEC 60730 standard
//******************************************************************************

uint8_t IEC60730_RUN_POST_test(){
	// The following function call configures to board to run the POST test
 	BoardConfig();


/*******************************************************************************
 * The following GPIO tests will be implemented by DRV team.
 ******************************************************************************/
	runGPIOOutputTest();

#ifdef ENABLE_STATUS_LED
	// Start sequence of POST tests
	blinkStatusLED(3);
#endif /*ENABLE_STATUS_LED*/

	runGPIOInputTest();
	runCPUTest();
	runPCTest();
	runVolatileMemoryTest();
	runADCTest();
	runOscillatorTest();
	runNonVolatileMemoryTest();
	runInterruptSweepTest();
	runSPITest();







	return POST_TESTS_PASSED;
}

//******************************************************************************
// The following function runs all PEST test required by IEC 60730 standard
//******************************************************************************
uint8_t IEC60730_RUN_PEST_test(){

#ifdef ENABLE_STATUS_LED
	// Start sequence of POST tests
	blinkStatusLED(3);
#endif /*ENABLE_STATUS_LED*/

	// Disable global interrupts
	__bic_SR_register(GIE);

	runCPUTest();
	runPCTest();
	runVolatileMemoryTest();
	runOscillatorTest();
	runSPITest();
	runNonVolatileMemoryTest();

	//  Enable global interrupts
	__bis_SR_register(GIE);

	return PEST_TESTS_PASSED;
}


//******************************************************************************
// The following function runs oscillator test to confirm main clock is running
// within user defined parameter.
//******************************************************************************
void runOscillatorTest(){

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_OSCILLATOR_TEST_testOsc()==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(OSCILLATOR_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/
}


//******************************************************************************
// The following function runs non-volatile memory test using user defined
// March algorithm.
//******************************************************************************
void runVolatileMemoryTest(){

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_MARCH_TEST_testRam((uint16_t*)RAM_START_ADDRESS, \
			(uint16_t*)RAM_END_ADDRESS)==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(MARCH_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/
}


//******************************************************************************
// The following function runs CPU test.
//******************************************************************************
void runCPUTest(void){

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_CPU_TEST_testCpuRegisters()==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(CPU_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/
}


//******************************************************************************
// The following function runs PC test.
//******************************************************************************
void runPCTest(void){
#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_PC_TEST_testPcRegister()==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(PC_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/
}


//******************************************************************************
// The following function runs ADC test.
//******************************************************************************
void runADCTest(){
	IEC60730_ADC_TEST_adcTest_Handle adcTest;

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	// Initialize SPI communication between MSP and AFE
	SPI_Init();

	TI_SMS_MOTOR_ADC_Init();

	// Configure ADC Test Handler to test 1.2V bandgap channel
	adcTest.muxChannel = ADC_1_2_V_BANDGAP_REF_CH;
	adcTest.pinCount = ADC_EXPECTED_1_2_BG_REF_CONV;

	if(IEC60730_ADC_TEST_testAdcInput(&adcTest)==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
	}

	// Configure ADC Test Handler to measure DAC channel
	adcTest.muxChannel = ADC_DAC_CH;
	adcTest.pinCount = ADC_EXPECTED_DAC_CONV;

	if(IEC60730_ADC_TEST_testAdcInput(&adcTest)==TEST_FAILURE){
			IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
	}

#ifdef INJECTION_OF_ERROR_ENABLED
	if(iecError.adcError == TRUE){
		// Enable Vcc resistor divider in Analog chip
		SPI_Write(ASSIMISC2,TURN_ON_VCC_DIVIDER);

		// Configure ADC Test Handler to test resistor divider of Vcc
		adcTest.muxChannel = ADC_VCC_DIV_12_CH;
		adcTest.pinCount = ADC_VCC_DIV_12_CONV;

		if(IEC60730_ADC_TEST_testAdcInput(&adcTest)==TEST_FAILURE){
					IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
		}
	}

#endif /*INJECTION_OF_ERROR_ENBLED*/

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

}


//******************************************************************************
// The following function runs Non Volatile Memory test using previously saved
// CRC checksum.
//******************************************************************************
void runNonVolatileMemoryTest(void){

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)NON_VOL_MEM_START_ADDRESS, \
			NON_VOL_MEM_SIZE,(uint16_t*) CRC_CHECKSUM_LOCATION)==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/
}

//******************************************************************************
// The following function runs GPIO Input test
//******************************************************************************
void runGPIOInputTest(void){

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_GPIO_TEST_testGpioInput(PORT_4,PIN7,PIN7_HIGH)==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(GPIO_INPUT_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/


}

//******************************************************************************
// The following function runs GPIO Output test
//******************************************************************************
void runGPIOOutputTest(void){
	//TODO Implement GPIO Output test by DRV team.
}

//******************************************************************************
// The following function test SPI communication between Digital and Analog side
//******************************************************************************
void runSPITest(void){
	uint8_t registerValue=0, i=0;

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	for(i=2;i>0;i--){

		drv91670SpiResetAfeSpi();
	    drv91670SpiReleaseAfeSpi();

		registerValue = SPI_Read(ASSIDAC1) & 0xF0;

		if( registerValue != MAJOR_MINOR_VERSION){
			IEC60730_FAIL_SAFE_failSafe(SPI_TEST_FAILURE_BLINK);
		}
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/


}


//******************************************************************************
// The following function runs Interrupt test of user-defined interrupt vectors
//
//******************************************************************************
void runInterruptSweepTest(){
	// To exclude an interrupt for Interrupt Sweep Test write FALSE.
	IEC_ISR_TEST_VECTOR iecTestVector={
		TEST_PORT1_INTERRUPT,  			   	//testPort1Interrupt
		TEST_PORT2_INTERRUPT,              	//testPort2Interrupt
		TEST_PORT3_INTERRUPT,              	//testPort3Interrupt
		TEST_T0A0_INTERRUPT,                //testT0A0Interrupt
		TEST_T0A1_INTERRUPT,   				//testT0A1Interrupt
		TEST_T1A0_INTERRUPT,   				//testT1A0Interrupt
		TEST_T1A1_INTERRUPT,   				//testT1A1Interrupt
		TEST_T0B0_INTERRUPT,   				//testT0B0Interrupt
		TEST_T0B1_INTERRUPT,   				//testT0B1Interrupt
		TEST_USCIA0_INTERRUPT, 				//testUSCIA0Interrupt
		TEST_USCIB0_INTERRUPT,            	//testUSCIB0Interrupt
		TEST_T2A0_INTERRUPT,   				//testT2A0Interrupt
		TEST_T2A1_INTERRUPT,   				//testT2A1Interrupt
		TEST_DMA_INTERRUPT,              	//testDMAInterrupt
		TEST_T3A0_INTERRUPT,                //testT3A0Interrupt
		TEST_T3A1_INTERRUPT,   				//testT3A1Interrupt
		TEST_T4A0_INTERRUPT,   				//testT4A0Interrupt
		TEST_T4A1_INTERRUPT,   				//testT4A1Interrupt
	    TEST_PORT4_INTERRUPT,            	//testPort4Interrupt
		TEST_WDT_INTERRUPT,              	//testWDTInterrupt
	};

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT |= TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	if(IEC60730_INTERRUPT_interruptSweep(&iecTestVector) ==TEST_FAILURE){
		IEC60730_FAIL_SAFE_failSafe(INTERRUPT_TEST_FAILURE_BLINK);
	}

#ifdef BENCHMARK_ENABLED
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

}

//******************************************************************************
// The following function is called when a test failure occurs.
//
// If ENABLE_STATUS_LED is defined in "TI_DRV_IEC60730.h":
// 		1) IEC60730_FAIL_SAFE_failSafe function blinks STATUS_LED
//		   based on error detected.
//      2) Generates a Power Up Clear (PUC).
//
// IF ENABLE_STATUS_LED is NOT defined in TI_DRV_IEC60730.h":
//      1) Generates a Power Up Clear (PUC).
//
//******************************************************************************
static void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks){
#ifdef ENABLE_STATUS_LED
	// Turn off STATUS_LED
	STATUS_LED_OUT |= STATUS_LED_PIN;

	blinkStatusLED(blinks);
#endif /*ENABLE_STATUS_LED*/

	// Generate PUC by writing wrong Password to WDTCTL.
	WDTCTL = ~WDTPW ;

}



#ifdef ENABLE_STATUS_LED
static void blinkStatusLED(uint8_t blinks){
	uint8_t i=0;
	for(i=blinks;i>0;i--){
			// Turn ON STATUS_LED
			STATUS_LED_OUT &= ~STATUS_LED_PIN;
			// Turn OFF STATUS_LED
			STATUS_LED_OUT |= STATUS_LED_PIN;
	}
	STATUS_LED_OUT &= ~STATUS_LED_PIN;
}
#endif /*ENABLE_STATUS_LED*/



static void BoardConfig(void){

	// Clock configuration

	// Configure Simple Clock System (SCS) for
	// MCLK  = 25MHz/1
	// SMCLK = 25MHz/1
	// ACLK  = 25MHz/8
	CCSCTL0 = SCSKEY + DIVA__8 + DIVS__1 + DIVM__1;

	// GPIO Configuration
	// The following section configures GPIO based on the macros that have been
	// defined in "TI_DRV_IEC60730.h"

	// Configure SMCLK_PIN to output SMCLK based on user definition of
	// OUTPUT_SMCLK
#ifdef OUTPUT_SMCLK
	SMCLK_DIR |= SMCLK_PIN;
	SMCLK_SEL |= SMCLK_PIN;
#endif /*OUTPUT_SMCLK*/

	// Configure STATUS_LED_PIN in output direction and turn on STATUS_LED
#ifdef ENABLE_STATUS_LED
	STATUS_LED_DIR |= STATUS_LED_PIN;
	// Turn ON STATUS_LED
	STATUS_LED_OUT &= ~STATUS_LED_PIN;
#endif /*ENABLE_STATUS_LED*/

	// Set TEST_LED_PIN as output and clear pin
#ifdef BENCHMARK_ENABLED
	TEST_LED_DIR |= TEST_LED_PIN;
	TEST_LED_OUT &= ~TEST_LED_PIN;
#endif /*BENCHMARK_ENABLED*/

	// Disable external oscillator
#ifdef EXTERNAL_OSC_HAS_ENABLE_PIN
	ENABLE_OSC_DIR |= ENABLE_OSC_PIN;
	ENABLE_OSC_OUT &= ~ENABLE_OSC_PIN;
#endif /*EXTERNAL_OSC_HAS_ENABLE_PIN*/



}


//******************************************************************************
// The following function triggers every interrupt available on the device.
//******************************************************************************
uint16_t IEC60730_INTERRUPT_interruptSweep(IEC_ISR_TEST_VECTOR *iecTestVector){

	uint8_t *pTemp = (uint8_t*)iecTestVector;
	uint16_t i , expectedIntCount =0;


	// Enable global interrupts
	__bis_SR_register(GIE);

	if(iecTestVector->testPort1Interrupt){
		P1IE  |=  BIT7;
		P1IFG |=  BIT7;
	}

	if(iecTestVector->testPort2Interrupt){
		P2IE |=   BIT7;
		P2IFG |=  BIT7;
	}

	if(iecTestVector->testPort3Interrupt){
		P3IE |=   BIT7;
		P3IFG |=  BIT7;
	}


	if(iecTestVector->testPort4Interrupt){
		P4IE  |=   BIT7;
		P4IFG |=  BIT7;

	}

	if(iecTestVector->testDMAInterrupt){
		DMA0CTL |= (DMAIE + DMAIFG);
	}

	if(iecTestVector->testT0A0Interrupt){
		TA0CCTL0 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT0A1Interrupt){
		TA0CCTL1 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT1A0Interrupt){
		TA1CCTL0 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT1A1Interrupt){
		TA1CCTL1 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT2A0Interrupt){
		TA2CCTL0 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT2A1Interrupt){
		TA2CCTL1 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT3A0Interrupt){
		TA3CCTL0 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT3A1Interrupt){
		TA3CCTL1 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT4A0Interrupt){
		TA4CCTL0 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT4A1Interrupt){
		TA4CCTL1 |= CCIE +CCIFG;
	}

	if(iecTestVector->testT0B0Interrupt){
		TB0CCTL0 |= CCIE + CCIFG;
	}

	if(iecTestVector->testT0B1Interrupt){
		TB0CCTL1 |= CCIE + CCIFG;
	}

	if(iecTestVector->testUSCIA0Interrupt){
		// Initialize USCI state machine
		UCA0CTL1 &= ~UCSWRST;
		// Enable USCI_A0 TX interrupt
		UCA0IE |= UCTXIE;
	}

	if(iecTestVector->testUSCIB0Interrupt){
		// Initialize USCI state machine
		UCB0CTL1 &= ~UCSWRST;
		// Enable USCI_B0 TX interrupt
		UCB0IE |= UCTXIE;
	}

	if(iecTestVector->testWDTInterrupt){
		// Set-up watch dog in interval to be able to jump to WDT ISR.
		WDTCTL = WDTPW + WDTTMSEL;
		// Enable WDY interrupt.
		SFRIE1 |= WDTIE;
		// Generate Software WDT interrupt.
		SFRIFG1 |= WDTIFG;
	}

	// Added delay operation to ensure Watchdog Timer Interrupt is triggered
	__delay_cycles(3);


	// Clear embedded flag form gInterruptCounter to avoid running Interrupt
	// Sweep test for ever.
	gInterruptCounter &= ~BITF;

	// Disable Global interrupts Interrupt test completion
	__bic_SR_register(GIE);


	// Determine expected value for gInterruptCounter
	for(i=INTERRUPT_VECTOR_COUNT;i>0;i--){
		if(*pTemp){
			expectedIntCount += *pTemp;
		}
		pTemp++;
	}

	if(gInterruptCounter == expectedIntCount){
		return SIG_INTERRUPT_TEST;

	}

	return TEST_FAILURE;
}


