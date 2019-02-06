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
// Data logger routines.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"
#include "flash.h"
#include "ports.h"

// logic
#include "datalog.h"
#include "date.h"
#include "clock.h"
#include "altitude.h"
#include "temperature.h"
#include "bluerobin.h"

// *************************************************************************************************
// Prototypes section
void start_datalog(void);
void stop_datalog(void);
void datalog_sm(u8 * data, u8 len, u8 cmd);

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section
struct datalog sDatalog;

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          reset_datalog
// @brief       Reset data logger memory and init variables.
// @param       none
// @return      none
// *************************************************************************************************
void reset_datalog(void)
{
    u8 i;

    // Clear data logger memory
    for (i = DATALOG_PAGE_START; i <= DATALOG_PAGE_END; i++)
    {
        flash_erase_page(i);
    }

    sDatalog.flags.all = 0;
    sDatalog.mode = DATALOG_MODE_TEMPERATURE + DATALOG_MODE_ALTITUDE;
    sDatalog.interval = DATALOG_INTERVAL;
    sDatalog.delay = 0;
    sDatalog.wptr = (u16 *) DATALOG_MEMORY_START;
    sDatalog.idx = 0;
}

// *************************************************************************************************
// @fn          sx_alarm
// @brief       Sx button turns alarm on/off.
// @param       u8 line         LINE1
// @return      none
// *************************************************************************************************
void sx_datalog(u8 line)
{
    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    // Toggle data logger state
    if (!sDatalog.flags.flag.on)
    {
        if (!sDatalog.flags.flag.memory_full)
        {
            // Turn on data logger
            start_datalog();
        }
        else                    // Memory full
        {
            // Show "nomem" message
            message.flag.prepare = 1;
            message.flag.type_nomem = 1;
        }
    }
    else
    {
        // Turn off data logger
        stop_datalog();
    }
}

// *************************************************************************************************
// @fn          start_datalog
// @brief       Begin to log data.
// @param       none
// @return      none
// *************************************************************************************************
void start_datalog(void)
{
    // Start BlueRobin RX
    if ((sDatalog.mode & DATALOG_MODE_HEARTRATE) != 0)
    {
        // Keep existing connection
        if (!is_bluerobin())
        {
            // Start BlueRobin
            if (!start_bluerobin())
            {
                // No connection established? -> Close stack
                stop_bluerobin();
            }
        }
    }

    // Start pressure measurement
    if ((sDatalog.mode & (DATALOG_MODE_TEMPERATURE | DATALOG_MODE_ALTITUDE)) != 0)
    {
        // Start altitude measurement
        start_altitude_measurement();
    }

    // Set datalogger icon
    display_symbol(LCD_ICON_RECORD, SEG_ON_BLINK_OFF);

    // Start data logging
    datalog_sm(NULL, 0, DATALOG_CMD_START);
}

// *************************************************************************************************
// @fn          stop_datalog
// @brief       Stop data logging.
// @param       none
// @return      none
// *************************************************************************************************
void stop_datalog(void)
{
    if ((sDatalog.mode & DATALOG_MODE_HEARTRATE) != 0)
    {
        // Stop BlueRobin connection
        if (is_bluerobin())
            stop_bluerobin();
    }

    // Stop data logging and write out buffer
    datalog_sm(NULL, 0, DATALOG_CMD_CLOSE);

    if ((sDatalog.mode & (DATALOG_MODE_TEMPERATURE | DATALOG_MODE_ALTITUDE)) != 0)
    {
        // Stop altitude measurement
        stop_altitude_measurement();
    }

    // Clear datalogger icon
    display_symbol(LCD_ICON_RECORD, SEG_OFF_BLINK_OFF);
}

u8 is_datalog(void)
{
    return (sDatalog.flags.flag.on);
}

// *************************************************************************************************
// @fn          display_datalog
// @brief       Display data logger information.
// @param       u8 line LINE1, LINE2
//                              u8 update       DISPLAY_LINE_UPDATE_FULL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void display_datalog(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        display_chars(LCD_SEG_L2_4_0, (u8 *) " DLOG", SEG_ON);
    }
}

