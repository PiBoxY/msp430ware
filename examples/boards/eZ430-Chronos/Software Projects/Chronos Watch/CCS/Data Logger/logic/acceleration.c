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
// Acceleration measurement functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"
#include "bmp_as.h"
#include "cma_as.h"
#include "as.h"

// logic
#include "acceleration.h"
#include "simpliciti.h"
#include "user.h"

// *************************************************************************************************
// Global Variable section
struct accel sAccel;

// Conversion values from data to mgrav taken from BMA250 datasheet (rev 1.05, figure 4)
	const u16 bmp_mgrav_per_bit[7] = { 16, 31, 63, 125, 250, 500, 1000 };
// Conversion values from data to mgrav taken from CMA3000-D0x datasheet (rev 0.4, table 4)
	const u16 cma_mgrav_per_bit[7] = { 18, 36, 71, 143, 286, 571, 1142 };

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          reset_acceleration
// @brief       Reset acceleration variables.
// @param       none
// @return      none
// *************************************************************************************************
void reset_acceleration(void)
{
}

// *************************************************************************************************
// @fn          acceleration_value_is_positive
// @brief       Returns 1 if 2's complement number is positive
// @param       u8 value        2's complement number
// @return      u8                      1 = number is positive, 0 = number is negavtive
// *************************************************************************************************
u8 acceleration_value_is_positive(u8 value)
{
    return ((value & BIT7) == 0);
}

// *************************************************************************************************
// @fn          convert_acceleration_value_to_mgrav
// @brief       Converts measured value to mgrav units
// @param       u8 value        g data from sensor
// @return      u16                     Acceleration (mgrav)
// *************************************************************************************************
u16 convert_acceleration_value_to_mgrav(u8 value)
{
    u16 result;
    u8 i;

    if (!acceleration_value_is_positive(value))
    {
        // Convert 2's complement negative number to positive number
        value = ~value;
        value += 1;
    }

    result = 0;
    for (i = 0; i < 7; i++)
    {
    	if (bmp_used)
    	{
            result += ((value & (BIT(i))) >> i) * bmp_mgrav_per_bit[i];
    	}
    	else
    	{
            result += ((value & (BIT(i))) >> i) * cma_mgrav_per_bit[i];
    	}
    }

    return (result);
}
