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


#include "msp430g2553.h"
#include "stdint.h"
#include "IEC60730_cpu_test.h"
#include "IEC60730_pc_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include "IEC60730_clock_fail_test.h"
#include "IEC60730_march_test.h"
#include "IEC60730_crc_test.h"
#include "IEC60730_adc_test.h"
#include "IEC60730_gpio_test.h"



// The following defines are used to specify which interrupts to test
#define TEST_PORT1_INTERRUPT 			0x0001
#define TEST_PORT2_INTERRUPT 			0x0002
#define TEST_ADC10_INTERRUPT 			0x0004
#define TEST_USCIAB0TX_INTERRUPT 		0x0008
#define TEST_USCIAB0RX_INTERRUPT 		0x0010
#define TEST_TIMER0_A1_INTERRUPT 		0x0020
#define TEST_TIMER0_A0_INTERRUPT 		0x0040
#define TEST_WDT_INTERRUPT 		        0x0080
#define TEST_COMPARATORA_INTERRUPT 		0x0100
#define TEST_TIMER1_A1_INTERRUPT 		0x0200
#define TEST_TIMER1_A0_INTERRUPT 		0x0400


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

uint16_t IEC60730_INTERRUPT_TEST_testInterrupt(uint16_t interuptTestRegister);
void initWDT(void);
void initCLK(void);
void initGPIO(void);
void resetWDT(void);
void initADC(void);
void runPOST(void);
void runPST(void);

// Global variables
uint16_t gInterruptCounter=0;

void main(void) {



	initWDT();
	initCLK();
	initGPIO();
	initADC();

	runPOST();

	for(;;){

            runPST();
	}
}


void runPST(){
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

	if (IEC60730_INTERRUPT_TEST_testInterrupt(TEST_PORT1_INTERRUPT  |
										TEST_PORT2_INTERRUPT 		|
										TEST_ADC10_INTERRUPT 	 	|
										TEST_USCIAB0TX_INTERRUPT 	|
										TEST_USCIAB0RX_INTERRUPT 	|
										TEST_TIMER0_A1_INTERRUPT 	|
										TEST_TIMER0_A0_INTERRUPT 	|
										TEST_WDT_INTERRUPT 			|
										TEST_COMPARATORA_INTERRUPT 	|
										TEST_TIMER1_A1_INTERRUPT 	|
										TEST_TIMER1_A0_INTERRUPT)== TEST_FAILURE){

#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(INTERRUPT_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_MARCH_TEST_testRam((uint16_t*)RAM_START_ADDRESS, (uint16_t*)RAM_END_ADDRESS) == TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(MARCH_TEST_FAILURE_BLINK);
#endif
	}

	 if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)0xC000,0x3FC0,(uint16_t*)CRC_CHECKSUM_LOCATION)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		 IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
	  }

	// Select input channel 1 for ADC
	ADC10CTL1 = INCH_8;

	// Set-up struct to test ADC input channel 8 with expected value of 0x3FF using internal voltage
	// reference of 2.5V
	adcTestHandle.muxChannel=8;
	adcTestHandle.pinCount=0x3FF;
	adcTestHandle.useInternalInput=INT_REF_2_5_V;

	if(IEC60730_ADC_TEST_testAdcInput(&adcTestHandle) == TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
#endif
	}

	// Select input channel 11 for ADC
	ADC10CTL1 = INCH_11;

	adcTestHandle.muxChannel=11;
	adcTestHandle.pinCount=720;
	adcTestHandle.useInternalInput=INT_REF_2_5_V;

	if(IEC60730_ADC_TEST_testAdcInput(&adcTestHandle)== TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_GPIO_TEST_testGpioOutput(PORT_2, PIN0|PIN1|PIN2) == TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(GPIO_OUTPUT_TEST_FAILURE_BLINK);
#endif
	}

	if(IEC60730_GPIO_TEST_testGpioInput(PORT_2, PIN3|PIN4|PIN5, PIN3_LOW|PIN4_LOW|PIN5_LOW) == TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(GPIO_INPUT_TEST_FAILURE_BLINK);
#endif
	}
}


void initWDT(void){
#if ENABLED_WDT
		WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;
#else
		WDTCTL = WDTPW + WDTHOLD;
#endif
}


void initCLK(void){

#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
#elif defined(MAIN_CLOCK_FREQUENCY_8MHz)
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
#elif defined(MAIN_CLOCK_FREQUENCY_12MHz)
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL = CALDCO_12MHZ;
#endif
}


void initGPIO(void){

	P1DIR |= BIT0 + BIT6;
	P1OUT &= ~BIT0;
	P1OUT |= BIT6;

	P2DIR |= BIT0 + BIT1 +BIT2;
	P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);

	P2REN |= BIT3 + BIT4 + BIT5;

	P3DIR = 0xFF;
	P3OUT = 0x00;

}



void initADC(void){

	// Initialize ADC
	//
	// Select Vr+ = Vref+ and Vr-= Vss
	// Enable reference output.
	// Set reference generator to 2.5V
	// Turn on ADC module
	//
	ADC10CTL0 = SREF_1 + REFOUT + REFON + REF2_5V + ADC10ON + ADC10IE;

	// Enable P1.4 in analog mode
	ADC10AE0 |= BIT1 + BIT2 + BIT3 + BIT4 + BITB;

}

