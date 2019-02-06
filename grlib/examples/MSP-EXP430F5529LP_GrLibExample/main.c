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
#include <msp430f5529.h>
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "radioButton.h"
#include "checkbox.h"
#include "driverlib.h"
#include "touch_F5529LP.h"
#include "LcdDriver/kitronix320x240x16_ssd2119_spi.h"
#include "Images/images.h"

//Touch screen context
touch_context g_sTouchContext;
Graphics_ImageButton primitiveButton;
Graphics_ImageButton imageButton;
Graphics_Button yesButton;
Graphics_Button noButton;

// Graphic library context
Graphics_Context g_sContext;

//Flag to know if a demo was run
bool g_ranDemo = false;

void Delay();
void boardInit(void);
void clockInit(void);
void timerInit(void);
void initializeDemoButtons(void);
void drawMainMenu(void);
void runPrimitivesDemo(void);
void runImagesDemo(void);
void drawRestarDemo(void);

#if defined(__IAR_SYSTEMS_ICC__)
int16_t __low_level_init(void) {
    // Stop WDT (Watch Dog Timer)
    WDTCTL = WDTPW + WDTHOLD;
    return(1);
}

#endif

void main(void)
{
    // Initialization routines
    boardInit();
    clockInit();
    timerInit();
    initializeDemoButtons();

    // LCD setup using Graphics Library API calls
    Kitronix320x240x16_SSD2119Init();
    Graphics_initContext(&g_sContext, &g_sKitronix320x240x16_SSD2119);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);

    //
    touch_initInterface();

    drawMainMenu();

    // Loop to detect touch
    while(1)
    {
        touch_updateCurrentTouch(&g_sTouchContext);

        if(g_sTouchContext.touch)
        {
            if(Graphics_isImageButtonSelected(&primitiveButton,
                                              g_sTouchContext.x,
                                              g_sTouchContext.y))
            {
                Graphics_drawSelectedImageButton(&g_sContext,&primitiveButton);
                runPrimitivesDemo();
            }
            else if(Graphics_isImageButtonSelected(&imageButton,
                                                   g_sTouchContext.x,
                                                   g_sTouchContext.y))
            {
                Graphics_drawSelectedImageButton(&g_sContext,&imageButton);
                runImagesDemo();
            }

            if(g_ranDemo == true)
            {
                g_ranDemo = false;
                drawMainMenu();
            }
        }
    }
}

void initializeDemoButtons(void)
{
    // Initiliaze primitives Demo Button
    primitiveButton.xPosition = 20;
    primitiveButton.yPosition = 50;
    primitiveButton.borderWidth = 5;
    primitiveButton.selected = false;
    primitiveButton.imageWidth = Primitives_Button4BPP_UNCOMP.xSize;
    primitiveButton.imageHeight = Primitives_Button4BPP_UNCOMP.ySize;
    primitiveButton.borderColor = GRAPHICS_COLOR_WHITE;
    primitiveButton.selectedColor = GRAPHICS_COLOR_RED;
    primitiveButton.image = &Primitives_Button4BPP_UNCOMP;

    // Initiliaze images Demo Button
    imageButton.xPosition = 180;
    imageButton.yPosition = 50;
    imageButton.borderWidth = 5;
    imageButton.selected = false;
    imageButton.imageWidth = Primitives_Button4BPP_UNCOMP.xSize;
    imageButton.imageHeight = Primitives_Button4BPP_UNCOMP.ySize;
    imageButton.borderColor = GRAPHICS_COLOR_WHITE;
    imageButton.selectedColor = GRAPHICS_COLOR_RED;
    imageButton.image = &Images_Button4BPP_UNCOMP;

    yesButton.xMin = 80;
    yesButton.xMax = 150;
    yesButton.yMin = 80;
    yesButton.yMax = 120;
    yesButton.borderWidth = 1;
    yesButton.selected = false;
    yesButton.fillColor = GRAPHICS_COLOR_RED;
    yesButton.borderColor = GRAPHICS_COLOR_RED;
    yesButton.selectedColor = GRAPHICS_COLOR_BLACK;
    yesButton.textColor = GRAPHICS_COLOR_BLACK;
    yesButton.selectedTextColor = GRAPHICS_COLOR_RED;
    yesButton.textXPos = 100;
    yesButton.textYPos = 90;
    yesButton.text = "YES";
    yesButton.font = &g_sFontCm18;

    noButton.xMin = 180;
    noButton.xMax = 250;
    noButton.yMin = 80;
    noButton.yMax = 120;
    noButton.borderWidth = 1;
    noButton.selected = false;
    noButton.fillColor = GRAPHICS_COLOR_RED;
    noButton.borderColor = GRAPHICS_COLOR_RED;
    noButton.selectedColor = GRAPHICS_COLOR_BLACK;
    noButton.textColor = GRAPHICS_COLOR_BLACK;
    noButton.selectedTextColor = GRAPHICS_COLOR_RED;
    noButton.textXPos = 200;
    noButton.textYPos = 90;
    noButton.text = "NO";
    noButton.font = &g_sFontCm18;
}

