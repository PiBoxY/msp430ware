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
#include <stdint.h>
#include "driverlib.h"
#include "grlib.h"
#include "radioButton.h"
#include "checkbox.h"
#include "LcdDriver/Dogs102x64_UC1701.h"
#include "Images/images.h"

void boardInit(void);
void clockInit(void);
void delay(void);

Graphics_Context g_sContext;
const Graphics_Rectangle myRectangle1 = { 5, 10, 60, 50};
const Graphics_Rectangle myRectangle2 = { 30, 20, 100, 60};
const Graphics_Rectangle myRectangle3 = { 0, 0, 101, 63};

#if defined(__IAR_SYSTEMS_ICC__)
int16_t __low_level_init(void) {
    // Stop WDT (Watch Dog Timer)
    WDTCTL = WDTPW + WDTHOLD;
    return(1);
}

#endif

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

    // Basic GPIO initialization
    boardInit();
    clockInit();

    // Set up LCD
    Dogs102x64_UC1701_initLCD();

    //**********************************************
    //***************** Note ***********************
    //**********************************************
    // This LCD display has adjustable contrast.
    // The default contrast setting likely needs
    // adjustment for optimal performance.
    //**********************************************
    Dogs102x64_UC1701_setContrast(11);
    Dogs102x64_UC1701_setBacklight(11);

    Graphics_initContext(&g_sContext, &g_sDogs102x64_UC1701);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

    // Intro Screen
    Graphics_drawStringCentered(&g_sContext,
                                "How to use MSP",
                                AUTO_STRING_LENGTH,
                                51,
                                16,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "Graphics Library",
                                AUTO_STRING_LENGTH,
                                51,
                                32,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                "Primitives",
                                AUTO_STRING_LENGTH,
                                51,
                                48,
                                TRANSPARENT_TEXT);
    delay();
    Graphics_clearDisplay(&g_sContext);

    // Draw pixels and lines on the display
    Graphics_drawString(&g_sContext,
                        "Draw Pixels",
                        AUTO_STRING_LENGTH,
                        20,
                        0,
                        TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext,
                        "& Lines",
                        AUTO_STRING_LENGTH,
                        30,
                        10,
                        TRANSPARENT_TEXT);
    Graphics_drawPixel(&g_sContext, 10, 10);
    Graphics_drawPixel(&g_sContext, 10, 12);
    Graphics_drawPixel(&g_sContext, 12, 12);
    Graphics_drawPixel(&g_sContext, 12, 10);
    Graphics_drawLine(&g_sContext, 15, 15, 60, 60);
    Graphics_drawLine(&g_sContext, 10, 50, 90, 10);
    Graphics_drawLine(&g_sContext,
                      0,
                      Graphics_getDisplayHeight(&g_sContext) - 1,
                      Graphics_getDisplayWidth(&g_sContext) - 1,
                      Graphics_getDisplayHeight(&g_sContext) - 1);
    delay();
    Graphics_clearDisplay(&g_sContext);

    // Draw circles on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Circles",
                                AUTO_STRING_LENGTH,
                                51,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawCircle(&g_sContext, 30, 50, 10);
    Graphics_fillCircle(&g_sContext, 65, 37, 23);
    delay();

    Graphics_clearDisplay(&g_sContext);

    // Draw rectangles on the display
    Graphics_drawStringCentered(&g_sContext,
                                "Draw Rectangles",
                                AUTO_STRING_LENGTH,
                                51,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    // Text won't be visible on screen due to transparency
    Graphics_drawStringCentered(&g_sContext,
                                "Normal Text",
                                AUTO_STRING_LENGTH,
                                65,
                                30,
                                TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext,
                                "Opaque Text",
                                AUTO_STRING_LENGTH,
                                65,
                                40,
                                OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, ClrWhite);
    Graphics_setBackgroundColor(&g_sContext, ClrBlack);
    // Text draws with inverted color to become visible
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Text",
                                AUTO_STRING_LENGTH,
                                65,
                                50,
                                TRANSPARENT_TEXT);
    delay();
    Graphics_clearDisplay(&g_sContext);

    // Invert the foreground and background colors
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_fillRectangle(&g_sContext, &myRectangle3);
    Graphics_setForegroundColor(&g_sContext, ClrWhite);
    Graphics_setBackgroundColor(&g_sContext, ClrBlack);
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Colors",
                                AUTO_STRING_LENGTH,
                                51,
                                5,
                                TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    // Text won't be visible on screen due to transparency
    Graphics_drawStringCentered(&g_sContext,
                                "Normal Text",
                                AUTO_STRING_LENGTH,
                                65,
                                30,
                                TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext,
                                "Opaque Text",
                                AUTO_STRING_LENGTH,
                                65,
                                40,
                                OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    // Text draws with inverted color to become visible
    Graphics_drawStringCentered(&g_sContext,
                                "Invert Text",
                                AUTO_STRING_LENGTH,
                                65,
                                50,
                                TRANSPARENT_TEXT);
    delay();
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);

    //**************************************************************************
    // This is an alternative method to invert colors using built in feature of
    // LCD display.
    // The above method illustrates how to use the graphics primitives for color
    // inversion
    //**************************************************************************
    //Dogs102x64_UC1701_inverseDisplay();
    //delay();
    //Dogs102x64_UC1701_clearInverseDisplay();	// Change display back to normal
    //*************************************************************************

    Graphics_clearDisplay(&g_sContext);

    // Draw Images on the display
    Graphics_drawImage(&g_sContext, &LPRocket_96x37_1BPP_UNCOMP, 3, 13);
    delay();
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawImage(&g_sContext, &TI_Logo_69x64_1BPP_UNCOMP, 16, 0);
    delay();

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

    delay();
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext,
                                "Check Boxes",
                                AUTO_STRING_LENGTH,
                                48,
                                5,
                                TRANSPARENT_TEXT);

    Graphics_drawCheckBox(&g_sContext, &checkBox1);
    Graphics_drawCheckBox(&g_sContext, &checkBox2);
    delay();
    Graphics_clearDisplay(&g_sContext);

    Dogs102x64_UC1701_disable();
    LPM4;
}

