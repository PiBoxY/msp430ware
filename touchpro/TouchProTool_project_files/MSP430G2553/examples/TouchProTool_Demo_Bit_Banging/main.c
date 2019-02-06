/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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
/***************************************************************************************
 * Main.c
 *
 * Description: Detect and display changes in capacitance and finger positions along
 *              the 430BOOST-SENSE1, Capacitive Touch Booster Pack for the MSP430G2553
 *              launchpad using bit banging method of transmitting
 *              data from the target to the Cap Touch Display tool
 *
 * Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/

 /************************************************************************************************
 * @file   Main.c
 *
 * @brief
 *
 * @par    Project:
 *             Cap Touch Display Demo Using Bit Banging Method
 *
 * @par    Developed using:
 *             IAR Version : 5.50
 *
 *
 * @version     1.0.0 Initial Release
 *
 * @par    Supported Configurations:
 *              - main()
 *              - config()
 *              - data_frame_send()
 *              - UART_send_byte()
 ***********************************************************************************************/
/**********************************************************************************************
* I/O		MSP430G2553
*  		   -------------
* 		  |VCC		 GND|--> GND
* 		  |P1.0			|
* Data <--|P1.1			|
*		  |P1.2		    |
* 		  |P1.3			|
* 		  |P1.4		P1.7|
* 		  |P1.5     P1.6|
* 		  |P2.0     P2.5|
* 	E1 <--|P2.1		P2.4|--> E4
* 	E2 <--|P2.2  	P2.3|--> E3
*		   -------------
****************************************************************************************************/
#include "msp430.h"

#include "CTS_Layer.h"


void data_frame_send(void); 		// Cap Touch Display Data Logger
void UART_send_byte(unsigned char data2send);

#define Custom_Counts				//If defined, delta counts displayed, else Wheel Position
#define DataHead 0x55AA

#ifdef Custom_Counts
#define SENSOR_NUM 4     			//Defines the number of data channels to display
#else
#define SENSOR_NUM 1				//Defines 1 data channel to display wheel position info
#endif

uint16_t count[SENSOR_NUM];

#define TXD BIT1  					//P1.1  Bit bang this pin to send out the data to the tool
#define TXDOUT P1OUT  				//Output value
#define TXDDIR P1DIR  				//P1.1

unsigned int WheelPos;
/*
 * Function sets up the DCO and SMCLK clocks and GPIO ports
 */
void config(void) {
	WDTCTL = WDTPW + WDTHOLD;   	// Stop WDT

	BCSCTL1 = CALBC1_1MHZ; 			// Set range to 1MHz
	DCOCTL = CALDCO_1MHZ; 			// Set DCO to 1MHz.  Declared in msp430g2553.h
	BCSCTL2 |= DIVS_2; 				// Set SMCLK Divider to 4 (250kHz).  Sets scan/gate time to 2.048ms.  (512/250KHz)
	BCSCTL3 |= LFXT1S_2;        	// LFXT1 = VLO

	P1DIR = 0xFF;					//Sent P1 for data output
	P1OUT = 0;						//Output is low

	P2DIR = 0xFF;              		//Set P2 for data output
	P2SEL = 0;
	P2SEL2 = 0;
	P2OUT = 0;

	TXDOUT = 0;				   		//Hardware setup for tool
	TXDOUT |= TXD;             		//Hardware setup for  tool
	TXDDIR |= TXD;			   		//Hardware setup for tool
}
/*
 * Function initializes and updates baseline for wheel and monitors the changes in capacitance to
 * determine touch or no touch.
 *
 */
void main(void) {

#ifndef Custom_Counts
	static const int channelIndex =0;
#endif

	config();

	TI_CAPT_Init_Baseline(&wheel);  	  //calculate baseline
	TI_CAPT_Update_Baseline(&wheel,50);   //update the baseline

	while(1) {

#ifdef Custom_Counts
		TI_CAPT_Custom(&wheel, count);   //API call that measures changes in capacitance
										 //(that occur due to touch or no touch) for each element within the wheel

#else
		WheelPos = TI_CAPT_Wheel(&wheel);  //Gets finger position on the wheel

		if (WheelPos == 0XFFFF){
			WheelPos = 50;				//Limits the no touch value from 65535 to 50
		}
		count[channelIndex] = WheelPos;
#endif
		data_frame_send();				// Cap Touch Display Data Logger Call.

        P1OUT ^= 0x01; // toggle LED
	}
}

/*
 * Function calculates data length and check sum.  This is the
 * Cap Touch Display tool protocol
 */
void data_frame_send(void) {
	unsigned char check_sum = 0;
	unsigned char i = 0;
	unsigned int temp = 0;
	unsigned char data_length = 0;

	data_length = SENSOR_NUM * 3  + 1;    //# of channels * 3 + 1 (checksum).
										  //1 byte for channel number, 1 byte for high order data byte
										  //and 3rd byte is for low order byte.  The plus 1 is for checksum byte.

	temp = DataHead;
	temp >>= 8; 						   //Send high byte
	UART_send_byte((unsigned char) temp);

	check_sum = (unsigned char) temp;

	UART_send_byte((unsigned char) DataHead);

	check_sum += (unsigned char) DataHead;

	UART_send_byte(data_length);
	check_sum += data_length;

		for (i = 0; i < SENSOR_NUM; i++) {
		UART_send_byte(i + 1);
		check_sum += i + 1;

		temp = count[i];
		temp >>= 8; 						//Send high byte

		UART_send_byte((unsigned char) temp);
		check_sum += (unsigned char) temp;

		UART_send_byte((unsigned char) count[i]);
		check_sum += (unsigned char) count[i];
	}
	UART_send_byte((unsigned char) check_sum);
}
/*
 * Function sends the data as low byte and high byte using bit banging method on P1.1.
 * P1.1 is either cleared or set to send each value of count array
 */
void UART_send_byte(unsigned char data2send) {
	unsigned int temp = 0;
	unsigned int i;

	temp = data2send | (0xff00); 		//send low_8_bit
	temp <<= 1;
	for (i = BIT0; i < BITB; i <<= 1) {
		if (temp & i)
			TXDOUT |= TXD;
		else
			TXDOUT &= ~TXD;
     		__delay_cycles(90);			//Sets data rate to 9600 baud
	}
	TXDOUT |= TXD; 						// end of one byte
}
