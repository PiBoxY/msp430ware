/*******************************************************************************
 *
 *  LPM.c
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
#include "LPM.h"

#define LPM_MENU_MAX 6

static const char LPMText[] = {
    "Select LPM      \0"
    " 1. LPM0 - 1MHz \0"
    " 2. LPM3 - REFO \0"
    " 3. LPM3 - LFXT1\0"
    " 4. LPM3 - VLO  \0"
    " 5. LPM4        \0"
    " 6. Quit        \0"
};

// -------------- LPM Menu---------------------------------

/**********************************************************************//**
 * @brief  Executes the "LPM" menu option in the User Experience
 *         example code. This menu option allows one to observe the
 *                 effects of different Low-Power Modes. For accurate current measurements,
 *                 place a multimeter at JP1 and begin measurement once the LCD display has
 *                 shutdown. Expected values for different Low Power Modes can be found
 *             in the MSP430F5438A datasheet.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void menuLPM(void)
{
    unsigned char LPMmenuPos = 0;
    unsigned char quit = 0;
    unsigned int i;

    halButtonsInterruptEnable(BUTTON_ALL);
    halAccelerometerShutDown();
    halBoardStopOutputSystemClock();
    halLcdClearScreen();

    halLcdPrintLine((char *)LPMText, 0, 0);

    buttonsPressed = 0;

    while (!quit)
    {
        for (i = 0; i < 7; i++) //print menu
        {
            halLcdPrintLine((char *)&LPMText[i * 17 + 17], i + 1,       \
                            OVERWRITE_TEXT);
        }
        halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,       \
                        INVERT_TEXT | OVERWRITE_TEXT);

        while (!buttonsPressed)
        {
            // To emulate a real application instead of continuous jumps, use nops
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
            __no_operation();
        }
        if (buttonsPressed & BUTTON_DOWN)        //scroll down
        {
            halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (++LPMmenuPos >= LPM_MENU_MAX)    //wrap around
            {
                LPMmenuPos = 0;
            }
            halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_UP)          //scroll up
        {
            halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (LPMmenuPos-- <= 0)               //wrap around
            {
                LPMmenuPos = 5;
            }
            halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_SELECT)      //select menu option
        {
            switch (LPMmenuPos)
            {
                case 0: LowPowerMode0(); break;
                case 1: LowPowerMode3(0); break; // LPM3 - REFO
                case 2: LowPowerMode3(1); break; // LPM3 - LFXT1
                case 3: LowPowerMode3(2); break; // LPM3 - VLO
                case 4: LowPowerMode4(); break;
                case 5: quit = 1; break;
                default: break;
            }
            buttonsPressed = 0;
            if (!quit)
            {
                halLcdClearScreen();
                halLcdPrintLine((char *)LPMText, 0, 0);
                halLcdPrintLine((char *)&LPMText[LPMmenuPos * 17 + 17], LPMmenuPos + 1,       \
                                INVERT_TEXT | OVERWRITE_TEXT);
            }
        }
        buttonsPressed = 0;
    }
}

/* Enter LPM0. Wake on button press. */
void LowPowerMode0(void)
{
    buttonsPressed = 0;
    halLcdClearScreen();
    halLcdPrintLine("Entering LPM0\0", 3, OVERWRITE_TEXT);
    halLcdPrintLine("Press button \0", 4, OVERWRITE_TEXT);
    halLcdPrintLine("to wake      \0", 5, OVERWRITE_TEXT);

    //Delay for user to read screen
    __delay_cycles(30000000);


    //turn off the Lcd to conserve power
    halLcdSetBackLight(0);
    halLcdStandby();
    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(1000, 31);
    SFRIE1 |= OFIE;

    do
    {
        __bis_SR_register(LPM0_bits + GIE); // Enter LPM0
        __no_operation();                   // For debugger only
    }
    while (buttonsPressed == 0);            //Exit on button press

    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;
    halLcdInit();
    halLcdClearScreen();
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
}

void LowPowerMode3(char aclkSource)
{
    buttonsPressed = 0;
    halLcdClearScreen();
    halLcdPrintLine("Entering LPM3\0", 3, OVERWRITE_TEXT);
    halLcdPrintLine("Press button \0", 4, OVERWRITE_TEXT);
    halLcdPrintLine("to wake      \0", 5, OVERWRITE_TEXT);

    //Delay for user to read screen
    __delay_cycles(30000000);

    //turn off the Lcd to conserve power
    halLcdSetBackLight(0);
    halLcdStandby();

    switch (aclkSource)
    {
        case 0: SELECT_ACLK(SELA__REFOCLK);  break; // Select REFOSC to source ACLK
        case 1: SELECT_ACLK(SELA__XT1CLK);  break;  // Select LFXT1 to source ACLK
        case 2: SELECT_ACLK(SELA__VLOCLK);  break;  // Select VLO to source ACLK
        default:  break;
    }

    if (aclkSource != 1)
        SFRIE1 &= ~OFIE;
    do
    {
        __bis_SR_register(LPM3_bits + GIE);         // Enter LPM3
        __no_operation();                           // For debugger only
    }
    while (buttonsPressed == 0);                    //Exit on button press
    SFRIE1 |= OFIE;

    SELECT_ACLK(SELA__XT1CLK);                      // Select LFXT1 to source ACLK

    halLcdInit();
    halLcdClearScreen();
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
}

void LowPowerMode4(void)
{
    buttonsPressed = 0;
    halLcdClearScreen();
    halLcdPrintLine("Entering LPM4\0", 3, OVERWRITE_TEXT);
    halLcdPrintLine("Press button \0", 4, OVERWRITE_TEXT);
    halLcdPrintLine("to wake      \0", 5, OVERWRITE_TEXT);

    //Delay for user to read screen
    __delay_cycles(30000000);

    //turn off the Lcd to conserve power
    halLcdSetBackLight(0);
    halLcdStandby();
    UCSCTL8_L = 0;
    UCSCTL6_L |= SMCLKOFF;
    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(8000, 244);
    SFRIE1 |= OFIE;

    do
    {
        __bis_SR_register(LPM4_bits + GIE); // Enter LPM4
        __no_operation();                   // For debugger only
    }
    while (buttonsPressed == 0);            //Exit on button press

    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;
    halBoardStopOutputSystemClock();
    UCSCTL8_L = 0x0F;
    UCSCTL6_L &= ~SMCLKOFF;
    halLcdInit();
    halLcdClearScreen();
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
}