void boardInit(void)
{
    // Setup XT1 and XT2
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5
        );

    // Configure LED ports
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P8,
        GPIO_PIN1 + GPIO_PIN2
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_P8,
        GPIO_PIN1 + GPIO_PIN2
        );

    // Configure button ports
    // Buttons on P1.7/P2.2 are inputs
    GPIO_setAsInputPin(
        GPIO_PORT_PA,
        GPIO_PIN7 + GPIO_PIN10
        );

    // Configure CapTouch ports
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P6,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7
        );
    GPIO_setAsInputPin(
        GPIO_PORT_P6,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6 + GPIO_PIN7
        );

    // Configure Cma3000 ports
    // ACCEL_INT pin is input
    GPIO_setAsInputPin(
        GPIO_PORT_P2,
        GPIO_PIN5
        );

    // SCK
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P2,
        GPIO_PIN7
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN7
        );

    // ACCEL_SIMO, ACCEL_CS, ACCEL_PWR to low
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P3,
        GPIO_PIN3 + GPIO_PIN5 + GPIO_PIN6
        );

    // ACCEL_SOMI pin is input
    GPIO_setAsInputPin(
        GPIO_PORT_P3,
        GPIO_PIN4
        );

    // ACCEL_SIMO, ACCEL_CS, ACCEL_PWR as outp
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN3 + GPIO_PIN5 + GPIO_PIN6
        );

    // Configure Dogs102x6 ports
    // LCD_C/D, LCD_RST
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P5,
        GPIO_PIN6 + GPIO_PIN7
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_P5,
        GPIO_PIN6 + GPIO_PIN7
        );

    // LCD_CS, LCD_BL_EN
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P7,
        GPIO_PIN4 + GPIO_PIN6
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_P7,
        GPIO_PIN4 + GPIO_PIN6
        );

    // SIMO, SCK
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P4,
        GPIO_PIN1 + GPIO_PIN3
        );

    // SOMI pin is input
    GPIO_setAsInputPin(
        GPIO_PORT_P4,
        GPIO_PIN2
        );

    GPIO_setAsOutputPin(
        GPIO_PORT_P4,
        GPIO_PIN1 + GPIO_PIN3
        );

    // Configure SDCard ports
    // SD_CS to high
    GPIO_setOutputHighOnPin(
        GPIO_PORT_P3,
        GPIO_PIN7
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN7
        );

    // Configure Wheel ports
    // A5 ADC input
    GPIO_setAsInputPin(
        GPIO_PORT_P6,
        GPIO_PIN5
        );

    // POT_PWR
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P8,
        GPIO_PIN0
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P8,
        GPIO_PIN0
        );

    // Configure unused ports for low power
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P2,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN6
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P2,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN6
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P3,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P4,
        GPIO_PIN0 + GPIO_PIN6 + GPIO_PIN7
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P4,
        GPIO_PIN0 + GPIO_PIN6 + GPIO_PIN7
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P5,
        GPIO_PIN1
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P5,
        GPIO_PIN1
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P6,
        GPIO_PIN6 + GPIO_PIN7
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P6,
        GPIO_PIN6 + GPIO_PIN7
        );

    GPIO_setOutputLowOnPin(
        GPIO_PORT_P7,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3 + GPIO_PIN5 + GPIO_PIN7
        );
    GPIO_setAsOutputPin(
        GPIO_PORT_P7,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3 + GPIO_PIN5 + GPIO_PIN7
        );
}

void clockInit(void)
{
    UCS_setExternalClockSource(
        32768,
        0);

    // Set Vcore to accomodate for max. allowed system speed
    PMM_setVCore(
        PMM_CORE_LEVEL_3
        );

    // Use 32.768kHz XTAL as reference
    UCS_turnOnLFXT1(
        UCS_XT1_DRIVE_3,
        UCS_XCAP_3
        );

    // Set system clock to max (25MHz)
    UCS_initFLLSettle(
        25000,
        762
        );

    SFR_enableInterrupt(
        SFR_OSCILLATOR_FAULT_INTERRUPT
        );

    // Globally enable interrupts
    __enable_interrupt();
}

void delay(void)
{
    __delay_cycles(SYSTEM_CLOCK_SPEED * 4);
}