// *************************************************************************************************
// @fn          do_datalog
// @brief       Add data to data logging buffer. Called by second tick.
// @param       none
// @return      none
// *************************************************************************************************
void do_datalog(void)
{
    u8 temp[4];
    u8 count;

    // If logging delay is over, add new data
    if (--sDatalog.delay == 0)
    {
        // Store data when possible compressed (heartrate = 8 bits, temperature/altitude = min. 12
        // bits)
        if (sDatalog.mode == DATALOG_MODE_HEARTRATE)
        {
            temp[0] = sBlueRobin.heartrate;
            count = 1;
        }
        else if (sDatalog.mode == DATALOG_MODE_TEMPERATURE)
        {
            temp[0] = (sAlt.temperature_C >> 8) & 0xFF;
            temp[1] = sAlt.temperature_C & 0xFF;
            count = 2;
        }
        else if (sDatalog.mode == DATALOG_MODE_ALTITUDE)
        {
            temp[0] = (sAlt.altitude >> 8) & 0xFF;
            temp[1] = sAlt.altitude & 0xFF;
            count = 2;
        }
        else if (sDatalog.mode ==
                 (DATALOG_MODE_HEARTRATE | DATALOG_MODE_TEMPERATURE | DATALOG_MODE_ALTITUDE))
        {
            temp[0] = sBlueRobin.heartrate;
            temp[1] = (sAlt.temperature_C >> 4) & 0xFF;
            temp[2] = ((sAlt.temperature_C << 4) & 0xF0) | ((sAlt.altitude >> 8) & 0x0F);
            temp[3] = sAlt.altitude & 0xFF;
            count = 4;
        }
        else if (sDatalog.mode == (DATALOG_MODE_HEARTRATE | DATALOG_MODE_TEMPERATURE))
        {
            temp[0] = sBlueRobin.heartrate;
            temp[1] = (sAlt.temperature_C >> 8) & 0xFF;
            temp[2] = sAlt.temperature_C & 0xFF;
            count = 3;
        }
        else if (sDatalog.mode == (DATALOG_MODE_HEARTRATE | DATALOG_MODE_ALTITUDE))
        {
            temp[0] = sBlueRobin.heartrate;
            temp[1] = (sAlt.altitude >> 8) & 0xFF;
            temp[2] = sAlt.altitude & 0xFF;
            count = 3;
        }
        else if (sDatalog.mode == (DATALOG_MODE_TEMPERATURE | DATALOG_MODE_ALTITUDE))
        {
            temp[0] = (sAlt.temperature_C >> 4) & 0xFF;
            temp[1] = ((sAlt.temperature_C << 4) & 0xF0) | ((sAlt.altitude >> 8) & 0x0F);
            temp[2] = sAlt.altitude & 0xFF;
            count = 3;
        }

        // Add data to recording buffer
        datalog_sm((u8 *) &temp, count, DATALOG_CMD_ADD_DATA);

        // Reset delay counter
        sDatalog.delay = sDatalog.interval;

        // Write to flash if buffer is over write threshold and no BlueRobin event is close
        datalog_sm(NULL, 0, DATALOG_CMD_WRITE_DATA);
    }
}

// *************************************************************************************************
// @fn          datalog_add_to_buffer
// @brief       Add byte-data to data logging buffer
// @param       u8 * data               Pointer to byte-data
//                              u8 len                  Byte count
// @return      none
// *************************************************************************************************
void datalog_add_to_buffer(u8 * data, u8 len)
{
    u8 i;

    for (i = 0; i < len; i++)
    {
        // Copy values to buffer
        if (sDatalog.idx < DATALOG_BUFFER_SIZE)
        {
            sDatalog.buffer[sDatalog.idx++] = *(data + i);
        }
    }
}

