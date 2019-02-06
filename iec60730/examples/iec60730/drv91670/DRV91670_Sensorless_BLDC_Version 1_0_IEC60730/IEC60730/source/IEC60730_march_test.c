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
//! \addtogroup march_test_api
//! @{
//
//*****************************************************************************
 
//*****************************************************************************
//
//IEC60730_march_test.c - RAM Tests for IEC60730 Class B.
//
//*****************************************************************************
#include "../../TI_DRV_IEC60730.h"

//Global variables and functions private to the file

// Recovery data buffer for RAM testing.
#if defined(NON_DESTRUCTIVE)
#ifdef __ICC430__
#pragma location=RAM_START_ADDRESS + RAM_SIZE
__no_init static uint16_t  ram_iec_safe[RAM_TEST_BUFSIZE];
#else
#pragma SET_DATA_SECTION(".safe_ram")
static uint16_t  ram_iec_safe[RAM_TEST_BUFSIZE];
// Return to normal placement of bss ("static variables w/o initial values").
#endif
#endif // NON_DESTRUCTIVE


// Private Function Prototypes
#if defined(MARCH_C_TEST)
static uint8_t privateMarchCTestStart(uint16_t* pui16_StartAddr, uint16_t* pui16_EndAddr);
#elif defined(MARCH_X_TEST)
static uint8_t privateMarchXTestStart(uint16_t* pui16_StartAddr, uint16_t* pui16_EndAddr);
#endif // MARCH_X_TEST


//*****************************************************************************
//
//! Tests Variable memory (RAM memory)
//!
//! \param *pui16_StartAddr is the start address of RAM to be tested
//! \param *pui16_EndAddr is the end address of RAM to be tested
//!
//!
//! This function checks the RAM memory for DC fault using march test. The
//! following march tests are implemented. Both test can be run in destructive
//! or non-destructive mode based on the macro definition of MARCH_X_TEST or
//! MARCH_C_TEST in "IEC60730_user_config.h" file
//! - March X
//! - March C
//!
//!	The test will perform the desired march test over the range of RAM memory
//! specified by pui16_StartAddr and pui16_EndAddr.
//!
//! \return SIG_RAM_TEST.- if RAM test is passed
//!         TEST_FAILED. - if RAM test fail and JUMP_TO_FAILSAFE is disabled
//!						   in "IEC60730_user_config.h".
//
//*****************************************************************************
uint8_t IEC60730_MARCH_TEST_testRam(uint16_t* pui16_StartAddr, uint16_t* pui16_EndAddr){

	uint16_t* puint16CurrentPosition = (uint16_t*) (RAM_START_ADDRESS + RAM_SIZE);

#if defined(NON_DESTRUCTIVE)

	uint8_t i=0;
	uint16_t* puint16OffsetPosition = pui16_StartAddr;
#endif // NON_DESTRUCTIVE

// Feed WDT in case user has WDT enabled before running CRC test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT

	// Verify user has entered a valid address range
	if(pui16_EndAddr <= pui16_StartAddr){
		return TEST_FAILURE;
	}

	// Test SAFE RAM BUFFER using preferred MARCH algorithm
#if defined(NON_DESTRUCTIVE)
#if defined(MARCH_C_TEST)
	if(privateMarchCTestStart(puint16CurrentPosition,puint16CurrentPosition + RAM_TEST_BUFSIZE)==TEST_FAILURE){
		return TEST_FAILURE;
	}
#endif// MARCH_C_TEST

#if defined(MARCH_X_TEST)
	if(privateMarchXTestStart(puint16CurrentPosition,puint16CurrentPosition + RAM_TEST_BUFSIZE)==TEST_FAILURE){
			return TEST_FAILURE;
	}
#endif // MARCH_X_TEST
#endif //NON-DesTRUCTIVE

	puint16CurrentPosition = pui16_StartAddr;


#if defined(NON_DESTRUCTIVE)
	while(puint16CurrentPosition < pui16_EndAddr){
		if(puint16OffsetPosition + RAM_TEST_BUFSIZE < pui16_EndAddr){

			for(i=0;i<RAM_TEST_BUFSIZE;i++){
				ram_iec_safe[i]=*puint16OffsetPosition++;
			}

		}else{

			i=0;
			while(puint16OffsetPosition<pui16_EndAddr){
				ram_iec_safe[i++]=*puint16OffsetPosition++;

			}
		}

#if defined(MARCH_C_TEST)
		if(privateMarchCTestStart(puint16CurrentPosition,puint16OffsetPosition)==TEST_FAILURE){
			return TEST_FAILURE;
		}
#elif defined(MARCH_X_TEST)
		if(privateMarchXTestStart(puint16CurrentPosition,puint16OffsetPosition)==TEST_FAILURE){
			return TEST_FAILURE;
		}
#endif

		// Restore content of RAM memory
		if(i==8){
			for(i=0;i<RAM_TEST_BUFSIZE;i++){
				*puint16CurrentPosition++ = ram_iec_safe[i];
			}
		}else{
			i=0;
			while(puint16OffsetPosition<pui16_EndAddr){
				ram_iec_safe[i++]=*puint16OffsetPosition++;
			}

		}

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	}
#endif // NON_DESTRUCTIVE

#ifndef NON_DESTRUCTIVE
#if defined(MARCH_C_TEST)
	if(privateMarchCTestStart(pui16_StartAddr,pui16_EndAddr)==TEST_FAILURE){
		return TEST_FAILURE;
	}
#endif // MARCH_C_TEST

#if defined(MARCH_X_TEST)
	if(privateMarchXTestStart(pui16_StartAddr,pui16_EndAddr)==TEST_FAILURE){
		return TEST_FAILURE;
	}
#endif // MARCH_X_TEST
#endif // If NON_DESTRUCTIVE is not defined

	return SIG_RAM_TEST;


}



