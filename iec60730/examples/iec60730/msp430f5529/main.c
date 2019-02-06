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

#include "IEC60730_cpu_test.h"
#include "IEC60730_pc_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include "IEC60730_clock_fail_test.h"
#include "IEC60730_march_test.h"
#include "IEC60730_crc_test.h"
#include "IEC60730_adc_test.h"
#include "IEC60730_gpio_test.h"
#include "driverlib.h"


// Defines used by ucs.h to set MCLK frequency
#if defined(MAIN_CLOCK_FREQUENCY_12MHz)
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ	        12000
#define UCS_MCLK_FLLREF_RATIO				366
#elif defined (MAIN_CLOCK_FREQUENCY_8MHz)
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ	        8000
#define UCS_MCLK_FLLREF_RATIO				244
#elif defined(MAIN_CLOCK_FREQUENCY_1MHz)
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ	        1000
#define UCS_MCLK_FLLREF_RATIO				30
#endif

// The following defines are used to specify which interrupts to test
#define TEST_ADC12_INTERRUPT 			0x00000001
#define TEST_COMP_B_INTERRUPT 			0x00000002
#define TEST_DMA_INTERRUPT 				0x00000004
#define TEST_PORT1_INTERRUPT 			0x00000008
#define TEST_PORT2_INTERRUPT 			0x00000010
#define TEST_RTC_INTERRUPT 				0x00000020
#define TEST_TIMER0_A0_INTERRUPT 		0x00000040
#define TEST_TIMER0_A1_INTERRUPT 		0x00000080
#define TEST_TIMER0_B0_INTERRUPT 		0x00000100
#define TEST_TIMER0_B1_INTERRUPT 		0x00000200
#define TEST_TIMER1_A0_INTERRUPT 		0x00000400
#define TEST_TIMER1_A1_INTERRUPT		0x00000800
#define TEST_TIMER2_A0_INTERRUPT		0x00001000
#define TEST_TIMER2_A1_INTERRUPT		0x00002000
#define TEST_USB_UBM_INTERRUPT			0x00004000
#define TEST_USCI_A0_INTERRUPT			0x00008000
#define TEST_USCI_A1_INTERRUPT			0x00010000
#define TEST_USCI_B0_INTERRUPT			0x00020000
#define TEST_USCI_B1_INTERRUPT			0x00040000
#define TEST_WDT_INTERRUPT				0x00080000

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
void runPOST(void);
void runPST(void);
void configureWDT(void);
void initGPIO(void);
void initCLK(void);
void initADC(void);
void initREF(void);


uint32_t gInterruptCounter=0;

