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

#include <msp430.h>
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


#if JUMP_TO_FAILSAFE
void IEC60730_FAIL_SAFE_failSafe(void);
#endif

void runPOST(void);
void runPST(void);


void main(void) {
	
	// NOTE1:
	// Before running IEC60730 IEC60730_PC_TEST_testPcRegister()
	// verify you have created PC_TEST_SECTION_1 and PC_TEST_SECTION_2
	// in the linker command file. To a complete explanation on how to create
	// this section consult the IEC60730 API User's Guide.


	// NOTE2:
	// If you are not developing for MSP430G2553 or MSP430F5529 please make sure
	// you provide the correct values for RAM_START_ADDRESS, RAM_SIZE, STACK_SIZE


	// NOTE3:
	// For examples on how to test interrupts please refer to the example code for
	// MSP430G2553 and MSP430F5529 devices.

	// For complete instruction on how to setup an IEC60730 project please
	// consult the IEC60730 API User's Guide.

	
	runPOST();
	
	
	for(;;)
		runPST();
}

void IEC60730_FAIL_SAFE_failSafe(void){

}

void runPOST(void){

}
void runPST(void){

}
