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
 *  FR4133_IR_BP_Learn.c
 *     - MSP430 firmware application to demonstrate the IR learning
 *       capability of the MSP430FR4133 interfacing with the IR
 *       BoosterPack.
 *
 ******************************************************************************/

/******************************************************************************
 *         MSP430FR4133-LaunchPad IR BoosterPack Learning Mode User Experience
 *
 * This application operates on the LaunchPad platform using the MSP430FR4133
 * device and the IR BoosterPack plugin board. IR receiver on IR Booterpack is
 * used to received IR signal. The received IR code will be stored in FRAM. IR
 * modulator inside MSP430FR4133 is used to control IR diode on the IR BoosterPack
 * to generate the IR emitting signal to reproduce the received IR waveform.
 *
 * There is a 4x4 keypad on the IR BoosterPack. If the button "copy" is pressed,
 * the firmware will enter IR learning mode. Press one button to specify it as
 * learning button. Except button "copy" and "ok", all other 14 buttons can be
 * specified to different IR code.  After the IR learning process is done, press
 * button "ok". IR code from IR receiver will be stored in FRAM. Each button has
 * its own FRAM memory address. Press the specified button again, the received IR
 * waveform will be reproduced.
 *
 * Texas Instruments, Inc.
 * Ver 0.1 Aug. 2014
 ******************************************************************************/

#include "msp430fr4133.h"
#include "HAL_FR4133LP_LCD.h"
#include "HAL_FR4133LP_Learn_Board.h"


unsigned char 	IR_stop;
unsigned char	button_num;		//button number
unsigned char 	code_num=0;
unsigned char	copy_mode=0;	//IR mode
#pragma PERSISTENT(rx_cnt); //store rx_cnt in FRAM
unsigned int    rx_cnt[14]={0}; //received bit counter
unsigned int    tx_cnt=1;       //transmitted bit counter
unsigned int	new_cnt=0;		//new timer counter
unsigned int 	old_cnt=0;		//old timer counter
unsigned int	time_cnt=0;		//time interval between two edges

unsigned int *FRAM_write_ptr = (unsigned int *)0xE000;
unsigned int *FRAM_read_ptr  = (unsigned int *)0xE000;

void BlinkLED ();
void IR_Mode_Setting();


int main( void )
{
	// Watchdog timer works as default setting
	WDTCTL = WDTPW + WDTHOLD;

	Init_GPIO();								//Initialize GPIO
	Init_Clock();								//Initialize Clock
	Init_LCD();									//Initialize LCD
	Init_keypadIO();							//Initialize Board

	LCD_Display_letter(pos1,11);//L
	LCD_Display_letter(pos2, 4);//E
	LCD_Display_letter(pos3, 0);//A
	LCD_Display_letter(pos4,17);//R
	LCD_Display_letter(pos5,13);//N

	P4DIR |= BIT0;								//Set P4.0 to toggle LED
	P4OUT &= ~BIT0;

	// Configure IR input pin
	P1DIR &= ~BIT6;								//Set P1.6 as input
	P1SEL0|=  BIT6;								//Set P1.6 as TA0.2 input

	copy_mode = 0;								//initial state
	IR_stop = 2; 								//disable IR

	while(1)
	{
		if(copy_mode == 1 && IR_stop == 1)
		{
			TA0CTL = TASSEL_2 + MC_2 + TACLR;	//SMCLK, Continuous mode
			TA0CCTL2=CM_3+SCS+CCIS_0+CAP+CCIE;	//set TA0.2 control register choose CCIxA
			while(IR_stop == 1);
			TA0CTL = 0;
			TA0CCTL2 = 0;
		}
		else if(copy_mode == 0 && IR_stop == 0)
		{
			// Configure IR output pin
			P1SEL0|= BIT0;						// use internal IR modulator

			// disable Port1 & Port2 interrupt
			P1IE = 0;
			P2IE = 0;

			// Configure IR modulation: ASK
			SYSCFG1 = IRDSSEL + IREN;
			TA0CCTL0 = CCIE;
			TA0CCTL2 = OUTMOD_0;				// output mode: output
			TA1CCTL2 = OUTMOD_7;        		// output mode: reset/set

			// 38kHz 1/4 duty-cycle carrier waveform length setting
			TA1CCR0 = 104;
			TA1CCR2 = 25;

			// envelope signal length setting
			TA0CCR0 = 640;   					//the initial time of TA0 should be longer than TA1

			// set timer operation mode
			TA1CTL = TASSEL_2 + MC_1 + TACLR;   //SMCLK, UP mode
			TA0CTL = TASSEL_2 + MC_1 + TACLR;	//SMCLK, UP mode

			// stop until the end of IR code
			while(IR_stop == 0);

			TA0CCTL0 = 0;
			TA0CCTL2 = 0;
			TA0CTL = 0;
			TA0CCR0 = 0;

			TA1CCTL0 = 0;
			TA1CCTL2 = 0;
			TA1CTL = 0;
			TA1CCR0 = 0;
			TA1CCR2 = 0;						//disable timer1

			P1IE |= (BIT3 + BIT4 + BIT5);		//enable GPIO interrupt
			P2IE |= BIT7;
		}
		__bis_SR_register(LPM3_bits | GIE); 	//enter low power mode
	}
}