void main(void) {

	// Setup watch dog timer based on
	configureWDT();
	initGPIO();
	initADC();
	initREF();
	initCLK();


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

		// OR interrupts to be tested
		if(IEC60730_INTERRUPT_TEST_testInterrupt(TEST_ADC12_INTERRUPT 		|
												TEST_COMP_B_INTERRUPT 		|
												TEST_DMA_INTERRUPT 			|
												TEST_PORT1_INTERRUPT 		|
												TEST_PORT2_INTERRUPT 		|
												TEST_RTC_INTERRUPT 			|
												TEST_TIMER0_A0_INTERRUPT 	|
												TEST_TIMER0_A1_INTERRUPT 	|
												TEST_TIMER0_B0_INTERRUPT 	|
												TEST_TIMER0_B1_INTERRUPT 	|
												TEST_TIMER1_A0_INTERRUPT 	|
												TEST_TIMER1_A1_INTERRUPT 	|
												TEST_TIMER2_A0_INTERRUPT 	|
												TEST_TIMER2_A1_INTERRUPT 	|
												TEST_USB_UBM_INTERRUPT 		|
												TEST_USCI_A0_INTERRUPT 		|
												TEST_USCI_A1_INTERRUPT 		|
												TEST_USCI_B0_INTERRUPT 		|
												TEST_USCI_B1_INTERRUPT 		|
												TEST_WDT_INTERRUPT) ==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(INTERRUPT_TEST_FAILURE_BLINK);
#endif
		}

		if(IEC60730_MARCH_TEST_testRam((uint16_t*) RAM_START_ADDRESS , (uint16_t*)RAM_END_ADDRESS)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(MARCH_TEST_FAILURE_BLINK);
#endif
		}

		// Test Bank A of Flash Memory
		if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)0x4400,0x8000,(uint16_t*)CRC_CHECKSUM_LOCATION)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
		}

		// Test Bank B of Flash Memory
		if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)0xC400,0x8000,(uint16_t*)CRC_CHECKSUM_LOCATION+1)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
		}

		// Test Bank C of Flash Memory
		if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)0x14400,0x8000,(uint16_t*)CRC_CHECKSUM_LOCATION+2)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
		}

		// Test Bank D of Flash Memory
		if(IEC60730_CRC_TEST_testNvMemory((uint16_t*)0x1C400,0x8000,(uint16_t*)CRC_CHECKSUM_LOCATION+3)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(CRC_TEST_FAILURE_BLINK);
#endif
		}

	    //Configure Memory Buffer

	    /* Base Addres of ADC12_A Module
	     * Configure memory buffer 0
	     * Map temp sensor to memory buffer 0
	     * Vref+ = Vref+ (int)
	     * Vref- = AVss
	     * Memory buffer 0 is not the end of a sequence
	     */

		ADC12_A_configureMemoryParam param = {0};
		param.memoryBufferControlIndex = ADC12_A_MEMORY_0;
		param.inputSourceSelect = ADC12_A_INPUT_A8;
		param.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
		param.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
		param.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
		ADC12_A_configureMemory(ADC12_A_BASE ,&param);

	    adcTestHandle.muxChannel=8;
	    adcTestHandle.pinCount=0xFFF;
	    adcTestHandle.useInternalInput=INT_REF_1_5_V;

	    if(IEC60730_ADC_TEST_testAdcInput(&adcTestHandle)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
#endif
		}

	    //Configure Memory Buffer

	    /* Base Addres of ADC12_A Module
	     * Configure memory buffer 0
	     * Map ADC_A_INPUT_A12 to memory buffer 0
	     * Vref+ = Vref+ (int)
	     * Vref- = AVss
	     * Memory buffer 0 is not the end of a sequence
	     */

	    ADC12_A_configureMemoryParam param1 = {0};
		param1.memoryBufferControlIndex = ADC12_A_MEMORY_0;
		param1.inputSourceSelect = ADC12_A_INPUT_A12;
		param1.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
		param1.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
		param1.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
		ADC12_A_configureMemory(ADC12_A_BASE ,&param1);

	    adcTestHandle.muxChannel=12;
	    adcTestHandle.pinCount=0xFFF;
	    adcTestHandle.useInternalInput=INT_REF_1_5_V;

	    if(IEC60730_ADC_TEST_testAdcInput(&adcTestHandle)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(ADC_TEST_FAILURE_BLINK);
#endif
		}

	    if(IEC60730_GPIO_TEST_testGpioOutput(PORT_7, PIN1|PIN2|PIN3|PIN7)==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(GPIO_OUTPUT_TEST_FAILURE_BLINK);
#endif
		}

	    if(IEC60730_GPIO_TEST_testGpioInput(PORT_3,
	    									PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6 | PIN7,
	    									PIN0_LOW | PIN1_LOW | PIN2_LOW | PIN3_LOW | PIN4_LOW | PIN5_LOW | PIN6_LOW | PIN7_HIGH )==TEST_FAILURE){
#if JUMP_TO_FAILSAFE
		IEC60730_FAIL_SAFE_failSafe(GPIO_INPUT_TEST_FAILURE_BLINK);
#endif
		}

}


void IEC60730_FAIL_SAFE_failSafe(uint8_t blinks){
	uint8_t i=0;
	GPIO_setOutputLowOnPin(GPIO_PORT_P8,
							GPIO_PIN2);
	for(i=blinks;i>0;i--){
		GPIO_setOutputHighOnPin(GPIO_PORT_P1,
								GPIO_PIN0);
		__delay_cycles(60000);
		GPIO_setOutputLowOnPin(GPIO_PORT_P1,
						       GPIO_PIN0);
		__delay_cycles(5000000);
		WDT_A_resetTimer(WDT_A_BASE);
	}

	while(1);
}

