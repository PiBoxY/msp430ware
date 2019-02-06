/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//#############################################################################
//
//! \file   boostxl-capkeypad.c
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "system.h"
#include "host_interface.h"
#include "boostxl-capkeypad.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

#define BOOSTXLCAPKEYPAD_KEYPAD_ID (0)
#define BOOSTXLCAPKEYPAD_PROX_ID (1)

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief These are references to the element structures that are defined
//! in CAPT_UserConfig.c.
extern tElement keypadSensor_E00;
extern tElement keypadSensor_E01;
extern tElement keypadSensor_E02;
extern tElement keypadSensor_E03;
extern tElement keypadSensor_E04;
extern tElement keypadSensor_E05;
extern tElement keypadSensor_E06;
extern tElement keypadSensor_E07;
extern tElement keypadSensor_E08;
extern tElement keypadSensor_E09;
extern tElement keypadSensor_E10;
extern tElement keypadSensor_E11;

//! \brief BXLCapKeypad_timeoutCounter is the active mode timeout counter.
//! This is used to track how long (in samples) it has been since a user was
//! present.  It is a down counter.  When the counter reaches zero, active
//! mode is exited and wake-on-proximity sleep mode is entered.
//!
uint16_t BXLCapKeypad_timeoutCounter;

//! \brief Active LPM bits
//!
uint8_t BXLCapKeypad_activeLPM = LPM0_bits;

//! \brief Wake-on-Prox LPM bits
//!
uint8_t BXLCapKeypad_wopLPM = LPM4_bits;

//*****************************************************************************
// Function Declarations
//*****************************************************************************

//! \brief This function controls calibrating the keypad.
//! This routine is used instead of CAPT_calibrateUI(), beause the prox
//! sensor requires the enabling of multiple TX's, which is not supported
//! by the standard library calibration function.
//! \param none
//! \return none
static void BXLCapKeypad_calibrateKeypad(void);

//! \brief This function updates the keypad and prox sensor immediately.
//! It does not update the status word, nor does it stream data to the
//! CapTIvate Design Center.
//! \param none
//! \return none
static void BXLCapKeypad_updateKeypad(void);

//! \brief Handle sensor error conditions, such as a negative touch or
//! a max count error.  The response to either event is to perform
//! an immediate re-calibration.
//! \param sensor is a pointer to the sensor to test
//! \return none
static void BXLCapKeypad_sensorErrorHandler(tSensor *sensor);

//! \brief Update the system status word based on the CapTIvate sensor
//! structures.
//! \param none
//! \return none
static void BXLCapKeypad_updateStatusWord(void);

//! \brief Update the g_uiApp LPM to reflect the current app state
//! and state low power mode, as specified in BXLCapKeypad_activeLPM and
//! BXLCapKeypad_wopLPM
//! \param none
//! \return none
static void BXLCapKeypad_updateCurrentAppLPM(void);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void BXLCapKeypad_init(void)
{
    MAP_CAPT_powerOn();
    MAP_CAPT_init();
    MAP_CAPT_initSensorIO(&keypadSensor);
    MAP_CAPT_enableSensorIO(&keypadSensor);
    MAP_CAPT_initSensor(&keypadSensor);
    MAP_CAPT_initSensorIO(&proximitySensor);
    MAP_CAPT_enableSensorIO(&proximitySensor);
    MAP_CAPT_initSensor(&proximitySensor);
    MAP_CAPT_registerCallback(&keypadSensor, &BXLCapKeypad_sensorErrorHandler);
    MAP_CAPT_registerCallback(&proximitySensor, &BXLCapKeypad_sensorErrorHandler);
    CAPT_initCommInterface(&g_uiApp);

    BXLCapKeypad_calibrateKeypad();
    BXLCapKeypad_timeoutCounter = g_uiApp.ui16InactivityTimeout;

    MAP_CAPT_stopTimer();
    MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
    MAP_CAPT_selectTimerSourceDivider(CAPT_TIMER_CLKDIV__1);
    MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod));
    MAP_CAPT_startTimer();
    MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);
}

void BXLCapKeypad_stop(void)
{
    MAP_CAPT_stopTimer();
    MAP_CAPT_reset();
    MAP_CAPT_powerOff();
    LEDControl_suspendLEDs();
}