void drawMainMenu(void)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "MSP Graphics Library Demo",
                                AUTO_STRING_LENGTH,
                                159,
                                15,
                                TRANSPARENT_TEXT);

    // Draw TI banner at the bottom of screnn
    Graphics_drawImage(&g_sContext,
                       &TI_platform_bar_red4BPP_UNCOMP,
                       0,
                       Graphics_getDisplayHeight(
                           &g_sContext) - TI_platform_bar_red4BPP_UNCOMP.ySize);

    // Draw Primitives image button
    Graphics_drawImageButton(&g_sContext, &primitiveButton);

    // Draw Images image button
    Graphics_drawImageButton(&g_sContext, &imageButton);
}

void runPrimitivesDemo(void)
{
    int16_t ulIdx;
    uint32_t color;

    Graphics_Rectangle myRectangle1 = { 10, 50, 155, 120};
    Graphics_Rectangle myRectangle2 = { 150, 100, 300, 200};
    Graphics_Rectangle myRectangle3 = { 0, 0, 319, 239};

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawString(&g_sContext, "Draw Pixels & Lines", AUTO_STRING_LENGTH,
                        60, 5, TRANSPARENT_TEXT);
    Graphics_drawPixel(&g_sContext, 45, 45);
    Graphics_drawPixel(&g_sContext, 45, 50);
    Graphics_drawPixel(&g_sContext, 50, 50);
    Graphics_drawPixel(&g_sContext, 50, 45);
    Graphics_drawLine(&g_sContext, 60, 60, 200, 200);
    Graphics_drawLine(&g_sContext, 30, 200, 200, 60);
    Graphics_drawLine(&g_sContext, 0, Graphics_getDisplayHeight(
                          &g_sContext) - 1,
                      Graphics_getDisplayWidth(&g_sContext) - 1,
                      Graphics_getDisplayHeight(&g_sContext) - 1);
    Delay();
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Draw Rectangles",
                                AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    // Text won't be visible on screen due to transparency (foreground colors match)
    Graphics_drawStringCentered(&g_sContext, "Normal Text", AUTO_STRING_LENGTH,
                                225, 120, TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext, "Opaque Text", AUTO_STRING_LENGTH,
                                225, 150, OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    // Text draws with inverted foreground color to become visible
    Graphics_drawStringCentered(&g_sContext, "Invert Text", AUTO_STRING_LENGTH,
                                225, 180, TRANSPARENT_TEXT);
    Delay();
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    // Invert the foreground and background colors
    Graphics_fillRectangle(&g_sContext, &myRectangle3);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawStringCentered(&g_sContext, "Invert Colors",
                                AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
    Graphics_drawRectangle(&g_sContext, &myRectangle1);
    Graphics_fillRectangle(&g_sContext, &myRectangle2);
    // Text won't be visible on screen due to transparency
    Graphics_drawStringCentered(&g_sContext, "Normal Text", AUTO_STRING_LENGTH,
                                225, 120, TRANSPARENT_TEXT);
    // Text draws foreground and background for opacity
    Graphics_drawStringCentered(&g_sContext, "Opaque Text", AUTO_STRING_LENGTH,
                                225, 150, OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    // Text draws with inverted color to become visible
    Graphics_drawStringCentered(&g_sContext, "Invert Text", AUTO_STRING_LENGTH,
                                225, 180, TRANSPARENT_TEXT);
    Delay();
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Draw Circles", AUTO_STRING_LENGTH,
                                159, 15, TRANSPARENT_TEXT);
    Graphics_drawCircle(&g_sContext, 100, 100, 50);
    Graphics_fillCircle(&g_sContext, 200, 140, 70);
    Delay();
    Graphics_clearDisplay(&g_sContext);
    // Add some more color
    Graphics_setForegroundColor(&g_sContext, ClrLawnGreen);
    Graphics_setBackgroundColor(&g_sContext, ClrBlack);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Rainbow of Colored Lines",
                                AUTO_STRING_LENGTH, 159, 15, TRANSPARENT_TEXT);
    // Draw a quarter rectangle sweep of lines from red to purple.
    for(ulIdx = 128; ulIdx >= 1; ulIdx--)
    {
        // Red Color
        *((uint16_t*) (&color) + 1) = 255;
        // Blue and Green Colors
        *((uint16_t*) (&color)) =
            ((((128 - ulIdx) * 255) >> 7) << ClrBlueShift);

        Graphics_setForegroundColor(&g_sContext, color);
        Graphics_drawLine(&g_sContext, 160, 200, 32, ulIdx + 72);
    }
    // Draw a quarter rectangle sweep of lines from purple to blue.
    for(ulIdx = 128; ulIdx >= 1; ulIdx--)
    {
        // Red Color
        *((uint16_t*) (&color) + 1) = (ulIdx * 255) >> 7;
        // Blue and Green Colors
        *((uint16_t*) (&color)) = 255 << ClrBlueShift;

        Graphics_setForegroundColor(&g_sContext, color);
        Graphics_drawLine(&g_sContext, 160, 200, 160 - ulIdx, 72);
    }
    // Clear Red Color
    *((uint16_t*) (&color) + 1) = 0;
    // Draw a quarter rectangle sweep of lines from blue to teal.
    for(ulIdx = 128; ulIdx >= 1; ulIdx--)
    {
        // Blue and Green Colors
        *((uint16_t*) (&color)) =
            ((((128 -
                ulIdx) * 255) >> 7) << ClrGreenShift) | (255 << ClrBlueShift);

        Graphics_setForegroundColor(&g_sContext, color);
        Graphics_drawLine(&g_sContext, 160, 200, 288 - ulIdx, 72);
    }
    // Draw a quarter rectangle sweep of lines from teal to green.
    for(ulIdx = 128; ulIdx >= 0; ulIdx--)
    {
        // Blue and Green Colors
        *((uint16_t*) (&color)) =
            (255 << ClrGreenShift) | (((ulIdx * 255) >> 7) << ClrBlueShift);

        Graphics_setForegroundColor(&g_sContext, color);
        Graphics_drawLine(&g_sContext, 160, 200, 288, 200 - (ulIdx));
    }
    Delay();
    g_ranDemo = true;

    drawRestarDemo();
}

void runImagesDemo(void)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Draw Uncompressed Image",
                                AUTO_STRING_LENGTH, 159, 200, TRANSPARENT_TEXT);
    Delay();
    // Draw Image on the display
    Graphics_drawImage(&g_sContext, &lcd_color_320x2408BPP_UNCOMP, 0, 0);
    Delay();
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Draw RLE4 compressed Image",
                                AUTO_STRING_LENGTH, 159, 200, TRANSPARENT_TEXT);
    Delay();
    Graphics_drawImage(&g_sContext, &TI_logo_150x1501BPP_COMP_RLE4, 85, 45);
    Delay();

    g_ranDemo = true;

    drawRestarDemo();
}

