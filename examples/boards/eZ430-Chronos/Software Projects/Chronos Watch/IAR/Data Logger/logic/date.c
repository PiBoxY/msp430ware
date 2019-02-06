// *************************************************************************************************
//
//      Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
//
//
//        Redistribution and use in source and binary forms, with or without
//        modification, are permitted provided that the following conditions
//        are met:
//
//          Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//
//          Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the
//          distribution.
//
//          Neither the name of Texas Instruments Incorporated nor the names of
//          its contributors may be used to endorse or promote products derived
//          from this software without specific prior written permission.
//
//        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//        LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//        THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *************************************************************************************************
// Date functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"
#include "ports.h"

// logic
#include "date.h"
#include "user.h"

// *************************************************************************************************
// Prototypes section
void reset_date(void);
u8 get_numberOfDays(u8 month, u16 year);
void add_day(void);
void mx_date(u8 line);
void sx_date(u8 line);
void display_date(u8 line, u8 update);

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section
struct date sDate;

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          reset_date
// @brief       Reset date to start value.
// @param       none
// @return      none
// *************************************************************************************************
void reset_date(void)
{
    // Set date
    sDate.year = 2009;
    sDate.month = 8;
    sDate.day = 1;

    // Show day and month on display
    sDate.display = DISPLAY_DEFAULT_VIEW;
}

// *************************************************************************************************
// @fn          get_NumberOfDays
// @brief       Return number of days for a given month
// @param       month           month as char
//                              year            year as int
// @return                              day count for given month
// *************************************************************************************************
u8 get_numberOfDays(u8 month, u16 year)
{
    switch (month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return (31);

        case 4:
        case 6:
        case 9:
        case 11:
            return (30);

        // 1. A year that is divisible by 4 is a leap year. (Y % 4) == 0
        // 2. Exception to rule 1: a year that is divisible by 100 is not a leap year. (Y % 100) !=
        // 0
        // 3. Exception to rule 2: a year that is divisible by 400 is a leap year. (Y % 400) == 0

        case 2:
            if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
                return (29);
            else
                return (28);

        default:
            return (0);
    }
}

// *************************************************************************************************
// @fn          add_day
// @brief       Add one day to current date. Called when clock changes from 23:59 to 00:00
// @param       none
// @return      none
// *************************************************************************************************
void add_day(void)
{
    // Add 1 day
    sDate.day++;

    // Check if day overflows into next month
    if (sDate.day > get_numberOfDays(sDate.month, sDate.year))
    {
        // Add 1 month and reset to day to 1
        sDate.day = 1;
        sDate.month++;

        // Check if month overflows into next year
        if (sDate.month > 12)
        {
            // Add 1 year and reset month and day to 1
            sDate.day = 1;
            sDate.month = 1;
            sDate.year++;
        }
    }

    // Indicate to display function that new value is available
    display.flag.full_update = 1;
}

// *************************************************************************************************
// @fn          sx_date
// @brief       Date user routine. Toggles view between DD.MM and YYYY.
// @param       line            LINE1, LINE2
// @return      none
// *************************************************************************************************
void sx_date(u8 line)
{
    // Toggle display items
    if (sDate.display == DISPLAY_DEFAULT_VIEW)
        sDate.display = DISPLAY_ALTERNATIVE_VIEW;
    else
        sDate.display = DISPLAY_DEFAULT_VIEW;
}

// *************************************************************************************************
// @fn          display_date
// @brief       Display date in DD.MM format (metric units) or MM.DD (English units).
// @param       u8 line                 LINE1, LINE2
//                              u8 update               DISPLAY_LINE_UPDATE_FULL,
// DISPLAY_LINE_UPDATE_PARTIAL
// @return      none
// *************************************************************************************************
void display_date(u8 line, u8 update)
{
    u8 *str;

    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        if (sDate.display == DISPLAY_DEFAULT_VIEW)
        {
            // Convert day to string
            str = int_to_array(sDate.day, 2, 0);
            if (sys.flag.use_metric_units)
            {
                display_chars(switch_seg(line, LCD_SEG_L1_3_2, LCD_SEG_L2_3_2), str, SEG_ON);
            }
            else
            {
                display_chars(switch_seg(line, LCD_SEG_L1_1_0, LCD_SEG_L2_1_0), str, SEG_ON);
            }

            // Convert month to string
            str = int_to_array(sDate.month, 2, 0);
            if (sys.flag.use_metric_units)
            {
                display_chars(switch_seg(line, LCD_SEG_L1_1_0, LCD_SEG_L2_1_0), str, SEG_ON);
            }
            else
            {
                display_chars(switch_seg(line, LCD_SEG_L1_3_2, LCD_SEG_L2_3_2), str, SEG_ON);
            }

            // Display "." to separate day and month
            display_symbol(switch_seg(line, LCD_SEG_L1_DP1, LCD_SEG_L2_DP), SEG_ON);
        }
        else
        {
            // Convert year to string
            str = int_to_array(sDate.year, 4, 0);
            display_chars(switch_seg(line, LCD_SEG_L1_3_0, LCD_SEG_L2_3_0), str, SEG_ON);

            // Clear "."
            display_symbol(switch_seg(line, LCD_SEG_L1_DP1, LCD_SEG_L2_DP), SEG_OFF);
        }
    }
    else if (update == DISPLAY_LINE_CLEAR)
    {
        // Show day and month on display when coming around next time
        sDate.display = DISPLAY_DEFAULT_VIEW;
    }
}

