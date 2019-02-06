/*******************************************************************************
 *
 *  balanceBall.c
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

/**********************************************************************//**
 * @brief  Executes the "UniBall" menu option of the User Experience
 *         example software.
 *
 * Uses the accelerometer to track a ball that erases the TI logo as it
 * moves around the screen. The application times out and resets the TI
 * logo periodically.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#include "hal_MSP-EXP430F5438.h"
#include "UserExperience_F5438A.h"
#include "balanceBall.h"

void balanceBall(void)
{
    int x, y, dx, dy, dz, redrawCounter;
    unsigned char quit = 0;

    halAccelerometerInit();

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT);
    buttonsPressed  = 0;

    //Starting location of the Ball
    x = 80;
    y = 16;

    halLcdClearScreen();
    halLcdImage(TI_BUG, 14, 106, 10, 0);

    RTCExit64Hz = 1;
    RTCPS0CTL |= RT0PSIE;
    redrawCounter = 0;
    halAdcSetQuitFromISR(0);

    while (!quit)
    {
        halAdcStartRead();

        __bis_SR_register(LPM0_bits + GIE);
        __no_operation();

        if (buttonsPressed & BUTTON_SELECT || buttonsPressed & BUTTON_S1)
            quit = 1;

        halAccelerometerRead(&dx, &dy, &dz);
        dx >>= 5;
        dy >>= 5;
        if ((ABS(dx) > 2) || (ABS(dy) > 2))
        {
            halLcdCircle(x, y, 7, PIXEL_OFF);
            halLcdCircle(x, y, 6, PIXEL_OFF);
            halLcdCircle(x, y, 5, PIXEL_OFF);
            halLcdCircle(x, y, 4, PIXEL_OFF);
            halLcdCircle(x, y, 3, PIXEL_OFF);
            halLcdCircle(x, y, 2, PIXEL_OFF);
            halLcdCircle(x, y, 1, PIXEL_OFF);
            x -= dx;
            y += dy;

            if (x < 0 || x >= LCD_COL)
                x += dx;
            if (y < 0 || y >= LCD_ROW)
                y -= dy;

            halLcdCircle(x, y, 2, PIXEL_ON);
            halLcdCircle(x, y, 3, PIXEL_ON);
            halLcdCircle(x, y, 4, PIXEL_ON);
            halLcdCircle(x, y, 5, PIXEL_ON);
            halLcdCircle(x, y, 6, PIXEL_ON);
        }
        if (++redrawCounter == 320)             //Redraw canvas after ~5s
        {
            halLcdClearScreen();
            halLcdImage(TI_BUG, 14, 106, 10, 0);
            redrawCounter = 0;
            halLcdCircle(x, y, 2, PIXEL_ON);
            halLcdCircle(x, y, 3, PIXEL_ON);
            halLcdCircle(x, y, 4, PIXEL_ON);
            halLcdCircle(x, y, 5, PIXEL_ON);
            halLcdCircle(x, y, 6, PIXEL_ON);
        }
    }

    RTCPS0CTL &= ~RT0PSIE;
    RTCExit64Hz = 0;
}

