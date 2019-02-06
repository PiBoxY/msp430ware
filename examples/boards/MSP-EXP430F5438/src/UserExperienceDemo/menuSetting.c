/*******************************************************************************
 *
 *  menuSetting.c
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
#include "menuSetting.h"
#include "flashUtils.h"

const char settingMenuText[] = {
    "Settings Menu  \0"
    " 1. Set Time  \0"
    " 2. Contrast  \0"
    " 3. Backlight \0"
    " 4. Set Acc.  \0"
    " 5. Version   \0"
    " 6. Quit      \0"
    " PlaceHolder  \0"
};

/*-------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Stores calibration and user-config data into INFOB flash
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void saveSettings(void)
{
    // Erase Flash segment containing the user calibration data
    flashEraseSegment((unsigned long)&boardMode);

    // Unlock the Flash memory and program the user calibration data
    // using in-system programming.
    FLASH_UNLOCK;
    boardMode = boardModeLOCAL;
    Acc_X_Calibrated_Offset =  Acc_X_Calibrated_OffsetLOCAL;
    Acc_Y_Calibrated_Offset =  Acc_Y_Calibrated_OffsetLOCAL;
    Acc_Z_Calibrated_Offset = Acc_Z_Calibrated_OffsetLOCAL;
    wakeUpOnAcc = wakeUpOnAccLOCAL;
    lcdBackLightLevelSetting = lcdBackLightLevelSettingLOCAL;
    lcdContrastSetting =  lcdContrastSettingLOCAL;
    lastAudioByteFlash = lastAudioByte;
    FLASH_LOCK;
}

/**********************************************************************//**
 * @brief  Loads calibration and user-config data from INFOB flash.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void loadSettings(void)
{
    // Read back stored configuration from Flash
    boardModeLOCAL = boardMode;
    Acc_X_Calibrated_OffsetLOCAL = Acc_X_Calibrated_Offset;
    Acc_Y_Calibrated_OffsetLOCAL = Acc_Y_Calibrated_Offset;
    Acc_Z_Calibrated_OffsetLOCAL = Acc_Z_Calibrated_Offset;
    lcdBackLightLevelSettingLOCAL = lcdBackLightLevelSetting;
    lcdContrastSettingLOCAL = lcdContrastSetting;
    lastAudioByte = lastAudioByteFlash;
    wakeUpOnAccLOCAL = wakeUpOnAcc;

    // Ensure accelerometer calibration settings are not too low/high. Invalid
    // settings could have been caused by an erased (or not yet programmed)
    // calibration location
    if (Acc_X_Calibrated_OffsetLOCAL > 4096 || Acc_X_Calibrated_OffsetLOCAL <= 0){
        Acc_X_Calibrated_OffsetLOCAL = (ACC_X_LOW_OFFSET + ACC_X_HIGH_OFFSET) / 2;
        Acc_Y_Calibrated_OffsetLOCAL = (ACC_Y_LOW_OFFSET + ACC_Y_HIGH_OFFSET) / 2;
        Acc_Z_Calibrated_OffsetLOCAL = (ACC_Z_LOW_OFFSET + ACC_Z_HIGH_OFFSET) / 2;
    }

    // Ensure contrast setting is not too low/high. An invalid setting could have
    // been caused by an erased (or not yet programmed) calibration location
    if (lcdContrastSettingLOCAL < 70 || lcdContrastSettingLOCAL > 127){
        lcdContrastSettingLOCAL = 105;
    }

    // Ensure backlight setting is not too low/high. An invalid setting could have
    // been caused by an erased (or not yet programmed) calibration location
    if (lcdBackLightLevelSettingLOCAL > 16){
        lcdBackLightLevelSettingLOCAL = 16;
    }

    // Apply accelerometer calibration values
    halAccelerometerSetCalibratedOffset(Acc_X_Calibrated_OffsetLOCAL,
                                        Acc_Y_Calibrated_OffsetLOCAL,
                                        Acc_Z_Calibrated_OffsetLOCAL);
}

/*-------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Interface to adjust the contrast of the LCD.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void setContrast(void)
{
    unsigned char quit = 0, previousP2IE;

    halLcdClearScreen();

    halLcdPrintLine(" Adjust Contrast", 0, 0);

    halLcdPrintLineCol(" - ", 8, 1, 0);
    halLcdPrintLineCol(" + ", 8, 13, INVERT_TEXT);

    halLcdLine(6, 94, 32, 94, PIXEL_ON);
    halLcdLine(6, 94, 6, 109, PIXEL_ON);
    halLcdLine(32, 94, 32, 109, PIXEL_ON);
    halLcdLine(6, 109, 32, 109, PIXEL_ON);

    halLcdLine(102, 94, 128, 94, PIXEL_ON);
    halLcdLine(128, 94, 128, 109, PIXEL_ON);
    halLcdLine(102, 94, 102, 109,  PIXEL_ON);
    halLcdLine(102, 109, 128, 109,  PIXEL_ON);

    previousP2IE = P2IE;
    P2IE = BUTTON_SELECT + BUTTON_S1 + BUTTON_S2;

    while (!quit)
    {
        TA0CTL &= ~TAIFG;

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();

        if (buttonsPressed & BUTTON_S2)
            halLcdSetContrast(halLcdGetContrast() + 5);
        else if (buttonsPressed & BUTTON_S1)
            halLcdSetContrast(halLcdGetContrast() - 5);
        if (buttonsPressed & BUTTON_SELECT)
            quit = 1;

        buttonsPressed = 0;
    }
    P2IE = previousP2IE;
    lcdContrastSettingLOCAL = halLcdGetContrast();
}

/*-------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Interface to adjust the brightness of the backlight.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void setBackLight(void)
{
    unsigned char quit = 0, previousP2IE;
    unsigned char backlight;

    halLcdClearScreen();
    halLcdPrintLine("Adjust Backlight",  0, 0);

    halLcdPrintLineCol(" - ", 8, 1, 0);
    halLcdPrintLineCol(" + ", 8, 13, INVERT_TEXT);

    halLcdLine(6, 94, 32, 94, PIXEL_ON);
    halLcdLine(6, 94, 6, 109, PIXEL_ON);
    halLcdLine(32, 94, 32, 109, PIXEL_ON);
    halLcdLine(6, 109, 32, 109, PIXEL_ON);

    halLcdLine(102, 94, 128, 94, PIXEL_ON);
    halLcdLine(128, 94, 128, 109, PIXEL_ON);
    halLcdLine(102, 94, 102, 109,  PIXEL_ON);
    halLcdLine(102, 109, 128, 109,  PIXEL_ON);

    previousP2IE = P2IE;
    P2IE = BUTTON_SELECT + BUTTON_S1 + BUTTON_S2;

    while (!quit)
    {
        TA0CTL &= ~TAIFG;

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();

        backlight = halLcdGetBackLight();
        if (buttonsPressed & BUTTON_S2)
            if (backlight < 16)
                halLcdSetBackLight(backlight + 2);
            else
                halLcdSetBackLight(16);
        else if (buttonsPressed & BUTTON_S1)
            if (backlight > 0)
                halLcdSetBackLight(backlight - 2);
            else
                halLcdSetBackLight(0);

        if (buttonsPressed & BUTTON_SELECT)
            quit = 1;
        buttonsPressed = 0;
    }

    P2IE = previousP2IE;
    lcdBackLightLevelSettingLOCAL = halLcdGetBackLight();
}

/*----------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Interface to calibrate the accelerometer reading and to
 *         specify whether the board will wake up on an accelerometer tilt.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void configureAccelerometer()
{
    unsigned char quit = 0;

    halLcdClearScreen();
    halLcdPrintLine("  Accelerometer ", 0,  OVERWRITE_TEXT);
    halLcdPrintLine("Calibrate: Hit UP", 1, OVERWRITE_TEXT);

    halLcdPrintLine(" Wake Up on Acc", 4, 0);
    halLcdPrintLine("Tilting will wake", 5, 0);
    halLcdPrintLine("up the board", 6, 0);

    halLcdLine(6, 94, 32, 94, PIXEL_ON);
    halLcdLine(6, 94, 6, 109, PIXEL_ON);
    halLcdLine(32, 94, 32, 109, PIXEL_ON);
    halLcdLine(6, 109, 32, 109, PIXEL_ON);

    halLcdLine(102, 94, 128, 94, PIXEL_ON);
    halLcdLine(128, 94, 128, 109, PIXEL_ON);
    halLcdLine(102, 94, 102, 109,  PIXEL_ON);
    halLcdLine(102, 109, 128, 109,  PIXEL_ON);

    halButtonsInterruptDisable(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_SELECT + BUTTON_S1 + BUTTON_S2 + BUTTON_UP);

    while (!quit)
    {
        if (wakeUpOnAccLOCAL)
        {
            halLcdPrintLineCol("No ", 8, 1,  OVERWRITE_TEXT);
            halLcdPrintLineCol("Yes", 8, 13, INVERT_TEXT | OVERWRITE_TEXT);
        }
        else
        {
            halLcdPrintLineCol("No ", 8, 1,  INVERT_TEXT | OVERWRITE_TEXT);
            halLcdPrintLineCol("Yes", 8, 13,  OVERWRITE_TEXT);
        }

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();

        if (buttonsPressed & BUTTON_S2)
            wakeUpOnAccLOCAL = 1;
        else if (buttonsPressed & BUTTON_S1)
            wakeUpOnAccLOCAL = 0;
        if (buttonsPressed & BUTTON_UP)
        {
            halAccelerometerInit();
            halAccelerometerCalibrate();
            halAccelerometerGetCalibratedOffset(&Acc_X_Calibrated_OffsetLOCAL,   \
                                                &Acc_Y_Calibrated_OffsetLOCAL,   \
                                                &Acc_Z_Calibrated_OffsetLOCAL);
            halAccelerometerShutDown();
            halLcdPrintLine("Acc. Calibrated", 2, OVERWRITE_TEXT);
        }
        if (buttonsPressed & BUTTON_SELECT)
            quit = 1;
        buttonsPressed = 0;
    }
}

/*----------------------------------------------------------------------------*/

/**********************************************************************//**
 * @brief  Displays current firmware version
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void firmwareVersion(void)
{
    char versionString[17] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',0};

    versionString[7] = (char) SoftwareMajorVersion;
    versionString[8] = '.';
    versionString[9] = (char) SoftwareMinorVersion_H;
    versionString[10] = (char) SoftwareMinorVersion_L;

    halLcdClearScreen();
    halLcdPrintLine("     Current     ", 1,  OVERWRITE_TEXT);
    halLcdPrintLine("     Firmware    ", 2, OVERWRITE_TEXT);
    halLcdPrintLine("     Version     ", 3, OVERWRITE_TEXT);

    halLcdPrintLine(versionString, 5, 0);

    halButtonsInterruptEnable(BUTTON_ALL);

    __bis_SR_register(LPM3_bits + GIE);
    __no_operation();

    buttonsPressed = 0;
}
