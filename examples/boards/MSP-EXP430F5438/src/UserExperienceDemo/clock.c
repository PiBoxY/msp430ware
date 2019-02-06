/*******************************************************************************
 *
 *  clock.c
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
#include "clock.h"

#define HOUR10    (0)
#define HOUR01    (1)
#define COLON2    (2)
#define MINUTE10  (3)
#define MINUTE01  (4)
#define COLON1    (5)
#define SECOND10  (6)
#define SECOND01  (7)

#define TIME_OUT        10

const unsigned char cos[] = {40, 40, 39, 38, 36, 34, 32, 30,
                             28, 24, 20, 16, 12,  8,  4,  0,
                             32, 32, 31, 30, 28, 27, 25, 24,
                             22, 19, 16, 13, 10,  6,  3,  0,
                             32, 32, 31, 30, 28, 27, 25, 24,
                             22, 19, 16, 13, 10,  6,  3,  0,
                             24, 24, 23, 22, 21, 20, 19, 18,
                             16, 14, 12, 10,  7,  5,  2,  0};

char time[9] = {'0', '4', ':', '3', '0', ':', '0', '0', '\0'};

int second = 0, minute = 30, hour = 4;
int secondOld, minuteOld, hourOld;

int ClockY = (LCD_ROW >> 1) - 3;
int ClockX = LCD_COL >> 1;

/**********************************************************************//**
 * @brief  Adds one to the current hour when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void hourAdd(void)
{
    unsigned int hourBCD = GetRTCHOUR();

    if ((hourBCD & 0x0F) == 9)
    {
        hourBCD &= 0xF0;                    // low digit = 0
        hourBCD += 0x10;                    // Add 1 to high digit
        SetRTCHOUR(hourBCD);
    }
    else if ((hourBCD & 0x0F) == 0x03 && (hourBCD & 0xF0) == 0x20)
    {
        hourBCD = 0;
        SetRTCHOUR(hourBCD);
    }
    else
    {
        hourBCD++;
        SetRTCHOUR(hourBCD);
    }
}

/**********************************************************************//**
 * @brief  Adds one to the current minute when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void minuteAdd(void)
{
    unsigned int minuteBCD = GetRTCMIN();

    if ((minuteBCD & 0x0F) == 9)
    {
        minuteBCD &= 0xF0;                  // Low digit = 0
        if ((minuteBCD & 0xF0) == 0x50)     // If high digit = 5
            SetRTCMIN(0x00);                // Reset to 0
        else
        {
            minuteBCD += 0x10;              // Add 1 to high digit
            SetRTCMIN(minuteBCD);
        }
    }
    else
    {
        minuteBCD++;
        SetRTCMIN(minuteBCD);
    }
}

/**********************************************************************//**
 * @brief  Adds one to the current second when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void secondAdd(void)
{
    unsigned int secondBCD = GetRTCSEC();

    RTCCTL01 |= RTCHOLD;

    if ((secondBCD & 0x0F) == 9)
    {
        secondBCD &= 0xF0;                  // low digit = 0
        if ((secondBCD & 0xF0) == 0x50)     // if high digit = 5
        {
            SetRTCSEC(0x00);                // reset to 0
        }
        else
        {
            secondBCD += 0x10;
            SetRTCSEC(secondBCD);           // add 1 to high digit
        }
    }
    else
    {
        secondBCD++;
        SetRTCSEC(secondBCD);
    }

    RTCCTL01 &= ~RTCHOLD;
}

/**********************************************************************//**
 * @brief  Subtracts one from the current hour when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void hourSub(void)
{
    unsigned int hourBCD = GetRTCHOUR();

    if ((hourBCD & 0x0F) == 0x00)           // Is least-significant digit = 0?
    {
        if ((hourBCD & 0xF0) == 0x00)
        {
            hourBCD = 0x23;
            SetRTCHOUR(hourBCD);
        }
        else
        {
            hourBCD |= 0x09;                // Set lowest digit to '9'
            hourBCD -= 0x10;                // Subtract one from the lowest digit
            SetRTCHOUR(hourBCD);
        }
    }
    else
    {
        hourBCD--;
        SetRTCHOUR(hourBCD);
    }
}

/**********************************************************************//**
 * @brief  Subtracts one from the current minute when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void minuteSub(void)
{
    unsigned int minuteBCD = GetRTCMIN();

    if ((minuteBCD & 0x0F) == 0x00)
    {
        if (minuteBCD == 0)
            SetRTCMIN(0x59);
        else
        {
            minuteBCD |= 0x09;                   //low digit = 9
            minuteBCD -= 0x10;
            SetRTCMIN(minuteBCD);
        }
    }
    else
    {
        minuteBCD--;
        SetRTCMIN(minuteBCD);
    }
}

/**********************************************************************//**
 * @brief  Subtracts one from the current second when setting the time.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

static void secondSub(void)
{
    unsigned int secondBCD = GetRTCSEC();

    RTCCTL01 |= RTCHOLD;

    if ((secondBCD & 0x0F) == 0x00)
    {
        if (secondBCD == 0)
            SetRTCSEC(0x59);
        else
        {
            secondBCD |= 0x09;              //low digit = 9
            secondBCD -= 0x10;
            SetRTCSEC(secondBCD);
        }
    }
    else
    {
        secondBCD--;
        SetRTCSEC(secondBCD);
    }

    RTCCTL01 &= ~RTCHOLD;
}

/**********************************************************************//**
 * @brief  Updates the hours, minutes and seconds on the digital clock
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void updateDigitalClock(void)
{
    unsigned int hourBCD   = GetRTCHOUR();
    unsigned int minuteBCD = GetRTCMIN();
    unsigned int secondBCD = GetRTCSEC();

    time[HOUR10]   = '0' + (hourBCD   >> 4);
    time[HOUR01]   = '0' + (hourBCD   &  0x0F);
    time[MINUTE10] = '0' + (minuteBCD >> 4);
    time[MINUTE01] = '0' + (minuteBCD &  0x0F);
    time[SECOND10] = '0' + (secondBCD >> 4);
    time[SECOND01] = '0' + (secondBCD &  0x0F);
}

/**********************************************************************//**
 * @brief  Draws the updated digital clock.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void digitalClockDraw(void)
{
    updateDigitalClock();
    halLcdPrintLineCol(&time[HOUR10], 8, 0,  OVERWRITE_TEXT);
}

/**********************************************************************//**
 * @brief  Draws the specified hand on the analog clock.
 *
 * @param  time  The time to be represented by the hand (0 - 59)
 *
 * @param  pixel The grayscale intensity of the line
 *
 * @param  hand  Which hand is being drawn.
 *
 * - 1 - Second hand
 * - 2 - Minute hand
 * - 3 - Hour   hand
 *
 * @return none
 *************************************************************************/

