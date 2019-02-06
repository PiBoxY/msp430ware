/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
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
/*
 * HAL_FR4133LP_Board.c
 *
 *  Created on: Jul 30, 2014
 *      Author: a0222631
 */
#include "msp430fr4133.h"
#include "HAL_FR4133LP_LCD.h"
#include "HAL_FR4133LP_Board.h"

unsigned char key_num=0;
unsigned char buttonDebounce = 1;

//Initialize Clock
void Init_Clock()
{
	P4SEL0 |= BIT1 | BIT2;                  // set XT1 pin as second function

	do
	{
		CSCTL7 &= ~(XT1OFFG | DCOFFG);      // Clear XT1 and DCO fault flag
		SFRIFG1 &= ~OFIFG;
	} while (SFRIFG1 & OFIFG);              // Test oscillator fault flag

	CSCTL3 |= SELREF__XT1CLK;               // Set XT1CLK as FLL reference source
	CSCTL1 &= ~(DCORSEL_7);                 // Clear DCO frequency select bits first
	CSCTL1 |= DCORSEL_3;                    // Set DCO = 8MHz
	CSCTL2 = FLLD_0 + 243;                  // DCODIV = 8MHz

	do
	{
		__delay_cycles(7 * 31 * 8);         // Requires 7 reference clock delay before
											// polling FLLUNLOCK bits
											// @8 MHz, ~1736 cycles
	} while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));// Poll until FLL is locked

	CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;  // Set ACLK = XT1CLK = 32768Hz
												// DCOCLK = MCLK and SMCLK source
	CSCTL5 |= DIVM_0 | DIVS_1;              // MCLK = DCOCLK = 8MHZ,
											// SMCLK = MCLK/2 = 4MHz
}

//Initialize GPIO
void Init_GPIO()
{

	// Configure all GPIO to Output Low
	// Make sure there is no pin conflict
	P1OUT = 0x40; P2OUT = 0x00; P3OUT = 0x00; P4OUT = 0x00;
	P5OUT = 0x02; P6OUT = 0x00; P7OUT = 0x00; P8OUT = 0x00;

	P1DIR = 0xBF; P2DIR = 0xFF; P3DIR = 0xFF; P4DIR = 0xFF;
	P5DIR = 0xFD; P6DIR = 0xFF; P7DIR = 0xFF; P8DIR = 0xFF;

	PM5CTL0 &= ~LOCKLPM5;           			// Disable the GPIO power-on default high-impedance mode
												// to activate previously configured port settings

}

//Initialize Keypad IO
void Init_keypadIO()
{
	// Configure Keypad GPIO
	P1DIR &= ~(BIT3 + BIT4 + BIT5);				//Set P1.3/1.4/1.5/2.7 as keypad input
	P2DIR &= ~BIT7;
	P1REN |= (BIT3 + BIT4 + BIT5);				//Pull up P1.3/1.4/1.5/2.7
	P2REN |= BIT7;
	P1OUT |= (BIT3 + BIT4 + BIT5);				//P1.3/1.4/1.5/2.7 output high
	P2OUT |= BIT7;
	P1IES |= (BIT3 + BIT4 + BIT5);				//Set P1.3/1.4/1.5/2.7 interrupt edge(high to low)
	P2IES |= BIT7;
	P1IE  |= (BIT3 + BIT4 + BIT5);				//Enable P1.3/1.4/1.5/2.7 interrupt
	P2IE  |= BIT7;

	P8DIR |= (BIT0 + BIT1);						//Set P8.0/8.1/1.1/2.5 as keypad output
	P1DIR |= BIT1;
	P2DIR |= BIT5;
	P8OUT &= ~(BIT0 + BIT1);					//P8.0/8.1/1.1/2.5 output low
	P1OUT &= ~BIT1;
	P2OUT &= ~BIT5;

	P1IFG = 0;									//Clear Port1 IFG
	P2IFG = 0;									//Clear Port2 IFG
	_EINT();
}


// keypad scan function
unsigned char scan_key(void)
{
	unsigned char row_sel=0;
	unsigned char key_in=0;
	unsigned char keycol=0;
	unsigned char keyrow=0;
	unsigned char key_array[KEYNUM+1];
	unsigned char i=0;
	unsigned char j=0;

	// clear the array before scan
	for (i = 0 ; i < (KEYNUM + 1 ); i++)
		key_array[i] = 0;

	keycol = BIT0;

	for (i = 0 ; i < COL ; i ++)
	{
		P8DIR &= ~(BIT0 + BIT1);
		P1DIR &= ~BIT1;
		P2DIR &= ~BIT5;
		switch(i)
		{
		case 0:
			P8OUT &= ~BIT1;
			P8DIR |= BIT1;						// select only one column
		case 1:
			P1OUT &= ~BIT1;
			P1DIR |= BIT1;						// select only one column
		case 2:
			P8OUT &= ~BIT0;
			P8DIR |= BIT0;						// select only one column
		case 3:
			P2OUT &= ~BIT5;
			P2DIR |= BIT5;						// select only one column
		}
		__delay_cycles(100);
		key_in = (P1IN & 0x38) | (P2IN & 0x80); // get key input value
		if((key_in&0x08)!=0)					// find the pressed button row
			row_sel|=0x08;
		if((key_in&0x10)!=0)
			row_sel|=0x04;
		if((key_in&0x20)!=0)
			row_sel|=0x02;
		if((key_in&0x80)!=0)
			row_sel|=0x01;
		keyrow = BIT3;
		for (j = 0 ; j< ROW ; j++)
		{
			if ((row_sel & keyrow) == 0)
				key_array[(j + i * ROW  + 1)] = 1;
			keyrow = keyrow >> 1;
		}
		keycol = keycol << 1 ;
		row_sel=0;
	}

	for ( i = 0 ; i < (KEYNUM + 1 ); i++) // get pressed button number
	{
		if (key_array[i])
			key_num = i;
	}

	LCD_Clear();
	LCD_Display_Buttons(key_num);
	LCD_Display_TX();

	P8DIR |= BIT0 + BIT1;   						//Set P8.0/8.1/1.1/2.5 as keypad output
	P1DIR |= BIT1;
	P2DIR |= BIT5;
	P8OUT &= ~(BIT0 + BIT1);						//P8.0/8.1/1.1/2.5 output low
	P1OUT &= ~BIT1;
	P2OUT &= ~BIT5;

	return key_num;
}

// Sets up the WDT as a button debouncer, only activated once a
// button interrupt has occurred.
void Buttons_startWDT()
{
	// WDT as 250ms interval counter
	SFRIFG1 &= ~WDTIFG;
	WDTCTL = WDTPW + WDTSSEL_1 + WDTTMSEL + WDTCNTCL + WDTIS_5;
	SFRIE1 |= WDTIE;
}


// Handles Watchdog Timer interrupts.
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	if (buttonDebounce == 2)
	{
		buttonDebounce = 1;

		SFRIFG1 &= ~WDTIFG;
		SFRIE1 &= ~WDTIE;
		WDTCTL = WDTPW + WDTHOLD;
	}
}

//********Port1 interrupt ISR*********//
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	P1IFG = 0;										// clear IFG
	if (buttonDebounce == 1)
	{
		buttonDebounce = 2;
		Buttons_startWDT();
		LPM3_EXIT;									// exit low power mode
	}
}

//********Port2 interrupt ISR*********//
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	P2IFG = 0;										// clear IFG
	if (buttonDebounce == 1)
	{
		buttonDebounce = 2;
		Buttons_startWDT();
		LPM3_EXIT;									// exit low power mode
	}
}