#if defined(MARCH_X_TEST)
//*****************************************************************************
//
//! Perform March X test on RAM memory
//!
//! \param *pui16_StartAddr is the start address of RAM to be tested
//! \param *pui16_EndAddr is the end address of RAM to be tested
//!
//!
//! The following function performs March X test on RAM memory specified by
//! start address and end address.
//!
//! \return SIG_RAM_TEST.- if RAM test is passed
//!         TEST_FAILED. - if RAM test fail and JUMP_TO_FAILSAFE is disabled
//!						   in "IEC60730_user_config.h".
//
//*****************************************************************************
static uint8_t privateMarchXTestStart(uint16_t* pui16_StartAddr, uint16_t* pui16_EndAddr){

	uint16_t* puint16CurrentPosition = pui16_StartAddr;
	uint16_t testMask;
	uint8_t i = 0;
// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	uint16_t wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	// Step #1: write 0 to all cells
	while(puint16CurrentPosition<pui16_EndAddr){
		*puint16CurrentPosition=0;
		puint16CurrentPosition++;
	}

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	wdtConfig=WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	// Step #2: for each cell read 0 and write 1 in ascending order

	// Set pointer to lower side of RAM
	puint16CurrentPosition = pui16_StartAddr;

	while(puint16CurrentPosition<pui16_EndAddr){
		testMask = 0x0001;
		// For each 16 cells
		for(i=0;i<16;i++){

			// Verify that read value is 0
			if((*puint16CurrentPosition & testMask)!=0){
				return TEST_FAILURE;
			}

			// Write 1 to cell
#ifdef INJECTION_OF_ERROR_ENABLED
			if(iecError.ramError == TRUE){
				*puint16CurrentPosition &= ~testMask;
			}else{
				*puint16CurrentPosition |= testMask;
			}
#else
			*puint16CurrentPosition |= testMask;
#endif// INJECTION_OF_ERROR_ENABLED

			// Left shift testMask

			testMask <<= 1;

		}

		puint16CurrentPosition++;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
		wdtConfig =  WDTCTL & 0x00ff;
		WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT
	}



	// Step #3: for each cell read 1 and write 0 in descending order

	// Set pointer to higher side of RAM
	puint16CurrentPosition--;

	while(puint16CurrentPosition>=pui16_StartAddr){
			testMask = 0x8000;
			// For each 16 cells
			for(i=0;i<16;i++){

				// Verify that read value is 1
				if((*puint16CurrentPosition & testMask)!=0){
					// Write 0 to cell
					*puint16CurrentPosition &= ~testMask;
				}else{
					return TEST_FAILURE;
				}

				// Left shift testMask

				testMask >>= 1;


			}

			puint16CurrentPosition--;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
			wdtConfig =  WDTCTL & 0x00ff;
			WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

		}


	// Step #4; read 0 for all cells

	puint16CurrentPosition++;

	while(puint16CurrentPosition<pui16_EndAddr){
		if(*puint16CurrentPosition!=0){
			return TEST_FAILURE;
		}
		puint16CurrentPosition++;
	}

	return SIG_RAM_TEST;

}

#endif //MARCH_X_TEST