void BXLCapKeypad_calibrateKeypad(void)
{
    // Calibrate the keypad
    CAPT_calibrateSensor(&keypadSensor);

    // Calibrate the proximity sensor
    MAP_CAPT_setChannelActive(1, 3);
    MAP_CAPT_setChannelActive(0, 0);
    CAPT_calibrateSensor(&proximitySensor);
    MAP_CAPT_clearChannelActive(1, 3);
    MAP_CAPT_clearChannelActive(0, 0);
}

void BXLCapKeypad_updateKeypad(void)
{
    // Update keypad sensor, test for re-calibration
    CAPT_updateSensor(&keypadSensor, g_uiApp.ui8AppLPM);

    // Update prox sensor, test for re-calibration
    MAP_CAPT_setChannelActive(1, 3);
    MAP_CAPT_setChannelActive(0, 0);
    CAPT_updateSensor(&proximitySensor, g_uiApp.ui8AppLPM);
    MAP_CAPT_clearChannelActive(1, 3);
    MAP_CAPT_clearChannelActive(0, 0);
}

void BXLCapKeypad_sensorErrorHandler(tSensor *sensor)
{
    if ((MAP_CAPT_testForMaxCountRecalibration(sensor) == true) ||\
        (MAP_CAPT_testForNegativeTouchRecalibration(sensor) == true) ||\
        (MAP_CAPT_testForRecalibration(sensor) == true))
    {
        CAPT_initSensor(sensor);
        if (sensor == &proximitySensor)
        {
            MAP_CAPT_setChannelActive(1, 3);
            MAP_CAPT_setChannelActive(0, 0);
            CAPT_calibrateSensor(sensor);
            MAP_CAPT_clearChannelActive(1, 3);
            MAP_CAPT_clearChannelActive(0, 0);
        }
        else
        {
            CAPT_calibrateSensor(sensor);
        }
    }
}

void BXLCapKeypad_sleep(void)
{
    __bic_SR_register(GIE);
    if (!(g_bConvTimerFlag ||g_bDetectionFlag || g_bConvCounterFlag || g_bMaxCountErrorFlag))
    {
        __bis_SR_register(g_uiApp.ui8AppLPM | GIE);
    }
    else
    {
        __bis_SR_register(GIE);
    }
}

