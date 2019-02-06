/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//! \file   FourDigitDisplay.c
//
//! \brief  HAL for seven segment display
//
//*****************************************************************************

//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************
#include <msp430.h>
#include <driverlib.h>
#include "FourDigitDisplay.h"
#include "Captivate.h"

//*****************************************************************************
// Definitions
//*****************************************************************************
#define DIGIT_DISPLAY_ACTIVE_TIME               (1000)

//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void DigitDisplayScan_Timer_Init(void);

//*****************************************************************************
// Definitions
//*****************************************************************************
#define NUM_OF_DIGITS					        (4)
#define MAX_SUPPORTED_CHARS_PER_DIGIT	        (25)

//*****************************************************************************
// Externals
//*****************************************************************************
extern void DisplayTimerEventHandler(void);


//*****************************************************************************
// Global Variables
//*****************************************************************************

uint8_t ui8Digit_Buffer[4] = {0, 1, 2, 3};
const uint16_t g_DigitLookup[MAX_SUPPORTED_CHARS_PER_DIGIT] =
{
	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),				// '0'
	(SEG_B | SEG_C), 												// '1'
	(SEG_A | SEG_B | SEG_D | SEG_E | SEG_G), 						// '2'
	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_G), 						// '3'
	(SEG_B | SEG_C | SEG_F | SEG_G), 								// '4'
	(SEG_A | SEG_C | SEG_D | SEG_F | SEG_G), 						// '5'
	(SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),				// '6'
	(SEG_A | SEG_B | SEG_C),										// '7'
	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),		// '8'
	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),				// '9'
	(SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),				// 'A'
	(SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),						// 'B'
	(SEG_A | SEG_D | SEG_E | SEG_F),								// 'C'
	(SEG_B | SEG_C | SEG_D | SEG_E | SEG_G),						// 'D'
	(SEG_A | SEG_D | SEG_E | SEG_F | SEG_G),						// 'E'
	(SEG_A | SEG_E | SEG_F | SEG_G),								// 'F'
	(SEG_D | SEG_E | SEG_F ),                                       // 'L'
	(SEG_B | SEG_C | SEG_D | SEG_E | SEG_G),                        // 'd'
	(SEG_P),                                                        // '.'
	0,                                                              // 'blank'
	(SEG_G),														// '-'
	(SEG_D),                                                        // 'one bar'
	(SEG_G | SEG_D),                                                // 'two bars'
	(SEG_A | SEG_G | SEG_D),                                        // 'three bars'
	(SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),        // 'all segments'

};


//*****************************************************************************
// Function Implementations
//*****************************************************************************
void FourDigitDisplay_init(void)
{
    FourDigitDisplay_writeNum(eBlank,eBlank,eBlank,eBlank);

    P1OUT  = (0x00);
    P1DIR |= (BIT0 | BIT6 | BIT7);

    P2OUT  =  (0x00);
    P2DIR  |=  (BIT2);


	//
    // Initialize IO Expander
    //
    TCA9534_Init(TCA5934_ADDRS);
    TCA9534_SetAsOutputs(TCA5934_ADDRS, TCA9534_ALL);

}

void FourDigitDisplay_writeNum(uint8_t ui8Num1, uint8_t ui8Num2, uint8_t ui8Num3, uint8_t ui8Num4)
{
	ui8Digit_Buffer[0] = ui8Num4;
	ui8Digit_Buffer[1] = ui8Num3;
	ui8Digit_Buffer[2] = ui8Num2;
	ui8Digit_Buffer[3] = ui8Num1;
}

void DigitDisplayUpdate(void)
{
    static uint8_t ui8Index = 0;

        if(++ui8Index > 3)
                ui8Index = 0;

        TCA9534_SetOutput(TCA5934_ADDRS, 0x00);

        switch (ui8Index)
        {
        case 0:
                P1OUT |= BIT0 + BIT6 + BIT7;
                P2OUT &= ~BIT2;

                break;
        case 1:
                P1OUT |= BIT0 + BIT7;
                P2OUT |= BIT2;
                P1OUT &= ~BIT6;
                break;
        case 2:
                P1OUT |= BIT0 + BIT6;
                P2OUT |= BIT2;
                P1OUT &= ~BIT7;
                break;
        case 3:
                P1OUT |= BIT6 + BIT7;
                P2OUT |= BIT2;
                P1OUT &= ~BIT0;
                break;
        }

        TCA9534_SetOutput(TCA5934_ADDRS, g_DigitLookup[ui8Digit_Buffer[ui8Index]]);
}

void DigitDisplayScan_Timer_Init(void)
{
    TA0CCTL0 |= CCIE;                            // TACCR0 interrupt enabled
    TA0CCR0 = 200;								 // ~40Hz
    TA0CTL = TASSEL__ACLK;              		 // ACLK
}

void DigitDisplayScan_Timer_Start(void)
{
	TA0CTL |= MC__UP;							 // UP mode
}

void DigitDisplayScan_Timer_Stop(void)
{
	TA0CTL &= ~MC__UPDOWN;
}


