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
 * main.c
 * User Experience Code for the MSP-EXP430FR5969
 *
 *
 *************************************************************************//****
 * NOTE: To invert display colors, open sharp96x96.h, and under the Invert Display
 *       option of the user configuration section, comment in/out the necessary
 *       #define options for your specific desires.
 *       NORMAL_DISPLAY = white background with black foreground
 *       INVERT_DISPLAY = black background with white foreground
 *******************************************************************************
 *
 * FR59xx_EXP.c
 * User Experience Code for the MSP-EXP430FR5969
 * Initialization, main menu, and button control functions
 *
 * February 2012
 *
 *************************************************************************//****
 * Rev 1.2 - driverlib integration
 ******************************************************************************/

#include "driverlib.h"
#include "Sharp96x96.h"
#include "CTS/CTS_wolverine.h"

#include "FR59xx_EXP.h"
#include "ULPMeter.h"
#include "FRAMSpeedApp.h"
#include "ClockApp.h"
#include "ActivePowerMeasure.h"
#include "Game.h"
#include "myTimer.h"
#include "msp430fr59xx_CHR.h"
#include "UpdateDisplay.h"

//-----------------------------------------------------------------------------
volatile unsigned char startupStatus;       // blink LED1 in start up status
//-----------------------------------------------------------------------------
// Menu/ISR Global Variables
volatile unsigned char mode = 2;                // Initialize Mode to Main Menu
volatile unsigned char select = 0;              // Select button flag

//-----------------------------------------------------------------------------
// Module-private prototype declarations
void SystemInit(void);
void SystemInitFromLPM35(void);

//-----------------------------------------------------------------------------
int _system_pre_init(void)
{
    // Stop Watchdog timer
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);     // Stop WDT

    /*==================================*/
    /* Choose if segment initialization */
    /* should be done or not. */
    /* Return: 0 to omit initialization */
    /* 1 to run initialization */
    /*==================================*/
    return 1;
}

//-----------------------------------------------------------------------------
int main(void)
{
    // Protect code from being overwritten
    MPU_initTwoSegments(__MSP430_BASEADDRESS_MPU__,0x0DC0,MPU_EXEC|MPU_READ,MPU_EXEC|MPU_READ|MPU_WRITE);
    MPU_start(__MSP430_BASEADDRESS_MPU__);

    // If this code starts up the first time initialize some key global state variables
    if (decimalPlaces == 0xFFFFFFFF) {
        decimalPlaces = 999999999;
    }

    if (HighScore == 0xFFFFFFFF) {
        HighScore = 0;
    }

    // Check if a wakeup from LPMx.5 was detected and use a dedicated device-init flow and continue
    // previous program execution in that case.
    if (SYSRSTIV == SYSRSTIV_LPM5WU) {
        mode = APP_ULP_ADC;
        select = SEL_ADC_LPM35_MODE;
        setClock = 1;   // After exiting from LPM3.5, set up date and time, or time will be 00:00.....
        SystemInitFromLPM35();
    }
    else {              // Start up normally
        SystemInit();   // Initialize board

        // Display TI Logo
        LCD_drawPicture(1);
        TA1_sleep(8192);         // 2s
        // Display Wolverine Slash
        LCD_drawPicture(2);
        TA1_sleep(8192);         // 2s
        // Write "Wolverine User Experience" on LCD
        LCD_introWrite();
        TA1_sleep(8192);         // 2s
    }

    // Init CapTouch Sliders
    TI_CAPT_Init_Baseline(&slider0);
    TI_CAPT_Init_Baseline(&slider1);

    TI_CAPT_Update_Baseline(&slider0, 10);
    TI_CAPT_Update_Baseline(&slider1, 10);

    while (1) {
        // Choose which app to enter based on mode variable
        switch (mode) {
        case APP_MAIN_MENU:
            mode = MainMenu();
            break;
        case APP_CLOCK:
            Clock_App();
            break;
        case APP_FRAM_SPEED:
            FRAM_speedApp();
            break;
        case APP_ULP_ADC:
            ULP_meter();
            break;
        case APP_ACTIVE_TEST:
            ActivePowerApp();
            break;
        case APP_SLIDERBALL:
            SliderBall_app();
            break;
//      case ENERGY_TRACE:
//              mode = APP_MAIN_MENU;
//      break;
        default: break;
        }
    }
}

