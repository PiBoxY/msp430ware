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

#include "msp430g2553.h"
#include "grlib.h"
#include "Sharp96x96.h"
#include "HAL_MSP-EXP430G2_Sharp96x96.h"


// Function Prototypes
void clockInit(void);
void boardInit(void);
void timerInit(void);
void flashInit(void);
void timerInit(void);
void Delay_long(void);
void Delay_short(void);


// Global variables
tContext g_sContext;
tRectangle g_sRect;
const tRectangle myRectangle1 = { 5, 15, 65, 45};
const tRectangle myRectangle2 = { 10, 40, 90, 90};
const tRectangle myRectangleOption1 = { 5, 15, 11, 21};
const tRectangle myRectangleOption2 = { 5, 25, 11, 31};
const tRectangle myRectangleOption3 = { 5, 35, 11, 41};
const tRectangle myRectangleOption4 = { 5, 45, 11, 51};
const tRectangle myRectangleOption5 = { 5, 55, 11, 61};
const tRectangle myRectangleFrame = { 10, 40, 90, 60};
tRectangle myRectangleProgress = { 10, 40, 30, 60};


void main(void)
{
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;

    // Initialize the boards
    boardInit();
    clockInit();
    timerInit();
    flashInit();

    __bis_SR_register(GIE);

    // Set up the LCD
    LCDInit();
    GrContextInit(&g_sContext, &g_sharp96x96LCD);
    GrContextForegroundSet(&g_sContext, ClrBlack);
    GrContextBackgroundSet(&g_sContext, ClrWhite);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    GrClearDisplay(&g_sContext);
    GrFlush(&g_sContext);

    // Intro Screen
    GrStringDrawCentered(&g_sContext, 
                         "How to use", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         15, 
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "the MSP430", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         35, 
                         TRANSPARENT_TEXT);
    GrStringDraw(&g_sContext, 
                 "Graphics Library", 
                 AUTO_STRING_LENGTH, 
                 1, 
                 51, 
                 TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "Primitives", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         75, 
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Draw pixels and lines on the display
    GrStringDrawCentered(&g_sContext, 
                         "Draw Pixels", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         5, 
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "& Lines", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         15, 
                         TRANSPARENT_TEXT);
    GrPixelDraw(&g_sContext, 30, 30);
    GrPixelDraw(&g_sContext, 30, 32);
    GrPixelDraw(&g_sContext, 32, 32);
    GrPixelDraw(&g_sContext, 32, 30);
    GrLineDraw(&g_sContext, 35, 35, 90, 90);
    GrLineDraw(&g_sContext, 5, 80, 80, 20);
    GrLineDraw(&g_sContext, 
               0, 
               GrContextDpyHeightGet(&g_sContext) - 1, 
               GrContextDpyWidthGet(&g_sContext) - 1, 
               GrContextDpyHeightGet(&g_sContext) - 1);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Draw circles on the display
    GrStringDraw(&g_sContext, 
                 "Draw Circles", 
                 AUTO_STRING_LENGTH, 
                 10, 
                 5, 
                 TRANSPARENT_TEXT);
    GrCircleDraw(&g_sContext, 30, 70, 20);
    GrCircleFill(&g_sContext, 60, 50, 30);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Draw rectangles on the display
    GrStringDrawCentered(&g_sContext, 
                         "Draw Rectangles", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         5, 
                         TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangle1);
    GrRectFill(&g_sContext, &myRectangle2);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Combining Primitive screen
    GrStringDrawCentered(&g_sContext, 
                         "Combining", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         15, 
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "Primitives to", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         35, 
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "create menus", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         51, 
                         TRANSPARENT_TEXT);
    GrStringDrawCentered(&g_sContext, 
                         "and animations", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         75, 
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Draw a Menu screen
    GrStringDrawCentered(&g_sContext, 
                         "Create a Menu", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         5, 
                         TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangleOption1);
    GrStringDraw(&g_sContext,"Option #1", 10,15,15,TRANSPARENT_TEXT);
    GrRectFill(&g_sContext, &myRectangleOption2);
    GrStringDraw(&g_sContext,"Option #2", 10,15,25,TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangleOption3);
    GrStringDraw(&g_sContext,"Option #3", 10,15,35,TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangleOption4);
    GrStringDraw(&g_sContext,"Option #4", 10,15,45,TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangleOption5);
    GrStringDraw(&g_sContext,"Option #5", 10,15,55,TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay_long();
    GrClearDisplay(&g_sContext);

    // Show progress bar screen
    // The following animation consist on displaying a progress bar and 
    // updating the progress bar in increments of 25%.
    GrStringDrawCentered(&g_sContext, 
                         "Show progress", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         5, 
                         TRANSPARENT_TEXT);
    GrRectDraw(&g_sContext, &myRectangleFrame);
    GrStringDrawCentered(&g_sContext,
                         "Processing...", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         75, 
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay_short();

    // Update display with 25 %. Initial value of "myRectangleProgress" are set 
    // to update bar with a 25 % increment.
    GrRectFill(&g_sContext, &myRectangleProgress);
    GrFlush(&g_sContext);
    Delay_short();

    // Set myRectangleProgress values to update progress bar with 50 %
    myRectangleProgress.sXMin = 30;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 50;
    myRectangleProgress.sYMax = 60;

    GrRectFill(&g_sContext, &myRectangleProgress);
    GrFlush(&g_sContext);
    Delay_short();

    // Set myRectangleProgress values to update progress bar with 75 %
    myRectangleProgress.sXMin = 50;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 70;
    myRectangleProgress.sYMax = 60;

    GrRectFill(&g_sContext, &myRectangleProgress);
    GrFlush(&g_sContext);
    Delay_short();

    // Set myRectangleProgress values to update progress bar with 100 %
    myRectangleProgress.sXMin = 70;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 90;
    myRectangleProgress.sYMax = 60;
    GrRectFill(&g_sContext, &myRectangleProgress);

    GrStringDrawCentered(&g_sContext,
                         "DONE!", 
                         AUTO_STRING_LENGTH, 
                         48, 
                         85, 
                         TRANSPARENT_TEXT);
    GrFlush(&g_sContext);
    Delay_long();



    while(1);

}

void clockInit(void)
{
    // Set DCO frequency at 8MHz
    // ACLK	= VLO/1
    // SMCLK= DCO/1
    // MCLK	= DCO/1
    // If calibration constant erased
    if (CALBC1_8MHZ==0xFF)
    {
        // do not load, trap CPU!!
       while(1);
    }
    DCOCTL = 0;
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;
    BCSCTL3 |= LFXT1S_2;
}

void boardInit()
{
    // Set all port as outputs and drive pins low
    P1OUT = 0;
    P2OUT = 0;
    P1DIR = 0xFF;
    P2DIR = 0xFF;
}

void flashInit()
{
    // Configure flash memory timing
    // MCLK/18 for Flash Timing Generator
    // 8MHz/(16+1+1) =  444.44 KHz
    FCTL2 = FWKEY + FSSEL0 + FN4 + FN0;

}

void timerInit()
{
	TA0CTL = TASSEL_2 +ID_3+ MC_1+ TAIE +TACLR;
	TA0CCTL0 |= CCIE;
	TA0CCR0 = 32000-1;
}


void Delay_long()
{
    __delay_cycles(SYSTEM_CLOCK_SPEED * 4);
}


void Delay_short()
{
    __delay_cycles(SYSTEM_CLOCK_SPEED * 0.25);
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	Sharp96x96_SendToggleVCOMCommand();
	TA0CCR0 += 32000-1;
	TA0CTL &= ~TAIFG;

}
