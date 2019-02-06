/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 *
 * UpdateDisplay.c
 * User Experience Code for the MSP-EXP430FR5969
 * Display things on the LCD
 *
 * February 2012
 *
 *************************************************************************//****
 * Rev 1.1 - FRAM Write > 1GB bug fixed --> totalString array for updating LCD
 *           with write speeds was extended, thus preventing decimalString array
 *           from being overwritten.
 ******************************************************************************/
#include <string.h>

#include "driverlib.h"
#include "grlib.h"
#include "Sharp96x96.h"

#include "UpdateDisplay.h"
#include "FR59xx_EXP.h"
#include "Game.h"
#include "Preloaded_Images/TI_logo.h"
#include "Preloaded_Images/Wolverine_slash.h"

#define NORMAL_DISPLAY

/**********************************************************************//**
 * @brief  Turn on LCD
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
// TODO: Move to display driver
void LCD_turnOn(void)
{
    P4OUT |= BIT2;                          // Turn on LCD (Vcc = HI)
}

/**********************************************************************//**
 * @brief  Pull LCD DISP pin high
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
// TODO: Move to display driver
void LCD_display(void)
{
    P4OUT |= BIT3;                            // Enable LCD output (DISP = HI)
}

/**********************************************************************//**
 * @brief  Turn off LCD
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
// TODO: Move to display driver
void LCD_turnOff(void)
{
    P4OUT &= ~BIT3;
    __delay_cycles(500);
    P4OUT &= ~BIT2;         // Turn off LCD (DISP = LO & Vcc = LO)
}

/**********************************************************************//**
 * @brief  Write "430BOOST-SHARP96 Out of Box Demo" before
 *         showing main menu.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_introWrite(void)
{
    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);

    GrStringDraw(&sContext, "430BOOST-", -1, 20, 20, 0);
    GrStringDraw(&sContext, "SHARP96", -1, 24, 34, 0);
    GrStringDraw(&sContext, "Out of Box Demo", -1, 4, 48, 0);
    GrStringDraw(&sContext, UE_VERSION_STRING, -1, 30, 62, 0);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Create main menu for UE (highlight correct choice)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displayMainMenu(unsigned char choice)
{
    char outString[32];
    unsigned char text_start = 0, i;

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);

    // Write "MAIN MENU:"
    GrStringDraw(&sContext, "MAIN MENU:", -1, 16, 1, 0);

    // Write menu options
    GrStringDraw(&sContext, "Clock", -1, 2, 14, 0);
    GrStringDraw(&sContext, "FRAM Speed", -1, 2, 28, 0);
    GrStringDraw(&sContext, "Battery Free", -1, 2, 42, 0);
    GrStringDraw(&sContext, "Active Mode", -1, 2, 56, 0);
    GrStringDraw(&sContext, "SliderBall Game", -1, 2, 70, 0);
    GrStringDraw(&sContext, "Enter", -1, 66, 84, 0);
//  GrStringDraw(&sContext, "6. Energy Trace", -1, 2, 84, 0);

    // Overwrite chosen option with inverse colors
    switch (choice) {
    case 4:
        text_start = 14;
        strcpy(outString, "Clock");
        break;
    case 6:
        text_start = 28;
        strcpy(outString, "FRAM Speed");
        break;
    case 8:
        text_start = 42;
        strcpy(outString, "Battery Free");
        break;
    case 10:
        text_start = 56;
        strcpy(outString, "Active Mode");
        break;
    case 12:
        text_start = 70;
        strcpy(outString, "SliderBall Game");
        break;
//    case 14:
//      text_start = 84;
//      strcpy(outString,"6. Energy Trace");
//      break;
    default: break;
    }
    GrContextForegroundSet(&sContext, ClrBlack); //ClrBlack       this affects the highlight color
    for (i = text_start; i < (text_start + 14); i++) {
        GrLineDrawH(&sContext, 0, 96, i);
    }
    GrContextForegroundSet(&sContext, ClrWhite);    //ClrWhite      this affects the text color in the highlight
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, text_start, 0);
    GrContextForegroundSet(&sContext, ClrBlack);    //black         this affects the text of everything else.

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Create menu for Battery Free app (highlight correct choice)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displayULPSubMenu(unsigned char choice)
{
    char outString[32];
    unsigned char text_start = 0, i;

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrContextForegroundSet(&sContext, ClrBlack);

    GrStringDraw(&sContext, "BATTERY FREE:", -1, 2, 1, 0);
    GrStringDraw(&sContext, "Run App", -1, 2, 14, 0);
    GrStringDraw(&sContext, "Transmit Data", -1, 2, 28, 0);
    GrStringDraw(&sContext, "Exit         Enter", -1, 2, 84, 0);

    // Overwrite chosen option with inverse colors
    if (choice == 4) {
        text_start = 14;
        strcpy(outString, "Run App");
    }
    else {
        text_start = 28;
        strcpy(outString, "Transfer Data");
    }

    GrContextForegroundSet(&sContext, ClrBlack);
    for (i = text_start; i < (text_start + 14); i++) {
        GrLineDrawH(&sContext, 0, 96, i);
    }
    GrContextForegroundSet(&sContext, ClrWhite);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, text_start, 0);
    GrContextForegroundSet(&sContext, ClrBlack);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Write intro/instructions for ULP_ADC app
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_introULP(void)
{
    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);

    GrStringDraw(&sContext, "WARNING:", -1, 22, 2, 0);
    GrStringDraw(&sContext, "Entering LPM3.5", -1, 10, 14, 0);
    GrStringDraw(&sContext, "Press Right Button", -1, 1, 36, 0);
    GrStringDraw(&sContext, "To Enter/Exit", -1, 14, 48, 0);
    GrStringDraw(&sContext, "LPM3.5", -1, 33, 60, 0);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Draw battery based on how many "bars of charge" remain. Also
 *         write the time elapsed since going to LPM3.5 (in the format of
 *         hh:mm:ss) at the bottom of the screen.
 *
 * @param  rm_bars - remaining bars of charge
 *
 * @return none
 *************************************************************************/
