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

#ifndef _IEC60730_GPIO_TEST_H_
#define _IEC60730_GPIO_TEST_H_

//*****************************************************************************
//
//The following include is added to use C99 data types
//
//*****************************************************************************
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif


//*****************************************************************************
//
//The following defines are used to determine the valid parameter to be passed
//as to IEC60730_GPIO_TEST_testGpioOutput
//
//*****************************************************************************

#define PORT_1     		0x0001
#define PORT_2     		0x0002
#define PORT_3     		0x0004
#define PORT_4    		0x0008
#define PORT_5     		0x0010
#define PORT_6     		0x0020
#define PORT_7     		0x0040
#define PORT_8    		0x0080
#define PORT_9    	 	0x0100
#define PORT_10     	0x0200
#define PORT_11    		0x0400
#define PORT_A    		PORT_1 + PORT_2
#define PORT_B     		PORT_3 + PORT_4
#define PORT_C    		PORT_5 + PORT_6
#define PORT_D    		PORT_7 + PORT_8
#define PORT_E    		PORT_9 + PORT_10
#define PORT_F    		PORT_11
#define PORT_J    		0x1000


//*****************************************************************************
//
//The following are values that can be passed to GPIO TEST API
//as the gpioPin parameter.
//
//*****************************************************************************

#define PIN0   			0x0001
#define PIN1   			0x0002
#define PIN2   			0x0004
#define PIN3   			0x0008
#define PIN4   			0x0010
#define PIN5   			0x0020
#define PIN6   			0x0040
#define PIN7   			0x0080
#define PIN8   			0x0100
#define PIN9   			0x0200
#define PIN10   		0x0400
#define PIN11   		0x0800
#define PIN12   		0x1000
#define PIN13   		0x2000
#define PIN14   		0x4000
#define PIN15   		0x8000


//*****************************************************************************
//
//The following are values that can be passed to GPIO TEST API
//as the expectedValue parameter.
//
//*****************************************************************************

#define PIN0_HIGH   			0x0001
#define PIN1_HIGH   			0x0002
#define PIN2_HIGH   			0x0004
#define PIN3_HIGH   			0x0008
#define PIN4_HIGH   			0x0010
#define PIN5_HIGH   			0x0020
#define PIN6_HIGH   			0x0040
#define PIN7_HIGH   			0x0080
#define PIN8_HIGH   			0x0100
#define PIN9_HIGH   			0x0200
#define PIN10_HIGH		   		0x0400
#define PIN11_HIGH   			0x0800
#define PIN12_HIGH   			0x1000
#define PIN13_HIGH   			0x2000
#define PIN14_HIGH   			0x4000
#define PIN15_HIGH   			0x8000
#define PIN0_LOW				0x0000
#define PIN1_LOW				0x0000
#define PIN2_LOW				0x0000
#define PIN3_LOW				0x0000
#define PIN4_LOW				0x0000
#define PIN5_LOW				0x0000
#define PIN6_LOW				0x0000
#define PIN7_LOW				0x0000
#define PIN8_LOW				0x0000
#define PIN9_LOW				0x0000
#define PIN10_LOW				0x0000
#define PIN11_LOW				0x0000
#define PIN12_LOW				0x0000
#define PIN13_LOW				0x0000
#define PIN14_LOW				0x0000
#define PIN15_LOW				0x0000


//*****************************************************************************
//
//Prototypes for the APIs.
//
//*****************************************************************************
extern uint8_t IEC60730_GPIO_TEST_testGpioOutput(uint16_t port,
		uint16_t gpioPin);

extern uint8_t IEC60730_GPIO_TEST_testGpioInput(uint16_t port,
		uint16_t gpioPin,
		uint16_t expectedValue);


#ifdef __cplusplus
}
#endif // extern "C"




#endif
