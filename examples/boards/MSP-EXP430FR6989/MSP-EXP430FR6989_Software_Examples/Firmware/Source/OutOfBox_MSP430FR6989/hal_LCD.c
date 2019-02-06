/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 * hal_LCD.c
 *
 * Hardware abstraction layer for the FH-1138P Segmented LCD
 * on MSP-EXP430FR6989
 *
 * February 2015
 * E. Chen
 *
 ******************************************************************************/

#include <driverlib.h>
#include "hal_LCD.h"
#include "string.h"


// LCD memory map for numeric digits
const char digit[10][2] =
{
    {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
    {0x60, 0x20},  /* "1" */
    {0xDB, 0x00},  /* "2" */
    {0xF3, 0x00},  /* "3" */
    {0x67, 0x00},  /* "4" */
    {0xB7, 0x00},  /* "5" */
    {0xBF, 0x00},  /* "6" */
    {0xE4, 0x00},  /* "7" */
    {0xFF, 0x00},  /* "8" */
    {0xF7, 0x00}   /* "9" */
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

void Init_LCD()
{
    LCD_C_initParam initParams = {0};
    initParams.clockSource = LCD_C_CLOCKSOURCE_ACLK;
    initParams.clockDivider = LCD_C_CLOCKDIVIDER_1;
    initParams.clockPrescalar = LCD_C_CLOCKPRESCALAR_16;
    initParams.muxRate = LCD_C_4_MUX;
    initParams.waveforms = LCD_C_LOW_POWER_WAVEFORMS;
    initParams.segments = LCD_C_SEGMENTS_ENABLED;

    LCD_C_init(LCD_C_BASE, &initParams);
    // LCD Operation - VLCD generated internally, V2-V4 generated internally, v5 to ground

    /*  'FR6989 LaunchPad LCD1 uses Segments S4, S6-S21, S27-S31 and S35-S39 */
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_4,
                                LCD_C_SEGMENT_LINE_4);
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_6,
                                LCD_C_SEGMENT_LINE_21);
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_27,
                                LCD_C_SEGMENT_LINE_31);
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_35,
                                LCD_C_SEGMENT_LINE_39);

    LCD_C_setVLCDSource(LCD_C_BASE, LCD_C_VLCD_GENERATED_INTERNALLY,
                        LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS,
                        LCD_C_V5_VSS);

    // Set VLCD voltage to 3.20v
    LCD_C_setVLCDVoltage(LCD_C_BASE,
                         LCD_C_CHARGEPUMP_VOLTAGE_3_02V_OR_2_52VREF);

    // Enable charge pump and select internal reference for it
    LCD_C_enableChargePump(LCD_C_BASE);
    LCD_C_selectChargePumpReference(LCD_C_BASE,
                                    LCD_C_INTERNAL_REFERENCE_VOLTAGE);

    LCD_C_configChargePump(LCD_C_BASE, LCD_C_SYNCHRONIZATION_ENABLED, 0);

    // Clear LCD memory
    LCD_C_clearMemory(LCD_C_BASE);

    //Turn LCD on
    LCD_C_on(LCD_C_BASE);
}

/*
 * Scrolls input string across LCD screen from left to right
 */
void displayScrollText(char *msg)
{
    int length = strlen(msg);
    int oldmode = mode;
    int i;
    int s = 5;
    char buffer[6] = "      ";
    for (i=0; i<length+7; i++)
    {
        if (mode != oldmode)
            break;
        int t;
        for (t=0; t<6; t++)
            buffer[t] = ' ';
        int j;
        for (j=0; j<length; j++)
        {
            if (((s+j) >= 0) && ((s+j) < 6))
                buffer[s+j] = msg[j];
        }
        s--;

        showChar(buffer[0], pos1);
        showChar(buffer[1], pos2);
        showChar(buffer[2], pos3);
        showChar(buffer[3], pos4);
        showChar(buffer[4], pos5);
        showChar(buffer[5], pos6);

        __delay_cycles(200000);
    }
}

/*
 * Displays input character at given LCD digit/position
 * Only spaces, numeric digits, and uppercase letters are accepted characters
 */
void showChar(char c, int position)
{
    if (c == ' ')
    {
        // Display space
        LCDMEM[position] = 0;
        LCDMEM[position+1] = 0;
    }
    else if (c >= '0' && c <= '9')
    {
        // Display digit
        LCDMEM[position] = digit[c-48][0];
        LCDMEM[position+1] = digit[c-48][1];
    }
    else if (c >= 'A' && c <= 'Z')
    {
        // Display alphabet
        LCDMEM[position] = alphabetBig[c-65][0];
        LCDMEM[position+1] = alphabetBig[c-65][1];
    }
    else
    {
        // Turn all segments on if character is not a space, digit, or uppercase letter
        LCDMEM[position] = 0xFF;
        LCDMEM[position+1] = 0xFF;
    }
}

/*
 * Clears memories to all 6 digits on the LCD
 */
void clearLCD()
{
    LCDMEM[pos1] = LCDBMEM[pos1] = 0;
    LCDMEM[pos1+1] = LCDBMEM[pos1+1] = 0;
    LCDMEM[pos2] = LCDBMEM[pos2] = 0;
    LCDMEM[pos2+1] = LCDBMEM[pos2+1] = 0;
    LCDMEM[pos3] = LCDBMEM[pos3] = 0;
    LCDMEM[pos3+1] = LCDBMEM[pos3+1] = 0;
    LCDMEM[pos4] = LCDBMEM[pos4] = 0;
    LCDMEM[pos4+1] = LCDBMEM[pos4+1] = 0;
    LCDMEM[pos5] = LCDBMEM[pos5] = 0;
    LCDMEM[pos5+1] = LCDBMEM[pos5+1] = 0;
    LCDMEM[pos6] = LCDBMEM[pos6] = 0;
    LCDMEM[pos6+1] = LCDBMEM[pos6+1] = 0;

    LCDM14 = LCDBM14 = 0x00;
    LCDM18 = LCDBM18 = 0x00;
    LCDM3 = LCDBM3 = 0x00;
}
