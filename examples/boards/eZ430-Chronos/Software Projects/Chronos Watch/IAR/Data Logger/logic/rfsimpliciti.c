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
// SimpliciTI functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"
#include "ps.h"
#include "ports.h"
#include "timer.h"
#include "radio.h"
#include "flash.h"

// logic
#include "acceleration.h"
#include "rfsimpliciti.h"
#include "bluerobin.h"
#include "simpliciti.h"
#include "clock.h"
#include "date.h"
#include "temperature.h"
#include "altitude.h"
#include "datalog.h"

// *************************************************************************************************
// Prototypes section
void simpliciti_get_data_callback(void);
void start_simpliciti_sync(void);

// *************************************************************************************************
// Defines section

// Each packet index requires 2 bytes, so we can have 9 packet indizes in 18 bytes usable payload
#define BM_SYNC_BURST_PACKETS_IN_DATA           (9u)

// *************************************************************************************************
// Global Variable section
struct RFsmpl sRFsmpl;

// flag contains status information, trigger to send data and trigger to exit SimpliciTI
unsigned char simpliciti_flag;

// 4 data bytes to send
unsigned char simpliciti_data[SIMPLICITI_MAX_PAYLOAD_LENGTH];

// 4 byte device address overrides SimpliciTI end device address set in "smpl_config.dat"
unsigned char simpliciti_ed_address[4];

// 1 = send one or more reply packets, 0 = no need to reply
//unsigned char simpliciti_reply;
unsigned char simpliciti_reply_count;

// 1 = send packets sequentially from burst_start to burst_end, 2 = send packets addressed by their
// index
u8 burst_mode;

// Start and end index of packets to send out
u16 burst_start, burst_end;

// Array containing requested packets
u16 burst_packet[BM_SYNC_BURST_PACKETS_IN_DATA];

// Current packet index
u8 burst_packet_index;

// Byte-Pointer to flash memory
u8 *flash_ptr;

// *************************************************************************************************
// Extern section
extern void (*fptr_lcd_function_line1)(u8 line, u8 update);

// *************************************************************************************************
// @fn          reset_rf
// @brief       Reset SimpliciTI data.
// @param       none
// @return      none
// *************************************************************************************************
void reset_rf(void)
{
    // No connection
    sRFsmpl.mode = SIMPLICITI_OFF;
}

// *************************************************************************************************
// @fn          sx_sync
// @brief       Start SimpliciTI. Button DOWN connects/disconnects to access point.
// @param       u8 line         LINE2
// @return      none
// *************************************************************************************************
void sx_sync(u8 line)
{
    // Exit if battery voltage is too low for radio operation
    if (sys.flag.low_battery)
        return;

    // Exit if BlueRobin stack is active
    if (is_bluerobin())
        return;

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;
    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;

    // Start SimpliciTI in sync mode
    start_simpliciti_sync();

    BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;
}

// *************************************************************************************************
// @fn          display_sync
// @brief       SimpliciTI display routine.
// @param       u8 line                 LINE2
//                              u8 update               DISPLAY_LINE_UPDATE_FULL
// @return      none
// *************************************************************************************************
void display_sync(u8 line, u8 update)
{
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        display_chars(LCD_SEG_L2_5_0, (u8 *) "  SYNC", SEG_ON);
    }
}

// *************************************************************************************************
// @fn          is_rf
// @brief       Returns TRUE if SimpliciTI receiver is connected.
// @param       none
// @return      u8
// *************************************************************************************************
u8 is_rf(void)
{
    return (sRFsmpl.mode != SIMPLICITI_OFF);
}

