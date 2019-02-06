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
// Temperature measurement functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "temperature.h"
#include "display.h"

// logic
#include "altitude.h"

// *************************************************************************************************
// Prototypes section
s16 convert_C_to_F(s16 value);
s16 convert_F_to_C(s16 value);

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          convert_C_to_F
// @brief       Convert °C to °F
// @param       s16 value               Temperature in °C
// @return      s16                     Temperature in °F
// *************************************************************************************************
s16 convert_C_to_F(s16 value)
{
    s16 DegF;

    // Celsius in Fahrenheit = (( TCelsius × 9 ) / 5 ) + 32
    DegF = ((value * 9 * 10) / 5 / 10) + 32 * 10;

    return (DegF);
}

// *************************************************************************************************
// @fn          convert_F_to_C
// @brief       Convert °F to °C
// @param       s16 value               Temperature in 2.1 °F
// @return      s16                     Temperature in 2.1 °C
// *************************************************************************************************
s16 convert_F_to_C(s16 value)
{
    s16 DegC;

    // TCelsius =( TFahrenheit - 32 ) × 5 / 9
    DegC = (((value - 320) * 5)) / 9;

    return (DegC);
}

// *************************************************************************************************
// @fn          display_temperature
// @brief       Common display routine for metric and English units.
// @param       u8 line                 LINE1
//                              u8 update               DISPLAY_LINE_UPDATE_FULL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void display_temperature(u8 line, u8 update)
{
    u8 *str;
    s16 temperature;

    // Redraw whole screen
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        // Start measurement
        start_altitude_measurement();

        // Display °C / °F
        display_symbol(LCD_SEG_L1_DP1, SEG_ON);
        display_symbol(LCD_UNIT_L1_DEGREE, SEG_ON);
        if (sys.flag.use_metric_units)
            display_char(LCD_SEG_L1_0, 'C', SEG_ON);
        else
            display_char(LCD_SEG_L1_0, 'F', SEG_ON);

        // Display temperature
        display_temperature(LINE1, DISPLAY_LINE_UPDATE_PARTIAL);
    }
    else if (update == DISPLAY_LINE_UPDATE_PARTIAL)
    {
        // When using English units, convert °C to °F (temp*1.8+32)
        if (!sys.flag.use_metric_units)
        {
            temperature = convert_C_to_F(sAlt.temperature_C);
        }
        else
        {
            temperature = sAlt.temperature_C;
        }

        // Indicate temperature sign through arrow up/down icon
        if (temperature < 0)
        {
            // Convert negative to positive number
            temperature = ~temperature;
            temperature += 1;

            display_symbol(LCD_SYMB_ARROW_UP, SEG_OFF);
            display_symbol(LCD_SYMB_ARROW_DOWN, SEG_ON);
        }
        else                    // Temperature is >= 0
        {
            display_symbol(LCD_SYMB_ARROW_UP, SEG_ON);
            display_symbol(LCD_SYMB_ARROW_DOWN, SEG_OFF);
        }

        // Display result in xx.x format
        str = int_to_array(temperature, 3, 1);
        display_chars(LCD_SEG_L1_3_1, str, SEG_ON);
    }
    else if (update == DISPLAY_LINE_CLEAR)
    {
        // Stop measurement
        stop_altitude_measurement();

        // Clean up function-specific segments before leaving function
        display_symbol(LCD_SYMB_ARROW_UP, SEG_OFF);
        display_symbol(LCD_SYMB_ARROW_DOWN, SEG_OFF);
        display_symbol(LCD_UNIT_L1_DEGREE, SEG_OFF);
        display_symbol(LCD_SEG_L1_DP1, SEG_OFF);
    }
}

