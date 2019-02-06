#include "msp430fr6989.h"

const unsigned char lcd_num[10] = {
	0xFC,		// 0
	0x60,		// 1
	0xDB,		// 2
	0xF3,		// 3
	0x67,		// 4
	0xB7,		// 5
	0xBF,		// 6
	0xE0,		// 7
	0xFF,		// 8
	0xF7,		// 9
};

const unsigned char lcd_small_num[10] = {
	0xCF,		// 0
	0x06,		// 1
	0xAD,		// 2
	0x2F,		// 3
	0x66,		// 4
	0x6B,		// 5
	0xEB,		// 6
	0x0E,		// 7
	0xEF,		// 8
	0x6F,		// 9
};

void init_LCD(void)
{
	// LCD_C
#if 0		// all segments enable for the LCD
	LCDCPCTL0	= 0xffff;		// Enable LCD S0-S15
	LCDCPCTL1	= 0xfc3f;		// Enable LCD S16-21, S26-S31
	LCDCPCTL2	= 0x0fff;		// Enable LCD S32-43
#endif
#if 1		// only the segments for small digits are enabled
	LCDCPCTL0	= 0x0000;
	LCDCPCTL1	= 0xC000;
	LCDCPCTL2	= 0x003F;
#endif
	
	LCDCMEMCTL	= LCDCLRM;											// Clear LCD memory
	LCDCVCTL	= VLCD_1 + LCDCPEN;									// Use charge pump
	LCDCCPCTL	= LCDCPCLKSYNC;										// Synchronize charge pump with internal clock
	
	// flcd = 32768/((1+1)*2^4) = 1024
	// flcd = 2 * MUX * Frame, flcd = 1000, frame = 128fps
	LCDCCTL0	= LCDDIV_1 + LCDPRE_4 + LCD4MUX + LCDLP + LCDON;	// 4 MUX, Low power waveform, use ACLK, turn on LCD
}

void lcd_display_num(unsigned int num, unsigned char small)
{
	unsigned char thousand = 0, hundred = 0, ten = 0;
	const unsigned char *disp_num;
	volatile unsigned char *disp_mem_thousand, *disp_mem_hundred, *disp_mem_ten, *disp_mem_num;
	if (small)
	{
		disp_num			= lcd_small_num;
		disp_mem_thousand	= &LCDM19;
		disp_mem_hundred	= &LCDM18;
		disp_mem_ten		= &LCDM17;
		disp_mem_num		= &LCDM16;
	}
	else
	{
		disp_num			= lcd_num;
		disp_mem_thousand	= &LCDM5;
		disp_mem_hundred	= &LCDM7;
		disp_mem_ten		= &LCDM9;
		disp_mem_num		= &LCDM11;
	}
	
	while (num >= 1000)
	{
		num -= 1000;
		thousand++;
	}
	while (num >= 100)
	{
		num -= 100;
		hundred++;
	}
	while (num >= 10)
	{
		num -= 10;
		ten++;
	}

	*disp_mem_num = disp_num[num];

	if (ten || hundred || thousand)		*disp_mem_ten = disp_num[ten];
	else								*disp_mem_ten = 0x00;

	if (hundred || thousand)		*disp_mem_hundred = disp_num[hundred];
	else							*disp_mem_hundred = 0x00;

	if (thousand)					*disp_mem_thousand = disp_num[thousand];
	else							*disp_mem_thousand = 0x00;
}