void LCD_drawBattery(signed int rm_bars)
{
    char outString[32];
    int i, j, a;
    char dataValue[10] = "";
    unsigned int day, hour, min, sec;

    day = RTCDAY; hour = RTCHOUR; min = RTCMIN; sec = RTCSEC;

    hour += 24 * (day - 1); // Add 24 hours for every day it overflows

    GrContextForegroundSet(&sContext, ClrBlack);
    GrClearDisplay(&sContext);

    // Write "Charge Indicator"
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, "Charge Indicator", -1, 8, 10, 0);

    // Draw Battery Shell
    for (i = 30; i < 72; i++) {
        if (i < 36) {
            GrLineDrawH(&sContext, 4, 81, i);
        }
        else if (i < 41) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 91, i);
        }
        else if (i < 61) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 81, i);
            GrLineDrawH(&sContext, 86, 91, i);
        }
        else if (i < 66) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 91, i);
        }
        else {
            GrLineDrawH(&sContext, 4, 81, i);
        }
    }

    // Draw bars within battery
    if (rm_bars != 0) {
        a = 12;
        for (i = 0; i < rm_bars; i++) {
            for (j = 0; j < 6; j++) {
                GrLineDrawV(&sContext, a, 38, 63);
                a++;
            }
            a += 2;
        }
    }

    // Write time (hh:mm:ss) below battery
    if (hour >= 10) {
        UnsignedInt_To_ASCII(hour, dataValue);
        strcpy(outString, dataValue);
    }
    else {
        strcpy(outString, "");
        dataValue[0] = 0x30;
        dataValue[1] = hour % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    strcat(outString, ":");
    if (min >= 10) {
        UnsignedInt_To_ASCII(min, dataValue);
        strncat(outString, dataValue, 2);
    }
    else {
        dataValue[0] = 0x30;
        dataValue[1] = min % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    strcat(outString, ":");
    if (sec >= 10) {
        UnsignedInt_To_ASCII(sec, dataValue);
        strncat(outString, dataValue, 2);
    }
    else {
        dataValue[0] = 0x30;
        dataValue[1] = sec % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    GrStringDraw(&sContext, (int8_t*)outString, -1, 26, 80, 0);

    GrFlush(&sContext);

}

/**********************************************************************//**
 * @brief  Draw low battery indicator (battery with "X" through it), along
 *         with a message that tells the user to plug the EXP board into the
 *         PC via USB.
 *
 * @param  rm_bars - remaining bars of charge
 *
 * @return none
 *************************************************************************/
void LCD_drawLowBattery(signed int rm_bars)
{
    int i, j, a;

    GrContextForegroundSet(&sContext, ClrBlack);
    GrClearDisplay(&sContext);

    // Write "Low Battery, Plug in USB"
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, "LOW BATTERY", -1, 8, 2, 0);
    GrStringDraw(&sContext, "Plug into USB", -1, 13, 14, 0);

    // Draw Battery Shell
    for (i = 34; i < 76; i++) {
        if (i < 40) {
            GrLineDrawH(&sContext, 4, 81, i);
        }
        else if (i < 45) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 91, i);
        }
        else if (i < 65) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 81, i);
            GrLineDrawH(&sContext, 86, 91, i);
        }
        else if (i < 70) {
            GrLineDrawH(&sContext, 4, 9, i);
            GrLineDrawH(&sContext, 76, 91, i);
        }
        else {
            GrLineDrawH(&sContext, 4, 81, i);
        }
    }

    // Draw bars within battery
    if (rm_bars != 0) {
        a = 12;
        for (i = 0; i < rm_bars; i++) {
            for (j = 0; j < 6; j++) {
                GrLineDrawV(&sContext, a, 42, 67);
                a++;
            }
            a += 2;
        }
    }

    // Draw "X" over battery
    for (i = 17; i < 24; i++) {
        GrLineDraw(&sContext, i, 26, 54 + i, 80);
        GrLineDraw(&sContext, 54 + i, 26, i, 80);
    }

    // Write instructions for reentering LPM3.5 once USB plugged in
    GrStringDraw(&sContext, "Done", -1, 71, 83, 0);

    GrFlush(&sContext);

}

