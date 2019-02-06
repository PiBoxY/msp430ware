/*******************************************************************************
 *
 *  UserExperience.c - User Experience Demo
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

#include "msp430.h"
#include "hal_MSP-EXP430F5438.h"
#include "UserExperience_F5438A.h"
#include "UserExperienceGraphics.h"
#include "flashUtils.h"
#include "clock.h"
#include "balanceBall.h"
#include "usbTest.h"
#include "audio.h"
#include "menuSetting.h"
#include "PMM.h"
#include "LPM.h"
#include "PowerTest.h"
#include "adcTemp.h"
#include "FFT.h"

#define TIME_OUT          10
#define TIME_OUT2         3
#define MENU_MAX          7
#define SETTING_MENU_MAX  6
#define MENU_ITEM_WIDTH   15

enum {  LPM4_MODE, LPM3_MODE, ACTIVE_MODE, APPLICATION_MODE };
enum {  APP_CLOCK, APP_BAL_BALL, APP_USB, APP_AUDIO,  PMM_MCLK, ADC_TEMP, MENU_SETTING};
enum {  SET_TIME, SET_CONTRAST, SET_BACKLIGHT, CONFIGURE_ACCELEROMETER, VERSION, EXIT_SETTING};

//--Calibration constants and user configuration values stored in INFOB Flash--
// On the MSP430F5438A this memory section is 128B long and ranges from 0x197Fh to 0x1900
#ifdef __TI_COMPILER_VERSION__

// If CCS compiler is used
#    pragma DATA_SECTION(boardMode, ".infoB");
unsigned char boardMode;

#    pragma DATA_SECTION(lcdBackLightLevelSetting, ".infoB");
unsigned char lcdBackLightLevelSetting;

#    pragma DATA_SECTION(lcdContrastSetting, ".infoB");
unsigned char lcdContrastSetting;

#    pragma DATA_SECTION(lastAudioByteFlash, ".infoB");
#    pragma DATA_ALIGN(lastAudioByteFlash, 2);
unsigned long lastAudioByteFlash;

#    pragma DATA_SECTION(Acc_X_Calibrated_Offset, ".infoB");
#    pragma DATA_ALIGN(Acc_X_Calibrated_Offset, 2);
int Acc_X_Calibrated_Offset;

#    pragma DATA_SECTION(Acc_Y_Calibrated_Offset, ".infoB");
#    pragma DATA_ALIGN(Acc_Y_Calibrated_Offset, 2);
int Acc_Y_Calibrated_Offset;

#    pragma DATA_SECTION(Acc_Z_Calibrated_Offset, ".infoB");
#    pragma DATA_ALIGN(Acc_Z_Calibrated_Offset, 2);
int Acc_Z_Calibrated_Offset;

#    pragma DATA_SECTION(wakeUpOnAcc, ".infoB");
unsigned char wakeUpOnAcc;

#elif __IAR_SYSTEMS_ICC__

// If IAR compiler is used
#    define INFOB _Pragma("location=\"INFOB\"")
INFOB __no_init unsigned char boardMode;
INFOB __no_init unsigned char lcdBackLightLevelSetting;
INFOB __no_init unsigned char lcdContrastSetting;
INFOB __no_init unsigned long lastAudioByteFlash;
INFOB __no_init int Acc_X_Calibrated_Offset;
INFOB __no_init int Acc_Y_Calibrated_Offset;
INFOB __no_init int Acc_Z_Calibrated_Offset;
INFOB __no_init unsigned char wakeUpOnAcc;

#endif

unsigned char audioISR = 0;
unsigned char fftISR = 0;
unsigned char adcTempISR = 0;
unsigned char boardModeLOCAL;
int Acc_X_Calibrated_OffsetLOCAL;
int Acc_Y_Calibrated_OffsetLOCAL;
int Acc_Z_Calibrated_OffsetLOCAL;
unsigned char lcdBackLightLevelSettingLOCAL;
unsigned char lcdContrastSettingLOCAL;
unsigned char wakeUpOnAccLOCAL;

static const char menuText[] = {
    "MSP-EXP430F5438\0"
    " 1. Clock     \0"
    " 2. UniBall   \0"
    " 3. USB-UART  \0"
    " 4. Audio Apps\0"
    " 5. Power Test\0"
    " 6. ADC Temp  \0"
    " 7. Settings  \0"
};

unsigned char menuPos, settingMenuPos;
unsigned char timeOutCounter  = 0;
unsigned char CpuMode, accWake = 0, menuPos, settingMenuPos;

volatile unsigned char buttonsPressed;
volatile unsigned char buttonDebounce;

char VccStr[] = "0.0V";
unsigned char RTCAccHalfSec = 0, RTCExit64Hz = 0, RTCExitSec = 0, RTCAccSec = 0;

/**********************************************************************//**
 * @brief  Checks for the board revision
 *
 * @param  none
 *
 * @return Whether or not the board revision matches the software
 * - 0 - The board revision does not match the software
 * - 1 - The board revision matches the software
 *************************************************************************/

