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
 * ActivePowerMeasure.c
 * User Experience Code for the MSP-EXP430FR5969
 * Active Power/CHR Application
 *
 * Program Execution: User selects choices for both frequency and cache hit
 *   ratio. Intro screen is then displayed showing directions for entering and
 *   exiting the cache hit code. Upon pressing the select button, the user
 *   enters/exits the cache hit code.
 *
 * Directions for use: At first, frequency menu is displayed, to be navigated by
 *   the user with the left CapTouch slider and the select button. Next, a
 *   similar menu is displayed with the possible cache hit ratios that can be
 *   run. When the user selects this option, another intro screen is displayed.
 *   The select button can then be used to enter/exit the cache hit code. When
 *   the cache hit code is exited via the select button, the user returns to the
 *   frequency select menu. If the menu button is pressed, the user returns to
 *   the main user's experience menu.
 *
 * February 2012
 *
 ******************************************************************************/

#include "msp430.h"

#include "driverlib.h"
#include "grlib.h"
#include "Sharp96x96.h"
#include "CTS/CTS_wolverine.h"

#include "ActivePowerMeasure.h"
#include "FR59xx_EXP.h"
#include "msp430fr59xx_CHR.h"
#include "myTimer.h"
#include "UpdateDisplay.h"

#define SET_1MHZ    2
#define SET_2MHZ    4
#define SET_4MHZ    6
#define SET_8MHZ    8

#define CHR_50   2
#define CHR_66   4
#define CHR_75   6
#define CHR_100  8

unsigned char frequencyChoice;
unsigned char cacheRatioChoice;

unsigned char chooseFrequency(void);
unsigned char chooseCacheRatio(void);
void Active_intro(void);
void eUSCI_turnOn(void);
void eUSCI_turnOff(void);
void Active_setClock(unsigned char frequency);

/**********************************************************************//**
 * @brief  Active Power Application - choose frequency and cache hit ratio,
 *         then run active power mode code.
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void ActivePowerApp(void)
{
    while (mode == APP_ACTIVE_TEST) {
        select = SEL_CHOOSE_FREQ;

        eUSCI_turnOn();

        //First menu (choose frequency)
        frequencyChoice = chooseFrequency();

        //Second menu (choose cache hit ratio)
        cacheRatioChoice = chooseCacheRatio();

        //Display Instructions for App
        Active_intro();

        TA0_turnOff();                        // Turn off VCOM toggle timer
        LCD_turnOff();

        if (select == SEL_RUN_CHR) {

            eUSCI_turnOff();

            // Disable CapSense Pins
            GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
            GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);

            Active_setClock(frequencyChoice);
            switch (cacheRatioChoice) {
            case CHR_50:
                fram_chr_50();                 // run 50% cache hit code
                break;
            case CHR_66:
                fram_chr_66();                 // run 66% cache hit code
                break;
            case CHR_75:
                fram_chr_75();                 // run 75% cache hit code
                break;
            case CHR_100:
                fram_chr_100();                 // run 100% cache hit code
                break;
            default: break;
            }
            Active_setClock(SET_8MHZ);             // Reset clock to 8MHz

            // Re-enable CapSense
            GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
            GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
        }

        LCD_turnOn();
        LCD_display();
        TA0_enableVCOMToggle();                 // Enable VCOM toggle
    }
    eUSCI_turnOn();
}

/**********************************************************************//**
 * @brief  Display frequency options on LCD, choose one using left CapTouch
 *         and right button. Return the choice that was made.
 *
 * @param  none
 *
 * @return choice - choice selected
 *************************************************************************/
unsigned char chooseFrequency(void)
{
    unsigned char choice = 0;

    while (select == SEL_CHOOSE_FREQ) {
        CTS_getReading();

        // Choose a frequency
        if (sliderPos[0] <= 18) {
            choice = SET_8MHZ;
        }
        else if (sliderPos[0] <= 24) {
            choice = SET_4MHZ;
        }
        else if (sliderPos[0] <= 30) {
            choice = SET_2MHZ;
        }
        else {
            choice = SET_1MHZ;
        }
        LCD_displayFrequencyMenu(choice);         // show menu, highlighting selected option
        LCD_display();
    }
    return choice;
}

/**********************************************************************//**
 * @brief  Display cache hit ratios on LCD, choose one using left CapTouch
 *         and the right button. Return the choice that was made.
 *
 * @param  none
 *
 * @return choice - choice selected
 *************************************************************************/
unsigned char chooseCacheRatio(void)
{
    unsigned char choice = 0;

    while (select == SEL_CHOOSE_CHR) {
        CTS_getReading();

        // Choose a cache hit ratio
        if (sliderPos[0] <= 18) {
            choice = CHR_100;
        }
        else if (sliderPos[0] <= 24) {
            choice = CHR_75;
        }
        else if (sliderPos[0] <= 30) {
            choice = CHR_66;
        }
        else {
            choice = CHR_50;
        }
        LCD_displayCacheRatioMenu(choice);         // show menu, highlighting selected option
    }

    return choice;
}

/**********************************************************************//**
 * @brief  Display intro/instructions until button is pushed to enter into
 *         specific cache hit code
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void Active_intro(void)
{
    while (select == SEL_SHOW_INTRO) {
        LCD_activePowerIntro();                 // Display active power intro screen

        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();
    }
}

/**********************************************************************//**
 * @brief  Turn on eUSCI
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void eUSCI_turnOn(void)
{
    EUSCI_A_SPI_enable(__MSP430_BASEADDRESS_EUSCI_A1__);
}

/**********************************************************************//**
 * @brief  Turn off eUSCI
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void eUSCI_turnOff(void)
{
    EUSCI_A_SPI_disable(__MSP430_BASEADDRESS_EUSCI_A1__);
}

/**********************************************************************//**
 * @brief  Set the clock to specified frequency (in MHz)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void Active_setClock(unsigned char frequency)
{
    switch (frequency) {
    case SET_1MHZ:                              // Set DCO = 1MHz
        CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_0);
        break;
    case SET_2MHZ:                              // Set DCO = 2.67MHz
        CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_1);
        break;
    case SET_4MHZ:                              // Set DCO = 4MHz
        CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_3);
        break;
    case SET_8MHZ:                              // Set DCO = 8MHz
        CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);
        break;
    default: break;
    }

    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}