/**********************************************************************//**
 * @brief  This function displays "Sending Data Please Wait" screen while
 *         DMA transfers data to the UART buffer
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displaySendingScreen(void)
{
    GrContextForegroundSet(&sContext, ClrBlack);
    GrClearDisplay(&sContext);

    // Write "Sending Data, Please Wait"
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, "Sending Data", -1, 14, 14, 0);
    GrStringDraw(&sContext, "Please Wait", -1, 17, 26, 0);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function displays "Data Send Complete" screen after data has
 *         completed transferring
 *
 * @param  bytes - number of bytes of data that were transmitted
 *
 * @return none
 *************************************************************************/
void LCD_showDataSent(unsigned int bytes)
{
    char dataValue[10] = "";
    int length;

    GrContextForegroundSet(&sContext, ClrBlack);
    GrClearDisplay(&sContext);

    // Write "Data Send Complete"
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, "Data Send", -1, 20, 14, 0);
    GrStringDraw(&sContext, "Complete", -1, 24, 26, 0);

    // Write number of bytes sent via the UART transmission
    length = UnsignedInt_To_ASCII(bytes, dataValue);
    GrStringDraw(&sContext, (int8_t*)dataValue, -1, 48 - (4 * length), 50, 0);
    GrStringDraw(&sContext, "Bytes", -1, 32, 62, 0);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function displays the FRAM write speed, total kb written,
 *         and the FRAM endurance
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displaySpeed(unsigned long speed, unsigned long total,
                      unsigned long decimal)
{
    char outString[32];
    char speedString[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char totalString[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char decimalString[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);

    // Convert data to ASCII
    UnsignedLong_To_ASCII(speed, speedString);
    UnsignedLong_To_ASCII(total, totalString);
    UnsignedLong_To_ASCII(decimal, decimalString);

    // Take ASCII strings and format for final display
    // Format Speed (in KBps)
    strcat(speedString, " KBps");
    // Format Total KB
    strcat(totalString, " KB");
    // Format Decimal Places
    strcpy(outString, "99.9");
    strcat(outString, decimalString);
    strcat(outString, "%");

    GrStringDraw(&sContext, "Write Speed:", -1, 1, 2, 0);
    GrLineDrawH(&sContext, 1, 63, 14);
    GrStringDraw(&sContext, (int8_t*)speedString, -1, 1, 16, 0);
    GrStringDraw(&sContext, "Total KB Written:", -1, 1, 34, 0);
    GrLineDrawH(&sContext, 1, 89, 46);
    GrStringDraw(&sContext, (int8_t*)totalString, -1, 1, 48, 0);
    GrStringDraw(&sContext, "FRAM Endurance:", -1, 1, 66, 0);
    GrLineDrawH(&sContext, 1, 83, 78);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 1, 80, 0);

    GrFlush(&sContext);
    __no_operation();
}

/**********************************************************************//**
 * @brief  This function updates the LCD buffer with the RTC clock contents
 *
 *         NOTE: for regular clock operation, the values given to blink and
 *         value variables do not matter
 *
 * @param  blink - flag for blinking variables during clock setting
 *         value - value to blink while setting clock
 *         choose-1:display set in the right; 0: dispaly ULP
 *         LPM3ON-1:display SECOFF in the right of bottom line; 0:display SECON in the right of bottom line
 *
 * @return none
 *************************************************************************/
void LCD_updateClock(unsigned char blink, unsigned int value, unsigned char choose, unsigned char LPM3ON)
{
    char outString[32];
    char dataValue[10] = "";
    unsigned int year, month, day, hour, min, sec, weekday;
    unsigned char select_save, dow_spacing;
    char dow[10] = "";

    select_save = select;                   // Save select value in case of interrupt

    year = RTCYEAR; month = RTCMON; day = RTCDAY;
    hour = RTCHOUR; min = RTCMIN; sec = RTCSEC; weekday = RTCDOW;

    switch (select_save) {  // overwrite RTC value for parameter being set
    case  2:                // Clock running        // TODO: Populate symbolic constants for all those
        break;
    case  4:                // Setting hour
        hour = value;
        break;
    case  6: // Setting minute
        min = value;
        break;
    case  8: // Setting weekday
        weekday = value;
        break;
    case 10: // Setting month
        month = value;
        break;
    case 12: // Setting day of month
        day = value;
        break;
    case 14: // Setting year
        year = value;
        break;
    default: break;
    }

    switch (weekday) { // Decode day of week
    case 0:
        strcpy(dow, "SUNDAY");
        dow_spacing = 26;
        break;
    case 1:
        strcpy(dow, "MONDAY");
        dow_spacing = 24;
        break;
    case 2:
        strcpy(dow, "TUESDAY");
        dow_spacing = 21;
        break;
    case 3:
        strcpy(dow, "WEDNESDAY");
        dow_spacing = 13;
        break;
    case 4:
        strcpy(dow, "THURSDAY");
        dow_spacing = 18;
        break;
    case 5:
        strcpy(dow, "FRIDAY");
        dow_spacing = 26;
        break;
    case 6:
        strcpy(dow, "SATURDAY");
        dow_spacing = 17;
        break;
    default: break;
    }

    GrClearDisplay(&sContext);
    GrContextForegroundSet(&sContext, ClrBlack);

    // Write Day of Week
    if ((select_save != 8) || (blink == 1)) { // if setting DOW, blink it
        GrContextFontSet(&sContext, &g_sFontCmss12);
        GrStringDraw(&sContext, (int8_t*)dow, -1, dow_spacing, 12, 0);
    }

    // Write time (hour and minutes)
    strcpy(dataValue, "");  // Clear dataValue
    if (hour >= 10) {       // write hour value
        UnsignedInt_To_ASCII(hour, dataValue);
        strcpy(outString, dataValue);
    }
    else {
        strcpy(outString, "");
        dataValue[0] = 0x30;
        dataValue[1] = hour % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    strcat(outString, ":");                     // append colon between hour and minute
    if ((select_save != 6) || (blink == 1)) {   // append minute value to time
        if (min >= 10) {                        // only if it is not blinking
            UnsignedInt_To_ASCII(min, dataValue);
            strncat(outString, dataValue, 2);
        }
        else {
            dataValue[0] = 0x30;
            dataValue[1] = min % 10 + 0x30;
            strncat(outString, dataValue, 2);
        }
    }
    GrContextFontSet(&sContext, &g_sFontCmss32);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 10, 30, 0); // write time (hr:min)

    // if blinking hour, overwrite it with background color
    if ((select_save == 4) && (blink == 0)) {
        GrContextForegroundSet(&sContext, ClrWhite);
        strcpy(dataValue, ""); // Clear dataValue
        if (hour >= 10) {
            UnsignedInt_To_ASCII(hour, dataValue);
            strcpy(outString, dataValue);
        }
        else {
            strcpy(outString, "");
            dataValue[0] = 0x30;
            dataValue[1] = hour % 10 + 0x30;
            strncat(outString, dataValue, 2);
        }
        GrStringDraw(&sContext, (int8_t*)outString, -1, 10, 30, 0);
        GrContextForegroundSet(&sContext, ClrBlack);
    }

    // Write seconds
    strcpy(dataValue, ""); // Clear dataValue
    if (sec >= 10) {
        UnsignedInt_To_ASCII(sec, dataValue);
        strcpy(outString, dataValue);
    }
    else {
        strcpy(outString, "");
        dataValue[0] = 0x30;
        dataValue[1] = sec % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 75, 45, 0);

    // Write date (mm/dd/yyyy)
    strcpy(dataValue, "");  // Clear dataValue
    if (month >= 10) {      // first write month
        UnsignedInt_To_ASCII(month, dataValue);
        strcpy(outString, dataValue);
    }
    else {
        strcpy(outString, "");
        dataValue[0] = 0x30;
        dataValue[1] = month % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    strcat(outString, "/"); // append "/"
    if (day >= 10) {        // then write day
        UnsignedInt_To_ASCII(day, dataValue);
        strncat(outString, dataValue, 2);
    }
    else {
        dataValue[0] = 0x30;
        dataValue[1] = day % 10 + 0x30;
        strncat(outString, dataValue, 2);
    }
    strcat(outString, "/");                     // append another slash
    if ((select_save != 14) || (blink == 1)) {  // If blinking year, conditionally append it
        UnsignedInt_To_ASCII(year, dataValue);
        strncat(outString, dataValue, 4);
    }
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 18, 70, 0); // write date (mm/dd/yyyy)

    // If setting the day, overwrite it (and month) with background color
    if ((select_save == 12) && (blink == 0)) {
        GrContextForegroundSet(&sContext, ClrWhite);
        strcpy(dataValue, ""); // Clear dataValue
        if (month >= 10) {
            UnsignedInt_To_ASCII(month, dataValue);
            strcpy(outString, dataValue);
        }
        else {
            strcpy(outString, "");
            dataValue[0] = 0x30;
            dataValue[1] = month % 10 + 0x30;
            strncat(outString, dataValue, 2);
        }
        strcat(outString, "/");
        if (day >= 10) {
            UnsignedInt_To_ASCII(day, dataValue);
            strncat(outString, dataValue, 2);
        }
        else {
            dataValue[0] = 0x30;
            dataValue[1] = day % 10 + 0x30;
            strncat(outString, dataValue, 2);
        }
        GrStringDraw(&sContext, (int8_t*)outString, -1, 18, 70, 0);
    }

    if (((select_save == 10) || (select_save == 12)) && (blink == 0)) {
        if (select_save == 10) { // setting month, overwrite with background color
            GrContextForegroundSet(&sContext, ClrWhite);
        }
        else { // setting day, rewrite the month & append a slash
            GrContextForegroundSet(&sContext, ClrBlack);
        }
        strcpy(dataValue, ""); // Clear dataValue
        if (month >= 10) {
            UnsignedInt_To_ASCII(month, dataValue);
            strcpy(outString, dataValue);
        }
        else {
            strcpy(outString, "");
            dataValue[0] = 0x30;
            dataValue[1] = month % 10 + 0x30;
            strncat(outString, dataValue, 2);
        }
        if (select_save == 12) { // Add slash if setting day
            strcat(outString, "/");
        }
        GrStringDraw(&sContext, (int8_t*)outString, -1, 18, 70, 0);
        if (select_save == 10) {
            GrContextForegroundSet(&sContext, ClrBlack);
        }
    }

    // Write "Exit" in bottom left corner and "Set"/"SecON"/"SecOFF" in bottom right corner
    if (choose) {
        GrStringDraw(&sContext, "Exit           Set", -1, 2, 84, 0);
    }
    else {
        if (LPM3ON) {
            GrStringDraw(&sContext, "Exit        SecON", -1, 2, 84, 0);
        }
        else {
            GrStringDraw(&sContext, "Exit       SecOFF", -1, 2, 84, 0);
        }
    }

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Display Frequency choices for ActivePowerApp and highlight current
 *         selection
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displayFrequencyMenu(unsigned char choice)
{
    char outString[32];
    unsigned char text_start = 0, i;

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrContextForegroundSet(&sContext, ClrBlack);

    // Write "MAIN MENU:"
    GrStringDraw(&sContext, "Choose Frequency:", -1, 2, 1, 0);

    // Write menu options
    GrStringDraw(&sContext, "1 MHz", -1, 2, 14, 0);
    GrStringDraw(&sContext, "2.67 MHz", -1, 2, 28, 0);
    GrStringDraw(&sContext, "4 MHz", -1, 2, 42, 0);
    GrStringDraw(&sContext, "8 MHz", -1, 2, 56, 0);
    GrStringDraw(&sContext, "Exit         Enter", -1, 2, 84, 0);

    // Overwrite chosen option with inverse colors
    switch (choice) {
    case 2:
        text_start = 14;
        strcpy(outString, "1 MHz");
        break;
    case 4:
        text_start = 28;
        strcpy(outString, "2.67 MHz");
        break;
    case 6:
        text_start = 42;
        strcpy(outString, "4 MHz");
        break;
    case 8:
        text_start = 56;
        strcpy(outString, "8 MHz");
        break;
    default: break;
    }

    GrContextForegroundSet(&sContext, ClrBlack);
    for (i = text_start; i < (text_start + 14); i++) {
        GrLineDrawH(&sContext, 0, 96, i);
    }
    GrContextForegroundSet(&sContext, ClrWhite);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, text_start, 0);
    GrContextForegroundSet(&sContext, ClrBlack);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Display cache hit ratio choices for ActivePowerApp and highlight
 *         current selection
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_displayCacheRatioMenu(unsigned char choice)
{
    char outString[32];
    unsigned char text_start = 0, i;

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrContextForegroundSet(&sContext, ClrBlack);

    // Write "MAIN MENU:"
    GrStringDraw(&sContext, "Choose Cache Hit:", -1, 2, 1, 0);

    // Write menu options
    GrStringDraw(&sContext, "50%", -1, 2, 14, 0);
    GrStringDraw(&sContext, "66%", -1, 2, 28, 0);
    GrStringDraw(&sContext, "75%", -1, 2, 42, 0);
    GrStringDraw(&sContext, "100%", -1, 2, 56, 0);
    GrStringDraw(&sContext, "Exit         Enter", -1, 2, 84, 0);

    // Overwrite chosen option with inverse colors
    switch (choice) {
    case 2:
        text_start = 14;
        strcpy(outString, "50%");
        break;
    case 4:
        text_start = 28;
        strcpy(outString, "66%");
        break;
    case 6:
        text_start = 42;
        strcpy(outString, "75%");
        break;
    case 8:
        text_start = 56;
        strcpy(outString, "100%");
        break;
    default: break;
    }

    GrContextForegroundSet(&sContext, ClrBlack);
    for (i = text_start; i < (text_start + 14); i++) {
        GrLineDrawH(&sContext, 0, 96, i);
    }
    GrContextForegroundSet(&sContext, ClrWhite);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, text_start, 0);
    GrContextForegroundSet(&sContext, ClrBlack);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Display intro/instructions for active power app
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_activePowerIntro(void)
{
    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);

    GrStringDraw(&sContext, "Active Power App:", -1, 2, 16, 0);
    GrStringDraw(&sContext, "Press Right Button", -1, 1, 36, 0);
    GrStringDraw(&sContext, "To Enter/Exit", -1, 12, 48, 0);
    GrStringDraw(&sContext, "Cache Hit Code", -1, 8, 60, 0);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  Display different difficulties for SliderBall Game
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void LCD_sliderBallMenu(unsigned char choice)
{
    char outString[32];
    unsigned char text_start = 0, i;

    GrClearDisplay(&sContext);
    GrContextFontSet(&sContext, &g_sFontCmss12);
    GrContextForegroundSet(&sContext, ClrBlack);

    // Write "MAIN MENU:"
    GrStringDraw(&sContext, "Choose Difficulty:", -1, 2, 1, 0);

    // Write menu options
    GrStringDraw(&sContext, "Easy", -1, 2, 14, 0);
    GrStringDraw(&sContext, "Normal", -1, 2, 28, 0);
    GrStringDraw(&sContext, "Hard", -1, 2, 42, 0);
    GrStringDraw(&sContext, "INSANE", -1, 2, 56, 0);
    GrStringDraw(&sContext, "Exit         Enter", -1, 2, 84, 0);

    // Overwrite chosen option with inverse colors
    switch (choice) {
    case 2:
        text_start = 14;
        strcpy(outString, "Easy");
        break;
    case 4:
        text_start = 28;
        strcpy(outString, "Normal");
        break;
    case 6:
        text_start = 42;
        strcpy(outString, "Hard");
        break;
    case 8:
        text_start = 56;
        strcpy(outString, "INSANE");
        break;
    default: break;
    }

    GrContextForegroundSet(&sContext, ClrBlack);
    for (i = text_start; i < (text_start + 14); i++) {
        GrLineDrawH(&sContext, 0, 96, i);
    }
    GrContextForegroundSet(&sContext, ClrWhite);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, text_start, 0);
    GrContextForegroundSet(&sContext, ClrBlack);

    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function draws the paddles and ball used in the SliderBall
 ****game
 *
 * @param  position_l - position of left paddle
 *         position_r - position of right paddle
 *         x,y - ball's horizontal/vertical (respectively) position
 *
 * @return none
 *************************************************************************/
void LCD_drawSliderBall(unsigned char position_l, unsigned char position_r,
                        unsigned char x, unsigned char y)
{
    unsigned char LCD_line_l, LCD_line_r; //, i;

    // Take CT position and turn it into the line on the LCD it represents
    LCD_line_l = (unsigned char)(105 - (52 * position_l / 18));
    LCD_line_r = (unsigned char)(105 - (52 * position_r / 18));

    // Clear part of buffer used for gameplay
    unsigned char i = 0, j = 0;
    for (i = 14; i < 83; i++) {
        for (j = 0; j < 12; j++) {
            DisplayBuffer[i][j] = 0xFF;
        }
    }

    // Draw Paddles and Ball
    GrLineDrawV(&sContext, 5, LCD_line_l - 8, LCD_line_l + 8);  // Draw Left Paddle
    GrLineDrawV(&sContext, 90, LCD_line_r - 8, LCD_line_r + 8); // Draw Right Paddle
    GrCircleDraw(&sContext, x, y, 3);                           // Draw Ball

    // Send Buffer to LCD
    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function displays how many lives the user has left
 *
 * @param  lives - how many lives should be showing
 *
 * @return none
 *************************************************************************/
void LCD_updateLives(unsigned char lives)
{
    char outString[32];
    char dataValue[10] = "";

    // Clear part of buffer used for Lives
    unsigned char i = 0, j = 0;

    for (i = 0; i < 13; i++) {
        for (j = 0; j < 12; j++) {
            DisplayBuffer[i][j] = 0xFF;
        }
    }

    // Write Lives at top of screen
    strcpy(outString, "Lives: ");
    UnsignedInt_To_ASCII(lives, dataValue);
    strncat(outString, dataValue, 1);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, 0, 0);

    // Send Buffer to LCD
    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function displays the user's score
 *
 * @param  score - the user's score to be displayed
 *
 * @return none
 *************************************************************************/
void LCD_updateScore(unsigned int score)
{
    char outString[32];
    char dataValue[10] = "";
    int digits;

    // Clear part of buffer used for scoring
    unsigned char i = 0, j = 0;

    for (i = 84; i < 94; i++) {
        for (j = 4; j < 12; j++) {
            DisplayBuffer[i][j] = 0xFF;
        }
    }

    // Write Score
    strcpy(outString, "Score: ");
    digits = UnsignedInt_To_ASCII(score, dataValue);
    strncat(outString, dataValue, digits);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 2, 84, 0);

    // Send Buffer to LCD
    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function writes the game over message and shows the user's
 *         final score when either a button has been pushed or the user
 *         has run out of lives in the game
 *
 * @param  score - final score of user
 *
 * @return none
 *************************************************************************/
void LCD_displayGameOver(unsigned int score)
{
    char outString[32];
    char dataValue[10] = "";
    int digits;

    GrClearDisplay(&sContext);

    GrStringDraw(&sContext, "GAME OVER!", -1, 14, 5, 0);

    // Write Final Score
    GrStringDraw(&sContext, "Final Score: ", -1, 20, 25, 0);
    strcpy(outString, "");
    digits = UnsignedInt_To_ASCII(score, dataValue);
    strncat(outString, dataValue, digits);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 52 - (4 * digits), 40, 0);

    // Write High Score
    GrStringDraw(&sContext, "High Score: ", -1, 20, 55, 0);
    strcpy(outString, "");
    digits = UnsignedInt_To_ASCII(HighScore, dataValue);
    strncat(outString, dataValue, digits);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 52 - (4 * digits), 70, 0);

    // Send Buffer to LCD
    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function lets the user know they set a new high score
 *         and overwrites the highscore in FRAM with the user's score.
 *
 * @param  score - final score of user
 *
 * @return none
 *************************************************************************/
void LCD_displayNewHighScore(unsigned int score)
{
    char outString[32];
    char dataValue[10] = "";
    int digits;

    HighScore = score;                      // set new high score

    GrClearDisplay(&sContext);

    GrStringDraw(&sContext, "Congratulations!", -1, 10, 15, 0);
    GrStringDraw(&sContext, "NEW HIGH SCORE:", -1, 2, 40, 0);

    // Write Final Score
    strcpy(outString, "");
    digits = UnsignedInt_To_ASCII(score, dataValue);
    strncat(outString, dataValue, digits);
    GrStringDraw(&sContext, (int8_t*)outString, -1, 52 - (4 * digits), 55, 0);

    // Send Buffer to LCD
    GrFlush(&sContext);
}

/**********************************************************************//**
 * @brief  This function takes a pre-loaded image from FRAM and displays
 *         it on the LCD.
 *
 * @param  choice - 1 = TI logo, 2 = Wolverine Logo,
 *
 * @return none
 *************************************************************************/
void LCD_drawPicture(int choice)
{
    int i;

    unsigned char command = SHARP_LCD_CMD_WRITE_LINE; // Image update mode
    command = command ^ VCOMbit;                      //COM inversion bit
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4); // Set P2.4 High for CS

    // Check if SPI is bus is busy first
    while(EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));
    // Send command
    EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, command);

    if (choice == 1) {
        for (i = 0; i < 0x540; i++) {
#ifdef NORMAL_DISPLAY
            while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                    EUSCI_B_SPI_TRANSMIT_INTERRUPT));
            EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoTI[i]);
