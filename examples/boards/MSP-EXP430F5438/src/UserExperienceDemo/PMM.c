/*******************************************************************************
 *
 *  PMM.c
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#include "hal_MSP-EXP430F5438.h"
#include "UserExperience_F5438A.h"
#include "PMM.h"
#include "5xx_ACTIVE_test.h"

#define VCORE_MENU_MAX 4
#define MCLK_MENU_MAX 7

static const char VcoreText[] = {
    "Vcore\0"
    "1.35V\0"
    "1.55V\0"
    "1.75V\0"
    "1.85V\0"
};

static const char MCLKText[] = {
    "MCLK \0"
    " 1Mhz\0"
    " 4Mhz\0"
    " 8Mhz\0"
    "12Mhz\0"
    "16Mhz\0"
    "20Mhz\0"
    "25Mhz\0"
};

static const unsigned char MAX_MCLK_ALLOWED[] = {3, 5, 6, 7};
long dummyCalculation, wastingTime, wastingCycles;
unsigned char menuLeftPos, menuRightPos = 0, menuRightMaxAllowed;
unsigned char quit = 0, activeMenu = 1;
unsigned int i, j, delay;

// -------------- PMM and MCLK Menu---------------------------------

/**********************************************************************//**
 * @brief  Executes the "PMM-MCLK" menu option in the User Experience
 *         example code. This menu option allows one to change the frequency
 *         of operation for the MSP430 and the VCore setting.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void menuPMMMCLK(void)
{
    halButtonsInterruptEnable(BUTTON_ALL);
    halAccelerometerShutDown();
    halLcdClearScreen();
    quit = 0;

    halBoardOutputSystemClock();

    if (Get_Device_Type() == F5438A)
    {
        menuLeftPos = 0;
        menuRightPos = 0;

        menuRightMaxAllowed = MAX_MCLK_ALLOWED[menuLeftPos];
        menuDisplay();
    }
    else
    {
        menuRightMaxAllowed = 5;
        menuLeftPos = 2;
        menuDisplay();

        for (i = menuRightMaxAllowed; i < MCLK_MENU_MAX; i++)
        {
            halLcdLine(96, 12 * (i + 1) + 6, 136, 12 * (i + 1) + 6, PIXEL_ON);
            halLcdLine(96, 12 * (i + 1) + 7, 136, 12 * (i + 1) + 7, PIXEL_ON);
        }
    }

    buttonsPressed = 0;

    SetVCore(menuLeftPos);
    SFRIE1 &= ~OFIE;                                                             //disable
                                                                                 // oscillator fault
                                                                                 // before
                                                                                 // configuring FLL
                                                                                 // because of
                                                                                 // DCOFFG
    switch (menuRightPos)
    {
        case 0: Init_FLL_Settle(1000, 31); break;                                //Set to 1MHz
        case 1: Init_FLL_Settle(4000, 122); break;                               //Set to 4MHz
        case 2: Init_FLL_Settle(8000, 244); break;                               //Set to 8MHz
        case 3: Init_FLL_Settle(12000, 366); break;                              //Set to 12MHz
        case 4: Init_FLL_Settle(16000, 488); break;                              //Set to 16MHz
        case 5: Init_FLL_Settle(20000, 610); break;                              //Set to 20MHz
        case 6: Init_FLL_Settle(25000, 762); break;                              //Set to 25MHz
    }
    SFRIE1 |= OFIE;
    while (!quit)
    {
        // The LED is enabled to show the relative difference between
        // frequencies of operation.
        while (!buttonsPressed)
        {
            LED_PORT_OUT ^= LED_1;
            for (i = 0; i < 0xFFFF; i++)
                if (buttonsPressed)
                    break;
        }

        if (buttonsPressed & BUTTON_LEFT)                                        //switch to the
                                                                                 // left column
        {
            activeMenu = 0;                                                      //Set the cursor to
                                                                                 // the left column
                                                                                 // menu
            halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                               1,  INVERT_TEXT | OVERWRITE_TEXT);
            halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6], menuRightPos + 1,      \
                               12,  GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
        }

        if (buttonsPressed & BUTTON_RIGHT)                                       //switch to the
                                                                                 // right column
        {
            activeMenu = 1;                                                      //Set the cursor to
                                                                                 // the right column
                                                                                 // menu
            halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                               1,  GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
            halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6], menuRightPos + 1,      \
                               12,  INVERT_TEXT | OVERWRITE_TEXT);
        }

        if (buttonsPressed & BUTTON_DOWN)                                        //scroll down
        {
            if (Get_Device_Type() == F5438A)
            {
                if (activeMenu == 0)                                             //scroll down the
                                                                                 // left column
                {
                    halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                                       1,  OVERWRITE_TEXT);
                    if (++menuLeftPos >= VCORE_MENU_MAX)                         //handle max
                                                                                 // allowed mclk for
                                                                                 // new voltage
                                                                                 // level
                    {
                        menuLeftPos = 0;
                        menuRightMaxAllowed = MAX_MCLK_ALLOWED[menuLeftPos];
                        for (i = menuRightMaxAllowed + 1; i < MCLK_MENU_MAX + 1; i++)
                        {
                            halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,
                                               GRAYSCALE_TEXT | OVERWRITE_TEXT); //grayscale levels
                                                                                 // that are not
                                                                                 // allowed
                        }
                        if (menuRightPos >= menuRightMaxAllowed)
                        {
                            menuRightPos = menuRightMaxAllowed - 1;
                        }
                        halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 +
                                                             6], menuRightPos + 1, \
                                           12,
                                           GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
                    }
                    else
                    {
                        for (i = menuRightMaxAllowed + 1; i < MAX_MCLK_ALLOWED[menuLeftPos] + 1;
                             i++)
                            halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,  OVERWRITE_TEXT);
                        menuRightMaxAllowed = MAX_MCLK_ALLOWED[menuLeftPos];
                    }
                    halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,       \
                                       1, INVERT_TEXT | OVERWRITE_TEXT);
                }
                else //scroll down the right column
                {
                    halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                                       menuRightPos + 1, 12,  OVERWRITE_TEXT);
                    if (++menuRightPos >= menuRightMaxAllowed)
                        menuRightPos = 0;
                    halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                                       menuRightPos + 1, 12, INVERT_TEXT | OVERWRITE_TEXT);
                }
            }
        }

        if (buttonsPressed & BUTTON_UP) //scroll up
        {
            if (Get_Device_Type() == F5438A)
            {
                if (activeMenu == 0) //scroll up the left column
                {
                    halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                                       1,  OVERWRITE_TEXT);
                    if (menuLeftPos-- <= 0)
                    {
                        menuLeftPos = 3;
                        for (i = menuRightMaxAllowed + 1; i < MCLK_MENU_MAX + 1; i++)
                        {
                            halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,  OVERWRITE_TEXT);
                        }
                    }
                    menuRightMaxAllowed = MAX_MCLK_ALLOWED[menuLeftPos];
                    for (i = menuRightMaxAllowed + 1; i < MCLK_MENU_MAX + 1; i++)
                    {
                        halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,
                                           GRAYSCALE_TEXT | OVERWRITE_TEXT);
                    }
                    if (menuRightPos >= menuRightMaxAllowed)
                    {
                        menuRightPos = menuRightMaxAllowed - 1;
                    }
                    halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6], menuRightPos + 1, \
                                       12, GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
                    halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,       \
                                       1, INVERT_TEXT | OVERWRITE_TEXT);
                }
                else
                {
                    halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                                       menuRightPos + 1, 12,  OVERWRITE_TEXT);
                    if (menuRightPos-- <= 0)
                        menuRightPos = menuRightMaxAllowed - 1;
                    halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                                       menuRightPos + 1, 12, INVERT_TEXT | OVERWRITE_TEXT);
                }
            }
        }

        if (buttonsPressed & BUTTON_S1)
        {
            buttonsPressed = 0;
            halLcdClearScreen();
            halLcdPrintLine("    Screen Off  \0", 3, OVERWRITE_TEXT);
            halLcdPrintLine("   Press button \0", 4, OVERWRITE_TEXT);
            halLcdPrintLine("     to wake    \0", 5, OVERWRITE_TEXT);
            //Delay for user to read screen
            switch (menuRightPos)
            {
                case 0: __delay_cycles(2000000); delay = 2000; break;
                case 1: __delay_cycles(8000000); delay = 8000; break;
                case 2: __delay_cycles(16000000); delay = 16000; break;
                case 3: __delay_cycles(24000000); delay = 24000; break;
                case 4: __delay_cycles(32000000); delay = 32000; break;
                case 5: __delay_cycles(40000000); delay = 40000; break;
                case 6: __delay_cycles(50000000); delay = 50000; break;
                default: delay = 2000; break;
            }
            //turn off the Lcd to conserve power
            halLcdSetBackLight(0);
            halLcdClearScreen();
            halLcdStandby();
            LED_PORT_OUT &= ~LED_1;                         //turn off LED_1;
            halBoardStopOutputSystemClock();

            while (!buttonsPressed){                        //wait for button push
                for (i = 0; i < delay; i++)                 //delay between LED_2 pulses
                    for (j = 0; j < 10; j++)
                        ACTIVE_MODE_TEST();                 //Emulate a real application instead of
                                                            // continuous jumps

                //pulse LED_2 to show board is still active
                LED_PORT_OUT |= LED_2;
                for (i = 0; i < (delay >> 4); i++)          //delay so LED_2 pulse is visible
                    ACTIVE_MODE_TEST();                     //Emulate a real application instead of
                                                            // continuous jumps
                LED_PORT_OUT &= ~LED_2;
            }

            buttonsPressed = 0;                             //clear button press
            halLcdInit();                                   //turn screen back on
            halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
            halBoardOutputSystemClock();
            menuDisplay();
        }

        if (buttonsPressed)
        {
            SetVCore(menuLeftPos);                          //update vcore and clock
            SFRIE1 &= ~OFIE;                                //disable oscillator fault before
                                                            // configuring FLL because of DCOFFG
            switch (menuRightPos)
            {
                case 0: Init_FLL_Settle(1000, 31); break;   //Set to 1MHz
                case 1: Init_FLL_Settle(4000, 122); break;  //Set to 4MHz
                case 2: Init_FLL_Settle(8000, 244); break;  //Set to 8MHz
                case 3: Init_FLL_Settle(12000, 366); break; //Set to 12MHz
                case 4: Init_FLL_Settle(16000, 488); break; //Set to 16MHz
                case 5: Init_FLL_Settle(20000, 610); break; //Set to 20MHz
                case 6: Init_FLL_Settle(25000, 762); break; //Set to 25MHz
            }
            SFRIE1 |= OFIE;
        }

        if (buttonsPressed & BUTTON_SELECT)
            quit = 1;
        buttonsPressed = 0;
    }
    SFRIE1 &= ~OFIE;                                        //disable oscillator fault before
                                                            // configuring FLL because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;                                         //disable oscillator fault before
                                                            // configuring FLL because of DCOFFG
    halBoardStopOutputSystemClock();
    LED_PORT_OUT &= ~LED_1;
}

void menuDisplay(void)
{
    for (i = 0; i < 5; i++)
    {
        halLcdPrintLineCol((char *)&VcoreText[i * 6], i, 1, OVERWRITE_TEXT);
    }
    for (i = 0; i < MCLK_MENU_MAX + 1; i++)
        if (i <= menuRightMaxAllowed)
            halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,  OVERWRITE_TEXT);
        else halLcdPrintLineCol((char *)&MCLKText[i * 6], i, 12,  GRAYSCALE_TEXT | OVERWRITE_TEXT);
    if (activeMenu == 0)
    {
        halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                           1,  INVERT_TEXT | OVERWRITE_TEXT);
        halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                           menuRightPos + 1, 12, GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
    }
    else
    {
        halLcdPrintLineCol((char *)&VcoreText[menuLeftPos * 6 + 6], menuLeftPos + 1,      \
                           1,  GRAYSCALE_TEXT | INVERT_TEXT | OVERWRITE_TEXT);
        halLcdPrintLineCol((char *)&MCLKText[menuRightPos * 6 + 6],    \
                           menuRightPos + 1, 12, INVERT_TEXT | OVERWRITE_TEXT);
    }
    halLcdPrintLine(" LCD ", 6, 0);
    halLcdPrintLine(" OFF?", 7, 0);
    halLcdLine(20, 96, 20, 106, PIXEL_ON);
    halLcdLine(21, 96, 21, 106, PIXEL_ON);
    halLcdLine(15, 100, 21, 106, PIXEL_ON);
    halLcdLine(15, 101, 21, 107, PIXEL_ON);
    halLcdLine(26, 100, 20, 106, PIXEL_ON);
    halLcdLine(26, 101, 20, 107, PIXEL_ON);
}