// *************************************************************************************************
// @fn          start_simpliciti_sync
// @brief       Start SimpliciTI (sync mode).
// @param       none
// @return      none
// *************************************************************************************************
void start_simpliciti_sync(void)
{
    // Clear LINE1
    clear_line(LINE1);
    fptr_lcd_function_line1(LINE1, DISPLAY_LINE_CLEAR);

    // Stop data logging and close session
    stop_datalog();

    // Turn on beeper icon to show activity
    display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);

    // Prepare radio for RF communication
    open_radio();

    // Set SimpliciTI mode
    sRFsmpl.mode = SIMPLICITI_SYNC;

    // Set SimpliciTI timeout to save battery power
    sRFsmpl.timeout = SIMPLICITI_TIMEOUT;

    // Start SimpliciTI stack. Try to link to access point.
    // Exit with timeout or by a DOWN button press.
    if (simpliciti_link())
    {
        // Enter sync routine. This will send ready-to-receive packets at regular intervals to the
        // access point.
        // The access point always replies a command (NOP if no other command is set)
        simpliciti_main_sync();
    }

    // Set SimpliciTI state to OFF
    sRFsmpl.mode = SIMPLICITI_OFF;

    // Powerdown radio
    close_radio();

    // Clear last button events
    Timer0_A4_Delay(CONV_MS_TO_TICKS(BUTTONS_DEBOUNCE_TIME_OUT));
    BUTTONS_IFG = 0x00;
    button.all_flags = 0;

    // Clear icons
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);

    // Force full display update
    display.flag.full_update = 1;
}

// *************************************************************************************************
// @fn          simpliciti_sync_decode_ap_cmd_callback
// @brief       For SYNC mode only: Decode command from access point and trigger actions.
// @param       none
// @return      none
// *************************************************************************************************
void simpliciti_sync_decode_ap_cmd_callback(void)
{
    u8 i;
    s16 t1, offset;

    // Default behaviour is to send no reply packets
    simpliciti_reply_count = 0;

    switch (simpliciti_data[0])
    {
        case SYNC_AP_CMD_NOP:
            break;

        case SYNC_AP_CMD_GET_STATUS: // Send watch parameters
            simpliciti_data[0] = SYNC_ED_TYPE_STATUS;
            // Send single reply packet
            simpliciti_reply_count = 1;
            break;

        case SYNC_AP_CMD_SET_WATCH:  // Set watch parameters
            sys.flag.use_metric_units = (simpliciti_data[1] >> 7) & 0x01;
            sTime.hour = simpliciti_data[1] & 0x7F;
            sTime.minute = simpliciti_data[2];
            sTime.second = simpliciti_data[3];
            sDate.year = (simpliciti_data[4] << 8) + simpliciti_data[5];
            sDate.month = simpliciti_data[6];
            sDate.day = simpliciti_data[7];
            // Set temperature and temperature offset
            t1 = (s16) ((simpliciti_data[10] << 8) + simpliciti_data[11]);
            offset = t1 - (sAlt.temperature_C - sAlt.temperature_C_offset);
            sAlt.temperature_C_offset = offset;
            sAlt.temperature_C = t1;
            // Set altitude
            sAlt.altitude = (s16) ((simpliciti_data[12] << 8) + simpliciti_data[13]);
            update_pressure_table(sAlt.altitude, sAlt.pressure, sAlt.temperature_K);
            // Data logging mode
            sDatalog.mode = simpliciti_data[14];
            // Data logging interval
            sDatalog.interval = simpliciti_data[15];

            display_chars(LCD_SEG_L2_5_0, (u8 *) "  DONE", SEG_ON);
            sRFsmpl.display_sync_done = 1;
            break;

        case SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_1:
            // Send sequential packets out in a burst
            simpliciti_data[0] = SYNC_ED_TYPE_MEMORY;
            // Get burst start and end packet
            burst_start = (simpliciti_data[1] << 8) + simpliciti_data[2];
            burst_end = (simpliciti_data[3] << 8) + simpliciti_data[4];
            // Set burst mode
            burst_mode = 1;
            // Number of packets to send
            simpliciti_reply_count = burst_end - burst_start + 1;
            break;

        case SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_2:
            // Send specified packets out in a burst
            simpliciti_data[0] = SYNC_ED_TYPE_MEMORY;
            // Store the requested packets
            for (i = 0; i < BM_SYNC_BURST_PACKETS_IN_DATA; i++)
            {
                burst_packet[i] = (simpliciti_data[i * 2 + 1] << 8) + simpliciti_data[i * 2 + 2];
            }
            // Set burst mode
            burst_mode = 2;
            // Number of packets to send
            simpliciti_reply_count = BM_SYNC_BURST_PACKETS_IN_DATA;
            break;

        case SYNC_AP_CMD_ERASE_MEMORY: // Erase data logger memory
            for (i = DATALOG_PAGE_START; i <= DATALOG_PAGE_END; i++)
            {
                flash_erase_page(i);
            }
            sDatalog.wptr = (u16 *) DATALOG_MEMORY_START;
            sDatalog.flags.flag.memory_full = 0;
            break;

        case SYNC_AP_CMD_EXIT:         // Exit sync mode
            simpliciti_flag |= SIMPLICITI_TRIGGER_STOP;
            break;
    }

}