/**********************************************************************//**
 * @brief  Initializes the System
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void SystemInit(void)
{
    // Set the DCO to 8MHz (it's also the device's power-on setting). Do not change this frequency!
    // It impacts the cap touch scan window.
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);

    // Configure clock source and clock dividers. After this the clock configuration will be as follows:
    // ACLK=LFXT1/1=32,768Hz; SMCLK=DCOCLK/1=8MHz; and MCLK=DCOCLK/1=8MHz.
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Set all GPIO to output low to minimize current draw by eliminating floating pins.
    GPIO_setOutputLowOnPin(GPIO_PORT_PA, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PA, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PB, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);

    // Configure the left button (S2) connected to P4.6. For this enable the internal pull-up resistor and
    // setup the pin interrupt to trigger on rising edges.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_selectInterruptEdge(GPIO_PORT_P4, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);

    // Configure the right button (S3) connected to P1.1. For this enable the internal pull-up resistor and
    // setup the pin interrupt to trigger on rising edges.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // CapSense Setup. GPIO pins P1.3-1.5 and P3.4-3.6 are used for capacitive touch so let's
    // switch them to inputs.
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);

    // Enable LFXT functionality on PJ.4 and PJ.5. For this we only need to configure PJ.4 to
    // LFXIN and the port module logic takes care of the rest.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    PMM_unlockLPM5();

    // Perform the required LFXT startup procedure now that all of the port pins are configured.
    CS_setExternalClockSource(32768, 0);
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);

    // Initialize LCD driver and the context for the LCD
    Sharp96x96_initDisplay();
    TA0_enableVCOMToggle();
    GrContextInit(&sContext, &g_sharp96x96LCD);
    GrContextForegroundSet(&sContext, ClrBlack);
    GrContextBackgroundSet(&sContext, ClrWhite);

    onLED();                                   //blink LED1
}

/**********************************************************************//**
 * @brief  Enter LPM3.5
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void SYS_enterLPM35(void)
{
    // Request the disabling of the core voltage regulator when device enters
    // LPM3 (or LPM4) so that we can effectively enter LPM3.5 (or LPM4.5).
    PMM_turnOffRegulator();

    // Enter LPM3.5 mode with interrupts enabled. Note that this function will
    // not return, but rather the device will go through a RESET-like sequence.
    __bis_SR_register(LPM3_bits | GIE);
}

/**********************************************************************//**
 * @brief  Wakeup From LPM3.5 and re-init ports, clock, and eUSCI
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void SystemInitFromLPM35(void)
{
    // Set the DCO to 8MHz (it's also the device's power-on setting). Do not change this frequency!
    // It impacts the cap touch scan window.
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_6);

    // Configure clock source and clock dividers. After this the clock configuration will be as follows:
    // ACLK=LFXT1/1=32,768Hz; SMCLK=DCOCLK/1=8MHz; and MCLK=DCOCLK/1=8MHz.
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Set all GPIO to output low to minimize current draw by eliminating floating pins.
    GPIO_setOutputLowOnPin(GPIO_PORT_PA, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PA, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PB, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN6 | GPIO_PIN7 | GPIO_PIN8 | GPIO_PIN9 | GPIO_PIN10 | GPIO_PIN11 | GPIO_PIN12 | GPIO_PIN13 | GPIO_PIN14 | GPIO_PIN15);

    // Configure the left button (S2) connected to P4.6. For this enable the internal pull-up resistor and
    // setup the pin interrupt to trigger on rising edges. Note that we are not actually enabling the
    // interrupt for this button in case of wakeup from LPM3.5. We'll enable the pin interrupt later
    // as needed but we want to have at least everything prepared here.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_selectInterruptEdge(GPIO_PORT_P4, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);

    // Configure the right button (S3) connected to P1.1. For this enable the internal pull-up resistor and
    // setup the pin interrupt to trigger on rising edges. Note that we are not clearing the associated
    // interrupt flag since we want to be able to determine the wakeup source.
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    // Enable LFXT functionality on PJ.4 and PJ.5. For this we only need to configure PJ.4 to
    // LFXIN and the port module logic takes care of the rest.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    // Re-initialize the RTC module
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);
    RTC_B_enableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_TIME_EVENT_INTERRUPT);
    RTC_B_setCalendarEvent(__MSP430_BASEADDRESS_RTC_B__, RTC_B_CALENDAREVENT_MINUTECHANGE);
    RTC_B_startClock(__MSP430_BASEADDRESS_RTC_B__);

    // Disable the LPMx.5 lock state to unlock the GPIO module to activate previously configured port
    // settings as well as to activate the RTC module settings.
    PMM_unlockLPM5();

    // Perform the required LFXT startup procedure now that all of the port pins are configured.
    CS_setExternalClockSource(32768, 0);
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);

    // Initialize the context for the LCD
    GrContextInit(&sContext, &g_sharp96x96LCD);
    GrContextForegroundSet(&sContext, ClrBlack);
    GrContextBackgroundSet(&sContext, ClrWhite);

    // Re-enable interrupts. Any port interrupts (PxIFG) or RTC interrupt flag that may have gotten
    // set during LPMx.5 will now trigger an actual interrupt event.
    __enable_interrupt();
}

/**********************************************************************//**
 * @brief  Display Main Menu, choose next option
 *
 * @param  none
 *
 * @return mode - choice selected
 *************************************************************************/
