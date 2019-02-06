/*******************************************************************************
 *
 *  PowerTest.c
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
#include "LPM.h"
#include "PMM.h"
#include "PowerTest.h"

#define POWER_MENU_MAX 3

static const char PowerText[] = {
    "Select Mode  \0"
    " 1. Active   \0"
    " 2. Low Power\0"
    " 3. Quit     \0"
};

// -------------- Power Test Menu---------------------------------

/**********************************************************************//**
 * @brief  Executes the "PowerTest" menu option in the User Experience
 *         example code. This menu option allows one to observe the
 *                 effects of Active mode versus different Low-Power Modes
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void menuPower(void)
{
    unsigned char PowermenuPos = 0;
    unsigned char quit = 0;
    unsigned int i;

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_UP + BUTTON_DOWN);
    halLcdClearScreen();

    halLcdPrintLine((char *)PowerText, 0, 0);

    buttonsPressed = 0;

    while (!quit)
    {
        for (i = 0; i < 4; i++)
        {
            halLcdPrintLine((char *)&PowerText[i * 14 + 14], i + 1,       \
                            OVERWRITE_TEXT);
            halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
        }

        while (!buttonsPressed) ;

        if (buttonsPressed & BUTTON_DOWN) //scroll down
        {
            halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (++PowermenuPos >= POWER_MENU_MAX)
            {
                PowermenuPos = 0;
            }
            halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_UP)   //scroll up
        {
            halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,      \
                            OVERWRITE_TEXT);
            if (PowermenuPos-- <= 0)
            {
                PowermenuPos = 2;
            }
            halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,       \
                            INVERT_TEXT | OVERWRITE_TEXT);
            buttonsPressed = 0;
        }

        if (buttonsPressed & BUTTON_SELECT)
        {
            switch (PowermenuPos)
            {
                case 0: menuPMMMCLK();    //Active Mode
                    halButtonsInterruptDisable(BUTTON_ALL);
                    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_UP + BUTTON_DOWN); break;
                case 1: menuLPM(); break; //Low-Power Mode
                case 2: quit = 1; break;
                default: break;
            }
            if (!quit)
            {
                halLcdClearScreen();
                halLcdPrintLine((char *)PowerText, 0, 0);
                halLcdPrintLine((char *)&PowerText[PowermenuPos * 14 + 14], PowermenuPos + 1,       \
                                INVERT_TEXT | OVERWRITE_TEXT);
            }
        }
        buttonsPressed = 0;
    }
}

