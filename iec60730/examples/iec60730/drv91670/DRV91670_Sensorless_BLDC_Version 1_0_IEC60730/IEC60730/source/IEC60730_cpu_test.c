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
//! \addtogroup cpu_test_api
//! @{
//
//*****************************************************************************
 
//*****************************************************************************
//
//IEC60730_pc_test.c - PC Tests for IEC60730 Class B.
//
//*****************************************************************************
#include "../../TI_DRV_IEC60730.h"
#include "../../TI_SMS_Motor_Type_Def.h"



static void stabilizeClocks(void);
//*****************************************************************************
//
//! Test CPU registers
//!
//! This C-callable assembly routine tests CPU core registers for stuck at bits.
//! The following registers are tested:
//!	- R4
//!	- SP
//!	- SR
//!	- R5-R15
//! The registers are tested in the order listed above
//!
//! Modified registers are \b R4, \b SP, \b SR, and \b R5-R15
//!
//! \return SIG_CPU_REG_TEST.- if test does not detects stuck at bits.
//!         TEST_FAILED. - if test detects stuck at bits in CPU registers and
//! 						JUMP_TO_FAILSAFE is disabled in
//!						"IEC60730_user_config.h".
//
//*****************************************************************************


uint8_t IEC60730_CPU_TEST_testCpuRegisters(){
// Feed WDT in case user has WDT enabled before running CPU test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT
       
	// First test General-Purpose Register 4 for Stuck at bits.
	// If R4 passes test this register will be used to save content
	// of SP, SR and R5-R15.

	// Test R4
#if __MSP430X__

	__asm(" pushm.a  #1,R4");
	__asm(" mova     #0xAAAAA,R4");
	__asm(" cmp.a    #0xAAAAA,R4");
	__asm(" jne      RestorePC");

	__asm(" mova     #0x55555,R4");
	__asm(" cmp.a    #0x55555,R4");
	__asm(" jne      RestorePC");
	__asm(" popm.a   #1,R4");

	// Test SP
	__asm(" mova     SP,R4");
	__asm(" mova     #0xAAAAA,SP");
	__asm(" cmp.a    #0xAAAAA,SP");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55554,SP");
	__asm(" cmp.a    #0x55554,SP");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,SP");


	// Test SR
	// Note: SR is a 16-bit Register. and Bits 15-12 cannot be
	// manipulated.

	__asm(" mov      SR,R4");
	__asm(" mov      #0xAAAA,SR");
	__asm(" cmp      #0x0AAA,SR");
	__asm(" jne      TestFailed");

	__asm(" mov      #0x5545,SR");
	__asm(" cmp      #0x0545,SR");
	__asm(" jne      TestFailed");
	__asm(" mov      R4,SR");


	// Since system clock generators are modified by
	// testing SR we need to wait until clocks
	// are stable.

	stabilizeClocks();

	// Test R5
	__asm(" mova     R5,R4");
	__asm(" mova     #0xAAAAA,R5");
	__asm(" cmp.a    #0xAAAAA,R5");
	__asm(" jne      TestFailed");


	__asm(" mova     #0x55555,R5");
	__asm(" cmp.a    #0x55555,R5");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R5");


	// Test R6
	__asm(" mova     R6,R4");
	__asm(" mova     #0xAAAAA,R6");
	__asm(" cmp.a    #0xAAAAA,R6");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R6");
	__asm(" cmp.a    #0x55555,R6");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R6");


	// Test R7
	__asm(" mova     R7,R4");
	__asm(" mova     #0xAAAAA,R7");
	__asm(" cmp.a    #0xAAAAA,R7");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R7");
	__asm(" cmp.a    #0x55555,R7");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R7");


	// Test R8
	__asm(" mova     R8,R4");
#ifdef INJECTION_OF_ERROR_ENABLED
	if(iecError.cpuError == TRUE){
		__asm(" mova     #0xAAA5A,R8");
	}else{
		__asm(" mova     #0xAAAAA,R8");
	}
#else
	__asm(" mova     #0xAAAAA,R8");
#endif//INJECTION_OF_ERROR_ENABLED
	__asm(" cmp.a    #0xAAAAA,R8");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R8");
	__asm(" cmp.a    #0x55555,R8");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R8");


	// Test R9
	__asm(" mova     R9,R4");
	__asm(" mova     #0xAAAAA,R9");
	__asm(" cmp.a    #0xAAAAA,R9");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R9");
	__asm(" cmp.a    #0x55555,R9");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R9");


	// Test R10
	__asm(" mova     R10,R4");
	__asm(" mova     #0xAAAAA,R10");
	__asm(" cmp.a    #0xAAAAA,R10");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R10");
	__asm(" cmp.a    #0x55555,R10");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R10");


	// Test R11
	__asm(" mova     R11,R4");
	__asm(" mova     #0xAAAAA,R11");
	__asm(" cmp.a    #0xAAAAA,R11");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R11");
	__asm(" cmp.a    #0x55555,R11");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R11");


	// Test R12
	__asm(" mova     R12,R4");
	__asm(" mova     #0xAAAAA,R12");
	__asm(" cmp.a    #0xAAAAA,R12");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R12");
	__asm(" cmp.a    #0x55555,R12");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R12");


	// Test R13
	__asm(" mova     R13,R4");
	__asm(" mova     #0xAAAAA,R13");
	__asm(" cmp.a    #0xAAAAA,R13");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R13");
	__asm(" cmp.a    #0x55555,R13");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R13");


	// Test R14
	__asm(" mova     R14,R4");
	__asm(" mova     #0xAAAAA,R14");
	__asm(" cmp.a    #0xAAAAA,R14");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R14");
	__asm(" cmp.a    #0x55555,R14");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R14");


	// Test R15
	__asm(" mova     R15,R4");
	__asm(" mova     #0xAAAAA,R15");
	__asm(" cmp.a    #0xAAAAA,R15");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mova     #0x55555,R15");
	__asm(" cmp.a    #0x55555,R15");
	__asm(" jne      TestFailed");
	__asm(" mova     R4,R15");

	// All register passed test!

	__asm(" mova      #0x0001,R12");
	__asm(" reta");

	// Stuck at bit(s) detected test failed!
	__asm("RestorePC:  popm.a R4");
	__asm("TestFailed: mova	#0000,R12");

	return TEST_FAILURE;

#else
	__asm(" push	 R4");
	__asm(" mov.w    #0xAAAA,R4");
	__asm(" cmp.w    #0xAAAA,R4");
	__asm(" jne      RestorePC");

	__asm(" mov.w    #0x5555,R4");
	__asm(" cmp.w    #0x5555,R4");
	__asm(" jne      RestorePC");
	__asm(" pop      R4");

	// Test SP
	__asm(" mov.w    SP,R4");
	__asm(" mov.w    #0xAAAA,SP");
	__asm(" cmp.w    #0xAAAA,SP");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5554,SP");
	__asm(" cmp.w    #0x5554,SP");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,SP");


	// Test SR
	// Note: SR is a 16-bit Register. and Bits 15-12 cannot be
	// manipulated.

	__asm(" mov      SR,R4");
	__asm(" mov      #0xAAAA,SR");
	__asm(" cmp      #0xAAAA,SR");
	__asm(" jne      TestFailed");

	__asm(" mov      #0x5545,SR");
	__asm(" cmp      #0x5545,SR");
	__asm(" jne      TestFailed");
	__asm(" mov      R4,SR");


	// Since system clock generators are modified by
	// testing SR we need to wait until clocks
	// are stabilized.

	stabilizeClocks();

	// Test R5
	__asm(" mov.w    R5,R4");
	__asm(" mov.w    #0xAAAA,R5");
	__asm(" cmp.w   #0xAAAA,R5");
	__asm(" jne      TestFailed");


	__asm(" mov.w    #0x5555,R5");
	__asm(" cmp.w   #0x5555,R5");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R5");


	// Test R6
	__asm(" mov.w    R6,R4");
	__asm(" mov.w    #0xAAAA,R6");
	__asm(" cmp.w   #0xAAAA,R6");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R6");
	__asm(" cmp.w   #0x5555,R6");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R6");


	// Test R7
	__asm(" mov.w    R7,R4");
	__asm(" mov.w    #0xAAAA,R7");
	__asm(" cmp.w   #0xAAAA,R7");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R7");
	__asm(" cmp.w   #0x5555,R7");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R7");


	// Test R8
	__asm(" mov.w    R8,R4");
	__asm(" mov.w    #0xAAAA,R8");
	__asm(" cmp.w   #0xAAAA,R8");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R8");
	__asm(" cmp.w   #0x5555,R8");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R8");


	// Test R9
	__asm(" mov.w    R9,R4");
	__asm(" mov.w    #0xAAAA,R9");
	__asm(" cmp.w   #0xAAAA,R9");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R9");
	__asm(" cmp.w   #0x5555,R9");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R9");


	// Test R10
	__asm(" mov.w    R10,R4");
	__asm(" mov.w    #0xAAAA,R10");
	__asm(" cmp.w   #0xAAAA,R10");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R10");
	__asm(" cmp.w   #0x5555,R10");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R10");


	// Test R11
	__asm(" mov.w    R11,R4");
	__asm(" mov.w    #0xAAAA,R11");
	__asm(" cmp.w   #0xAAAA,R11");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R11");
	__asm(" cmp.w   #0x5555,R11");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R11");


	// Test R12
	__asm(" mov.w    R12,R4");
	__asm(" mov.w    #0xAAAA,R12");
	__asm(" cmp.w   #0xAAAA,R12");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R12");
	__asm(" cmp.w   #0x5555,R12");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R12");


	// Test R13
	__asm(" mov.w    R13,R4");
	__asm(" mov.w    #0xAAAA,R13");
	__asm(" cmp.w   #0xAAAA,R13");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R13");
	__asm(" cmp.w   #0x5555,R13");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R13");


	// Test R14
	__asm(" mov.w    R14,R4");
	__asm(" mov.w    #0xAAAA,R14");
	__asm(" cmp.w   #0xAAAA,R14");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R14");
	__asm(" cmp.w   #0x5555,R14");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R14");


	// Test R15
	__asm(" mov.w    R15,R4");
	__asm(" mov.w    #0xAAAA,R15");
	__asm(" cmp.w   #0xAAAA,R15");
	__asm(" jne      TestFailed");


	// Stack pointer is always aligned to even addresses.
	__asm(" mov.w    #0x5555,R15");
	__asm(" cmp.w    #0x5555,R15");
	__asm(" jne      TestFailed");
	__asm(" mov.w    R4,R15");

	// All register passed test!

	__asm(" mov      #0xCC,R12");
	__asm(" ret");

	// Stuck at bit(s) detected test failed!
	__asm("RestorePC:  pop R4");
	__asm("TestFailed: mov	#00,R12");

	return TEST_FAILURE;

#endif

}


static void stabilizeClocks(void){

#if defined(__MSP430_HAS_CS__)
		do
		{
			// Clear XT1 and XT2 fault flag
			CSCTL5 &= ~(XT1OFFG + XT2OFFG);

			// Clear OScillator fault
			SFRIFG1 &= ~OFIFG;
		}while (SFRIFG1&OFIFG);

#elif defined(__MSP430_HAS_UCS__)
		do
		{
			// Clear XT1 and XT2 fault flag
			UCSCTL7 &= ~(XT1LFOFFG + XT2OFFG);

			// Clear OScillator fault
			SFRIFG1 &= ~OFIFG;
		}while (SFRIFG1&OFIFG);

#elif defined(__MSP430_HAS_BC2__)
		do
		{
			// Clear OScillator fault
			IFG1 &= ~OFIFG;
		}while (IFG1 & OFIFG);

#endif
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
