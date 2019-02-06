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
//! \addtogroup clock_test_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//IEC60730_clock_fail_test.c - Clock Tests for IEC60730 Class B.
//
//*****************************************************************************
#include <msp430.h>
#include "IEC60730_clock_fail_test.h"
#include "IEC60730_user_config.h"
#include "IEC60730_system_config.h"
#include "stdint.h"


//*****************************************************************************
//
//! Tests MCLK for proper operation at user defined frequency.
//!
//!
//!
//! The following function verifies that MCLK is oscillating at the frequency
//! specified by the MAIN_CLOCK_FREQUENCY macro. The user must define the
//! allowed +/- percentage frequency drift using the macro
//! PERCENT_FREQUENCY_DRIFT in "IEC60730_user_config.h". The test is passed
//! if freqCounter is between FREQUENCY_COUNT_MAX and FREQUENCY_COUNT_MIN.
//! TAx must be source by ACLK with a high precision clock source.
//! To increase accuracy of oscillator measurement, it is suggested to
//! source LF or XT1 with a 32768 Hz crystal. If the application
//! uses a different frequency for LF or XT1, the LFXT1_FREQUENCY macro in
//! "IEC60730_user_config.h" file must be updated with correct
//! frequency.
//!
//! NOTE: The test requires TAx to be source by ACLK, and configured in
//! Up mode. Also, TAIE will be disable. Therefore, if the application
//! requires TAIE to be enable, the user must set TAIE upon test completion.
//!
//! Modified registers are \b TAxCCRO, \b TAxCCTL0, and \b TAxCTL
//!
//! \return SIG_CLOCK_TEST .- If test is passed.
//!         TEST_FAILED. - if ADC test fail and JUMP_TO_FAILSAFE is disabled
//!						   in "IEC60730_user_config.h".
//
//*****************************************************************************


uint8_t IEC60730_OSCILLATOR_TEST_testOsc(){

	uint16_t freqCounter=0;
	const uint16_t freqMin = FREQUENCY_COUNT_MIN;
	const uint16_t freqMax = FREQUENCY_COUNT_MAX;


	// Feed WDT in case user has WDT enabled before running CPU test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT

	// Assign CCRn the value of CCRn_VALUE_FOR_10_mSEC to count up in
	// 10 msec intervals. CCRn_VALUE_FOR_10_mSEC macro is defined in
	// "IEC60730_system_config.h" file. However, the macro is calculated based
	// on MAIN_CLOCK_FREQUENCY, LFXT1_FREQUENCY, MAIN_CLOCK_DIVIDER and
	// LFXT1_FREQUENCY_DIVIDER and WDT_ENABLED.

#if defined(USE_TA0)

#if defined(USE_CCR0)
	TA0CCR0 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA0CCR1 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR1
#if defined(USE_CCR2)
	TA0CCR2 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR2
#if defined(USE_CCR3)
	TA0CCR3 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR3
#if defined(USE_CCR4)
	TA0CCR4 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR4

	TA0CTL = TASSEL_1 + MC_2 +TACLR;

	// Increase freqCounter until TAxR  reaches CCRn_VALUE_FOR_10_mSEC

#if defined(USE_CCR0)
	TA0CCTL0 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA0CCTL0&CCIFG));
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA0CCTL1 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA0CCTL1&CCIFG));
#endif //USE_CCR1
#if defined(USE_CCR2)
	TA0CCTL2 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA0CCTL2&CCIFG));
#endif //USE_CCR2
#if defined(USE_CCR3)
	TA0CCTL3 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA0CCTL3&CCIFG));
#endif //USE_CCR3
#if defined(USE_CCR4)
	TA0CCTL4 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA0CCTL4&CCIFG));
#endif //USE_CCR4

	// Stop TimerA0
	TA0CTL = MC_0;
#endif //USE_TA0

#if defined(USE_TA1)

#if defined(USE_CCR0)
	TA1CCR0 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA1CCR1 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR2)
	TA1CCR2 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR3)
	TA1CCR3 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR4)
	TA1CCR4 = CCRn_VALUE_FOR_10_mSEC;
