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
//! \addtogroup gpio_test_api
//! @{
//
//***************************************************************************** 
 
//*****************************************************************************
//
//IEC60730_gpio_test.c - GPIO Tests for IEC60730 Class B.
//
//*****************************************************************************
#include "../../TI_DRV_IEC60730.h"


static uint8_t privateValidatePinRange(uint16_t port, uint16_t gpioPin);

//*****************************************************************************
//
//! Tests output functionality of GPIO module
//!
//! \param port is the port number to be tested. Consult device datasheet to
//!			determine which ports are available in your device.
//!       Valid values are
//!        \b PORT_1
//!        \b PORT_2
//!        \b PORT_3
//!        \b PORT_4
//!        \b PORT_5
//!        \b PORT_6
//!        \b PORT_7
//!        \b PORT_8
//!        \b PORT_9
//!        \b PORT_10
//!        \b PORT_11
//!        \b PORT_A
//!        \b PORT_B
//!        \b PORT_C
//!        \b PORT_D
//!        \b PORT_E
//!        \b PORT_F
//!        \b PORT_J
//! \param gpioPin is the GPIO pin number(s) that will be test. The
//!			following values can be ORed to test multiple pins.
//!       Valid values are
//!        \b PIN0
//!        \b PIN1
//!        \b PIN2
//!        \b PIN3
//!        \b PIN4
//!        \b PIN5
//!        \b PIN6
//!        \b PIN7
//!        \b PIN8
//!        \b PIN9
//!        \b PIN10
//!        \b PIN11
//!        \b PIN12
//!        \b PIN13
//!        \b PIN14
//!        \b PIN15
//!
//!
//! This function performs an output plausibility check on the GPIO module.
//! The function sets and clears the pin specified by gpioPin.
//! The function will check if the user has passed valid port and gpioPin
//! values. If the MSP430 device does not have the selected PORTx or if the
//! value value for gpioPin is outside the valid range, the function will
//! call IEC60730_FAIL_SAFE_failSafe() if JUMP_TO_FAILSAFE is enabled in
//! "IEC60730_user_config.h", otherwise TEST_FAILURE is returned.
//!
//! These are the valid parameters for gpioPin:
//! -PORT1-PORT11 	valid range (0x0000-0x00FF)
//! -PORTA-PORTF 	valid range (0x0000-0xFFFF)
//! -PORTJ			valid range (0x0000-0x000F)
//!
//! PxOUT are modified by this test but the original state of PxOUT, if
//! test is PASSED.
//!
//! Modified registers are \b PxOUT.
//!
//! \return If the test passes.- SIG_GPIO_TEST
//!			If the test fails.- TEST_FAILURE
//
//*****************************************************************************