void drawRestarDemo(void)
{
    g_ranDemo = false;
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Would you like to go back",
                                AUTO_STRING_LENGTH,
                                159,
                                45,
                                TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "to the main menu?",
                                AUTO_STRING_LENGTH,
                                159,
                                65,
                                TRANSPARENT_TEXT);

    // Draw Primitives image button
    Graphics_drawButton(&g_sContext, &yesButton);

    // Draw Images image button
    Graphics_drawButton(&g_sContext, &noButton);

    do
    {
        touch_updateCurrentTouch(&g_sTouchContext);
        if(Graphics_isButtonSelected(&noButton, g_sTouchContext.x,
                                     g_sTouchContext.y))
        {
            Graphics_drawSelectedButton(&g_sContext, &noButton);
            g_ranDemo = true;
        }
        else
        {
            if(g_ranDemo)
            {
                Graphics_drawReleasedButton(&g_sContext, &noButton);
                g_ranDemo = false;
            }
        }
    }
    while(!Graphics_isButtonSelected(&yesButton, g_sTouchContext.x,
                                     g_sTouchContext.y));

    Graphics_drawSelectedButton(&g_sContext, &yesButton);

    g_ranDemo = true;
    __delay_cycles(10000000);
}

void boardInit(void)
{
    // Setup XT1 and XT2
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5
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
}

void timerInit()
{
    Timer_A_initUpModeParam timerAUpModeParams =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        UINT16_MAX,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_SKIP_CLEAR
    };
    //Configure timer A to count cycles/64
    Timer_A_initUpMode(
        TIMER_A1_BASE,&timerAUpModeParams);
}

void Delay(){
    __delay_cycles(SYSTEM_CLOCK_SPEED * 3);
}
