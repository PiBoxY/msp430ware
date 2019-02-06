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
#include "radioButton.h"
#include "checkbox.h"
#include "LcdDriver/Hitachi138x110_HD66753.h"
#include "LcdDriver/HAL_MSP_EXP430F5438_HITACHI138x110_HD66753.h"
#include "images/images.h"
#include "driverlib.h"

Graphics_Context g_sContext;
Graphics_Rectangle g_sRect;
int32_t stringWidth = 0;

void boardInit(void);
void clockInit();
void Delay(void);

const Graphics_Rectangle myRectangle1 = { 5, 30, 80, 80};
const Graphics_Rectangle myRectangle2 = { 40, 50, 130, 100};
const Graphics_Rectangle myRectangle3 = { 0, 0, 138, 110};

/**********************************************************************//**
 * @brief  This is the example code's main function.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void main(void)
{
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

    // Basic GPIO Initialization
    boardInit();
    clockInit();

    // Set up LCD
    Hitachi138x110_HD66753_initDisplay();
    Graphics_initContext(&g_sContext, &g_sHitachi138x110_HD66753);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_setFont(&g_sContext, &g_sFontCmss12b);
    Graphics_clearDisplay(&g_sContext);

    //**********************************************
    //***************** Note ***********************
    //**********************************************
    // This LCD display has adjustable contrast.
    // The default contrast setting likely needs
    // adjustment for optimal performance.
    //**********************************************
    Hitachi138x110_HD66753_setContrast(110);
    Hitachi138x110_HD66753_setBacklight(8);

    // Intro Screen
    Graphics_drawStringCentered(&g_sContext,
                                "How to use MSP",
                                AUTO_STRING_LENGTH,
                                69,
                                27,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "Graphics Library",
                                AUTO_STRING_LENGTH,
                                69,
                                54,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "Primitives",
                                AUTO_STRING_LENGTH,
                                69,
                                81,
                                TRANSPARENT_TEXT);
    Delay();

    Graphics_clearDisplay(&g_sContext);

    // Draw pixels and lines on the display
    Graphics_drawString(&g_sContext,
                        "Draw Pixels & Lines",
                        AUTO_STRING_LENGTH,
                        10,
                        2,
                        TRANSPARENT_TEXT);
    Graphics_drawPixel(&g_sContext, 20, 20);
    Graphics_drawPixel(&g_sContext, 20, 22);
    Graphics_drawPixel(&g_sContext, 22, 22);
    Graphics_drawPixel(&g_sContext, 22, 20);
    Graphics_drawLine(&g_sContext, 25, 25, 100, 100);
    Graphics_drawLine(&g_sContext, 15, 100, 130, 30);
    Graphics_drawLine(&g_sContext,
                      0,
                      Graphics_getDisplayHeight(&g_sContext) - 1,
                      Graphics_getDisplayWidth(&g_sContext) - 1,
                      Graphics_getDisplayHeight(&g_sContext) - 1);
    Delay();
    Graphics_clearDisplay(&g_sContext);

    // Draw circles on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Circles",
                                AUTO_STRING_LENGTH,
                                69,
                                10,
                                TRANSPARENT_TEXT);
    Graphics_drawCircle(&g_sContext, 35, 90, 15);
    Graphics_fillCircle(&g_sContext, 90, 65, 40);
    Delay();
    Graphics_clearDisplay(&g_sContext);

    // Draw rectangles on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Rectangles",
                                AUTO_STRING_LENGTH,
                                69,
                                10,
                                TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    // Text won't be visible on screen due to transparency
    Graphics_drawStringCentered(&g_sContext,
                                "Normal Text",
                                AUTO_STRING_LENGTH,
                                85,
                                60,
                                TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext,
                                "Opaque Text",
                                AUTO_STRING_LENGTH,
                                85,
                                75,
                                OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, ClrWhite);
    Graphics_setBackgroundColor(&g_sContext, ClrBlack);
    // Text draws with inverted color to become visible
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Text",
                                AUTO_STRING_LENGTH,
                                85,
                                90,
                                TRANSPARENT_TEXT);
    Delay();

    // Invert the foreground and background colors
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_fillRectangle(&g_sContext, &myRectangle3);
    Graphics_setForegroundColor(&g_sContext, ClrWhite);
    Graphics_setBackgroundColor(&g_sContext, ClrBlack);
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Colors",
                                AUTO_STRING_LENGTH,
                                69,
                                10,
                                TRANSPARENT_TEXT);
    // Text won't be visible on screen due to transparency
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    Graphics_drawStringCentered(&g_sContext,
                                "Normal Text",
                                AUTO_STRING_LENGTH,
                                85,
                                60,
                                TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext,
                                "Opaque Text",
                                AUTO_STRING_LENGTH,
                                85,
                                75,
                                OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    // Text draws with inverted color to become visible
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Text",
                                AUTO_STRING_LENGTH,
                                85,
                                90,
                                TRANSPARENT_TEXT);
    Delay();
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    //**************************************************************************
    // This is an alternative method to invert colors using built in feature of
    // LCD display.
    // The above method illustrates how to use the graphics primitives for color
    // inversion
    //**************************************************************************
    //Hitachi138x110_InvertDisplay();
    //Delay();
    //Hitachi138x110_InvertDisplay();	// Change display back to normal
    //**************************************************************************
    Graphics_clearDisplay(&g_sContext);

    // Draw Images on the display
    Graphics_drawImage(&g_sContext, &LPRocket_130x50_1BPP_UNCOMP, 4, 30);
    Delay();
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawImage(&g_sContext, &TI_Logo_107x100_1BPP_UNCOMP, 15, 5);
    Delay();
    Graphics_clearDisplay(&g_sContext);

    // Draw Radio Button and Check Boxes
    Graphics_drawStringCentered(&g_sContext,
                                "Radio Buttons",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRadioButton(&g_sContext, &radioButton1);
    Graphics_drawRadioButton(&g_sContext, &radioButton2);

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
    Delay();
    Graphics_clearDisplay(&g_sContext);

    while(1)
    {
    }
}

void boardInit(void)
{
    //Tie unused ports
    GPIO_setAsOutputPin(
        GPIO_PORT_PA,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PA,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_PB,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PB,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_PC,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PC,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_PD,
        GPIO_PIN2 + GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
        GPIO_PIN6 + GPIO_PIN7 + GPIO_PIN8 + GPIO_PIN9 +
        GPIO_PIN10 + GPIO_PIN11 + GPIO_PIN12 + GPIO_PIN13 +
        GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_PD,
        GPIO_PIN0 + GPIO_PIN1
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PD,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    // P10.0 to USB RST pin,
    // ...if enabled with J5
    GPIO_setAsOutputPin(
        GPIO_PORT_P11,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 + GPIO_PIN12 +
        GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsInputPin(
        GPIO_PORT_PE,
        GPIO_PIN8
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PE,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_P11,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P11,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_PJ,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_PJ,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P6,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN7 +
        GPIO_PIN8 + GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 + GPIO_PIN15
        );

    GPIO_setOutputHighOnPin(
        GPIO_PORT_P6,
        GPIO_PIN6
        );

    // USB RX Pin, Input with
    // ...pulled down Resistor

    GPIO_setAsInputPinWithPullDownResistor(
        GPIO_PORT_P5,
        GPIO_PIN7
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P5,
        GPIO_PIN7
        );
}

void clockInit()
{
    UCS_setExternalClockSource(
        32768,
        0);

    UCS_turnOnLFXT1(
        UCS_XT1_DRIVE_3,
        UCS_XCAP_3
        );

    UCS_initFLLSettle(
        16000,
        488
        );

    SFR_enableInterrupt(
        SFR_OSCILLATOR_FAULT_INTERRUPT
        );
}

void Delay(void)
{
    __delay_cycles(SYSTEM_CLOCK_SPEED * 4);
}

#ifdef __ICC430__
int16_t __low_level_init(void) {
    WDT_A_hold(WDT_A_BASE); // Stop WDT (Watch Dog Timer)
    return(1);
}

#endif