void analogClockDrawHand(int time, unsigned char pixel, int hand)
{
    int handX, handY, i;

    if (time < 15)
    {
        handY = ClockY - cos[(hand << 4) +  time];
        handX = ClockX + cos[(hand << 4) + 15 - time];
    }
    else
    if (time < 30)
    {
        handY = ClockY + cos[(hand << 4) + 30 - time];
        handX = ClockX + cos[(hand << 4) + time - 15];
    }
    else
    if (time < 45)
    {
        handY = ClockY + cos[(hand << 4) + time - 30];
        handX = ClockX - cos[(hand << 4) + 45 - time];
    }
    else
    {
        handY =  ClockY - cos[(hand << 4) + 60 - time];
        handX =   ClockX - cos[(hand << 4) + time - 45];
    }
    for (i = -1; i < hand; i++)
        halLcdLine(ClockX + i, ClockY - i, handX + i, handY - i, pixel);
}

/**********************************************************************//**
 * @brief  Initializes the analog clock for display
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void analogClockInit(void)
{
    int i;

    halLcdClearScreen();
    halLcdCircle(LCD_COL >> 1, LCD_ROW >> 1, 49, PIXEL_ON);
    halLcdCircle(LCD_COL >> 1, LCD_ROW >> 1, 47, PIXEL_ON);

    for (i = 1; i < 12; i++)
        if ((i != 3) && (i != 6) && (i != 9))
        {
            analogClockDrawHand(i * 5, PIXEL_ON, 0);
            analogClockDrawHand(i * 5, PIXEL_OFF, 1);
        }
    halLcdClearImage(5, 40, ClockX - 20, ClockY - 20);
    halLcdPrintXY("1", 66, 11, 0);
    halLcdPrint("2", 0);
    halLcdPrintXY("3", 105, 46, 0);
    halLcdPrintXY("6", 70, 84,  0);
    halLcdPrintXY("9", 38, 46, 0);
}

/**********************************************************************//**
 * @brief  Updates the analog clock display due to a time event.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void analogClockUpdate(void)
{
    unsigned int hourBCD   = GetRTCHOUR();
    unsigned int minuteBCD = GetRTCMIN();
    unsigned int secondBCD = GetRTCSEC();

    second = secondBCD & 0xF0;              //high digit
    //value = 16*times - 4*times - 2 times = 10*times
    second = second - (second >> 2) - (second >> 3) + (secondBCD & 0x0F);

    minute = minuteBCD & 0xF0;              //high digit
    //value = 16*times - 4*times - 2 times = 10*times
    minute = minute - (minute >> 2) - (minute >> 3) + (minuteBCD & 0x0F);

    hour   = hourBCD & 0xF0;
    //value = 16*times - 4*times - 2 times = 10*times
    hour   = hour - (hour >> 2) - (hour >> 3) + (hourBCD & 0x0F);

    analogClockDrawHand(secondOld, PIXEL_OFF,  1);
    analogClockDrawHand(minuteOld, PIXEL_OFF,  2);
    analogClockDrawHand(hourOld * 5, PIXEL_OFF,  3);

    analogClockDrawHand(second, PIXEL_ON,  1);
    analogClockDrawHand(minute, PIXEL_ON,  2);

    if (hour >= 12)
        analogClockDrawHand((hour - 12) * 5, PIXEL_ON,  3);
    else
        analogClockDrawHand(hour * 5, PIXEL_ON,  3);

    secondOld = second;
    minuteOld = minute;
    hourOld   = hour;
}

/**********************************************************************//**
 * @brief  Executes the "Clock" menu option in the User Experience
 *         example code.
 *
 * - Initializes the analog clock.
 * - Sleeps between RTC events and until center button on the D-pad is pressed.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void displayAnalogClock(void)
{
    unsigned char counter;

    analogClockInit();

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT);
    RTCExitSec = 1;
    buttonsPressed = 0;
    counter = 0;

    while (!(buttonsPressed & BUTTON_SELECT))
    {
        buttonsPressed = 0;
        TA0CTL &= ~TAIFG;

        // Sleep until RTC event or center button on the D-pad is pressed
        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();                   // for debugger only

        analogClockUpdate();
        if (++counter > TIME_OUT)
            halLcdSetBackLight(0);
    }

    RTCExitSec = 0;
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
}

/**********************************************************************//**
 * @brief  Executes the user interface for setting the time.
 *
 * Uses the D-pad to move between hours, minutes, and seconds as well as
 * increments or decrements their set values.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void setTime(void)
{
    unsigned char cursorPosition = 0;

    RTCExitSec = 1;                        //While setting time, clock still runs in background
    halLcdClearScreen();

    halLcdPrintLineCol(&time[HOUR10],  5, 5, INVERT_TEXT | OVERWRITE_TEXT);
    halLcdPrintLineCol(&time[COLON2], 5, 7,  OVERWRITE_TEXT);

    halButtonsInterruptEnable(BUTTON_ALL);

    while (((buttonsPressed & BUTTON_SELECT) != BUTTON_SELECT) &&
           ((buttonsPressed & BUTTON_S1) != BUTTON_S1))
    {
        buttonsPressed = 0;

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();

        switch (buttonsPressed)
        {
            case BUTTON_LEFT:   if (cursorPosition == 0)
                    cursorPosition = 2;
                else
                    cursorPosition--;
                break;
            case BUTTON_RIGHT:  if (cursorPosition == 2)
                    cursorPosition = 0;
                else
                    cursorPosition++;
                break;

            case BUTTON_UP:     switch (cursorPosition)
                {
                    case 0: hourAdd(); break;
                    case 1: minuteAdd(); break;
                    case 2: secondAdd(); break;
                }
                break;
            case BUTTON_DOWN:   switch (cursorPosition)
                {
                    case 0: hourSub(); break;
                    case 1: minuteSub(); break;
                    case 2: secondSub(); break;
                }
                break;
        }
        updateDigitalClock();

        switch (cursorPosition)
        {
            case 0:
                halLcdPrintLineCol(&time[HOUR10],  5, 5, INVERT_TEXT | OVERWRITE_TEXT);
                halLcdPrintLineCol(&time[COLON2], 5, 7,  OVERWRITE_TEXT);
                break;
            case 1:
                halLcdPrintLineCol(&time[HOUR10],  5, 5,  OVERWRITE_TEXT);
                halLcdPrintLineCol(&time[MINUTE10], 5, 8, INVERT_TEXT | OVERWRITE_TEXT);
                halLcdPrintLineCol(&time[COLON1],  5, 10,  OVERWRITE_TEXT);
                break;
            case 2:
                halLcdPrintLineCol(&time[HOUR10],  5, 5,  OVERWRITE_TEXT);
                halLcdPrintLineCol(&time[SECOND10], 5, 11, INVERT_TEXT | OVERWRITE_TEXT);
                break;
        }
    }
    RTCExitSec = 0;
}