#if defined(MARCH_C_TEST)
//*****************************************************************************
//
//! Perform March C test on RAM memory
//!
//! \param *pui16_StartAddr is the start address of RAM to be tested
//! \param *pui16_EndAddr is the end address of RAM to be tested
//!
//!
//! The following function performs March C test on RAM memory specified by
//! start address and end address.
//!
//! \return SIG_RAM_TEST.- if RAM test is passed
//!         TEST_FAILED. - if RAM test fail and JUMP_TO_FAILSAFE is disabled
//!						   in "IEC60730_user_config.h".
//
//*****************************************************************************
static uint8_t privateMarchCTestStart(uint16_t* pui16_StartAddr, uint16_t* pui16_EndAddr)
{
	uint16_t* puint16CurrentPosition = pui16_StartAddr;
	uint16_t testMask;
	uint8_t i = 0;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	uint16_t wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	// MARCH C
	// Write x(w0); a(r0,w1); a(r1,w0); d(r0,w1); d(r1,w0); x(r0);

	// Step #1: write 0 to all cells
	while(puint16CurrentPosition<pui16_EndAddr){
		*puint16CurrentPosition=0;
		puint16CurrentPosition++;
	}

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
	wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	// Step #2: for each cell read 0 and write 1 in ascending order

	// Set pointer to lower side of RAM
	puint16CurrentPosition = pui16_StartAddr;

	while(puint16CurrentPosition<pui16_EndAddr){
		testMask = 0x0001;
		// For each 16 cells
		for(i=0;i<16;i++){

			// Verify that read value is 0
			if((*puint16CurrentPosition & testMask)!=0){
				return TEST_FAILURE;
			}

			// Write 1 to cell
			*puint16CurrentPosition |= testMask;

			// Left shift testMask

			testMask <<= 1;


		}

		puint16CurrentPosition++;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
		wdtConfig =  WDTCTL & 0x00ff;
		WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT
	}

	// Step #3: for each cell read 1 and write 0 in ascending order

	// Set pointer to lower side of RAM
	puint16CurrentPosition = pui16_StartAddr;

	// Step #2: for each cell read 0 and write 1 in ascending order
	while(puint16CurrentPosition<pui16_EndAddr){
		testMask = 0x0001;
		// For each 16 cells
		for(i=0;i<16;i++){

			// Verify that read value is 1
			if((*puint16CurrentPosition & testMask)!=0){
				// Write 0 to cell
#ifdef INJECTION_OF_ERROR_ENABLED
				if(iecError.ramError == TRUE){
					*puint16CurrentPosition |= testMask;;
				}else{
					*puint16CurrentPosition &= ~testMask;
				}
#else
				*puint16CurrentPosition &= ~testMask;
#endif //INJECTION_OF_ERROR_ENABLED

			}else{
				return TEST_FAILURE;
			}

			// Left shift testMask

			testMask <<= 1;


		}

		puint16CurrentPosition++;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
		wdtConfig =  WDTCTL & 0x00ff;
		WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	}

	// Step #4: for each cell read 0 and write 1 in descending order

	puint16CurrentPosition--;

	while(puint16CurrentPosition>=pui16_StartAddr){
		testMask = 0x8000;
		// For each 16 cells
		for(i=0;i<16;i++){

			// Verify that read value is 0
			if((*puint16CurrentPosition & testMask)!=0){
				return TEST_FAILURE;
			}

			// Write 1 to cell
			*puint16CurrentPosition |= testMask;

			// Left shift testMask

			testMask >>= 1;


		}

		puint16CurrentPosition--;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
		wdtConfig =  WDTCTL & 0x00ff;
		WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	}

	// Step #5: for each cell read 1 and write 0 in descending order

	puint16CurrentPosition = pui16_EndAddr-1;

	while(puint16CurrentPosition>=pui16_StartAddr){
		testMask = 0x8000;
		// For each 16 cells
		for(i=0;i<16;i++){

			// Verify that read value is 1
			if((*puint16CurrentPosition & testMask)!=0){
				// Write 0 to cell
				*puint16CurrentPosition &= ~testMask;
			}else{
				return TEST_FAILURE;
			}

			// Left shift testMask

			testMask >>= 1;


		}

		puint16CurrentPosition--;

// Feed WDT
#if ENABLED_WDT
#if defined(MAIN_CLOCK_FREQUENCY_1MHz)
		wdtConfig =  WDTCTL & 0x00ff;
		WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // MAIN_CLOCK_FREQUENCY_1MHz
#endif // ENABLED_WDT

	}

	// Step #6; read 0 for all cells

	puint16CurrentPosition++;

	while(puint16CurrentPosition<pui16_EndAddr){
		if(*puint16CurrentPosition!=0){
			return TEST_FAILURE;
		}
		puint16CurrentPosition++;

	}
	return SIG_RAM_TEST;

}
#endif // MARCH_C_TEST

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