uint8_t IEC60730_GPIO_TEST_testGpioOutput(uint16_t port, uint16_t gpioPin){

	uint8_t	i=0;
	uint16_t currentValue=0, pinTest=0;
	volatile uint8_t* pPortAddress8;
	volatile uint16_t* pPortAddress16;

	switch(port){
#if defined(__MSP430_HAS_PORT1_R__)
		case PORT_1:
			pPortAddress8 = &P1OUT;
		break;
#endif // __MSP430_HAS_PORT1_R__

#if defined(__MSP430_HAS_PORT2_R__)
		case PORT_2:
			pPortAddress8 = &P2OUT;
		break;
#endif // __MSP430_HAS_PORT2_R__
#if defined(__MSP430_HAS_PORT3_R__)
		case PORT_3:
			pPortAddress8 = &P3OUT;
		break;
#endif // __MSP430_HAS_PORT3_R__
#if defined(__MSP430_HAS_PORT4_R__)
		case PORT_4:
			pPortAddress8 = &P4OUT;
		break;
#endif // __MSP430_HAS_PORT4_R__
#if defined(__MSP430_HAS_PORT5_R__)
		case PORT_5:
			pPortAddress8 = &P5OUT;
		break;
#endif // __MSP430_HAS_PORT5_R__
#if defined(__MSP430_HAS_PORT6_R__)
		case PORT_6:
			pPortAddress8 = &P6OUT;
		break;
#endif // __MSP430_HAS_PORT6_R__
#if defined(__MSP430_HAS_PORT7_R__)
		case PORT_7:
			pPortAddress8 = &P7OUT;
		break;
#endif // __MSP430_HAS_PORT7_R__
#if defined(__MSP430_HAS_PORT8_R__)
		case PORT_8:
			pPortAddress8 = &P8OUT;
		break;
#endif // __MSP430_HAS_PORT8_R__
#if defined(__MSP430_HAS_PORT9_R__)
		case PORT_9:
			pPortAddress8 = &P9OUT;
		break;
#endif // __MSP430_HAS_PORT9_R__
#if defined(__MSP430_HAS_PORT10_R__)
		case PORT_10:
			pPortAddress8 = &P10OUT;
		break;
#endif // __MSP430_HAS_PORT10_R__
#if defined(__MSP430_HAS_PORT11_R__)
		case PORT_11:
			pPortAddress8 = &P11OUT;
		break;
#endif // __MSP430_HAS_PORT11_R__
		default:
			pPortAddress8 = 0;
			break;
	}

	switch(port){
#if defined(__MSP430_HAS_PORTA_R__)
		case PORT_A:
			pPortAddress16 = &PAOUT;
		break;
#endif // __MSP430_HAS_PORTA_R__

#if defined(__MSP430_HAS_PORTB_R__)
		case PORT_B:
			pPortAddress16 = &PBOUT;
		break;
#endif // __MSP430_HAS_PORTB_R__
#if defined(__MSP430_HAS_PORTC_R__)
		case PORT_C:
			pPortAddress16 = &PCOUT;
		break;
#endif // __MSP430_HAS_PORTC_R__
#if defined(__MSP430_HAS_PORTD_R__)
		case PORT_D:
			pPortAddress16 = &PDOUT;
		break;
#endif // __MSP430_HAS_PORTD_R__
#if defined(__MSP430_HAS_PORTE_R__)
		case PORT_E:
			pPortAddress16 = &PEOUT;
		break;
#endif // __MSP430_HAS_PORTE_R__
#if defined(__MSP430_HAS_PORTF_R__)
		case PORT_F:
			pPortAddress16 = &PFOUT;
		break;
#endif // __MSP430_HAS_PORTF_R__
		default:
			pPortAddress16 = 0;
			break;
	}

	// Verify MSP430 device has selected port
	if((pPortAddress8==0) && (pPortAddress16 == 0)){
		return TEST_FAILURE;
	}else if (pPortAddress8!=0){
		// Verify user has selected valid gpioPin range based on port selection
		if(privateValidatePinRange(port,gpioPin)){
			// Save current state of output pins
			currentValue = *pPortAddress8;
			// Test every gpioPin in Port specified by the user
			for(i=0;i<8;i++){
				pinTest=1<<i;
				// If user want to test selected gpioPin
				if(gpioPin&pinTest){
					// Clear pin
					*pPortAddress8 &= ~(pinTest);
					// Set gpioPin
					*pPortAddress8 |= pinTest;
					// If output was set, continue test
					if(*pPortAddress8 & pinTest){
						// Clear gpioPin
						*pPortAddress8 &= ~(pinTest);
						// If pin was not cleared, test fails, else
						// continue test.
						if((*pPortAddress8 & pinTest)){
							return TEST_FAILURE;
						}
					}
					else{
						return TEST_FAILURE;
					}
				}
			}
			// Restore Port with original values
			*pPortAddress8=currentValue;
			return SIG_GPIO_TEST;
		}else{
			return TEST_FAILURE;
		}
	}else if(pPortAddress16!=0){
		// Verify user has selected valid gpioPin range based on port selection
		if(privateValidatePinRange(port,gpioPin)){
			// Save current state of output pins
			currentValue = *pPortAddress16;
			// Test every gpioPin in Port specified by the user
			for(i=0;i<16;i++){
				pinTest=1<<i;
				// If user want to test selected gpioPin
				if(gpioPin&pinTest){
					// Clear pin
					*pPortAddress16 &= ~(pinTest);
					// Set gpioPin
					*pPortAddress16 |= pinTest;
					// If output was set, continue test
					if(*pPortAddress16 & pinTest){
						// Clear gpioPin
						*pPortAddress16 &= ~(pinTest);
						// If pin was not cleared, test fails, else
						// continue test.
						if((*pPortAddress16 & pinTest)){
							return TEST_FAILURE;
						}
					}
					else{
						return TEST_FAILURE;
					}
				}
			}
			// Restore Port with original values
			*pPortAddress16=currentValue;
			return SIG_GPIO_TEST;
		}else{
			return TEST_FAILURE;
		}
	}
	return TEST_FAILURE;
}