#endif

	TA1CTL = TASSEL_1 + MC_2 +TACLR;

	// Increase freqCounter until TAxR  reaches CCRn_VALUE_FOR_10_mSEC
#if defined(USE_CCR0)
	TA1CCTL0 &= ~CCIFG;
	do{
		freqCounter++;
		}while(!(TA1CCTL0&CCIFG));
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA1CCTL1 &= ~CCIFG;
	do{
		freqCounter++;
		}while(!(TA1CCTL1&CCIFG));
#endif //USE_CCR1
#if defined(USE_CCR2)
	TA1CCTL2 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA1CCTL2&CCIFG));
#endif //USE_CCR2
#if defined(USE_CCR3)
	TA1CCTL3 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA1CCTL3&CCIFG));
#endif //USE_CCR3
#if defined(USE_CCR4)
	TA1CCTL4 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA1CCTL4&CCIFG));
#endif //USE_CCR4

	// Stop TimerA1
	TA1CTL = MC_0;

#endif // USE_TA1



#if defined(USE_TA2)

#if defined(USE_CCR0)
	TA2CCR0 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA2CCR1 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR2)
	TA2CCR2 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR3)
	TA2CCR3 = CCRn_VALUE_FOR_10_mSEC;
#endif //USE_CCR0
#if defined(USE_CCR4)
	TA2CCR4 = CCRn_VALUE_FOR_10_mSEC;
#endif

	TA2CTL = TASSEL_1 + MC_2 +TACLR;

	// Increase freqCounter until TAxR  reaches CCRn_VALUE_FOR_10_mSEC
#if defined(USE_CCR0)
	TA2CCTL0 &= ~CCIFG;
	do{
		freqCounter++;
		}while(!(TA2CCTL0&CCIFG));
#endif //USE_CCR0
#if defined(USE_CCR1)
	TA2CCTL1 &= ~CCIFG;
	do{
		freqCounter++;
		}while(!(TA2CCTL1&CCIFG));
#endif //USE_CCR1
#if defined(USE_CCR2)
	TA2CCTL2 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA2CCTL2&CCIFG));
#endif //USE_CCR2
#if defined(USE_CCR3)
	TA2CCTL3 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA2CCTL3&CCIFG));
#endif //USE_CCR3
#if defined(USE_CCR4)
	TA2CCTL4 &= ~CCIFG;
	do{
		freqCounter++;
	}while(!(TA2CCTL4&CCIFG));
#endif //USE_CCR4

	// Stop TimerA2
	TA2CTL = MC_0;

#endif // USE_TA2




	// Check if freqCounter is with the permissible frequency drift defined
	// by the PERCENT_FREQUENCY_DRIFT macro in "IEC60730_user_config.h".
	// It also checks that there are no external oscillator faults.
	// For MSP430FR57xx devices
#if defined(__MSP430_HAS_CS__)
	if((freqCounter>=freqMin)&&(freqCounter<=freqMax) && !(CSCTL5&(XT1OFFG))){
		return SIG_CLOCK_TEST;
	}
	// Check if freqCounter is with the permissible frequency drift defined
	// by the PERCENT_FREQUENCY_DRIFT macro in "IEC60730_user_config.h".
	// It also checks that there are no external oscillator faults.
	// For MSP430F5xx_MSP430F6xx devices
#elif defined(__MSP430_HAS_UCS__ )
	if((freqCounter>=freqMin)&&(freqCounter<=freqMax) && !(UCSCTL7&XT1LFOFFG)){
		return SIG_CLOCK_TEST;
	}
	// Check if freqCounter is with the permissible frequency drift defined
	// by the PERCENT_FREQUENCY_DRIFT macro in "IEC60730_user_config.h".
	// It also checks that there are no external oscillator faults.
	// For MSP430G23xx,MSP430G24xx,MSP430G25xx
#elif defined(__MSP430_HAS_BC2__)
	if((freqCounter>=freqMin)&&(freqCounter<=freqMax) && !(BCSCTL3&LFXT1OF)){
		return SIG_CLOCK_TEST;
	}
#endif

	// If freqCounter is outside the permissible frequency range, the test
	// fail
		return TEST_FAILURE;

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************