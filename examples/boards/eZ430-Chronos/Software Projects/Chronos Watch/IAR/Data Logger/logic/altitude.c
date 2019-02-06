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
// Altitude measurement functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "altitude.h"
#include "display.h"
#include "ps.h"
#include "bmp_ps.h"
#include "cma_ps.h"
#include "ports.h"
#include "timer.h"

// logic
#include "user.h"
#include "datalog.h"

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section
struct alt sAlt;

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          reset_altitude_measurement
// @brief       Reset altitude measurement.
// @param       none
// @return      none
// *************************************************************************************************
void reset_altitude_measurement(void)
{
    // Set default values
    sAlt.on = 0;
    sAlt.altitude = 0;
    sAlt.temperature_C = 0;
    sAlt.temperature_C_offset = 0;

    // Pressure sensor ok?
    if (ps_ok)
    {
        // Initialise pressure table
        init_pressure_table();

        // Do single conversion
        start_altitude_measurement();
        stop_altitude_measurement();

        // Apply calibration offset and recalculate pressure table
        if (sAlt.altitude_offset != 0)
        {
            sAlt.altitude += sAlt.altitude_offset;
            update_pressure_table(sAlt.altitude, sAlt.pressure, sAlt.temperature_K);
        }
    }
}

// *************************************************************************************************
// @fn          conv_m_to_ft
// @brief       Convert meters to feet
// @param       u16 m           Meters
// @return      u16             Feet
// *************************************************************************************************
s16 convert_m_to_ft(s16 m)
{
    return (((s32) 328 * m) / 100);
}

// *************************************************************************************************
// @fn          conv_ft_to_m
// @brief       Convert feet to meters
// @param       u16 ft          Feet
// @return      u16             Meters
// *************************************************************************************************
s16 convert_ft_to_m(s16 ft)
{
    return (((s32) ft * 61) / 200);
}

// *************************************************************************************************
// @fn          start_altitude_measurement
// @brief       Start altitude measurement
// @param       none
// @return      none
// *************************************************************************************************
void start_altitude_measurement(void)
{
    u8 timeout = 15;

    // Already on?
    if (sAlt.on)
        return;

    // Show warning if pressure sensor was not initialised properly
    if (!ps_ok)
    {
        display_chars(LCD_SEG_L1_2_0, (u8 *) "ERR", SEG_ON);
        return;
    }

    // Enable DRDY IRQ on rising edge
    PS_INT_IFG &= ~PS_INT_PIN;
    PS_INT_IE |= PS_INT_PIN;

    // Start pressure sensor
	if (bmp_used)
	{
        bmp_ps_start();
	}
	else
	{
        cma_ps_start();
	}

    // Set altitude measurement flag
    sAlt.on = 1;

    // Get updated altitude
    while (((PS_INT_IN & PS_INT_PIN) == 0) && (timeout > 0))
    {
        Timer0_A4_Delay(CONV_MS_TO_TICKS(100));
        timeout--;
    }

    // Failed to start?
    if (timeout == 0)
    {
        sAlt.on = 0;
    }

    do_altitude_measurement(FILTER_OFF);
}

// *************************************************************************************************
// @fn          stop_altitude_measurement
// @brief       Stop altitude measurement
// @param       none
// @return      none
// *************************************************************************************************
void stop_altitude_measurement(void)
{
    // Return if pressure sensor was not initialised properly
    if (!ps_ok)
        return;

    // Not on?
    if (!sAlt.on)
        return;

    // Logging data?
    if (is_datalog())
        return;

    // Stop pressure sensor
	if (bmp_used)
	{
        bmp_ps_stop();
	}
	else
	{
        cma_ps_stop();
	}

    // Clear on flag
    sAlt.on = 0;

    // Disable DRDY IRQ
    PS_INT_IE &= ~PS_INT_PIN;
    PS_INT_IFG &= ~PS_INT_PIN;
}

// *************************************************************************************************
// @fn          is_altitude_measurement
// @brief       Altitude measurement check
// @param       none
// @return      u8              1=Measurement ongoing, 0=measurement off
// *************************************************************************************************
u8 is_altitude_measurement(void)
{
    return (sAlt.on);
}