//*****************************************************************************
//
//! Tests input functionality of GPIO module
//!
//! \param port is the port number to be tested. Consult device datasheet to
//!			determine which ports are available in your device.
//!       Valid values are
//!        \b PORT_1
//!        \b PORT_2
//!        \b PORT_3
//!        \b PORT_4
//!        \b PORT_5
//!        \b PORT_6
//!        \b PORT_7
//!        \b PORT_8
//!        \b PORT_9
//!        \b PORT_10
//!        \b PORT_11
//!        \b PORT_A
//!        \b PORT_B
//!        \b PORT_C
//!        \b PORT_D
//!        \b PORT_E
//!        \b PORT_F
//!        \b PORT_J
//! \param gpioPin is the GPIO pin number(s) that will be test. The
//!			following values can be ORed to test multiple pins.
//!       Valid values are
//!        \b PIN0
//!        \b PIN1
//!        \b PIN2
//!        \b PIN3
//!        \b PIN4
//!        \b PIN5
//!        \b PIN6
//!        \b PIN7
//!        \b PIN8
//!        \b PIN9
//!        \b PIN10
//!        \b PIN11
//!        \b PIN12
//!        \b PIN13
//!        \b PIN14
//!        \b PIN15
//! \param expected is the GPIO pin number(s) that will be test.
//!       Valid values are
//!        \b PIN0_HIGH
//!		   \b PIN0_LOW
//!        \b PIN1_HIGH
//!		   \b PIN1_LOW
//!        \b PIN2_HIGH
//!		   \b PIN2_LOW
//!        \b PIN3_HIGH
//!		   \b PIN3_LOW
//!        \b PIN4_HIGH
//!		   \b PIN4_LOW
//!        \b PIN5_HIGH
//!		   \b PIN5_LOW
//!        \b PIN6_HIGH
//!		   \b PIN6_LOW
//!        \b PIN7_HIGH
//!		   \b PIN7_LOW
//!        \b PIN8_HIGH
//!		   \b PIN8_LOW
//!        \b PIN9_HIGH
//!		   \b PIN9_LOW
//!        \b PIN10_HIGH
//!		   \b PIN10_LOW
//!        \b PIN11_HIGH
//!		   \b PIN11_LOW
//!        \b PIN12_HIGH
//!		   \b PIN12_LOW
//!        \b PIN13_HIGH
//!		   \b PIN13_LOW
//!        \b PIN14_HIGH
//!		   \b PIN14_LOW
//!        \b PIN15_HIGH
//!		   \b PIN15_LOW
//!
//! This function performs an input plausibility check on the GPIO module.
//! The function compares the current state in PxIN with the expectedValue
//! and the test is passed if both values are equal.
//! If the MSP430 device does not have the selected PORTx or if the
//! value value for gpioPin is outside the valid range, the function will
//! call IEC60730_FAIL_SAFE_failSafe() if JUMP_TO_FAILSAFE is enabled in
//! "IEC60730_user_config.h" otherwise TEST_FAILURE is returned.
//!
//! These are the valid parameters for gpioPin:
//! -PORT1-PORT11 	valid range (0x0000-0x00FF)
//! -PORTA-PORTF 	valid range (0x0000-0xFFFF)
//! -PORTJ			valid range (0x0000-0x000F)
//!
//! \return If the test passes.- SIG_GPIO_TEST
//!			If the test fails.- TEST_FAILURE