uint16_t IEC60730_INTERRUPT_TEST_testInterrupt(uint32_t interruptTestRegister){

	__bis_SR_register(GIE);

	if(interruptTestRegister&TEST_ADC12_INTERRUPT){
		ADC12IE |= ADC12IE0;
		ADC12IFG |= ADC12IFG0;
	}

	if(interruptTestRegister&TEST_COMP_B_INTERRUPT){
		CBINT |= (CBIE + CBIFG);
	}

	if(interruptTestRegister&TEST_DMA_INTERRUPT){
		DMA0CTL |= (DMAIE + DMAIFG);
	}

	if(interruptTestRegister&TEST_PORT1_INTERRUPT){
		P1IE |=   BIT7;
		P1IFG |=  BIT7;
	}
	if(interruptTestRegister&TEST_PORT2_INTERRUPT){
		P2IE |=   BIT7;
		P2IFG |=  BIT7;
	}
	if(interruptTestRegister&TEST_RTC_INTERRUPT){
		RTCCTL01 |= (RTCTEVIE + RTCTEVIFG);

	}
	if(interruptTestRegister&TEST_TIMER0_A0_INTERRUPT){
		TA0CCTL0 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER0_A1_INTERRUPT){
		TA0CCTL1 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER0_B0_INTERRUPT){
		TB0CCTL0 |= CCIE + CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER0_B1_INTERRUPT){
		TB0CCTL1 |= CCIE + CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER1_A0_INTERRUPT){
		TA1CCTL0 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER1_A1_INTERRUPT){
		TA1CCTL1 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER2_A0_INTERRUPT){
		TA2CCTL0 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_TIMER2_A1_INTERRUPT){
		TA2CCTL1 |= CCIE +CCIFG;
	}
	if(interruptTestRegister&TEST_USB_UBM_INTERRUPT){
		// Unlock USB configuration registers
		USBKEYPID = USBKEY;
		USBPWRCTL |=  VBOFFIE + VBOFFIFG;
	}
	if(interruptTestRegister&TEST_USCI_A0_INTERRUPT){
		// Initialize USCI state machine
		UCA0CTL1 &= ~UCSWRST;
		// Enable USCI_A0 TX interrupt
		UCA0IE |= UCTXIE;
	}
	if(interruptTestRegister&TEST_USCI_A1_INTERRUPT){
		// Initialize USCI state machine
		UCA1CTL1 &= ~UCSWRST;
		// Enable USCI_A0 TX interrupt
		UCA1IE |= UCTXIE;
	}
	if(interruptTestRegister&TEST_USCI_B0_INTERRUPT){
		// Initialize USCI state machine
		UCB0CTL1 &= ~UCSWRST;
		// Enable USCI_A0 TX interrupt
		UCB0IE |= UCTXIE;
	}
	if(interruptTestRegister&TEST_USCI_B1_INTERRUPT){
		// Initialize USCI state machine
		UCB1CTL1 &= ~UCSWRST;
		UCB1IE |= UCTXIE;
	}
	if(interruptTestRegister&TEST_WDT_INTERRUPT){
		// Set-up watch dog in interval to be able to jump to WDT ISR.
		WDTCTL = WDTPW + WDTTMSEL;
		// Enable WDY interrupt.
		SFRIE1 |= WDTIE;
		// Generate Software WDT interrupt.
		SFRIFG1 |= WDTIFG;
	}

	// No operation required to allow WDT interrupt to be triggered
	__no_operation();

	// Disable Global interrupts Interrupt test completion
	__bic_SR_register(GIE);

	if(gInterruptCounter!= interruptTestRegister){
		return TEST_FAILURE;
	}

	return SIG_INTERRUPT_TEST;

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




void initCLK(void){

	// Configure ACLK to be source by XT1, with ACLK clock divider=1
	UCS_initClockSignal(UCS_ACLK,
						UCS_XT1CLK_SELECT,
						UCS_CLOCK_DIVIDER_1);


	UCS_turnOnLFXT1( UCS_XT1_DRIVE_0,
    			   UCS_XCAP_3);


#if defined(MAIN_CLOCK_EXTERNAL_XT2)
	// Configure MCLK to be source by XT2, with MCLK clock divider=1
	UCS_clockSignalInit(UCS_BASE,
						UCS_MCLK,
						UCS_XT2CLK_SELECT,
						UCS_CLOCK_DIVIDER_1);


	UCS_XT2Start( UCS_BASE,
				  UCS_XT2DRIVE_4MHZ_8MHZ);

#else

	// Configure FLLREFCLK to be source by XT1, with FLLREF clock divider=1
	UCS_initClockSignal(UCS_FLLREF,
						UCS_XT1CLK_SELECT,
						UCS_CLOCK_DIVIDER_1);

#if defined(MAIN_CLOCK_FREQUENCY_12MHz)
	PMM_setVCore(PMM_CORE_LEVEL_0);
#endif

    UCS_initFLLSettle(	UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,
    					UCS_MCLK_FLLREF_RATIO);

#endif

}


void initGPIO(void){

	// Enable RED LED in MSP430F5529 USB experimenters board
	GPIO_setAsOutputPin(GPIO_PORT_P1,
						GPIO_PIN0);

	GPIO_setOutputLowOnPin(GPIO_PORT_P1,
						   GPIO_PIN0);

	// Enable GREEN LED in MSP430F5529 USB experimenters board
	GPIO_setAsOutputPin(GPIO_PORT_P8,
						GPIO_PIN2);


	GPIO_setOutputHighOnPin(GPIO_PORT_P8,
							GPIO_PIN2);

	GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3,
										   GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);


	// Configure pins to use 32KHz external crystal in P5.4 and 5.5 in XIN and XOUT mode, respectively
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,GPIO_PIN4 + GPIO_PIN5);

	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P7, GPIO_PIN0);