void IR_Mode_Setting()
{
	code_num = button_num - 3;
	LCD_Clear();
	if(button_num == 2)							//button "copy" is pressed
	{
		copy_mode = 1;							//copy mode
		LCD_Display_letter(pos1, 2); //C
		LCD_Display_letter(pos2,14); //O
		LCD_Display_letter(pos3,15); //P
		LCD_Display_letter(pos4,24); //Y
	}
	else if(button_num == 1) 					//button "ok" is pressed
	{
		if(copy_mode == 1)
		{
			copy_mode = 0;						//emitting mode
		}
		LCD_Display_letter(pos1,14); //O
		LCD_Display_letter(pos2,10); //K
	}

	if(copy_mode == 1 && button_num > 2)
	{
		BlinkLED();
		FRAM_write_ptr = (unsigned int *)(0xE000 + ((code_num)<<9));	//set FRAM write address
		SYSCFG0 &= ~PFWP;
		rx_cnt[code_num] = 0;
		SYSCFG0 |= PFWP;
		IR_stop = 1;							//enable receiving flag
	}
	else if(copy_mode == 0 && button_num > 2)
	{
		if(rx_cnt[code_num] > 0)				//has valid IR code
		{
			LCD_Display_letter(pos1,18); //S
			LCD_Display_letter(pos2, 4); //E
			LCD_Display_letter(pos3,13); //N
			LCD_Display_letter(pos4, 3); //D
		}
		else									//no IR code
		{
			LCD_Display_letter(pos1,13); //N
			LCD_Display_letter(pos2,14); //O
			LCD_Display_letter(pos3,13); //N
			LCD_Display_letter(pos4, 4); //E
		}
		FRAM_read_ptr  = (unsigned int *)(0xE000 + ((code_num)<<9));	//set FRAM read address
		IR_stop = 0;							//enable emitting flag
	}
	else
	{
		if(copy_mode == 2)
		{
			LCD_Display_letter(pos1,11);//L
			LCD_Display_letter(pos2, 4);//E
			LCD_Display_letter(pos3, 0);//A
			LCD_Display_letter(pos4,17);//R
			LCD_Display_letter(pos5,13);//N
		}
		IR_stop = 2;							//disable IR
	}

}

//use blinking LED to indicate pressed button
void BlinkLED ()
{
	unsigned char i;
	for(i=0; i<8; i++)
	{
		P4OUT ^= BIT0;
		__delay_cycles(200000);
	}
	P4OUT &= ~BIT0;
}

//********Port1 interrupt ISR*********//
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	P1IFG = 0;									// clear IFG
	if (buttonDebounce == 1)
	{
		buttonDebounce = 2;
		Buttons_startWDT();
		button_num = scan_key();				// scan the keypad
		IR_Mode_Setting();
		LPM3_EXIT;								// exit low power mode
	}
}

//********Port2 interrupt ISR*********//
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	P2IFG = 0;									// clear IFG
	if (buttonDebounce == 1)
	{
		buttonDebounce = 2;
		Buttons_startWDT();
		button_num = scan_key();				// scan the keypad
		IR_Mode_Setting();
		LPM3_EXIT;								// exit low power mode
	}
}

//********Timer0 interrupt ISR*********//
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
	switch( TA0IV )
	{
	case  0:
	{
		if(tx_cnt < rx_cnt[code_num])
		{
			TA0CCTL2 ^= OUT;
			TA0CCR0 = *(FRAM_read_ptr+1);		//update emitting IR code
			*FRAM_read_ptr++;
			tx_cnt++;
		}
		else
		{
			IR_stop = 2;						//disable IR
			TA0CCTL0 &= ~CCIE;					// disable timer_A0 interrupt
			tx_cnt = 1;
		}
		break;
	}
	default: break;
	}
}

//********Timer1 interrupt ISR*********//
#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR (void)
{
	switch( TA0IV )
	{
	case  4:									//Interrupt Source: Capture 2
	{
		if(IR_stop == 1 && rx_cnt[code_num] < 255)
		{
			SYSCFG0 &= ~PFWP;
			rx_cnt[code_num]++;
			SYSCFG0 |= PFWP;
			old_cnt = new_cnt;					//Update the counter value
			new_cnt = TA0CCR2;
			time_cnt = new_cnt -old_cnt;		//Time interval
			SYSCFG0 &= ~PFWP;
			*FRAM_write_ptr = time_cnt;			//write FRAM to store data
			SYSCFG0 |= PFWP;
			FRAM_write_ptr++;
		}
		break;
	}
	default: break;
	}
}