uint8_t IEC60730_GPIO_TEST_testGpioInput(uint16_t port,
	uint16_t gpioPin,
	uint16_t expectedValue){

	uint8_t	i=0;
	uint16_t pinTest;
	volatile const uint8_t* pPortAddress8;
	volatile const uint16_t* pPortAddress16;

// Feed WDT in case user has WDT enabled before running GPIO test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT



	switch(port){
#if defined(__MSP430_HAS_PORT1_R__)
		case PORT_1:
			pPortAddress8 = &P1IN;
		break;
#endif // __MSP430_HAS_PORT1_R__

#if defined(__MSP430_HAS_PORT2_R__)
		case PORT_2:
			pPortAddress8 = &P2IN;
		break;
#endif // __MSP430_HAS_PORT2_R__
#if defined(__MSP430_HAS_PORT3_R__)
		case PORT_3:
			pPortAddress8 = &P3IN;
		break;
#endif // __MSP430_HAS_PORT3_R__
#if defined(__MSP430_HAS_PORT4_R__)
		case PORT_4:
			pPortAddress8 = &P4IN;
		break;
#endif // __MSP430_HAS_PORT4_R__
#if defined(__MSP430_HAS_PORT5_R__)
		case PORT_5:
			pPortAddress8 = &P5IN;
		break;
#endif // __MSP430_HAS_PORT5_R__
#if defined(__MSP430_HAS_PORT6_R__)
		case PORT_6:
			pPortAddress8 = &P6IN;
		break;
#endif // __MSP430_HAS_PORT6_R__
#if defined(__MSP430_HAS_PORT7_R__)
		case PORT_7:
			pPortAddress8 = &P7IN;
		break;
#endif // __MSP430_HAS_PORT7_R__
#if defined(__MSP430_HAS_PORT8_R__)
		case PORT_8:
			pPortAddress8 = &P8IN;
		break;
#endif // __MSP430_HAS_PORT8_R__
#if defined(__MSP430_HAS_PORT9_R__)
		case PORT_9:
			pPortAddress8 = &P9IN;
		break;
#endif // __MSP430_HAS_PORT9_R__
#if defined(__MSP430_HAS_PORT10_R__)
		case PORT_10:
			pPortAddress8 = &P10IN;
		break;
#endif // __MSP430_HAS_PORT10_R__
#if defined(__MSP430_HAS_PORT11_R__)
		case PORT_11:
			pPortAddress8 = &P11IN;
		break;
#endif // __MSP430_HAS_PORT11_R__
		default:
			pPortAddress8 = 0;
			break;
	}

	switch(port){
#if defined(__MSP430_HAS_PORTA_R__)
		case PORT_A:
			pPortAddress16 = &PAIN;
		break;
#endif // __MSP430_HAS_PORTA_R__
#if defined(__MSP430_HAS_PORTB_R__)
		case PORT_B:
			pPortAddress16 = &PBIN;
		break;
#endif // __MSP430_HAS_PORTB_R__
#if defined(__MSP430_HAS_PORTC_R__)
		case PORT_C:
			pPortAddress16 = &PCIN;
		break;
#endif // __MSP430_HAS_PORTC_R__
#if defined(__MSP430_HAS_PORTD_R__)
		case PORT_D:
			pPortAddress16 = &PDIN;
		break;
#endif // __MSP430_HAS_PORTD_R__
#if defined(__MSP430_HAS_PORTE_R__)
		case PORT_E:
			pPortAddress16 = &PEIN;
		break;
#endif // __MSP430_HAS_PORTE_R__
#if defined(__MSP430_HAS_PORTF_R__)
		case PORT_F:
			pPortAddress16 = &PFIN;
		break;
#endif // __MSP430_HAS_PORTF_R__
		default:
			pPortAddress16 = 0;
			break;
	}

	// Verify MSP430 device has selected port
	if((pPortAddress8==0) && (pPortAddress16 == 0)){
		return TEST_FAILURE;
	}else if (pPortAddress8!=0){
		// Verify user has selected valid gpioPin range based on port selection
		if(privateValidatePinRange(port,gpioPin)){
			for(i=0;i<8;i++){
				pinTest=1<<i;
				// Test if selected input put has the expectedValue
				if(gpioPin&pinTest){
					if((*pPortAddress8 & pinTest) != (expectedValue&pinTest)){
						return TEST_FAILURE;
					}
				}
			}

			return SIG_GPIO_TEST;
		}else{
			// If user passed an invalid gpioPin range
			return TEST_FAILURE;
		}
	}else if (pPortAddress16!=0){
		// Verify user has selected valid gpioPin range based on port selection
		if(privateValidatePinRange(port,gpioPin)){
			for(i=0;i<16;i++){
				pinTest=1<<i;
				// Test if selected input put has the expectedValue
				if(gpioPin&pinTest){
					if((*pPortAddress16 & pinTest) != (expectedValue&pinTest)){
						return TEST_FAILURE;
					}
				}
			}
			return SIG_GPIO_TEST;
		}else{
			// If user passed an invalid gpioPin range
			return TEST_FAILURE;


		}
	}

	return TEST_FAILURE;
}

