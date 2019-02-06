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
/*******************************************************************************
 *
 *  FR4133_IR_BP_RX.c
 *     - MSP430 firmware application to demonstrate the IR receiver
 *       capability of the MSP430FR4133 interfacing with the IR
 *       BoosterPack.
 ******************************************************************************/

/******************************************************************************
 *         MSP430FR4133-LaunchPad IR BoosterPack Receiver User Experience
 *
 * This application operates on the LaunchPad platform using the MSP430FR4133
 * device and the IR BoosterPack plugin board. TimerA is used to decode the signal
 * received from IR receiver module on IR BoosterPack. TimerA is driven by 4MHz
 * SMCLK .After successful decoding, LCD will display the letters on the pressed
 * button and one LED on P4.0 will blink.
 *
 * Texas Instruments, Inc.
 * Ver 0.2 Aug. 2014
 ******************************************************************************/

#include "msp430.h"
#include "HAL_FR4133LP_LCD.h"
#include "HAL_FR4133LP_Board.h"


unsigned int	new_cnt=0;		//new timer counter
unsigned int 	old_cnt=0;		//old timer counter
unsigned int	time_cnt=0;		//time interval between two edges
unsigned char 	rx_data[4];		//data buffer for IR receiver
unsigned char	data_cnt=0;		//byte counter
unsigned char 	edge_cnt=0;		//edge counter
unsigned char	bit_cnt=0;		//bit counter
unsigned char	header_cnt=0;	//header edge counter
unsigned char	button_num;		//IR button number
unsigned char	i;
unsigned char	IR_state;		//IR decoder state machine

#define IR_idle	 	0x00		//idle state
#define IR_data		0x01		//data
#define IR_end		0x02		//the end of IR code

void BlinkLED ();

int main( void )
{
	// Watchdog timer works as default setting
	WDTCTL = WDTPW + WDTHOLD;

	Init_GPIO();						//Initialize GPIO
	Init_Clock();						//Initialize Clock
	Init_LCD();							//Initialize LCD

	P1DIR &= ~BIT6;						//Set P1.6 as input
	P1SEL0|=  BIT6;						//Set P1.6 as TA0.2 input
	P4DIR |=  BIT0;						//Set P4.0 as output
	P4OUT &= ~BIT0;     				//output low

	LCD_Display_MSP_IR();				// Display "MSP--IR"
	LCD_Display_RX();					// Display "RX"

	TA0CTL = TASSEL_2 + MC_2 + TACLR;	//SMCLK, Continuous mode
	TA0CCTL2=CM_3+SCS+CCIS_0+CAP+CCIE;	//set TA0.2 control register choose CCIxA
	_EINT();

	IR_state = IR_idle;					//initialize decoder state machine
	while(1)
	{
		if(rx_data[2] == (unsigned char)(~rx_data[3]) && rx_data[0]==0x55 && rx_data[1]==0xaa)
		{
			button_num = rx_data[2];		//Get button number
			LCD_Clear();
			LCD_Display_Buttons(button_num);
			LCD_Display_RX();	//display RX
			BlinkLED();  		//blink LED
		}
		__bis_SR_register(LPM3_bits | GIE); //enter low power mode
	}
}

//use blinking LED to indicate pressed button
void BlinkLED ()
{
	for(i=0; i<8; i++)
	{
		P4OUT ^= BIT0;
		__delay_cycles(200000);
	}
	P4OUT &= ~BIT0;
}

//********Timer0 interrupt ISR*********//
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
	switch( TA0IV )
	{
	case  4:												//Interrupt Source: Capture 2
	{
		old_cnt = new_cnt;									//Update the counter value
		new_cnt = TA0CCR2;
		time_cnt = new_cnt -old_cnt;						//Time interval
		switch(IR_state)									//IR decoder state machine
		{
		case IR_idle:										//Header of IR code
		{
			for(i=0; i<4; i++)
				rx_data[i] = 0x00;							//Clear the data buffer
			if(header_cnt == 0)
				header_cnt = 1;
			else if(header_cnt == 1 && time_cnt > 32000 && time_cnt < 40000)	//9ms leading pulse burst
				header_cnt = 2;
			else if(header_cnt == 2 && time_cnt > 14000 && time_cnt < 22000)	//4.5ms space
			{
				header_cnt = 0;
				IR_state = IR_data;
				edge_cnt = 1;
			}
			else
				header_cnt = 0;
			break;
		}
		case IR_data:										    //Data of IR code(address + command)
		{
			if(time_cnt > 32000 && time_cnt < 40000)
			{
				edge_cnt = 0;								    //clear the counter
				data_cnt = 0;
				bit_cnt	 = 0;
				header_cnt = 2;
				IR_state = IR_idle;							    //Enter idle state and wait for new code
				break;
			}
			else{
				edge_cnt++;										//Both rising edge and falling edge are captured
				if(edge_cnt % 2 == 1)
				{
					data_cnt = (edge_cnt-2) / 16;
					if(time_cnt > 4500)							//Space is 1.68ms
						rx_data[data_cnt] |= 0x80;				//Logical '1'
					else										//Space is 0.56ms
						rx_data[data_cnt] &= 0x7f;				//Logical '0'
					if(bit_cnt != 7)
					{
						rx_data[data_cnt] = rx_data[data_cnt]>>1; //Shifting based on Byte
						bit_cnt++;
					}
					else
						bit_cnt = 0;							//Start a new byte
				}
				if(edge_cnt > 64)
					IR_state = IR_end;							//The end of address and command
				break;
			}
		}
		case IR_end:										//The end of IR code
		{
			edge_cnt = 0;									//clear the counter
			data_cnt = 0;
			bit_cnt	 = 0;
			if(time_cnt > 2000)								//A final 0.56ms pulse burst
				IR_state = IR_idle;							//Enter idle state and wait for new code
			break;
		}
		default:
		{
			IR_state = IR_idle;								//Set idle state as default state
			header_cnt = 0;									//Clear all the counter
			edge_cnt = 0;
			data_cnt = 0;
			bit_cnt	 = 0;
			break;
		}
		}
		LPM3_EXIT;											//Exit low power mode
	}
	default: break;
	}
}