unsigned char assert_board_version(void)
{

    P8DIR &= ~BIT7;                         // Set P8.7 input
    P8OUT |= BIT7;                          // Set pullup resistor
    P8REN |= BIT7;                          // Enable pull up resistors

    if (P8IN & BIT7)                        // Board rev != 0_03?
        return 0;                           // If so, indicate failure

    P8DIR |= BIT7;                          // Set P8.7 output
    P8OUT &= ~BIT7;                         // Set P8.7 = 0
    P8REN &= ~BIT7;                         // Disable pull up resistors

    return 1;
}

/**********************************************************************//**
 * @brief  Enters LPM3 and waits for either accelerometer tilt or a button tilt
 *         to activate the board.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void lowPowerMode3(void)
{
    int accX, accY, accZ;

    CpuMode = LPM3_MODE;

    halLcdClearScreen();
    halLcdImage(TI_BUG, 14, 106, 10, 0);

    halLcdSetBackLight(0);
    halLcdStandby();

    accWake = 0;
    if (wakeUpOnAcc)
    {
        RTCCTL0 |= RTCRDYIE;                //Enable interrupt
        RTCAccSec = 1;
        halAccelerometerInit();
        halAdcSetQuitFromISR(1);
    }
    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(12000, 366);
    SFRIE1 |= OFIE;

    /* Either a button press or an RTC interrupt will wake the CPU
     * from LPM3 mode. The RTC interrupt will periodically enable and
     * re-initialize the accelerometer to see if the user has registered
     * a change in the tilt that is greater than the accelerometer thresholds.
     * If so, accWake is set and the board is activated
     */
    do
    {
        halAccelerometerShutDown();

        __bis_SR_register(LPM3_bits + GIE); // Enter LPM3
        __no_operation();                   // For debugger only

        if (!buttonsPressed)
        {
            halAccelerometerRead(&accX, &accY, &accZ);
            accWake = (accX > ACC_X_THRESHOLD || accX < -ACC_X_THRESHOLD ||
                                                        accY > ACC_Y_THRESHOLD || accY <
                       -ACC_Y_THRESHOLD);
        }
    }
    while (accWake == 0 && buttonsPressed == 0);

    SFRIE1 &= ~OFIE;                        //disable oscillator fault before configuring FLL
                                            // because of DCOFFG
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;

    RTCCTL0 |= RTCRDYIE;                    //Enable interrupt
    RTCAccSec = 0;
    halAccelerometerShutDown();

    halLcdInit();
    halLcdClearScreen();
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
    CpuMode = ACTIVE_MODE;
}

// -------------- Active Mode with Menu---------------------------------------

/**********************************************************************//**
 * @brief  Enters LPM3 and waits for either accelerometer tilt or a button tilt
 *         to activate the board.
 *
 * @param  menuText The text that constitues the application menu.
 *
 * @param  menuPos  The line of the current menu option.
 *
 * @param  change
 *
 * - 0 - Move menu selection up
 * - 1 - Move menu selection down
 *
 * @param menuNum   The enumerated value that represents the current
 *                  menu selection.
 *
 * @return none
 *************************************************************************/

void menuUpdate(char *menuText, unsigned char *menuPos, int change,
                unsigned char menuNum)
{
    halLcdPrintLine(&menuText[*menuPos * MENU_ITEM_WIDTH + 16],
                    (*menuPos) + 1,  OVERWRITE_TEXT);

    if (change == 0)                        //Subtract
    {
        if (*menuPos > 0)
            (*menuPos)--;
        else
            *menuPos = (menuNum - 1);
    }
    else
    {
        if ((*menuPos) < menuNum - 1)
            (*menuPos)++;
        else
            *menuPos = 0;
    }

    halLcdPrintLine(&menuText[*menuPos * MENU_ITEM_WIDTH + 16],
                    (*menuPos) + 1, INVERT_TEXT | OVERWRITE_TEXT);

    halLcdImage(TI_TINY_BUG, 4, 36, 104, 12);
}

