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
// BlueRobin functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// system
#include "project.h"

// driver
#include "display.h"
#include "radio.h"
#include "ports.h"
#include "timer.h"
#include "rf1a.h"

// logic
#include "BlueRobin_RX_API.h"
#include "bluerobin.h"
#include "rfsimpliciti.h"
#include "user.h"

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section

// Set to TRUE if transmitter ID should be remembered when reconnecting
// Transmitter ID can be cleared by pressing button STAR for more than 3 seconds
#define REMEMBER_TX_ID                  (FALSE)

// *************************************************************************************************
// Global Variable section
struct bluerobin sBlueRobin;

// *************************************************************************************************
// Extern section

// Stop BlueRobin timer
extern void BRRX__StopTimer_v(void);

// Calibration value for FSCTRL0 register (corrects deviation of 26MHz crystal)
extern u8 rf_frequoffset;

// *************************************************************************************************
// @fn          reset_bluerobin
// @brief       Reset BlueRobin data.
// @param       none
// @return      none
// *************************************************************************************************
void reset_bluerobin(void)
{
    // Reset state is no connection
    sBlueRobin.state = BLUEROBIN_OFF;

    // Reset value of chest strap ID is 0 --> connect to next best chest strap
    sBlueRobin.cs_id = 0;

    // No new data available
    sBlueRobin.update = BLUEROBIN_NO_UPDATE;
    sBlueRobin.heartrate = 0;
}

// *************************************************************************************************
// @fn          mx_rfblue
// @brief       BlueRobin sub menu. Button STAR resets chest strap ID to 0.
// @param       u8 line LINE2
// @return      none
// *************************************************************************************************
void mx_bluerobin(u8 line)
{
#if REMEMBER_TX_ID == TRUE
    u8 i;

    // Reset chest strap ID
    sBlueRobin.cs_id = 0;

    display_chars(LCD_SEG_L1_2_0, (u8 *) "CLR", SEG_ON);
    for (i = 0; i < 4; i++)
        Timer0_A4_Delay(CONV_MS_TO_TICKS(500));
#endif

    // Clear simulated button event
    button.all_flags = 0;
}

// *************************************************************************************************
// @fn          sx_bluerobin
// @brief       BlueRobin direct function. Button UP connects/disconnects with transmitter unit.
// @param       u8 line         LINE1
// @return      none
// *************************************************************************************************
void sx_bluerobin(u8 line)
{
    u8 stop = 0;

    // Exit if battery voltage is too low for radio operation
    if (sys.flag.low_battery)
        return;

    // Exit if SimpliciTI stack is active
    if (is_rf())
        return;

    // UP: connect / disconnect transmitter
    if (button.flag.up)
    {
        if (sBlueRobin.state == BLUEROBIN_OFF)
        {
            // If no transmitter can be found, stop BlueRobin stack
            if (!start_bluerobin())
                stop = 1;
        }
        else if (sBlueRobin.state == BLUEROBIN_CONNECTED)
        {
            // Shutdown connection
            stop = 1;
        }
    }

    // Shutdown connection
    if (stop)
    {
        stop_bluerobin();
    }
}

// *************************************************************************************************
// @fn          display_heartrate
// @brief       Heart rate display routine.
// @param       u8 line         LINE1
//                              u8 update       DISPLAY_LINE_UPDATE_FULL,
// DISPLAY_LINE_UPDATE_PARTIAL, DISPLAY_LINE_CLEAR
// @return      none
// *************************************************************************************************
void display_heartrate(u8 line, u8 update)
{
    u8 *str;

    if (update != DISPLAY_LINE_CLEAR)
    {
        if (is_bluerobin())
        {
            str = int_to_array(sBlueRobin.heartrate, 3, 2);
            display_chars(LCD_SEG_L1_2_0, str, SEG_ON);
        }
        else
        {
            display_chars(LCD_SEG_L1_2_0, (u8 *) "---", SEG_ON);
        }
    }

    // Redraw whole screen
    if (!is_bluerobin())
    {
        if (update == DISPLAY_LINE_UPDATE_FULL)
        {
            display_symbol(LCD_ICON_HEART, SEG_ON);
        }
        else if (update == DISPLAY_LINE_CLEAR)
        {
            // Clear heart when not connected
            display_symbol(LCD_ICON_HEART, SEG_OFF);
        }
    }
}

// *************************************************************************************************
// @fn          is_bluerobin
// @brief       Returns TRUE if BlueRobin transmitter is connected.
// @param       none
// @return      u8
// *************************************************************************************************
u8 is_bluerobin(void)
{
    return (sBlueRobin.state == BLUEROBIN_CONNECTED);
}

// *************************************************************************************************
// @fn          is_bluerobin_searching
// @brief       Returns TRUE if BlueRobin is searching for a transmitter.
// @param       none
// @return      u8                      1 if it is trying to connect
// *************************************************************************************************
u8 is_bluerobin_searching(void)
{
    return (sBlueRobin.state == BLUEROBIN_SEARCHING);
}

