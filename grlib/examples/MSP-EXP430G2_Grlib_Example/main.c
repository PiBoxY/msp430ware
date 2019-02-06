/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
#include "radioButton.h"
#include "checkbox.h"
#include "LcdDriver/Sharp96x96.h"

// Function Prototypes
void clockInit(void);
void boardInit(void);
void timerInit(void);
void flashInit(void);
void timerInit(void);
void Delay_long(void);
void Delay_short(void);

// Global variables
Graphics_Context g_sContext;
const Graphics_Rectangle myRectangle1 = { 5, 15, 65, 45};
const Graphics_Rectangle myRectangle2 = { 10, 40, 90, 90};
const Graphics_Rectangle myRectangleFrame = { 10, 40, 90, 60};

#if defined(__IAR_SYSTEMS_ICC__)
int16_t __low_level_init(void) {
    // Stop WDT (Watch Dog Timer)
    WDTCTL = WDTPW + WDTHOLD;
    return(1);
}

#endif

void main(void)
{
    uint16_t gie;
    Graphics_Rectangle myRectangleProgress = {
        10,
        40,
        30,
        60
    };
    Graphics_RadioButton radioButton1 = {
        5,
        15,
        true,
        4,
        GRAPHICS_COLOR_BLACK,
        9,
        GRAPHICS_COLOR_BLACK,
        GRAPHICS_COLOR_WHITE,
        &g_sFontFixed6x8,
        "Option #1"
    };

    Graphics_RadioButton radioButton2 = {
        5,
        25,
        false,
        4,
        GRAPHICS_COLOR_BLACK,
        9,
        GRAPHICS_COLOR_BLACK,
        GRAPHICS_COLOR_WHITE,
        &g_sFontFixed6x8,
        "Option #2"
    };
    Graphics_CheckBox checkBox1 = {
        5,
        15,
        false,
        4,
        GRAPHICS_COLOR_BLACK,
        GRAPHICS_COLOR_WHITE,
        GRAPHICS_COLOR_BLACK,
        9,
        &g_sFontFixed6x8,
        "Option #1"
    };
    Graphics_CheckBox checkBox2 = {
        5,
        25,
        true,
        4,
        GRAPHICS_COLOR_BLACK,
        GRAPHICS_COLOR_WHITE,
        GRAPHICS_COLOR_BLACK,
        9,
        &g_sFontFixed6x8,
        "Option #2"
    };

    // Initialize the boards
    boardInit();
    clockInit();
    timerInit();
    flashInit();

    __bis_SR_register(GIE);

    // Set up the LCD
    Sharp96x96_initDisplay();
    Graphics_initContext(&g_sContext, &g_sharp96x96LCD);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    // Intro Screen
    Graphics_drawStringCentered(&g_sContext,
                                "How to use",
                                AUTO_STRING_LENGTH,
                                48,
                                15,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "the MSP",
                                AUTO_STRING_LENGTH,
                                48,
                                35,
                                TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,
                        "Graphics Library",
                        AUTO_STRING_LENGTH,
                        1,
                        51,
                        TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "Primitives",
                                AUTO_STRING_LENGTH,
                                48,
                                75,
                                TRANSPARENT_TEXT);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Draw pixels and lines on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Pixels",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);

    Graphics_drawStringCentered(&g_sContext,
                                "& Lines",
                                AUTO_STRING_LENGTH,
                                48,
                                15,
                                TRANSPARENT_TEXT);

    Graphics_drawPixel(&g_sContext, 30, 30);
    Graphics_drawPixel(&g_sContext, 30, 32);
    Graphics_drawPixel(&g_sContext, 32, 32);
    Graphics_drawPixel(&g_sContext, 32, 30);
    Graphics_drawLine(&g_sContext, 35, 35, 90, 90);
    Graphics_drawLine(&g_sContext, 5, 80, 80, 20);
    Graphics_drawLine(&g_sContext,
                      0,
                      Graphics_getDisplayHeight(&g_sContext) - 1,
                      Graphics_getDisplayWidth(&g_sContext) - 1,
                      Graphics_getDisplayHeight(&g_sContext) - 1);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Draw circles on the display
    Graphics_drawString(&g_sContext,
                        "Draw Circles",
                        AUTO_STRING_LENGTH,
                        10,
                        5,
                        TRANSPARENT_TEXT);
    Graphics_drawCircle(&g_sContext, 30, 70, 20);
    Graphics_fillCircle(&g_sContext, 60, 50, 30);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Draw rectangles on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Rectangles",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Combining Primitive screen
    Graphics_drawStringCentered(&g_sContext,
                                "Using",
                                AUTO_STRING_LENGTH,
                                48,
                                15,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "radioButtons",
                                AUTO_STRING_LENGTH,
                                48,
                                35,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "and checkBoxes",
                                AUTO_STRING_LENGTH,
                                48,
                                51,
                                TRANSPARENT_TEXT);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Draw a Menu screen
    Graphics_drawStringCentered(&g_sContext,
                                "Radio Buttons",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRadioButton(&g_sContext, &radioButton1);
    Graphics_drawRadioButton(&g_sContext, &radioButton2);

    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext,
                                "Check Boxes",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);

    Graphics_drawCheckBox(&g_sContext, &checkBox1);
    Graphics_drawCheckBox(&g_sContext, &checkBox2);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);
    Delay_long();
    Graphics_clearDisplay(&g_sContext);

    // Show progress bar screen
    // The following animation consist on displaying a progress bar and
    // updating the progress bar in increments of 25%.
    Graphics_drawStringCentered(&g_sContext,
                                "Show progress",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangleFrame);
    Graphics_drawStringCentered(&g_sContext,
                                "Processing...",
                                AUTO_STRING_LENGTH,
                                48,
                                75,
                                TRANSPARENT_TEXT);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_short();

    // Update display with 25 %. Initial value of "myRectangleProgress" are set
    // to update bar with a 25 % increment.
    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_short();

    // Set myRectangleProgress values to update progress bar with 50 %
    myRectangleProgress.sXMin = 30;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 50;
    myRectangleProgress.sYMax = 60;

    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_short();

    // Set myRectangleProgress values to update progress bar with 75 %
    myRectangleProgress.sXMin = 50;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 70;
    myRectangleProgress.sYMax = 60;

    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_short();

    // Set myRectangleProgress values to update progress bar with 100 %
    myRectangleProgress.sXMin = 70;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 90;
    myRectangleProgress.sYMax = 60;
    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);

    Graphics_drawStringCentered(&g_sContext,
                                "DONE!",
                                AUTO_STRING_LENGTH,
                                48,
                                85,
                                TRANSPARENT_TEXT);
    // Store current GIE state
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay_long();

    while(1)
    {
        ;
    }
}

void clockInit(void)
{
    // Set DCO frequency at 8MHz
    // ACLK	= VLO/1
    // SMCLK= DCO/1
    // MCLK	= DCO/1
    // If calibration constant erased
    if(CALBC1_8MHZ == 0xFF)
    {
        // do not load, trap CPU!!
        while(1)
        {
            ;
        }
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
    FCTL2 = FWKEY + FSSEL_1 + FN4 + FN0;
}

void timerInit()
{
    TA0CTL = TASSEL_2 + ID_3 + MC_1 + TAIE + TACLR;
    TA0CCTL0 |= CCIE;
    TA0CCR0 = 32000 - 1;
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
    TA0CCR0 += 32000 - 1;
    TA0CTL &= ~TAIFG;
}
