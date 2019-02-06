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
//! \addtogroup crc_test_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//IEC60730_crc_test.c - Non Volatile Memory Tests for IEC60730 Class B.
//
//*****************************************************************************
#include "IEC60730_crc_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include <msp430.h>


static uint16_t privateCalculateCRC(uint16_t *pStartAddress, uint16_t memorySize);


//*****************************************************************************
//
//! Tests invariable (non volatile) memory (FLASH)
//!
//! \param *pStartAddress is a pointer the start address of memory to be tested
//! \param memorySize size of memory to be tested
//! \param *pExpectedCrc is a pointer to the expected CRC value
//!
//! The following function check for memory corruption in non volatile memory.
//! The user must first calculate the CRC value of the memory to be checked.
//! This can be achieved by using the CRC_tool which is included in the utils
//! folders of the library. To learn how to use the CRC_tool please consult
//! the IEC60730 Class B API User's Guide. When the CRC value is obtain,
//! the user must store the CRC value in FLASH before calling the function.
//! The memorySize paramater is specified in 16 bit words and should not
//! exceed 65535 16 bit words.
//! The expectedCrc value is compared to the newly calculated CRC value.
//! The test passes if the two CRC values are identical.
//!
//! NOTE: memorySize should be even an value, otherwise the the test fails.
//!
//! \return SIG_NV_MEM_CRC_TEST.- if expected CRC and calculated CRC are
//!								  identical.
//!         TEST_FAILED. - if non volatile test fail and JUMP_TO_FAILSAFE is
//!						   disabled in "IEC60730_user_config.h".
//
//*****************************************************************************

uint8_t IEC60730_CRC_TEST_testNvMemory(uint16_t *pStartAddress,
		uint16_t memorySize, uint16_t *pExpectedCrc){

	uint16_t crc;
	uint16_t expectedCrc = *pExpectedCrc;

	// Verifies that memorySize is a even value
	if((memorySize & 0x0001)){
		return TEST_FAILURE;
	}

	crc = privateCalculateCRC(pStartAddress,memorySize);

	if(expectedCrc != crc){
		return TEST_FAILURE;
	}

	return(SIG_NV_MEM_CRC_TEST);
}


//*****************************************************************************
//
//! Calculates CRC16-CCITT for non volatile memory.
//!
//! \param *pStartAddress is a pointer the start address of memory to be tested
//! \param memorySize size of memory to be tested
//!
//! This function will calculate the CRC result of the non volatile memory
//! specified by the user. The function determines if the library is being
//! built for a MSP430 with CRC module and uses the module to make the CRC
//! calculation. Otherwise the CRC is calculated in software.
//! The polynomial implemented in software is x^16+x^12+x^5+1.
//!
//! \return uint16_t crc .- calculated crc
//
//*****************************************************************************

static uint16_t privateCalculateCRC(uint16_t *pStartAddress, uint16_t memorySize){

	uint32_t i;

// Feed WDT in case user has WDT enabled before running CRC test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT

// If MSP430 has a CRC module, checksum is calculated using CRC module
#if defined(__MSP430_HAS_CRC__)
	// Set initial value for CRC16-CCITT calculation
	CRCINIRES= CRC16_CCITT_SEED;

	for (i = 0; i < memorySize/2; i++)
	{
		//Add all of the values into the CRC signature
		CRCDIRB= *pStartAddress++;
	}

	return (CRCINIRES);

#else

	uint8_t j;
	uint16_t memoryContent;
	uint16_t crc = CRC16_CCITT_SEED;

	for(i = 0 ; i < memorySize/2 ; i ++)
	{
		// Byte reverse
		memoryContent = ((*pStartAddress & 0x00FF) << 8) +
						((*pStartAddress & 0xFF00) >> 8);

		for(j = 0 ; j < 16 ; j++)
		{
			if((memoryContent ^ crc) >> 15){
				crc = (crc << 1) ^ CRC16_CCITT_POLY;
			}else{
				crc <<= 1;
			}
			memoryContent <<= 1;
		}
		pStartAddress++;

	}

	return crc;

#endif //__MSP430_HAS_CRC__

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