// *************************************************************************************************
// @fn          get_bluerobin_data
// @brief       Read BlueRobin packet data from API.
// @param       none
// @return      none
// *************************************************************************************************
void get_bluerobin_data(void)
{
    brtx_state_t bChannelState;

    // Check connection status
    bChannelState = BRRX_GetState_t(HR_CHANNEL);

    switch (bChannelState)
    {
        case TX_ACTIVE:        // Read heart rate data from BlueRobin API
            sBlueRobin.heartrate = BRRX_GetHeartRate_u8();
            sBlueRobin.update = BLUEROBIN_NEW_DATA;
            break;

        case TX_OFF:           // Shutdown connection
            stop_bluerobin();
            break;

        // BR_SEARCH, BR_LEARN, BR_PAUSE: Keep old values until we receive new data
        default:
            break;
    }
}

// *************************************************************************************************
// @fn          start_bluerobin
// @brief       Start BlueRobin stack and search for a transmitter.
// @param       none
// @return      0 = no transmitter found, 1 = connected to a transmitter
// *************************************************************************************************
u8 start_bluerobin(void)
{
    u8 timeout, i;

    // Init BlueRobin timer and radio
    // Enable high current mode
    open_radio();

    // Initialize BR library
    BRRX_Init_v();

    // Set BR data transmission properties
    BRRX_SetPowerdownDelay_v(10); // Power down channel after 10 consecutive lost data packets (~9
                                  // seconds)
    BRRX_SetSearchTimeout_v(6);   // Stop searching after 8 seconds

    // Sensitivity in learn mode reduced --> connect only to close transmitters
    // Skip this part if chest strap id was set in a previous learn mode run
#if REMEMBER_TX_ID == TRUE
    if (sBlueRobin.cs_id == 0)
        BRRX_SetSignalLevelReduction_v(5);
#else
    // Forget previously learned transmitter ID and connect to next close transmitter
    sBlueRobin.cs_id = 0;
    BRRX_SetSignalLevelReduction_v(5);
#endif

    // Apply frequency offset compensation to radio register FSCTRL0
    // If calibration memory was erased, rf_frequoffset defaults to 0x00 and has no effect
    WriteSingleReg(FSCTRL0, rf_frequoffset);

    // New state is SEARCH
    sBlueRobin.state = BLUEROBIN_SEARCHING;

    // Blink RF icon to show searching
    display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);

    // Turn on radio and establish connection if channel not already started
    if (BRRX_GetState_t(HR_CHANNEL) == TX_OFF)
    {
        // Start in learn mode (connect to closest heart rate transmitter)
        BRRX_SetID_v(HR_CHANNEL, sBlueRobin.cs_id);
        BRRX_Start_v(HR_CHANNEL);

        // Wait until learning phase is over, additional timeout prevents race condition if hardware
        // works incorrect
        timeout = 40;
        while ((BRRX_GetState_t(HR_CHANNEL) == TX_SEARCH) && (timeout-- > 0))
        {
            Timer0_A4_Delay(CONV_MS_TO_TICKS(200));
        }

        // Timeout?
        if (timeout == 0)
        {
            display_chars(LCD_SEG_L1_3_0, (u8 *) "FAIL", SEG_ON);
            for (i = 0; i < 4; i++)
                Timer0_A4_Delay(CONV_MS_TO_TICKS(500));
        }
    }

    // Check if connection attempt was successful
    if (BRRX_GetState_t(HR_CHANNEL) == TX_ACTIVE)
    {
        // Successfully connected to transmitter
        sBlueRobin.state = BLUEROBIN_CONNECTED;

        // When in learn mode, copy chest strap ID
        if (sBlueRobin.cs_id == 0)
            sBlueRobin.cs_id = BRRX_GetID_u32(HR_CHANNEL);

        // Show steady RF icon to indicate established connection
        display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_OFF);
        display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_OFF);
        display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_OFF);

        // Show blinking icon
        display_symbol(LCD_ICON_HEART, SEG_ON_BLINK_ON);

        return (1);
    }
    else                        // Error -> Shutdown connection
    {
        // Clear RF icon
        display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
        display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
        display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);

        return (0);
    }
}

// *************************************************************************************************
// @fn          stop_bluerobin
// @brief       Stop communication and put peripherals in power-down mode.
// @param       none
// @return      none
// *************************************************************************************************
void stop_bluerobin(void)
{
    // Reset connection status byte
    sBlueRobin.state = BLUEROBIN_OFF;

    // Stop channel
    BRRX_Stop_v(HR_CHANNEL);

    // Powerdown radio
    close_radio();

    // Force full display update to clear heart rate and speed data
    sBlueRobin.heartrate = 0;
    display.flag.full_update = 1;

    // Clear heart and RF symbol
    display_symbol(LCD_ICON_HEART, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);
}

// *************************************************************************************************
// @fn          bluerobin_flash_write_window
// @brief       Returns 1 if next BlueRobin ISR is more than 15msec away.
// @param       none
// @return      u8
// *************************************************************************************************
u8 is_bluerobin_flash_write_window(void)
{
    if (is_bluerobin())
        return ((TA0CCR1 - TA0R) > 500);
    else
        return (1);
}