void BXLCapKeypad_run(void)
{
    uint16_t keyPWM, statPWM;
    bool userPresent;

    // Handle active mode case
    if (g_uiApp.state == eUIActive)
    {
        if (g_bConvTimerFlag == true)
        {
            // Update keypad and user presence status
            g_bConvTimerFlag = false;
            HOST_RELEASE_IRQ;
            BXLCapKeypad_updateKeypad();
            BXLCapKeypad_updateStatusWord();
            HOST_PULL_IRQ;
            LEDControl_updateLEDs();
            CAPT_writeSensorData(BOOSTXLCAPKEYPAD_KEYPAD_ID);
            CAPT_writeElementData(BOOSTXLCAPKEYPAD_KEYPAD_ID);
            CAPT_writeSensorData(BOOSTXLCAPKEYPAD_PROX_ID);
            CAPT_writeElementData(BOOSTXLCAPKEYPAD_PROX_ID);

            // If user is present, reset the timeout counter
            // If user is not present, decrement the timeout counter
            userPresent = CAPT_getGlobalUIProximityStatus(&g_uiApp);
            keyPWM = LEDControl_getPWMDutyCycle(0);
            statPWM = LEDControl_getPWMDutyCycle(1);
            if (userPresent == true)
            {
                BXLCapKeypad_timeoutCounter = g_uiApp.ui16InactivityTimeout;

                if (keyPWM < LEDCONTROL__LED_IDLE_PWM)
                {
                    keyPWM++;
                    LEDControl_setPWMDutyCycle(0, keyPWM);
                }

                if (statPWM < LP3943_PWM_100)
                {
                    statPWM += 4;
                    if (statPWM > LP3943_PWM_100)
                    {
                        statPWM = LP3943_PWM_100;
                    }
                    LEDControl_setPWMDutyCycle(1, statPWM);
                }
            }
            else if (System_topLevel.controlWord & SYSTEM__CTRL_WOP_ENABLE)
            {
                BXLCapKeypad_timeoutCounter--;

                if (BXLCapKeypad_timeoutCounter <= LEDCONTROL__LED_IDLE_PWM)
                {
                    LEDControl_setPWMDutyCycle(0, BXLCapKeypad_timeoutCounter);
                }
                else if (keyPWM < LEDCONTROL__LED_IDLE_PWM)
                {
                    keyPWM++;
                    LEDControl_setPWMDutyCycle(0, keyPWM);
                }

                if (BXLCapKeypad_timeoutCounter <= LP3943_PWM_100)
                {
                    LEDControl_setPWMDutyCycle(1, BXLCapKeypad_timeoutCounter);
                }
                else if (statPWM < LP3943_PWM_100)
                {
                    statPWM += 4;
                    if (statPWM > LP3943_PWM_100)
                    {
                        statPWM = LP3943_PWM_100;
                    }
                    LEDControl_setPWMDutyCycle(1, statPWM);
                }
            }

            // If the timeout countdown has expired, end active mode
            // and go into wake on proximity mode
            if ((BXLCapKeypad_timeoutCounter == 0) &&\
                    (System_topLevel.controlWord & SYSTEM__CTRL_WOP_ENABLE))
            {
                g_uiApp.state = eUIWakeOnProx;
                BXLCapKeypad_updateCurrentAppLPM();
                HOST_RELEASE_IRQ;
                LEDControl_suspendLEDs();
                MAP_CAPT_disableISR(CAPT_TIMER_INTERRUPT);
                MAP_CAPT_stopTimer();
                MAP_CAPT_clearTimer();
                MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_VLOCLK);
                MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES_VLO(g_uiApp.ui16WakeOnProxModeScanPeriod));
                MAP_CAPT_startTimer();
                g_bConvTimerFlag = false;
                MAP_CAPT_setChannelActive(1, 3);
                MAP_CAPT_setChannelActive(0, 0);
                MAP_CAPT_startWakeOnProxMode(
                        &proximitySensor,
                        0,
                        g_uiApp.ui8WakeupInterval
                    );
            }
        }
    }

    // Handle wake-on-proximity case
    else
    {
        // if a detection, conversion counter, or error flag occurred, wake the system
        // into active mode to measure all sensors
        if (g_bDetectionFlag || g_bConvCounterFlag || g_bMaxCountErrorFlag ||\
                !(System_topLevel.controlWord & SYSTEM__CTRL_WOP_ENABLE))
        {
            MAP_CAPT_stopWakeOnProxMode(&proximitySensor, 0);
            MAP_CAPT_clearChannelActive(1, 3);
            MAP_CAPT_clearChannelActive(0, 0);
            MAP_CAPT_stopTimer();
            MAP_CAPT_clearTimer();

            // If the conversion counter flag is true, but there is no detection and no max count error,
            // reseed all elements in the keypad and perform a conversion to refresh the LTA's of each key in the keypad,
            // as these are not measured in wake-on-proximity mode.
            if ((g_bConvCounterFlag == true) && (g_bDetectionFlag == false) && (g_bMaxCountErrorFlag == false) &&\
                    (System_topLevel.controlWord & SYSTEM__CTRL_WOP_ENABLE))
            {
                g_bConvCounterFlag = false;
                MAP_CAPT_flagAllElementsForReseed(&keypadSensor);
                BXLCapKeypad_updateKeypad();
                MAP_CAPT_setChannelActive(1, 3);
                MAP_CAPT_setChannelActive(0, 0);
                MAP_CAPT_startWakeOnProxMode(
                        &proximitySensor,
                        0,
                        g_uiApp.ui8WakeupInterval
                    );
                MAP_CAPT_startTimer();
            }

            // If the wakeup cause was not solely conversion counter flag, switch to active mode
            // to properly handle a wakeup event
            else
            {
                g_bDetectionFlag = false;
                g_bMaxCountErrorFlag = false;
                g_uiApp.state = eUIActive;
                BXLCapKeypad_updateCurrentAppLPM();
                BXLCapKeypad_timeoutCounter = g_uiApp.ui16InactivityTimeout;
                MAP_CAPT_selectTimerSource(CAPT_TIMER_SRC_ACLK);
                MAP_CAPT_writeTimerCompRegister(CAPT_MS_TO_CYCLES(g_uiApp.ui16ActiveModeScanPeriod));
                MAP_CAPT_clearIFG(CAPT_TIMER_INTERRUPT);
                MAP_CAPT_enableISR(CAPT_TIMER_INTERRUPT);
                MAP_CAPT_startTimer();
            }
        }
    }
}

