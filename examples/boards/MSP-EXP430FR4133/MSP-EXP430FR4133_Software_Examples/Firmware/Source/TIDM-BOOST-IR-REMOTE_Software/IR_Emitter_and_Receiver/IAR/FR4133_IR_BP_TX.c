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
 *  FR4133_IR_BP_TX.c
 *     - MSP430 firmware application to demonstrate the IR Emitter
 *       capability of the MSP430FR4133 interfacing with the IR
 *       BoosterPack.
 *
 ******************************************************************************/

/******************************************************************************
 *         MSP430FR4133-LaunchPad IR BoosterPack Emitter User Experience
 *
 * This application operates on the LaunchPad platform using the MSP430FR4133
 * device and the IR BoosterPack plugin board. IR modulator inside MSP430FR4133
 * is used to control IR diode on the IR BoosterPack to generate the IR emitting
 * signal.
 *
 * Two Timers are used,one is used to produce the IR code and the other one is
 * used to produce 38khz carrier wave. Both these two timers are driven by 4MHz
 * SMCLK. There is a 4x4 keypad on the IR BoosterPack. If the button is pressed,
 * the letters on the button will be displayed on the LCD. One LED is used to
 * indicate the IR transmitting on the IR BoosterPack.
 *
 * Texas Instruments, Inc.
 * Ver 0.2 Aug. 2014
 ******************************************************************************/

#include "msp430.h"
#include "HAL_FR4133LP_LCD.h"
#include "HAL_FR4133LP_Board.h"


unsigned char	IR_code;
unsigned char 	IR_flag;
unsigned char 	IR_stop;
unsigned char 	byte_cnt;
unsigned int	bit_sel;
unsigned char	*send_addr;
unsigned char 	send_data[4]={0x55, 0xaa, 0x00, 0xff};


int main( void )
{
	// Watchdog timer works as default setting
	WDTCTL = WDTPW + WDTHOLD;

	Init_GPIO();								//Initialize GPIO
	Init_Clock();								//Initialize Clock
	Init_keypadIO();							//Initialize Keypad
	Init_LCD();									//Initialize LCD

	LCD_Display_MSP_IR();						// Display "MSP--IR"
	LCD_Display_TX();							// Display "TX"

	IR_stop = 1; 								//disable IR emitter

	while(1)
	{
		// clear the flag and counter
		IR_flag  = 0;
		byte_cnt = 0;
		bit_sel = 0;

		if(IR_stop == 0)
		{
			// Configure IR output pin
			P1SEL0|= BIT0;						// use internal IR modulator

			// disable Port1 & Port2 interrupt during IR emitting
			P1IE = 0;
			P2IE = 0;

			IR_code = scan_key();				// scan the keypad

			// Configure IR modulation: ASK
			SYSCFG1 = IRDSSEL + IREN;
			TA1CCTL0 = CCIE;
			TA1CCTL2 = OUTMOD_7;				// output mode: reset/set
			TA0CCTL2 = OUTMOD_7;        		// output mode: reset/set

			// 38kHz 1/4 duty-cycle carrier waveform length setting
			TA0CCR0 = 104;
			TA0CCR2 = 25;
			TA1CCR0 = 640;   					//the initial time of TA0 should be longer than TA1
			TA1CCR2 = 320;

			// set timer operation mode
			TA0CTL = TASSEL_2 + MC_1 + TACLR;   //SMCLK, UP mode
			TA1CTL = TASSEL_2 + MC_1 + TACLR;	//SMCLK, UP mode

			// write button number into buffer
			send_data[2] = IR_code;
			send_data[3] = ~IR_code;
			send_addr = &send_data[0];

			// stop until the end of IR code
			while(IR_stop == 0);

			TA0CCTL0 = 0;
			TA0CCTL2 = 0;
			TA0CTL = 0;
			TA0CCR0 = 0;
			TA0CCR2 = 0;						//disable timer0

			TA1CCTL0 = 0;
			TA1CCTL2 = 0;
			TA1CTL = 0;
			TA1CCR0 = 0;
			TA1CCR2 = 0;						//disable timer1

			P1IE |= (BIT3 + BIT4 + BIT5);		//enable GPIO interrupt
			P2IE |= BIT7;
		}
		__bis_SR_register(LPM3_bits | GIE); 	//enter low power mode
		IR_stop = 0;							// enable IR code emitting
	}
}


//********Timer1 interrupt ISR*********//
#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
	switch( TA1IV )
	{
	case  0:
	{
		if(IR_flag)
		{
			IR_stop=1;								// stop IR modulator
			TA1CCTL0 &= ~CCIE;						// disable timer_A0 interrupt
		}

		if((byte_cnt==0)&& (bit_sel==0)) 			// leading pulse burst of IR code
		{
			TA1CCR0 = 53999;                        // 9ms high
			TA1CCR2 = 35999;                        // 4.5ms low
			byte_cnt++;						   		// byte counter
			bit_sel++;						   		// bit counter
		}
		else if((byte_cnt>= 5)&& (bit_sel==1)) 		// the end of IR code
		{
			TA1CCR0 = 2249;  						//0.562ms pulse burst to show the end
			TA1CCR2 = 2249;
			IR_flag=1;
		}
		else
		{
			if((*send_addr & bit_sel) == 0)			// data "0"
			{
				TA1CCR0 = 4499;   					// 0.562ms high  0.562ms low
				TA1CCR2 = 2249;
				bit_sel<<=1;
			}
			else									// data "1"
			{
				TA1CCR0 = 8999;   					// 0.562ms high  1.687ms low
				TA1CCR2 = 2249;
				bit_sel<<=1;
			}
			if(bit_sel>=256)						// start a new byte
			{
				send_addr++;
				byte_cnt++;
				bit_sel=1;
			}
		}
		break;
	}
	default: break;
	}
}
