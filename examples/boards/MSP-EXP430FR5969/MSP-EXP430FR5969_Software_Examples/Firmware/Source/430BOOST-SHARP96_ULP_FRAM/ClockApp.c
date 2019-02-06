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
 * ClockApp.c
 * User Experience Code for the MSP-EXP430FR5969
 * Clock Application
 *
 * Program Execution: Clock is initialized, started, and displayed. Device goes
 *   into LPM3 awaiting a 1s RTC interrupt to update the clock again.
 *
 * Directions for use: Clock may be used as is, or can be set using the left
 *   CapTouch slider (to increment/decrement parameter) and right select button
 *   (to cycle through parameters). Time on exit is retained unless ULP ADC mode
 *   is entered.
 *
 * February 2012
 *
 *************************************************************************//****
 * Rev 1.2 - driverlib integration
 ******************************************************************************/

#include "driverlib.h"
#include "grlib.h"
#include "Sharp96x96.h"
#include "CTS/CTS_wolverine.h"

#include "ClockApp.h"
#include "FR59xx_EXP.h"
#include "myTimer.h"
#include "UpdateDisplay.h"

//-----------------------------------------------------------------------------
// Clock Global Variables
volatile unsigned char setClock = 1;
volatile unsigned char freshFlag = 1;   //  1: update LCD; 0: no update to LCD
volatile unsigned char ULP = 0;         //   0: 1 second update in run_clock mode; 1: 1 minute update to measure current in LPM3
volatile unsigned char firstEnter = 1;

static void Clock_App_init(void);
static unsigned char Clock_setParameter(unsigned char value, unsigned char max,
                                        unsigned char min);
static unsigned int  Clock_setYear(unsigned int value, unsigned int max,
                                   unsigned int min);
static void Clock_App_exit(void);

/**********************************************************************//**
 * @brief  Main clock application
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void Clock_App(void)
{
    unsigned char current_param;
    unsigned int year_param;                     // treated different (max value = 4095)

    Clock_App_init();
    freshFlag = 1;                              // update when first enter
    ULP = 0;                                    //update every second when enter this app the first time
    firstEnter = 1;

    while (mode == APP_CLOCK) {
        switch (select) {
        case SEL_RUN_CLOCK:
            if (!ULP) {                                                         // every second update?
                LCD_updateClock(1, 1, firstEnter, ULP);                         // Update LCD buffer with clock
            }
            else {                                                              // update every minute
                if (freshFlag) {
                    LCD_updateClock(1, 1, firstEnter, ULP);                     // Update LCD buffer with clock
                    freshFlag = 0;
                }
            }
            __bis_SR_register(LPM3_bits + GIE);             // LPM3, wait for RTC interrupt
            __no_operation();
            break;
        case SEL_SET_HOUR:
            current_param = RTCHOUR;
            RTCHOUR = Clock_setParameter(current_param, 23, 0);
            break;
        case SEL_SET_MIN:
            current_param = RTCMIN;
            RTCMIN = Clock_setParameter(current_param, 59, 0);
            break;
        case SEL_SET_DOW:
            current_param = RTCDOW;
            RTCDOW = Clock_setParameter(current_param, 6, 0);
            break;
        case SEL_SET_MON:
            current_param = RTCMON;
            RTCMON = Clock_setParameter(current_param, 12, 1);
            break;
        case SEL_SET_DAY:
            current_param = RTCDAY;
            RTCDAY = Clock_setParameter(current_param, 31, 1);
            break;
        case SEL_SET_YEAR:
            year_param = RTCYEAR;
            RTCYEAR = Clock_setYear(year_param, 4095, 0);
            break;
        default: break;
        }
    }
    Clock_App_exit();
}

/**********************************************************************//**
 * @brief  Initialize the clock app (RTC)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void Clock_App_init(void)
{
    Calendar clockCalendar;

    // Disable CapSense Pins
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);

    if (setClock) {
        clockCalendar.Seconds = 0;              // Set clock to generic time
        clockCalendar.Minutes = 0;
        clockCalendar.Hours = 12;
        clockCalendar.DayOfWeek = 1;
        clockCalendar.DayOfMonth = 1;
        clockCalendar.Month = 1;
        clockCalendar.Year = 2013;

        RTC_B_initCalendar(__MSP430_BASEADDRESS_RTC_B__, &clockCalendar, RTC_B_FORMAT_BINARY);

        setClock = 0;
    }

    oldSliderPos[0] = 20;
    oldSliderPos[1] = 20;
    sliderPos[0] = 20;
    sliderPos[1] = 20;

    select = SEL_SET_HOUR;
}

/**********************************************************************//**
 * @brief  Set the designated parameter
 *
 * @param  value - previous/current value of parameter
 *         param - parameter index
 *         max   - maximum value for parameter
 *         min   - minimum value for parameter
 *
 * @return value - newly set value for parameter
 *************************************************************************/
static unsigned char Clock_setParameter(unsigned char value, unsigned char max,
                                        unsigned char min)
{
    unsigned char temp0 = 0;
    unsigned char blink = 1;

    temp0 = select;

    while (select == temp0) {
        CTS_getReading();

        if (blink) {                                    // only update values when user can see it updating
            if (sliderPos[0] > 26) {                    // if on top, increase by 1
                value++;
                if (value > max) {                      // wrap around setting
                    value = min;
                }
            }
            else if (sliderPos[0] < 14) {               // if on bottom, decrease by 1
                value--;
                if ((value < min) || (value > max)) {
                    value = max;
                }
            }
        }
        firstEnter = 1;
        LCD_updateClock(blink, value, firstEnter, ULP);
        TA1_sleep(512);         // sleep for 1/8 of a second
        blink ^= 1;
    }
    return value;
}

/**********************************************************************//**
 * @brief  Set the year - handled separately because it has a larger range
 *
 * @param  value - previous/current value of year
 *         max   - maximum value for parameter
 *         min   - minimum value for parameter
 *
 * @return value - newly set value for year
 *************************************************************************/
static unsigned int Clock_setYear(unsigned int value, unsigned int max,
                                  unsigned int min)
{
    unsigned char blink = 1;

    while (select == SEL_SET_YEAR) {
        CTS_getReading();

        if (blink) {                                    // only update values when user can see it updating
            if (sliderPos[0] > 26) {
                if (sliderPos[0] > 35) {                // if on very top, increase by 2
                    value++;
                }
                // if on top, but not very top, increase by only 1
                value++;
                if (value > max) {                      // wrap around setting
                    value = min;
                }
            }
            else if (sliderPos[0] < 14) {
                if (sliderPos[0] < 5) {                 // if on very bottom, decrease by 2
                    value--;
                }
                // if on bottom, but not very bottom, decrease by only 1
                value--;
                if ((value < min) || (value > max)) {
                    value = max;
                }
            }
        }
        firstEnter = 1;
        LCD_updateClock(blink, value, firstEnter, ULP);
        TA1_sleep(512);         // sleep for 1/8 of a second
        blink ^= 1;
    }
    return value;
}

/**********************************************************************//**
 * @brief  Exit the clock app
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
static void Clock_App_exit(void)
{
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);
    RTC_B_disableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_TIMER1_INTERRUPT);

    // Re-enable CapSense
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
}