void BXLCapKeypad_updateCapTIvateComm(void)
{
    if (System_topLevel.controlWord & SYSTEM__CTRL_UART_ENABLE)
    {
        // Ensure that the app low power modes are configured to
        // support UART communication
        BXLCapKeypad_activeLPM = LPM0_bits;
        BXLCapKeypad_wopLPM = LPM0_bits;
        BXLCapKeypad_updateCurrentAppLPM();
        g_uiApp.bElementDataTxEnable = true;
        g_uiApp.bSensorDataTxEnable = true;

        CAPT_checkForInboundPacket();
        if (CAPT_checkForRecalibrationRequest() == true)
        {
            if (g_uiApp.state == eUIWakeOnProx)
            {
                CAPT_stopWakeOnProxMode(
                        &CAPT_WAKEONPROX_SENSOR,
                        0
                    );
                BXLCapKeypad_calibrateKeypad();
                CAPT_startWakeOnProxMode(
                        &CAPT_WAKEONPROX_SENSOR,
                        0,
                        g_uiApp.ui8WakeupInterval
                    );
            }
            else
            {
                BXLCapKeypad_calibrateKeypad();
            }
        }
    }
    else
    {
        g_uiApp.bElementDataTxEnable = false;
        g_uiApp.bSensorDataTxEnable = false;
        BXLCapKeypad_activeLPM = LPM3_bits;
        BXLCapKeypad_wopLPM = LPM4_bits;
        BXLCapKeypad_updateCurrentAppLPM();
    }
}

void BXLCapKeypad_updateCurrentAppLPM(void)
{
    if (g_uiApp.state == eUIActive)
    {
        g_uiApp.ui8AppLPM = BXLCapKeypad_activeLPM;
    }
    else
    {
        g_uiApp.ui8AppLPM = BXLCapKeypad_wopLPM;
    }
}

void BXLCapKeypad_updateStatusWord(void)
{
    // Update last status word
    System_topLevel.lastStatusWord = System_topLevel.statusWord;

    // Clear all status flags
    System_topLevel.statusWord = 0;

    // Set all flags that are now set

    // Mode Status
    if (g_uiApp.state == eUIActive)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_ACTIVE;
    }

    // Guard Status
    if (proximitySensor.bSensorTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_GUARD;
        System_topLevel.statusWord &= ~SYSTEM__STAT_ALLKEYS;
        System_topLevel.statusWord &= ~SYSTEM__STAT_TOUCH;
        return;
    }

    // Proximity Status
    if (keypadSensor.bSensorProx == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_PROX;
    }

    // Touch Status
    if (keypadSensor.bSensorTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_TOUCH;
    }

    // Key 1 Status
    if (keypadSensor_E00.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY1;
    }

    // Key 4 Status
    if (keypadSensor_E01.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY4;
    }

    // Key 7 Status
    if (keypadSensor_E02.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY7;
    }

    // Enter Key Status
    if (keypadSensor_E03.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEYENTER;
    }

    // Key 2 Status
    if (keypadSensor_E04.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY2;
    }

    // Key 5 Status
    if (keypadSensor_E05.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY5;
    }

    // Key 8 Status
    if (keypadSensor_E06.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY8;
    }

    // Key 0 Status
    if (keypadSensor_E07.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY0;
    }

    // Key 3 Status
    if (keypadSensor_E08.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY3;
    }

    // Key 6 Status
    if (keypadSensor_E09.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY6;
    }

    // Key 9 Status
    if (keypadSensor_E10.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEY9;
    }

    // Cancel Key Status
    if (keypadSensor_E11.bTouch == true)
    {
        System_topLevel.statusWord |= SYSTEM__STAT_KEYCANCEL;
    }
}