/**********************************************************************//**
 * @brief  Draws and manages the selection of the menu options.
 *
 * @param  menuText The text that constitues the application menu.
 *
 * @param  menuNum  The enumerated value that represents the current
 *                  menu selection.
 *
 * @return The updated, or latest, menu selection.
 *************************************************************************/

unsigned char activeMenuMode(char *menuText, unsigned char menuNum)
{
    unsigned char menuPosition, quit = 0;
    int i;

    halAccelerometerShutDown();
    halAdcInitVcc();
    RTCExitSec = 1;                         // To update digital clock
    halButtonsInterruptEnable(BUTTON_ALL);

    halLcdClearScreen();

    menuPosition = 0;
    //Print menu title
    halLcdPrintLine(menuText, 0, 0);

    //Print menu items
    for (i = 1; i < menuNum; i++)
        halLcdPrintLine(&menuText[i * MENU_ITEM_WIDTH + 16], i + 1,  OVERWRITE_TEXT);
    //First line is inverted text, automatic selection
    halLcdPrintLine(&menuText[0 * MENU_ITEM_WIDTH + 16],  1,     \
                    INVERT_TEXT | OVERWRITE_TEXT);

    halLcdImage(TI_TINY_BUG, 4, 36, 104, 12);

    timeOutCounter = 0;
    buttonsPressed = 0;

    halAdcSetQuitFromISR(0);

    while (CpuMode == ACTIVE_MODE && quit == 0)
    {
        TA0CTL &= ~TAIFG;

        __bis_SR_register(LPM3_bits + GIE); //Returns if button pressed or clock ticks
        __no_operation();                   // For debugger only

        if (buttonsPressed)
        {
            switch (buttonsPressed)
            {
                case BUTTON_UP:       menuUpdate(menuText, &menuPosition, 0, menuNum); break;
                case BUTTON_DOWN:     menuUpdate(menuText, &menuPosition, 1, menuNum); break;
                case BUTTON_SELECT:   CpuMode = APPLICATION_MODE; break;
                case BUTTON_S2:       CpuMode = APPLICATION_MODE; break;
                case BUTTON_S1:       quit = 1;
                default: break;
            }
            timeOutCounter  = 0;
        }
        else                                //if no button pressed --> clock ticks
        {
            halAdcStartRead();
            digitalClockDraw();
            halAdcReadVcc(VccStr);
            halLcdPrintLineCol(VccStr, 8, 12,  OVERWRITE_TEXT);
            if (++timeOutCounter > TIME_OUT)
                CpuMode = LPM3_MODE;
        }
        buttonsPressed  = 0;
    }
    RTCExitSec = 0;
    halAdcShutDownVcc();

    return menuPosition;
}

/**********************************************************************//**
 * @brief  This is the example code's main function.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void UserExperience(void)
{
    /* Check for the version of the board. This code requires an
     * MSP-EXP430F5438 REV 0-03 (or later) to work. Trap code execution
     * if validation fails. */
    if (!assert_board_version()){
        while (1) ;
    }

    /* Check the silicon the code is running on. This code requires an
     * actual MSP430F5438A device to work. Trap code execution
     * if validation fails. */
    if (Get_Device_Type() != F5438A){
        while (1) ;
    }

    //Initialize clock and peripherals
    halBoardInit();
    LFXT_Start(XT1DRIVE_0);
    Init_FLL_Settle(16000, 488);
    SFRIE1 |= OFIE;

    loadSettings();

    //Initialize LCD and backlight
    halLcdInit();
    halLcdBackLightInit();
    halLcdSetBackLight(lcdBackLightLevelSettingLOCAL);
    halLcdSetContrast(lcdContrastSettingLOCAL);
    halLcdClearScreen();

    halLcdImage(TI_BUG, 14, 106, 10, 0);

    //Initialize buttons
    buttonDebounce = 1;
    halButtonsInit(BUTTON_ALL);
    halButtonsInterruptEnable(BUTTON_ALL);

    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3
    __no_operation();                       // For debugging only

    CpuMode = ACTIVE_MODE;
    setupRTC();

    while (1)                               // main loop
    {
        switch (CpuMode)
        {
            case LPM3_MODE:
                lowPowerMode3();
                break;
            case ACTIVE_MODE:
                menuPos = activeMenuMode((char *)menuText, MENU_MAX);
                break;
            case APPLICATION_MODE:
                CpuMode = ACTIVE_MODE;
                halButtonsInterruptEnable(BUTTON_ALL);

                switch (menuPos)
                {
                    case APP_BAL_BALL:
                        balanceBall();
                        break;
                    case APP_CLOCK:
                        displayAnalogClock();
                        break;
                    case APP_USB:
                        usbTest();
                        break;
                    case APP_AUDIO:
                        // FFTrecorder();
                        audioMenu();
                        saveSettings();
                        break;
                    case PMM_MCLK:
                        menuPower();
                        break;
                    case ADC_TEMP:
                        ADCTemp();
                        break;
                    case MENU_SETTING:
                        CpuMode = ACTIVE_MODE;
                        settingMenuPos = activeMenuMode((char *)settingMenuText, SETTING_MENU_MAX);
                        if (CpuMode == APPLICATION_MODE)
                        {
                            switch (settingMenuPos)
                            {
                                case SET_TIME:
                                    setTime();
                                    break;
                                case SET_CONTRAST:
                                    setContrast();
                                    break;
                                case SET_BACKLIGHT:
                                    setBackLight();
                                    break;
                                case CONFIGURE_ACCELEROMETER:
                                    configureAccelerometer();
                                    break;
                                case VERSION:
                                    firmwareVersion();
                                    break;
                                case EXIT_SETTING:
                                    saveSettings();
                                    menuPos = APP_CLOCK;
                                    CpuMode = ACTIVE_MODE;
                                    break;
                            }
                        }
                        //save settings if timeout in setting menu
                        if (CpuMode == LPM3_MODE)
                        {
                            saveSettings();
                        }
                }
                break;
        }
    }
}

