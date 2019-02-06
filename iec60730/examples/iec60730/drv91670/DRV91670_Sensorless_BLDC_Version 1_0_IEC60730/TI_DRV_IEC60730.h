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

#ifndef IEC60730_H_
#define IEC60730_H_

#include "DRV91670.h"
#include "TI_SMS_Motor_Init.h"
#include "TI_SMS_Motor_SPI_API.h"
#include "stdint.h"
#include "IEC60730_adc_test.h"
#include "IEC60730_clock_fail_test.h"
#include "IEC60730_cpu_test.h"
#include "IEC60730_crc_test.h"
#include "IEC60730_gpio_test.h"
#include "IEC60730_march_test.h"
#include "IEC60730_pc_test.h"
#include "IEC60730_system_config.h"
#include "IEC60730_user_config.h"


// The following definitions allow to enable STATUS_LED on DRV91670 EVM
#define ENABLE_STATUS_LED

// The following definitions allow to enable debug features for IEC60730
// functions
#define INJECTION_OF_ERROR_ENABLED
#define BENCHMARK_ENABLED
//#define OUTPUT_SMCLK

#define NON_VOL_MEM_START_ADDRESS	0x8000
#define NON_VOL_MEM_SIZE			0x8000

// The following macros are used to specify the GPIO used for
// STATUS_LED
#ifdef ENABLE_STATUS_LED
#define STATUS_LED_DIR 		P1DIR
#define STATUS_LED_OUT	 	P1OUT
#define STATUS_LED_PIN		BIT6

// The following defines number of STATUS_LED blinks to provide information of
// which test has failed
#define CPU_TEST_FAILURE_BLINK			2
#define PC_TEST_FAILURE_BLINK			4
#define OSCILLATOR_TEST_FAILURE_BLINK	6
#define MARCH_TEST_FAILURE_BLINK		8
#define CRC_TEST_FAILURE_BLINK			10
#define INTERRUPT_TEST_FAILURE_BLINK	12
#define ADC_TEST_FAILURE_BLINK			14
#define GPIO_INPUT_TEST_FAILURE_BLINK	16
#define GPIO_OUTPUT_TEST_FAILURE_BLINK	18
#define SPI_TEST_FAILURE_BLINK	        20

#endif /*ENABLE_STATUS_LED*/


// The following macros are used for SPI communication test
#define MAJOR_MINOR_VERSION  0xB0

// The following macros allows the definition of a GPIO that can be used for
// benchmarking purposes
#ifdef BENCHMARK_ENABLED
#define TEST_LED_DIR 		P4DIR
#define TEST_LED_OUT	 	P4OUT
#define TEST_LED_PIN		BIT6
#endif /*BENCHMARK_ENABLED*/

// The following macros allow the definition of GPIO used on DRV to output
// SMCLK signal
#ifdef OUTPUT_SMCLK
#define SMCLK_DIR			P4DIR
#define SMCLK_PIN			BIT5
#define SMCLK_SEL			P4SEL
#endif /*OUTPUT_SMCLK*/


// Test channels for ADC test and expected conversion values
#define ADC_1_2_V_BANDGAP_REF_CH		6
#define ADC_EXPECTED_1_2_BG_REF_CONV    0x01FF

#define ADC_DAC_CH						13
#define ADC_EXPECTED_DAC_CONV			0x0000


#define ADC_VCC_DIV_12_CH				7
#define ADC_VCC_DIV_12_CONV				0x01AA

#ifdef INJECTION_OF_ERROR_ENABLED


typedef struct {
	             uint8_t cpuError;
	             uint8_t pcError;
	             uint8_t ramError;
	             uint8_t adcError;
                }IEC_ERROR_INJECTION;

extern const IEC_ERROR_INJECTION iecError;
#endif /*INJECTION_OF_ERROR_ENABLED*/


// Interrupt test configuration

#define INTERRUPT_VECTOR_COUNT				20


#define TEST_WDT_INTERRUPT       WDT_VECTOR
#define TEST_PORT4_INTERRUPT     PORT4_VECTOR
#define TEST_T4A1_INTERRUPT      TIMER4_A1_VECTOR
#define TEST_T4A0_INTERRUPT      TIMER4_A0_VECTOR
#define TEST_T3A1_INTERRUPT      TIMER3_A1_VECTOR
#define TEST_T3A0_INTERRUPT      TIMER3_A0_VECTOR
#define TEST_DMA_INTERRUPT       DMA_VECTOR
#define TEST_T2A1_INTERRUPT      TIMER2_A1_VECTOR
#define TEST_T2A0_INTERRUPT      TIMER2_A0_VECTOR
#define TEST_USCIB0_INTERRUPT    USCI_B0_VECTOR
#define TEST_USCIA0_INTERRUPT    USCI_A0_VECTOR
#define TEST_T0B1_INTERRUPT      TIMER0_B1_VECTOR
#define TEST_T0B0_INTERRUPT      TIMER0_B0_VECTOR
#define TEST_T1A1_INTERRUPT      TIMER1_A1_VECTOR
#define TEST_T1A0_INTERRUPT      TIMER1_A0_VECTOR
#define TEST_T0A1_INTERRUPT      TIMER0_A1_VECTOR
#define TEST_T0A0_INTERRUPT      TIMER0_A0_VECTOR
#define TEST_PORT3_INTERRUPT     PORT3_VECTOR
#define TEST_PORT2_INTERRUPT     PORT2_VECTOR
#define TEST_PORT1_INTERRUPT     PORT1_VECTOR





typedef struct {
	uint8_t testPort1Interrupt;
	uint8_t testPort2Interrupt;
	uint8_t testPort3Interrupt;
	uint8_t testT0A0Interrupt;
	uint8_t testT0A1Interrupt;
	uint8_t testT1A0Interrupt;
	uint8_t testT1A1Interrupt;
	uint8_t testT0B0Interrupt;
	uint8_t testT0B1Interrupt;
	uint8_t testUSCIA0Interrupt;
	uint8_t testUSCIB0Interrupt;
	uint8_t testT2A0Interrupt;
	uint8_t testT2A1Interrupt;
	uint8_t testDMAInterrupt;
	uint8_t testT3A0Interrupt;
	uint8_t testT3A1Interrupt;
	uint8_t testT4A0Interrupt;
	uint8_t testT4A1Interrupt;
	uint8_t testPort4Interrupt;
	uint8_t testWDTInterrupt;
}IEC_ISR_TEST_VECTOR;

extern IEC_ISR_TEST_VECTOR iecTestVector;

// Defines for test success
#define POST_TESTS_PASSED				0xAA
#define PEST_TESTS_PASSED				0xBB

//******************************************************************************
// Function prototypes
//******************************************************************************
extern uint8_t IEC60730_RUN_POST_test(void);
extern uint8_t IEC60730_RUN_PEST_test(void);

#endif /* IEC60730_H_ */