// *************************************************************************************************
// Write buffer content to flash
// *************************************************************************************************
void datalog_write_buffer(void)
{
    u8 i = 0;
    u16 data;
    u8 eom = 0;
    volatile u16 temp;

    // Check if we cross end of memory threshold with this buffer write
    if (sDatalog.wptr >= (u16 *) (DATALOG_MEMORY_END - 1 - sDatalog.idx))
    {
        // Correct index to only write to end of memory
        // Leave 2 bytes for session end marker
        temp = (u16) sDatalog.wptr;
        sDatalog.idx = (u8) ((u16) DATALOG_MEMORY_END - 1 - temp);
        eom = 1;
    }

    // Write buffer content to flash
    while (i < sDatalog.idx - 1)
    {
        // Keep array order when writing to flash memory
        data = sDatalog.buffer[i++];
        data += (u16) (sDatalog.buffer[i++] << 8);

        // Write 16-bit word to flash
        flash_write(sDatalog.wptr++, data);
    }

    // Stop data logging and write session end marker
    if (eom)
    {
        // Write end marker
        flash_write((u16 *) (DATALOG_MEMORY_END - 1), 0xFFFE);
        // Clear buffer index
        sDatalog.idx = 0;
        // Clear flags
        sDatalog.flags.flag.on = 0;
        sDatalog.flags.flag.memory_full = 1;
        // Clear datalogger icon
        display_symbol(LCD_ICON_RECORD, SEG_OFF_BLINK_OFF);
    }
    else
    {
        // If index was odd number, 1 byte remains in buffer and must be written next time
        if ((sDatalog.idx & 0x01) == 0x01)
        {
            sDatalog.buffer[0] = sDatalog.buffer[sDatalog.idx - 1];
            sDatalog.idx = 1;
        }
        else                    // All bytes haven been written
        {
            sDatalog.idx = 0;
        }
    }
}

// *************************************************************************************************
// @fn          datalog_sm
// @brief       Data logging state machine
// @param       u8 line LINE1, LINE2
//                              u8 update       DISPLAY_LINE_UPDATE_FULL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void datalog_sm(u8 * data, u8 len, u8 cmd)
{
    u8 i;
    u16 temp;

    switch (cmd)
    {
        case DATALOG_CMD_START:
            if (!sDatalog.flags.flag.on && !sDatalog.flags.flag.memory_full)
            {
                // Clear index
                sDatalog.idx = 0;
                sDatalog.delay = sDatalog.interval;

                // Add session begin marker to buffer (2 byte)
                temp = 0xFFFB;
                datalog_add_to_buffer((u8 *) &temp, 2);

                // Add recording mode to buffer (1 byte)
                datalog_add_to_buffer((u8 *) &sDatalog.mode, 1);

                // Add recording interval to buffer (1 byte)
                datalog_add_to_buffer((u8 *) &sDatalog.interval, 1);

                // Add date to buffer (DD.MM.YYYY) (4 bytes)
                datalog_add_to_buffer((u8 *) &sDate.day, 1);
                datalog_add_to_buffer((u8 *) &sDate.month, 1);
                datalog_add_to_buffer((u8 *) &sDate.year, 2);

                // Add system time to buffer (HH.MM.SS) (3 bytes)
                datalog_add_to_buffer((u8 *) &sTime.hour, 3);

                // Data logging has started
                sDatalog.flags.flag.on = 1;
            }
            break;

        case DATALOG_CMD_CLOSE:
            if (sDatalog.flags.flag.on && !sDatalog.flags.flag.memory_full)
            {
                // If index is odd, add a dummy byte before writing session end marker
                if ((sDatalog.idx & 0x01) == 0x01)
                {
                    temp = 0x00;
                    datalog_add_to_buffer((u8 *) &temp, 1);
                }
                // Add session end marker to buffer (2 byte)
                temp = 0xFFFE;
                datalog_add_to_buffer((u8 *) &temp, 2);
                // Write buffer to flash
                datalog_write_buffer();
            }
            sDatalog.flags.flag.on = 0;
            break;

        case DATALOG_CMD_ADD_DATA:
            if (sDatalog.flags.flag.on && !sDatalog.flags.flag.memory_full)
            {
                datalog_add_to_buffer(data, len);
            }
            break;

        case DATALOG_CMD_WRITE_DATA:
            if (sDatalog.flags.flag.on && !sDatalog.flags.flag.memory_full)
            {
                // Over write threshold?
                if (sDatalog.idx > DATALOG_BUFFER_WRITE_THRESHOLD)
                {
                    // BlueRobin ISR call more than ~13ms away?
                    if (is_bluerobin_flash_write_window())
                        datalog_write_buffer();
                }
            }
            break;

        case DATALOG_CMD_ERASE:
            if (!sDatalog.flags.flag.on)
            {
                for (i = DATALOG_PAGE_START; i <= DATALOG_PAGE_END; i++)
                {
                    flash_erase_page(i);
                }
            }
            break;

    }
}