/**********************************************************************//**
 * @brief  Initializes the RTC calendar.
 *
 * Initial values are January 01, 2009, 12:30:05
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void setupRTC(void)
{
    RTCCTL01 = RTCMODE + RTCBCD + RTCHOLD + RTCTEV_1;

    SetRTCHOUR(0x04);
    SetRTCMIN(0x30);
    SetRTCSEC(0x05);
    SetRTCDAY(0x01);
    SetRTCMON(0x01);
    SetRTCYEAR(0x2009);

    RTCCTL01 &= ~RTCHOLD;

    RTCPS1CTL = RT1IP_5;                    // Interrupt freq: 2Hz
    RTCPS0CTL = RT0IP_7;                    // Interrupt freq: 128hz

    RTCCTL0 |= RTCRDYIE + RTCTEVIE;         // Enable interrupt
}

/**********************************************************************//**
 * @brief  Sets up the WDT as a button debouncer, only activated once a
 *         button interrupt has occurred.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/

void startWDT()
{
    //WDT as 250ms interval counter
    SFRIFG1 &= ~WDTIFG;
    WDTCTL = WDTPW + WDTSSEL_1 + WDTTMSEL + WDTCNTCL + WDTIS_5;
    SFRIE1 |= WDTIE;
}

/************************************************************************/
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    static unsigned char counter = 0;

    switch (RTCIV)
    {
        case 0x02:  if (RTCExitSec == 1)
                __bic_SR_register_on_exit(LPM3_bits);

            if (RTCAccSec == 1)
            {
                halAccelerometerInit();
                halAdcStartRead();
            }
            break;
        case 0x04:  break;
        case 0x08:  if (RTCExit64Hz == 1)
                if (++counter == 4)
                {
                    counter = 0;
                    __bic_SR_register_on_exit(LPM3_bits);
                }
            break;
        case 0x0A:  if (RTCAccHalfSec == 1)
            {
                ADC12IFG = 0;
                ADC12CTL0 |= ADC12ENC | ADC12SC;
            }
            break;

    }
    RTCCTL0 &= ~RTCRDYIFG;
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    if (buttonDebounce == 2)
    {
        buttonDebounce = 1;
        SFRIFG1 &= ~WDTIFG;
        SFRIE1 &= ~WDTIE;
        WDTCTL = WDTPW + WDTHOLD;
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if (buttonDebounce == 1)
    {
        buttonsPressed = P2IFG;
        buttonDebounce = 2;
        startWDT();
        __bic_SR_register_on_exit(LPM3_bits);
    }
    else if (0 == buttonDebounce)
    {
        buttonsPressed = P2IFG;
        __bic_SR_register_on_exit(LPM4_bits);
    }

    P2IFG = 0;
}

#pragma vector=UNMI_VECTOR
__interrupt void UNMI_ISR(void)
{
    switch (SYSUNIV)
    {
        case 0x0004:  LFXT_Start(XT1DRIVE_0); break;
        default: break;
    }
}