void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks){
	uint8_t i=0;
	P1OUT &= ~BIT6;

	for(i=blinks;i>0;i--){
		P1OUT |= BIT0;
		__delay_cycles(60000);
		P1OUT &= ~BIT0;
		__delay_cycles(5000000);

		WDTCTL = WDTPW + WDTCNTCL + WDTSSEL;
	}

	// Wait in infinite loop until watchdog expires.
	for(;;);

}


uint16_t IEC60730_INTERRUPT_TEST_testInterrupt(uint16_t interruptTestRegister){

	__bis_SR_register(GIE);

	if(interruptTestRegister&TEST_PORT1_INTERRUPT){
		P1IE |=   BIT7;
		P1IFG |=  BIT7;
	}

	if(interruptTestRegister&TEST_PORT2_INTERRUPT){
		P2IE |=   BIT7;
		P2IFG |=  BIT7;
	}

	if(interruptTestRegister&TEST_ADC10_INTERRUPT){
		ADC10CTL0 |= ADC10IE + ADC10IFG;
	}

	if(interruptTestRegister&TEST_USCIAB0TX_INTERRUPT){
		// Initialize USCI state machine
		UCA0CTL1 &= ~UCSWRST;
		// Enable USCI_A0 TX interrupt
		IE2 |= UCA0TXIE;
	}
	if(interruptTestRegister&TEST_USCIAB0RX_INTERRUPT){
		IE2 |= UCA0RXIE;
		IFG2 |= UCA0RXIFG;
	}
	if(interruptTestRegister&TEST_TIMER1_A1_INTERRUPT){
		TA1CCTL1 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER1_A0_INTERRUPT){
		TA1CCTL0 |= CCIE + CCIFG;
	}
	if(interruptTestRegister&TEST_WDT_INTERRUPT){
		// Set-up watch dog in interval to be able to just to WDT ISR.
		WDTCTL = WDTPW + WDTNMI + WDTTMSEL;
		// Enable WDY interrupt.
		IE1 |= WDTIE;
		// Generate Software WDT interrupt.
		IFG1 |= WDTIFG;
	}
	if(interruptTestRegister&TEST_COMPARATORA_INTERRUPT){
		CACTL1 |= CAIE + CAIFG;
	}
	if(interruptTestRegister&TEST_TIMER0_A1_INTERRUPT){
		TA0CCTL1 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER0_A0_INTERRUPT){
		TA0CCTL0 |= CCIE +CCIFG;
	}

	// Disable Global interrupts Interrupt test completion
	__bic_SR_register(GIE);

	if(gInterruptCounter!= interruptTestRegister){
		return TEST_FAILURE;
	}

	return SIG_INTERRUPT_TEST;

}

// ISR
#pragma vector=ADC10_VECTOR
__interrupt void ADC10ISR (void)
{
	gInterruptCounter |= TEST_ADC10_INTERRUPT;
	ADC10CTL0 &= ~ADC10IE;
}
#pragma vector=COMPARATORA_VECTOR
__interrupt void COMPARATORAISR (void)
{
	gInterruptCounter |= TEST_COMPARATORA_INTERRUPT;
	CACTL1 &= ~(CAIE + CAIFG);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1ISR (void)
{
	gInterruptCounter |= TEST_PORT1_INTERRUPT;
	P1IE &=  ~BIT7;
	P1IFG &= ~BIT7;
}
#pragma vector=PORT2_VECTOR
__interrupt void PORT2ISR (void)
{
	gInterruptCounter |= TEST_PORT2_INTERRUPT;
	P2IE &=  ~BIT7;
	P2IFG &= ~BIT7;
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0ISR (void)
{
	gInterruptCounter |= TEST_TIMER0_A0_INTERRUPT;
	TA0CCTL0 &= ~(CCIE +CCIFG);
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1ISR (void)
{
	gInterruptCounter |= TEST_TIMER0_A1_INTERRUPT;
	TA0CCTL1 &= ~(CCIE +CCIFG);

}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0ISR (void)
{
	gInterruptCounter |= TEST_TIMER1_A0_INTERRUPT;
	TA1CCTL0 &= ~(CCIE +CCIFG);

}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1ISR (void)
{
	gInterruptCounter |= TEST_TIMER1_A1_INTERRUPT;
	TA1CCTL1 &= ~(CCIE +CCIFG);
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIAB0RXISR (void)
{
	gInterruptCounter |= TEST_USCIAB0RX_INTERRUPT;
	IE2 &= ~UCA0RXIE;
	IFG2 &= ~UCA0RXIFG;

}
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIAB0TXISR (void)
{
	gInterruptCounter |= TEST_USCIAB0TX_INTERRUPT;
	IE2 &= ~UCA0TXIE;

}

#pragma vector=WDT_VECTOR
__interrupt void WDTISR (void)
{
	gInterruptCounter |= TEST_WDT_INTERRUPT;
	IE1 &= ~WDTIE;
	IFG1 &= ~WDTIFG;
#if ENABLED_WDT
		WDTCTL = WDTPW + WDTCNTCL +WDTSSEL;
#else
		WDTCTL = WDTPW + WDTHOLD;
#endif
}

#pragma vector=NMI_VECTOR
__interrupt void NMIISR (void)
{
	return;
}