#endif
#ifdef INVERT_DISPLAY
            if (i % 14 != 0) {
                while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                        EUSCI_B_SPI_TRANSMIT_INTERRUPT));
                EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoTI[i] ^ 0xFF);
            }
            else {
                while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                        EUSCI_B_SPI_TRANSMIT_INTERRUPT));
                EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoTI[i]);
            }
#endif
        }
    }
    else if (choice == 2) {
        for (i = 0; i < 0x540; i++) {
#ifdef NORMAL_DISPLAY
            while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                    EUSCI_B_SPI_TRANSMIT_INTERRUPT));
            EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoWolverine[i]);
#endif
#ifdef INVERT_DISPLAY
            if (i % 14 != 0) {
                while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                        EUSCI_B_SPI_TRANSMIT_INTERRUPT));
                EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoWolverine[i] ^ 0xFF);
            }
            else {
                while(!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE,
                        EUSCI_B_SPI_TRANSMIT_INTERRUPT));
                EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, logoWolverine[i]);
            }
#endif
        }
    }

    // Wait for last byte to be sent, then drop SCS
    while(EUSCI_B_SPI_isBusy(EUSCI_B0_BASE));

    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);    // Set P2.4 Low for CS
}

/**********************************************************************//**
 * @brief  UnsignedInt_To_ASCII
 *
 * @param  none
 *
 * @return i - number of digits that the Unsigned Int is
 *************************************************************************/
int UnsignedInt_To_ASCII(unsigned int hex, char *ASCII)
{
    int i, j;
    char flipped[5] = { 0, 0, 0, 0, 0 };

    // Convert from hex to ASCII
    for (i = 1; i < 6; i++) {
        flipped[i] = hex % 10 + 0x30;
        hex /= 10;
        if (hex == 0) {
            break;
        }
    }

    // Flip data values to correct position in array
    for (j = 0; j < 5; j++) {
        ASCII[j] = flipped[i - j];
    }

    return i;
}

/**********************************************************************//**
 * @brief  UnsignedLong_To_ASCII
 *
 * @param  none
 *
 * @return i - number of digits that the Unsigned Long is
 *************************************************************************/
int UnsignedLong_To_ASCII(unsigned long hex, char *ASCII)
{
    int i, j;
    char flipped[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // Convert from hex to ASCII
    for (i = 1; i < 10; i++) {
        flipped[i] = hex % 10 + 0x30;
        hex /= 10;
        if (hex == 0) {
            break;
        }
    }

    // Flip data values to correct position in array
    for (j = 0; j < i; j++) {
        ASCII[j] = flipped[i - j];
    }

    return i;
}