//*****************************************************************************
//
//! Checks if gpioPin range is valid based on port selection.
//!
//! \param port is the port number to be tested. Consult device datasheet to
//!			determine which ports are available in your device.
//!       Valid values are
//!        \b PORT_1
//!        \b PORT_2
//!        \b PORT_3
//!        \b PORT_4
//!        \b PORT_5
//!        \b PORT_6
//!        \b PORT_7
//!        \b PORT_8
//!        \b PORT_9
//!        \b PORT_10
//!        \b PORT_11
//!        \b PORT_A
//!        \b PORT_B
//!        \b PORT_C
//!        \b PORT_D
//!        \b PORT_E
//!        \b PORT_F
//!        \b PORT_J
//! \param gpioPin is the gpio pin number(s) that will be test. The
//!			following values can be ORed to test multiple pins.
//!       Valid values are
//!        \b PIN0
//!        \b PIN1
//!        \b PIN2
//!        \b PIN3
//!        \b PIN4
//!        \b PIN5
//!        \b PIN6
//!        \b PIN7
//!        \b PIN8
//!        \b PIN9
//!        \b PIN10
//!        \b PIN11
//!        \b PIN12
//!        \b PIN13
//!        \b PIN14
//!        \b PIN15
//!
//!
//! Long funtion description goes here.
//!
//!
//! Modified registers are \b REGISTER_1, \b REGSITER_2, and \b REGISTER_3
//!
//! \return None
//
//*****************************************************************************

static uint8_t privateValidatePinRange(uint16_t port, uint16_t gpioPin){
	if((port== PORT_1) | (port== PORT_2) | (port== PORT_3) | (port== PORT_4) | (port== PORT_5) |
	   (port== PORT_6) | (port== PORT_7) | (port== PORT_8) | (port== PORT_9) | (port== PORT_10) |
	   (port== PORT_11)){
		if(gpioPin>0x00ff){
			return TEST_FAILURE;
		}
	}else if(port== PORT_J){
		if(gpioPin>0x000f){

			return TEST_FAILURE;
		}
	}

// Feed WDT in case user has WDT enabled before running GPIO test
#if ENABLED_WDT
	uint16_t wdtConfig =  WDTCTL & 0x00ff;
	WDTCTL = WDTPW + WDTCNTCL + wdtConfig;
#endif // ENABLED_WDT


	 return 1;
 }
 
 //*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