// *************************************************************************************************
// @fn          do_altitude_measurement
// @brief       Perform single altitude measurement
// @param       u8 filter       Filter option
// @return      none
// *************************************************************************************************
void do_altitude_measurement(u8 filter)
{
    volatile u32 pressure;

    // If sensor is not ready, skip data read
    if ((PS_INT_IN & PS_INT_PIN) == 0)
        return;

    // Get temperature (format is *10 K) from sensor
	if (bmp_used)
	{
        sAlt.temperature_K = bmp_ps_get_temp();
	}
	else
	{
        sAlt.temperature_K = cma_ps_get_temp();
	}
    sAlt.temperature_C = sAlt.temperature_K - 2721 + sAlt.temperature_C_offset;

    // Get pressure (format is 1Pa) from sensor
	if (bmp_used)
	{
	    // Start sampling data in ultra low power mode
	    bmp_ps_write_register(BMP_085_CTRL_MEAS_REG, BMP_085_P_MEASURE);
	    // Get updated altitude
	    while ((PS_INT_IN & PS_INT_PIN) == 0) ;

	    pressure = bmp_ps_get_pa();
	}
	else
	{
        pressure = cma_ps_get_pa();
	}

    // Store measured pressure value
    if (filter == FILTER_OFF)
    {
        sAlt.pressure = pressure;
    }
    else
    {
        // Filter current pressure
        pressure = (u32) ((pressure * 0.2) + (sAlt.pressure * 0.8));

        // Store average pressure
        sAlt.pressure = pressure;
    }

    // Convert pressure (Pa) and temperature ( K) to altitude (m)
    sAlt.altitude = conv_pa_to_meter(sAlt.pressure, sAlt.temperature_K);
}

// *************************************************************************************************
// @fn          display_altitude
// @brief       Display routine. Supports display in meters and feet.
// @param       u8 line                 LINE1
//                              u8 update               DISPLAY_LINE_UPDATE_FULL,
// DISPLAY_LINE_UPDATE_PARTIAL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void display_altitude(u8 line, u8 update)
{
    u8 *str;
    s16 ft;

    // Start measurement
    start_altitude_measurement();

    // redraw whole screen
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        if (sys.flag.use_metric_units)
        {
            // Display "m" symbol
            display_symbol(LCD_UNIT_L1_M, SEG_ON);
        }
        else
        {
            // Display "ft" symbol
            display_symbol(LCD_UNIT_L1_FT, SEG_ON);
        }

        // Display altitude
        display_altitude(LINE1, DISPLAY_LINE_UPDATE_PARTIAL);
    }
    else if (update == DISPLAY_LINE_UPDATE_PARTIAL)
    {
        if (sys.flag.use_metric_units)
        {
            // Display altitude in xxxx m format, allow 3 leading blank digits
            if (sAlt.altitude >= 0)
            {
                str = int_to_array(sAlt.altitude, 4, 3);
                display_symbol(LCD_SYMB_ARROW_UP, SEG_ON);
                display_symbol(LCD_SYMB_ARROW_DOWN, SEG_OFF);
            }
            else
            {
                str = int_to_array(sAlt.altitude * (-1), 4, 3);
                display_symbol(LCD_SYMB_ARROW_UP, SEG_OFF);
                display_symbol(LCD_SYMB_ARROW_DOWN, SEG_ON);
            }
        }
        else
        {
            // Convert from meters to feet
            ft = convert_m_to_ft(sAlt.altitude);

            // Limit to 9999ft (3047m)
            if (ft > 9999)
                ft = 9999;

            // Display altitude in xxxx ft format, allow 3 leading blank digits
            if (ft >= 0)
            {
                str = int_to_array(ft, 4, 3);
                display_symbol(LCD_SYMB_ARROW_UP, SEG_ON);
                display_symbol(LCD_SYMB_ARROW_DOWN, SEG_OFF);
            }
            else
            {
                str = int_to_array(ft * (-1), 4, 3);
                display_symbol(LCD_SYMB_ARROW_UP, SEG_OFF);
                display_symbol(LCD_SYMB_ARROW_DOWN, SEG_ON);
            }
        }
        display_chars(LCD_SEG_L1_3_0, str, SEG_ON);
    }
    else if (update == DISPLAY_LINE_CLEAR)
    {
        // Stop measurement
        stop_altitude_measurement();

        // Clean up function-specific segments before leaving function
        display_symbol(LCD_UNIT_L1_M, SEG_OFF);
        display_symbol(LCD_UNIT_L1_FT, SEG_OFF);
        display_symbol(LCD_SYMB_ARROW_UP, SEG_OFF);
        display_symbol(LCD_SYMB_ARROW_DOWN, SEG_OFF);
    }
}

