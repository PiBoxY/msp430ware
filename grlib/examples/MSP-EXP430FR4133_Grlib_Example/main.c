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
#include "grlib.h"
#include "checkbox.h"
#include "radioButton.h"
#include "LcdDriver/Sharp96x96.h"
#include "driverlib.h"
#include "stdint.h"
#include "stdbool.h"

void clockInit(void);
void boardInit();
void timerInit();
void Delay(void);

// Global variables
Graphics_Context g_sContext;
const Graphics_Rectangle myRectangle1 = { 5, 15, 65, 45};
const Graphics_Rectangle myRectangle2 = { 10, 40, 90, 90};
const Graphics_Rectangle myRectangleFrame = { 10, 40, 90, 60};
uint8_t g_delayCounter = 0;

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

    boardInit();
    clockInit();
    timerInit();

    // Set up the LCD
    Sharp96x96_initDisplay();

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */

    PMM_unlockLPM5();

    __enable_interrupt();

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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);
    Delay();
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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
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

    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
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

    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext,
                                "Check Boxes",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);

    Graphics_drawCheckBox(&g_sContext, &checkBox1);
    Graphics_drawCheckBox(&g_sContext, &checkBox2);
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();
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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();

    // Update display with 25 %. Initial value of "myRectangleProgress" are set
    // to update bar with a 25 % increment.
    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();

    // Set myRectangleProgress values to update progress bar with 50 %
    myRectangleProgress.sXMin = 30;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 50;
    myRectangleProgress.sYMax = 60;

    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();

    // Set myRectangleProgress values to update progress bar with 75 %
    myRectangleProgress.sXMin = 50;
    myRectangleProgress.sYMin = 40;
    myRectangleProgress.sXMax = 70;
    myRectangleProgress.sYMax = 60;

    Graphics_fillRectangle(&g_sContext, &myRectangleProgress);
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();

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
    gie = __get_SR_register() & GIE;
    __disable_interrupt();
    // Flush Buffer to LCD
    Graphics_flushBuffer(&g_sContext);
    // Restore original GIE state
    __bis_SR_register(gie);

    Delay();

    __bis_SR_register(LPM0_bits + GIE);
}

void clockInit(void)
{
    //Set DCO FLL reference = REFO
    CS_initClockSignal(
        CS_FLLREF,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Set ACLK = REFO
    CS_initClockSignal(
        CS_ACLK,
        CS_REFOCLK_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    CS_initFLLSettle(
        12000,
        366
        );

    //Clear all OSC fault flag
    CS_clearAllOscFlagsWithTimeout(1000);
}

void boardInit()
{
    GPIO_setAsOutputPin(GPIO_PORT_PA,
                        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setAsOutputPin(GPIO_PORT_PB,
                        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setOutputLowOnPin(GPIO_PORT_PA,
                           GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                           GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                           GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                           GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);

    GPIO_setOutputLowOnPin(GPIO_PORT_PB,
                           GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
                           GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
                           GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
                           GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15);
}

void timerInit()
{
    Timer_A_initUpModeParam param =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        65535 - 1,
        TIMER_A_TAIE_INTERRUPT_ENABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
        TIMER_A_SKIP_CLEAR,
        false
    };

    Timer_A_initUpMode(TIMER_A1_BASE,&param);

    Timer_A_initCompareModeParam initComp1Param =
    {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_OUTBITVALUE,
        62500
    };

    Timer_A_initCompareMode(TIMER_A1_BASE,&initComp1Param);

    Timer_A_startCounter(TIMER_A1_BASE,
                         TIMER_A_UP_MODE
                         );
}

void Delay(void)
{
    //Reset delayCounter
    g_delayCounter = 0;

    Timer_A_enableCaptureCompareInterrupt(TIMER_A1_BASE,
                                          TIMER_A_CAPTURECOMPARE_REGISTER_1);

    Timer_A_clear(TIMER_A1_BASE);

    // Enter Low Power Mode 0 with Global Interrupts Enabled
    __bis_SR_register(LPM0_bits + GIE);

    Timer_A_disableCaptureCompareInterrupt(TIMER_A1_BASE,
                                           TIMER_A_CAPTURECOMPARE_REGISTER_1);
}

//------------------------------------------------------------------------------
// Timer1 A0 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    Sharp96x96_SendToggleVCOMCommand();

    // Add Offset to TACCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            Timer_A_getCaptureCompareCount(
                                TIMER_A1_BASE,
                                TIMER_A_CAPTURECOMPARE_REGISTER_0) +
                            (65535 - 1));

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_0);

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
}

//------------------------------------------------------------------------------
// Timer1 A1 Interrupt Service Routine. This ISR id use to generate delay on
// LCD before changing screen content. The ISR is triggered every 500 msec.
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
    g_delayCounter++;

    // Add Offset to TACCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_1,
                            Timer_A_getCaptureCompareCount(
                                TIMER_A1_BASE,
                                TIMER_A_CAPTURECOMPARE_REGISTER_1) + (62500));

    // If 5 seconds have elapsed, exit LPM0 mode
    if(g_delayCounter == 10)
    {
        __bic_SR_register_on_exit(LPM0_bits);
    }

    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,
                                         TIMER_A_CAPTURECOMPARE_REGISTER_1);

    Timer_A_clearTimerInterrupt(TIMER_A1_BASE);
}
