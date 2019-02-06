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
 * HAL_FR4133LP_LCD.c
 *
 *  Created on: Jul 30, 2014
 *      Author: a0222631
 */
#include "msp430fr4133.h"
#include "HAL_FR4133LP_LCD.h"

//LCD digit display table
const char digit[10] =
{
		0xFC,  /* "0" */
		0x60,  /* "1" */
		0xDB,  /* "2" */
		0xF3,  /* "3" */
		0x67,  /* "4" */
		0xB7,  /* "5" */
		0xBF,  /* "6" */
		0xE4,  /* "7" */
		0xFF,  /* "8" */
		0xF7   /* "9" */
};

//LCD alphabet display table
const char alphabet[28][2] =
{
		{0xEF, 0x00},  /* "A" */ //0
		{0xF1, 0x50},  /* "B" */ //1
		{0x9C, 0x00},  /* "C" */ //2
		{0xF0, 0x50},  /* "D" */ //3
		{0x9F, 0x00},  /* "E" */ //4
		{0x8F, 0x00},  /* "F" */ //5
		{0xBD, 0x00},  /* "G" */ //6
		{0x6F, 0x00},  /* "H" */ //7
		{0x90, 0x50},  /* "I" */ //8
		{0x78, 0x00},  /* "J" */ //9
		{0x0E, 0x22},  /* "K" */ //10
		{0x1C, 0x00},  /* "L" */ //11
		{0x6C, 0xA0},  /* "M" */ //12
		{0x6C, 0x82},  /* "N" */ //13
		{0xFC, 0x00},  /* "O" */ //14
		{0xCF, 0x00},  /* "P" */ //15
		{0xFC, 0x02},  /* "Q" */ //16
		{0xCF, 0x02},  /* "R" */ //17
		{0xB7, 0x00},  /* "S" */ //18
		{0x80, 0x50},  /* "T" */ //19
		{0x7C, 0x00},  /* "U" */ //20
		{0x0C, 0x28},  /* "V" */ //21
		{0x6C, 0x0A},  /* "W" */ //22
		{0x00, 0xAA},  /* "X" */ //23
		{0x00, 0xB0},  /* "Y" */ //24
		{0x90, 0x28},  /* "Z" */ //25
		{0x03, 0x50},  /* "+" */ //26
		{0x03, 0x00}   /* "-" */ //27
};

// Initialize LCD
void Init_LCD()
{
	// Configure LCD pins
	SYSCFG2 |= LCDPCTL;                              // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

	LCDPCTL0 = 0xFFFF;
	LCDPCTL1 = 0x07FF;
	LCDPCTL2 = 0x00F0;                               // L0~L26 & L36~L39 pins selected

	LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                  // flcd ref freq is xtclk

	//LCD Operation - Mode 2, internal 3.08v, charge pump 256Hz
	LCDVCTL = LCDCPEN | LCDSELVDD | VLCD_8 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);
	/*
	// LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
	LCDVCTL = LCDCPEN | LCDREFEN | VLCD_8 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);
	 */
	LCDMEMCTL |= LCDCLRM;                             // Clear LCD memory

	LCDCSSEL0 = 0x000F;                               // Configure COMs and SEGs
	LCDCSSEL1 = 0x0000;                               // L0, L1, L2, L3: COM pins
	LCDCSSEL2 = 0x0000;

	LCDM0 = 0x21;                                     // L0 = COM0, L1 = COM1
	LCDM1 = 0x84;                                     // L2 = COM2, L3 = COM3

	LCDCTL0 |= LCD4MUX | LCDON;                       // Turn on LCD, 4-mux selected

}

// Clear all the LCD display
void LCD_Clear()
{
	unsigned char i;
	for(i=2; i<20; i++)
		LCDMEM[i] = 0x00;
}

// LCD digit display function
void LCD_Display_digit(unsigned char pos, unsigned char ch)
{
	LCDMEM[pos] = digit[ch];
}

// LCD letter display function
void LCD_Display_letter(unsigned char pos, unsigned char ch)
{
	LCDMEM[pos]   = alphabet[ch][0];
	LCDMEM[pos+1] = alphabet[ch][1];
}

// Display "TX"
void LCD_Display_TX()
{
	LCDMEM[9]  |= 0x04;
	LCDMEM[19] |= 0x04;
}

// Display "RX"
void LCD_Display_RX()
{
	LCDMEM[9]  |= 0x04;
	LCDMEM[19] |= 0x01;
}

// Display "MSP--IR"
void LCD_Display_MSP_IR()
{
	LCD_Display_letter(pos1,12);//M
	LCD_Display_letter(pos2,18);//S
	LCD_Display_letter(pos3,15);//P
	LCD_Display_letter(pos4,27);//--
	LCD_Display_letter(pos5,8); //I
	LCD_Display_letter(pos6,17);//R
}

// Display Button
void LCD_Display_Buttons(unsigned char btn)
{
	switch(btn)		//Display button
	{
	case 1:
		LCD_Display_letter(pos1,14); //O
		LCD_Display_letter(pos2,10); //K
		break;
	case 2:
		LCD_Display_letter(pos1, 2); //C
		LCD_Display_letter(pos2,14); //O
		LCD_Display_letter(pos3,15); //P
		LCD_Display_letter(pos4,24); //Y
		break;
	case 3:
		LCD_Display_letter(pos1,19); //T
		LCD_Display_letter(pos2, 4); //E
		LCD_Display_letter(pos3,12); //M
		LCD_Display_letter(pos4,15); //P
		LCD_Display_letter(pos5,27); //-
		break;
	case 4:
		LCD_Display_letter(pos1,19); //T
		LCD_Display_letter(pos2, 4); //E
		LCD_Display_letter(pos3,12); //M
		LCD_Display_letter(pos4,15); //P
		LCD_Display_letter(pos5,26); //+
		break;
	case 5:
		LCD_Display_letter(pos1, 2); //C
		LCD_Display_letter(pos2,14); //O
		LCD_Display_letter(pos3,14); //O
		LCD_Display_letter(pos4,11); //L
		break;
	case 6:
		LCD_Display_digit(pos1,9);//9
		break;
	case 7:
		LCD_Display_digit(pos1,6);//6
		break;
	case 8:
		LCD_Display_digit(pos1,3);//3
		break;
	case 9:
		LCD_Display_digit(pos1,0);//0
		break;
	case 10:
		LCD_Display_digit(pos1,8);//8
		break;
	case 11:
		LCD_Display_digit(pos1,5);//5
		break;
	case 12:
		LCD_Display_digit(pos1,2);//2
		break;
	case 13:
		LCD_Display_letter(pos1,15); //P
		LCD_Display_letter(pos2,14); //O
		LCD_Display_letter(pos3,22); //W
		LCD_Display_letter(pos4, 4); //E
		LCD_Display_letter(pos5,17); //R
		break;
	case 14:
		LCD_Display_digit(pos1,7);//7
		break;
	case 15:
		LCD_Display_digit(pos1,4);//4
		break;
	case 16:
		LCD_Display_digit(pos1,1);//1
		break;
	default:
		LCD_Display_letter(pos1,12);//M
		LCD_Display_letter(pos2,18);//S
		LCD_Display_letter(pos3,15);//P
		LCD_Display_letter(pos4,27);//--
		LCD_Display_letter(pos5,8); //I
		LCD_Display_letter(pos6,17);//R
		break;
	}
}