unsigned char MainMenu(void)
{
    unsigned char appChoice = APP_MAIN_MENU;

    while (!select) {
        // Get left slider position
        CTS_getReading();

        // Highlight/scroll based on cap touch position
//    if(position < 8) {
//      choice = ENERGY_TRACE;
//    }
        if (sliderPos[0] < 12) {
            appChoice = APP_SLIDERBALL;
        }
        else if (sliderPos[0] < 18) {
            appChoice = APP_ACTIVE_TEST;
            if (startupStatus) {
                offLED();
            }

        }
        else if (sliderPos[0] < 24) {
            appChoice = APP_ULP_ADC;
            if (startupStatus) {
                offLED();
            }
        }
        else if (sliderPos[0] < 30) {
            appChoice = APP_FRAM_SPEED;
            if (startupStatus) {
                offLED();
            }
        }
        else {
            appChoice = APP_CLOCK;
            if (startupStatus) {
                offLED();
            }
        }
        LCD_displayMainMenu(appChoice);
        LCD_display();
    }

    if (startupStatus) {
        offLED();
    }

    select = 0;
    if (appChoice == APP_ULP_ADC) {
        select = 2;
    }

    return appChoice;
}

/**********************************************************************//**
 * @brief  Disables Left Switch
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void DisableLeftSwitch(void)
{
    GPIO_disableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
}

/**********************************************************************//**
 * @brief  Enables Left Switch
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void EnableLeftSwitch(void)
{
    GPIO_clearInterrupt(GPIO_PORT_P4, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN5);
}

/**********************************************************************//**
 * @brief  Disables Right Switch
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void DisableRightSwitch(void)
{
    GPIO_disableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

/**********************************************************************//**
 * @brief  Enables Right Switch
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void EnableRightSwitch(void)
{
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
}

/**********************************************************************//**
 * @brief  Starts Debounce Timer
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void StartDebounceTimer(void)
{
    Timer_B_initUpModeParam param = {0};
    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_1;
    param.timerPeriod = 9380;
    param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    param.captureCompareInterruptEnable_CCR0_CCIE =
    		TIMER_B_CCIE_CCR0_INTERRUPT_ENABLE;
    param.timerClear = TIMER_B_SKIP_CLEAR;
    param.startTimer = true;
    Timer_B_initUpMode(TIMER_B0_BASE, &param);
}

/**********************************************************************//**
 * @brief  turn on LED1 and blink it every second until user touch the capsense
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void onLED(void)
{
    // configure RTC interrupt every second
    RTC_B_definePrescaleEvent(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_1, RTC_B_PSEVENTDIVIDER_128);
    RTC_B_enableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_TIMER1_INTERRUPT);
    RTC_B_startClock(__MSP430_BASEADDRESS_RTC_B__);
    startupStatus = 1;

}
/**********************************************************************//**
 * @brief  turn off LED1 after user touch the Capsense
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
void offLED(void)
{
    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);
    RTC_B_disableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_TIMER1_INTERRUPT);
    GPIO_setOutputLowOnPin(GPIO_PORT_PB, GPIO_PIN14);
    startupStatus = 0;
}


// Interrupt Service Routines
/**********************************************************************//**
 * @brief  Port 1 ISR for Right Switch (S3) - Option Select Button
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
#elif defined(__GNUC__)
void PORT1_ISR(void) __attribute__((interrupt(PORT1_VECTOR)));
void PORT1_ISR(void)
#else
#error Compiler not supported!
#endif
{
    char wakeup = 0;

    switch (__even_in_range(P1IV, P1IV_P1IFG7)) {
    case P1IV_P1IFG0: break;                    // P1.0 Interrupt
    case P1IV_P1IFG1:                           // Option Select Interrupt
        DisableLeftSwitch();
        DisableRightSwitch();
        StartDebounceTimer();
        switch (mode) {                         // take action based on what mode the UE was in
        case APP_MAIN_MENU:
            select = 1;
            wakeup = 1;
            break;
        case APP_CLOCK:
            if (firstEnter) {
                if (select == SEL_RUN_CLOCK) {                 // stop clock and start setting parameters
                    RTC_B_holdClock(__MSP430_BASEADDRESS_RTC_B__);
                    RTCPS1CTL = 0;
                }
                select += 2;
                if (select > SEL_SET_YEAR) {                 // last parameter set, start clock
                    select = SEL_RUN_CLOCK;
                    firstEnter = 0;
                    RTCSEC = 0;                     // Reset seconds
                    RTC_B_definePrescaleEvent(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_1, RTC_B_PSEVENTDIVIDER_128);
                    RTC_B_enableInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_TIMER1_INTERRUPT);
                    RTC_B_startClock(__MSP430_BASEADDRESS_RTC_B__);
                }
            }
            else {
                select = SEL_RUN_CLOCK;
                if (ULP) {
                    ULP = 0;
                }
                else {
                    ULP = 1;
                }
                freshFlag = 1;
            }
            wakeup = 1;
            break;
        case APP_FRAM_SPEED:
            wakeup = 0;
            break;
        case APP_ULP_ADC:
            switch (select) {             // Do actions based on what mode is being exited
            case SEL_MENU_MODE:
                wakeup = 0;
                select = SEL_TITLE_MODE;
                break;
            case SEL_TITLE_MODE:
                wakeup = 1;
                select = SEL_ADC_LPM35_MODE;
                break;
            case SEL_ADC_LPM35_MODE:
                wakeup = 1;
                select = SEL_DISPLAY_MODE;
                break;
            case SEL_DISPLAY_MODE:
                wakeup = 1;
                select = SEL_ADC_LPM35_MODE;
                break;
            case SEL_BAT_LOW_MODE:
                wakeup = 1;
                select = SEL_ADC_LPM35_MODE;
                break;
            case SEL_UART_TRANSFER_MODE:
                wakeup = 0;
                select = SEL_TITLE_MODE;
                break;
            default: break;
            }
            break;
        case APP_ACTIVE_TEST:
            select += 2;
            if (select > SEL_RUN_CHR) {
                select = SEL_CHOOSE_FREQ;
            }
            wakeup = 0;
            if (select == SEL_RUN_CHR) {
                wakeup = 1;
                CHR_prep(0);                    // prep for entering CHR code
            }
            else if (select == SEL_CHOOSE_FREQ) {
                CHR_prep(1);                    // prep for exiting CHR code
            }
            break;
        case APP_SLIDERBALL:
            select += 2;
            wakeup = 1;
            break;
//        case ENERGY_TRACE:
//        break;
        default: break;
        }
        break;
    case P1IV_P1IFG2: break;                    // P1.2 Interrupt
    case P1IV_P1IFG3: break;                    // P1.3 Interrupt
    case P1IV_P1IFG4: break;                    // P1.4 Interrupt
    case P1IV_P1IFG5: break;                    // P1.5 Interrupt
    case P1IV_P1IFG6: break;                    // P1.6 Interrupt
    case P1IV_P1IFG7: break;                    // P1.7 Interrupt
    default: break;
    }

    if (wakeup) {
        __bic_SR_register_on_exit(LPM4_bits);
    }
}

/**********************************************************************//**
 * @brief  Port 4 ISR for Left Switch (S2) - Main Menu Button
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
#elif defined(__GNUC__)
void PORT4_ISR(void) __attribute__((interrupt(PORT4_VECTOR)));
void PORT4_ISR(void)
#else
#error Compiler not supported!
#endif
{
    char wakeup = 0;

    switch (__even_in_range(P4IV, P4IV_P4IFG7)) {
    case P4IV_P4IFG0: break;                    // P4.0 Interrupt
    case P4IV_P4IFG1: break;                    // P4.1 Interrupt
    case P4IV_P4IFG2: break;                    // P4.2 Interrupt
    case P4IV_P4IFG3: break;                    // P4.3 Interrupt
    case P4IV_P4IFG4: break;                    // P4.4 Interrupt
    case P4IV_P4IFG5:                           // Menu Button Interrupt
        DisableLeftSwitch();
        DisableRightSwitch();
        StartDebounceTimer();
        switch (mode) {
        case APP_MAIN_MENU:
            wakeup = 0;
            break;
        case APP_CLOCK:
            wakeup = 1;
            break;
        case APP_FRAM_SPEED:
            wakeup = 0;
            break;
        case APP_ULP_ADC:
            wakeup = 1;
            break;
        case APP_ACTIVE_TEST:
            if (select == SEL_SHOW_INTRO) {
                wakeup = 1;
            }
            else if (select == SEL_RUN_CHR) {
                CHR_prep(1);                 // prep for exiting CHR code
                wakeup = 0;
            }
            else {
                wakeup = 0;
            }
            break;
        case APP_SLIDERBALL:
            wakeup = 1;
            break;
//        case ENERGY_TRACE:
//        break;
        default: break;
        }
        select = 0;                             // Reset button pushes
        mode = APP_MAIN_MENU;                   // Reset mode
        break;
    case P4IV_P4IFG6: break;                    // P4.6 Interrupt
    case P4IV_P4IFG7: break;                    // P4.7 Interrupt
    default: break;
    }

    if (wakeup) {
        __bic_SR_register_on_exit(LPM4_bits);
    }
}

/**********************************************************************//**
 * @brief  RTC_ISR - Interrupt Service Routine for RTC
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR(void)
#elif defined(__GNUC__)
void RTC_ISR(void) __attribute__((interrupt(RTC_VECTOR)));
void RTC_ISR(void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(RTCIV, RTC_RTCOFIFG)) {
    case RTCIV_NONE:      break;
    case RTCIV_RTCRDYIFG: break;
    case RTCIV_RTCTEVIFG:
        takeADCReading = 1;
        RTC_B_clearInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_TIME_EVENT_INTERRUPT);
        __bic_SR_register_on_exit(LPM4_bits);
        break;
    case RTCIV_RTCAIFG:  break;             // RTCAIFG
    case RTCIV_RT0PSIFG: break;             // RT0PSIFG
    case RTCIV_RT1PSIFG:                    // RT1PSIFG
        if (startupStatus) {                // blink LED1 when start up
            //toggle LED1
            GPIO_toggleOutputOnPin(GPIO_PORT_PB, GPIO_PIN14);
        }
        else {
            if (RTCSEC == 0) {
                freshFlag = 1;
            }
            RTC_B_clearInterrupt(__MSP430_BASEADDRESS_RTC_B__, RTC_B_PRESCALE_TIMER1_INTERRUPT);
            __bic_SR_register_on_exit(LPM3_bits);
        }
        break;
    case RTCIV_RTCOFIFG: break;             // RTCOFIFG
    default: break;
    }
}

/**********************************************************************//**
 * @brief  TB0_ISR - Interrupt Service Routine for debounce timer (TB0)
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TB0_ISR(void)
#elif defined(__GNUC__)
void TB0_ISR(void) __attribute__((interrupt(TIMER0_B0_VECTOR)));
void TB0_ISR(void)
#else
#error Compiler not supported!
#endif
{
    Timer_B_disableCaptureCompareInterrupt(__MSP430_BASEADDRESS_T0B7__, TIMER_B_CAPTURECOMPARE_REGISTER_6);
    Timer_B_stop(__MSP430_BASEADDRESS_T0B7__);
    EnableRightSwitch();
    EnableLeftSwitch();
}

/**********************************************************************//**
 * @brief  SYSNMI_ISR - Interrupt Service Routine for NMI's
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#if defined (__TI_COMPILER_VERSION__) || defined (__IAR_SYSTEMS_ICC__)
#pragma vector=SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
#elif defined(__GNUC__)
void SYSNMI_ISR(void) __attribute__((interrupt(SYSNMI_VECTOR)));
void SYSNMI_ISR(void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(SYSSNIV, SYSSNIV_CBDIFG)) {
    case SYSSNIV_NONE:       break;
    case SYSSNIV_RES02:      break;
    case SYSSNIV_UBDIFG:     break;
    case SYSSNIV_RES06:      break;
    case SYSSNIV_MPUSEGPIFG: break;
    case SYSSNIV_MPUSEGIIFG: break;
    case SYSSNIV_MPUSEG1IFG: break;         // segment 1
    case SYSSNIV_MPUSEG2IFG:                // segment 2
        // Clear violation interrupt flag
        MPU_clearInterrupt(__MSP430_BASEADDRESS_MPU__, MPUSEG2IFG);

        // Turn LED on
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

        while (1);
    case SYSSNIV_MPUSEG3IFG: break;         // segment 3
    case SYSSNIV_VMAIFG:     break;
    case SYSSNIV_JMBINIFG:   break;
    case SYSSNIV_JMBOUTIFG:  break;
    case SYSSNIV_CBDIFG:     break;
    default:                 break;
    }
}
