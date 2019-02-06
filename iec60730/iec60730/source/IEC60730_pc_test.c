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

//*****************************************************************************
//
//! \addtogroup pc_test_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//IEC60730_pc_test.c - PC Tests for IEC60730 Class B.
//
//*****************************************************************************
#include "IEC60730_pc_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include <msp430.h>


//*****************************************************************************
// Function Definition
//*****************************************************************************
static void* IEC60730_PC_TEST_pcTestFunction1(void);
static void* IEC60730_PC_TEST_pcTestFunction2(void);


//*****************************************************************************
//
//! Tests Program Counter register for stuck at bits
//!
//! This function tests the Program Counter register for stuck at bits.
//! The routine call two test functions that return their addresses. Their
//! return values are compared to the PC test function address. If the value
//! matches, the function passes, if not it fails.
//! The PC test functions need to reside in separate memory locations such
//! that ,by the time all of them are called, all the Program Counter register
//! bits are set or cleared. Thus indirectly testing the PC register for
//! stuck at bits.
//! The user must define two sections named "pc_test_section_1",
//! "pc_test_section_2"
//!
//!
//! Modified registers are \b REGISTER_1, \b REGSITER_2, and \b REGISTER_3
//!
//! \return None
//
//*****************************************************************************
uint8_t IEC60730_PC_TEST_testPcRegister(void)
{
	uint8_t countPcTest;
	void* pReturnFunctionAddress;

// Feed WDT in case user has WDT enabled before running CPU test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT


	// Store function addresses in an array
	void* (*pPcTestFunctions[2])(void) =
	{
		IEC60730_PC_TEST_pcTestFunction1,
		IEC60730_PC_TEST_pcTestFunction2,
	};

	for(countPcTest = 0; countPcTest < 2; countPcTest++)
	{
		pReturnFunctionAddress = (*pPcTestFunctions[countPcTest])();
		// if the address returned doesn't match the address of
		// the function return an error
		if((void *)pPcTestFunctions[countPcTest] !=  pReturnFunctionAddress){
			return(TEST_FAILURE);
		}
	}

	// Return success if all the functions return their own
	// address
	return(SIG_PC_REG_TEST);

}



//*****************************************************************************
//
//! Returns address of IEC60730_PC_TEST_pcTestFunction1
//!
//!
//! \return address of IEC60730_PC_TEST_pcTestFunction1
//
//*****************************************************************************
#ifdef __ICC430__
#pragma location="PC_TEST_SECTION_1"
void* IEC60730_PC_TEST_pcTestFunction1(void){
	return((void *)IEC60730_PC_TEST_pcTestFunction1);
}
#else
#pragma CODE_SECTION(IEC60730_PC_TEST_pcTestFunction1,".pc_test_section_1")

void* IEC60730_PC_TEST_pcTestFunction1(void){
	return(IEC60730_PC_TEST_pcTestFunction1);
}
#endif

//*****************************************************************************
//
//! Returns address of IEC60730_PC_TEST_pcTestFunction2
//!
//!
//! \return address of IEC60730_PC_TEST_pcTestFunction2
//
//*****************************************************************************
#ifdef __ICC430__
#pragma location="PC_TEST_SECTION_2"
void* IEC60730_PC_TEST_pcTestFunction2(void){
	return((void *)IEC60730_PC_TEST_pcTestFunction2);
}
#else
#pragma CODE_SECTION(IEC60730_PC_TEST_pcTestFunction2,".pc_test_section_2")
void* IEC60730_PC_TEST_pcTestFunction2(void){
	return(IEC60730_PC_TEST_pcTestFunction2);
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