// *************************************************************************************************
// @fn          simpliciti_sync_get_data_callback
// @brief       For SYNC mode only: Access point has requested data. Copy this data into the TX
// buffer now.
// @param       u16 index               Index used for memory requests
// @return      none
// *************************************************************************************************
void simpliciti_sync_get_data_callback(unsigned int index)
{
    u8 i;
    u16 bytes_ready;

    volatile u16 addr, mem;

    // Calculate bytes ready for sync
    bytes_ready = (sDatalog.wptr - (u16 *) DATALOG_MEMORY_START) * 2;

    // simpliciti_data[0] contains data type and needs to be returned to AP
    switch (simpliciti_data[0])
    {
        case SYNC_ED_TYPE_STATUS: // Status packet
            // Time
            simpliciti_data[1] = (sys.flag.use_metric_units << 7) | (sTime.hour & 0x7F);
            simpliciti_data[2] = sTime.minute;
            simpliciti_data[3] = sTime.second;
            // Date
            simpliciti_data[4] = sDate.year >> 8;
            simpliciti_data[5] = sDate.year & 0xFF;
            simpliciti_data[6] = sDate.month;
            simpliciti_data[7] = sDate.day;
            // Unused
            simpliciti_data[8] = 0;
            simpliciti_data[9] = 0;
            // Temperature
            simpliciti_data[10] = sAlt.temperature_C >> 8;
            simpliciti_data[11] = sAlt.temperature_C & 0xFF;
            // Altitude
            simpliciti_data[12] = sAlt.altitude >> 8;
            simpliciti_data[13] = sAlt.altitude & 0xFF;
            // Data logging mode
            simpliciti_data[14] = sDatalog.mode;
            // Data logging interval
            simpliciti_data[15] = sDatalog.interval;
            // Bytes ready for download
            simpliciti_data[16] = bytes_ready >> 8;
            simpliciti_data[17] = bytes_ready & 0xFF;
            // Unused
            simpliciti_data[18] = 0;

            break;

        case SYNC_ED_TYPE_MEMORY:
            if (burst_mode == 1)
            {
                // Set burst packet address
                simpliciti_data[1] = ((burst_start + index) >> 8) & 0xFF;
                simpliciti_data[2] = (burst_start + index) & 0xFF;
                // Assemble payload
                flash_ptr = (u8 *) (DATALOG_MEMORY_START + (burst_start + index) * 16);
                for (i = 3; i < BM_SYNC_DATA_LENGTH; i++)
                    simpliciti_data[i] = *flash_ptr++;
            }
            else if (burst_mode == 2)
            {
                // Set burst packet address
                simpliciti_data[1] = (burst_packet[index] >> 8) & 0xFF;
                simpliciti_data[2] = burst_packet[index] & 0xFF;
                // Assemble payload
                flash_ptr = (u8 *) (DATALOG_MEMORY_START + burst_packet[index] * 16);
                for (i = 3; i < BM_SYNC_DATA_LENGTH; i++)
                    simpliciti_data[i] = *flash_ptr++;
            }
            break;
    }
}