#if defined(MAIN_CLOCK_EXTERNAL_XT2)
	// Configure pins to use XT2 crystal in P5.2 and 5.3 in XT2IN and XT2OUT mode, respectively
		GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,
													GPIO_PIN2 + GPIO_PIN3);
#endif

}


void initADC(void){

    //Initialize the ADC12_A Module
    /*
     * Base Address of ADC12_A Module
     * Use internal ADC12_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC12_A_init(ADC12_A_BASE,
        ADC12_A_SAMPLEHOLDSOURCE_SC,
        ADC12_A_CLOCKSOURCE_ADC12OSC,
        ADC12_A_CLOCKDIVIDER_1);

    ADC12_A_enable(ADC12_A_BASE);

    /*
     * Base Address of ADC12_A Module
     * For memory buffers 0-7 sample/hold for 256 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC12_A_setupSamplingTimer(ADC12_A_BASE,
        ADC12_A_CYCLEHOLD_256_CYCLES,
        ADC12_A_CYCLEHOLD_4_CYCLES,
        ADC12_A_MULTIPLESAMPLESDISABLE);
}


void initREF(void){
    //Configure internal reference
    //If ref generator busy, WAIT
    while ( REF_ACTIVE == Ref_isRefGenBusy(REF_BASE) ) ;
    //Select internal ref = 1.5V
    Ref_setReferenceVoltage(REF_BASE,
        REF_VREF1_5V);
    //Internal Reference ON
    Ref_enableReferenceVoltage(REF_BASE);

    //Delay (~75us) for Ref to settle
    __delay_cycles(75);
}


// ISR sections
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
	gInterruptCounter |= TEST_ADC12_INTERRUPT;
	ADC12IE &= ~ADC12IE0;
	ADC12IFG &= ~ADC12IFG0;
}


#pragma vector=COMP_B_VECTOR
__interrupt void COMPBISR (void)
{
	gInterruptCounter |= TEST_COMP_B_INTERRUPT;
	CBINT &= ~ (CBIE + CBIFG);

}



#pragma vector=DMA_VECTOR
__interrupt void DMAISR (void)
{
	gInterruptCounter |= TEST_DMA_INTERRUPT;
	DMA0CTL &= ~(DMAIE + DMAIFG);
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

#pragma vector=RTC_VECTOR
__interrupt void RTCISR (void)
{
	gInterruptCounter |= TEST_RTC_INTERRUPT;
	RTCCTL01 &= ~(RTCTEVIE + RTCTEVIFG);
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
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0ISR (void)
{
	gInterruptCounter |= TEST_TIMER0_B0_INTERRUPT;
	TB0CCTL0 &= ~(CCIE +CCIFG);
}
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1ISR (void)
{
	gInterruptCounter |= TEST_TIMER0_B1_INTERRUPT;
	TB0CCTL1 &= ~(CCIE +CCIFG);
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
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0ISR (void)
{
	gInterruptCounter |= TEST_TIMER2_A0_INTERRUPT;
	TA2CCTL0 &= ~(CCIE +CCIFG);
}
#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1ISR (void)
{
	gInterruptCounter |= TEST_TIMER2_A1_INTERRUPT;
	TA2CCTL1 &= ~(CCIE +CCIFG);
}

#pragma vector=USB_UBM_VECTOR
__interrupt void ISR (void)
{
	gInterruptCounter |= TEST_USB_UBM_INTERRUPT;
	USBPWRCTL &=  ~(VBOFFIE + VBOFFIFG);
	// Lock USB registers
	USBKEYID = 0x00;
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0ISR (void)
{
	gInterruptCounter |= TEST_USCI_A0_INTERRUPT;
	UCA0IE &= ~ UCTXIE;

}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1ISR (void)
{
	gInterruptCounter |= TEST_USCI_A1_INTERRUPT;
	UCA1IE &= ~ UCTXIE;
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0ISR (void)
{
	gInterruptCounter |= TEST_USCI_B0_INTERRUPT;
	UCB0IE &= ~ UCTXIE;
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1ISR (void)
{
	gInterruptCounter |= TEST_USCI_B1_INTERRUPT;
	UCB1IE &= ~ UCTXIE;
}

#pragma vector=WDT_VECTOR
__interrupt void WDTISR (void)
{
	gInterruptCounter |= TEST_WDT_INTERRUPT;
	SFRIE1 &= ~WDTIE;
	SFRIFG1 &= ~WDTIFG;

#if ENABLED_WDT
		WDTCTL = WDTPW + WDTCNTCL + WDTIS1;
#else
		WDTCTL = WDTPW + WDTHOLD;
#endif
}

#pragma vector=UNMI_VECTOR
__interrupt void UNMIISR (void)
{
	return;
}

#pragma vector=SYSNMI_VECTOR
__interrupt void SYSNMIISR (void)
{
	return;
}


