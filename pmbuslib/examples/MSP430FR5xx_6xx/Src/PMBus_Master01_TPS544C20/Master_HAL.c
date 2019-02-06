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
/*
 * ======== Master_HAL.c ========
 * Modules and GPIOs used by this HAL:
 *      =================================
 *      |   I2C         |   eUSCIB0     |
 *      |   I2C_SDA     |    P1.6       |
 *      |   I2C_SCL     |    P1.7       |
 *      |   SMBAlert    |    P1.5       |
 *      |   Control1    |    P1.4       |
 *      |   Control2    |    P1.3       |
 *      |   LED0        |    P1.0       |
 *      |   LED1        |    P4.6       |
 *      =================================
 * ---------------------------------------------------------------------------*/
#include <stdint.h>
#include "driverlib.h"
#include "Master_HAL.h"
#include "smbus.h"

//
// Local functions
//
static void MCU_Clock_Init(void);
static void MCU_GPIO_Init(void);

//
// Variables
//
static uint16_t Delay_ms_counter;

//*****************************************************************************
//
//! MCU initialization
//!
//! Initializes the MSP430
//!  Sets GPIOs to default state
//!  Configures clocks
//!
//! \return None
//
// *****************************************************************************
void HAL_MCU_Init(void)
{
    // Initialize GPIOs
    MCU_GPIO_Init();

    // Initialize the MSP430 Clocks
    MCU_Clock_Init();

    // Turn off Temp sensor
    REFCTL0 |= REFTCOFF;
    REFCTL0 &= ~REFON;

    return;
}

//*****************************************************************************
//
//! Clock Initialization
//!
//! This demo sets SMCLK/MCLK = DCO @ MCLK_MHZ (defined in project settings)
//!                ACLK = VLO
//!
//! \return None
//
// *****************************************************************************
static void MCU_Clock_Init(void)
{
#if (MCLK_MHZ == 1)
    //Set DCO Frequency to 8MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_6);
    //Initialize SMCLK=DCOCLK and CLK DIVIDER set to 8 = 1Mhz
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);
    //Initialize MCLK=DCOCLK and CLK DIVIDER set to 8 = 1Mhz
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_8);
#elif (MCLK_MHZ == 8)
    //Set DCO Frequency to 8MHz
    CS_setDCOFreq(CS_DCORSEL_0,CS_DCOFSEL_6);
    //Initialize SMCLK=DCOCLK and CLK DIVIDER set to 1 = 8Mhz
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Initialize MCLK=DCOCLK and CLK DIVIDER set to 1 = 8Mhz
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
#elif (MCLK_MHZ == 16)
    //Set DCO Frequency to 16MHz
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_4);
    //Initialize SMCLK=DCOCLK and CLK DIVIDER set to 1 = 16Mhz
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Initialize MCLK=DCOCLK and CLK DIVIDER set to 1 = 16Mhz
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
#elif (MCLK_MHZ == 24)
    //Set DCO Frequency to 24MHz
    CS_setDCOFreq(CS_DCORSEL_1,CS_DCOFSEL_6);
    //Initialize SMCLK=DCOCLK and CLK DIVIDER set to 1 = 24Mhz
    CS_initClockSignal(CS_SMCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
    //Initialize MCLK=DCOCLK and CLK DIVIDER set to 1 = 24Mhz
    CS_initClockSignal(CS_MCLK,CS_DCOCLK_SELECT,CS_CLOCK_DIVIDER_1);
#else
#error "Frequency not supported, change project setttings"
#endif

    //Initialize ACLK=VLO and no CLK DIVIDER set to 1
    CS_initClockSignal(CS_ACLK,CS_VLOCLK_SELECT,CS_CLOCK_DIVIDER_1);

    // Crystals are not used
    CS_turnOffHFXT();
    CS_turnOffLFXT();

    return;
}

//*****************************************************************************
//
//! GPIO Initialization
//!
//! Set GPIO pins to a default state
//!
//! \return None
//
// *****************************************************************************
static void MCU_GPIO_Init(void)
{
    // Set all pins as output low, except
    // I2C (P1[6:7])
    // PMBus Alert (P1.5)
    // Note that PMBusControl lines (P1.3-P1.4) are initialized as output low
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                           GPIO_PIN4);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 |
                           GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 |
                           GPIO_PIN6 | GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 |
                           GPIO_PIN6 | GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 |
                           GPIO_PIN6 | GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 |
                        GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    return;
}

//*****************************************************************************
//
//! Initialization of I2C GPIOs
//!
//! Initialize GPIOs required for I2C
//!
//! \return None
//
// *****************************************************************************
void HAL_I2C_InitGPIO(void)
{
    // Set I2C pins for SCL/SDA functionality
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, BIT6 + BIT7,
                                               GPIO_SECONDARY_MODULE_FUNCTION);
}

//*****************************************************************************
//
//! Get Base Address of I2C used for SMBus
//!
//! The base address is used by smbuslib PHY layer for FR5xx eUSCI.
//! The simpler implementation from 2xx doesn't use base address.
//!
//! \return Base address of I2C module, NULL if no base address used
//
// *****************************************************************************
uint32_t HAL_I2C_GetBaseAddr(void)
{
    // FR5xx implementation uses Base address
    return(EUSCI_B0_BASE);
}

//*****************************************************************************
//
//! Initialization of GPIOs used for PMBus functionality
//!
//! Initialize GPIOs required for PMBus (SMBAlert, Control)
//!
//! \return None
//
// *****************************************************************************
void HAL_PMBus_InitGPIO(void)
{
    // PMBus Alert (P1.5) as input with pullup
    // PMBusControl lines (P1[4:3] are already initialized as output low
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, BIT5);
}

//*****************************************************************************
//
//! Reads the status of SMBAlert line
//!
//! \return true if SMBAlert is asserted, false if it isn't
//
// *****************************************************************************
bool HAL_PMBus_GetSMBAlert(void)
{
    // SMBAlert is in P1.5
    if(GPIO_getInputPinValue(GPIO_PORT_P1, BIT5) == GPIO_INPUT_PIN_HIGH)
    {
        return(false);
    }
    else
    {
        return(true);
    }
}

//*****************************************************************************
//
//! Configures a PMBus Control Line
//!
//! Configures one of the PMBus Control Lines
//!
//! \param CtrlLine Selects Control line as defined by t_PMBCtrl
//! \param LogicState true to assert line (high), false to de-assert (low)
//!
//! \return None
//
// *****************************************************************************
void HAL_PMBus_SetControl(t_PMBCtrl CtrlLine,
                          bool LogicState)
{
    uint8_t Port;
    uint16_t pins;

    // ControlLine1 = P1.4
    if(CtrlLine == CtrlLine1)
    {
        Port = GPIO_PORT_P1;
        pins = BIT4;
    }
    else
    {
        // ControlLine2 = P1.3
        Port = GPIO_PORT_P1;
        pins = BIT3;
    }

    if(LogicState == true)
    {
        GPIO_setOutputHighOnPin(Port, pins);
    }
    else
    {
        GPIO_setOutputLowOnPin(Port, pins);
    }
}

//*****************************************************************************
//
//! Gets the value of a PMBus Control Lines
//!
//! \param CtrlLine Select Control line as defined by t_PMBCtrl
//!
//! \return true of line is asserted (high), false if deasserted (low)
//
// *****************************************************************************
bool HAL_PMBus_GetControl(t_PMBCtrl CtrlLine)
{
    uint8_t Port;
    uint16_t pins;

    // ControlLine1 = P1.4
    if(CtrlLine == CtrlLine1)
    {
        Port = GPIO_PORT_P1;
        pins = BIT4;
    }
    else
    {
        // ControlLine2 = P1.3
        Port = GPIO_PORT_P1;
        pins = BIT3;
    }

    if(GPIO_getInputPinValue(Port, pins) == GPIO_INPUT_PIN_HIGH)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

//*****************************************************************************
//
//! Turn on a LED
//!
//! \param LED LED0 for LED0, LED1 for LED1, any other value for both LEDs
//!
//! \return None
//
// *****************************************************************************
void HAL_LED_SetHigh(t_LED LED)
{
    if(LED == LED0)
    {
        GPIO_setOutputHighOnPin (GPIO_PORT_P1, BIT0);
    }
    else if(LED == LED1)
    {
        GPIO_setOutputHighOnPin (GPIO_PORT_P4, BIT6);
    }
    else
    {
        GPIO_setOutputHighOnPin (GPIO_PORT_P4, BIT6);
        GPIO_setOutputHighOnPin (GPIO_PORT_P1, BIT0);
    }
}

//*****************************************************************************
//
//! Turn off a LED
//!
//! \param LED LED0 for LED0, LED1 for LED1, any other value for both LEDs
//!
//! \return None
//
// *****************************************************************************
void HAL_LED_SetLow(t_LED LED)
{
    if(LED == LED0)
    {
        GPIO_setOutputLowOnPin (GPIO_PORT_P1, BIT0);
    }
    else if(LED == LED1)
    {
        GPIO_setOutputLowOnPin (GPIO_PORT_P4, BIT6);
    }
    else
    {
        GPIO_setOutputLowOnPin (GPIO_PORT_P4, BIT6);
        GPIO_setOutputLowOnPin (GPIO_PORT_P1, BIT0);
    }
}

//*****************************************************************************
//
//! Toggle a LED
//!
//! \param LED LED0 for LED0, LED1 for LED1, any other value for both LEDs
//!
//! \return None
//
// *****************************************************************************
void HAL_LED_Toggle(t_LED LED)
{
    if(LED == LED0)
    {
        GPIO_toggleOutputOnPin (GPIO_PORT_P1, BIT0);
    }
    else if(LED == LED1)
    {
        GPIO_toggleOutputOnPin (GPIO_PORT_P4, BIT6);
    }
    else
    {
        GPIO_toggleOutputOnPin (GPIO_PORT_P4, BIT6);
        GPIO_toggleOutputOnPin (GPIO_PORT_P1, BIT0);
    }
}

//*****************************************************************************
//
//! Millisecond delay
//!  This delay uses ACLK, based on VLO, so it's not very accurate
//!  The function will get blocked until the delay has elapsed
//!
//! \param ms Number of milliseconds
//!
//! \return None
//
// *****************************************************************************
void HAL_Timer_Delay_ms(uint16_t ms)
{
    //Start timer in up mode sourced by ACLK
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
    initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A0_BASE, &initContParam);

    // 1ms counter
    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable =
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue = (9400 / 1000);
    Timer_A_initCompareMode(TIMER_A0_BASE, &initCompParam);

    Delay_ms_counter = ms;

    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);
    Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,
                                          TIMER_A_CAPTURECOMPARE_REGISTER_0);
    Timer_A_startCounter(TIMER_A0_BASE,
                         TIMER_A_UP_MODE
                         );

    __disable_interrupt();
    while(Delay_ms_counter)
    {
        __bis_SR_register(LPM3_bits | GIE);
        __no_operation();
        __disable_interrupt();
    }

    __enable_interrupt();
}

//
// Interrupt Service Routines
//
//*****************************************************************************
//
//! I2C Interrupt Service Routine
//!
//! ISR for I2C module
//!   This ISR generates an event which should be declared in application
//!
//! \return None
//
// *****************************************************************************
#pragma vector=USCI_B0_VECTOR
__interrupt void HAL_I2C_ISR(void)
{
    // This event should be declared in Application
    if(HAL_I2C_Event() == true)
    {
        LPM3_EXIT;
    }
}

//*****************************************************************************
//
//! TimerA0 CCR0 Interrupt Service Routine
//!
//! ISR for Delay Timer
//!   This ISR is called every ms and decrements a counter used for delay
//!
//! \return None
//
// *****************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if(Delay_ms_counter > 0)
    {
        Delay_ms_counter--;
    }
    else
    {
        Timer_A_disableCaptureCompareInterrupt(TIMER_A0_BASE,
                                               TIMER_A_CAPTURECOMPARE_REGISTER_0);
        LPM3_EXIT;
    }
}

//*****************************************************************************
//
//! Low level initialization
//!
//! Disables the watchdog after reset
//!
//! \return 1: Initialize all variables
//
// *****************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
int __low_level_init(void)
#elif defined (__TI_COMPILER_VERSION__)
int _system_pre_init(void)
#endif
{
    WDT_A_hold(WDT_A_BASE);             //Stop WDT
    return(1);
}

//*****************************************************************************
//
//! Dummy ISR
//!
//! Dummy ISR in case an interrupt is enabled unintentionally
//!
//! \return none
//
// *****************************************************************************
#pragma vector=RTC_VECTOR,PORT2_VECTOR,TIMER0_A1_VECTOR,TIMER3_A1_VECTOR, \
    USCI_A1_VECTOR,PORT1_VECTOR,TIMER1_A1_VECTOR,TIMER2_A1_VECTOR, \
    TIMER0_B1_VECTOR,TIMER0_B0_VECTOR, \
    SYSNMI_VECTOR,AES256_VECTOR, TIMER3_A0_VECTOR, \
    TIMER1_A0_VECTOR, TIMER2_A0_VECTOR, COMP_E_VECTOR, UNMI_VECTOR
__interrupt void Dummy_ISR(void)
{
    while(1)
    {
        ;
    }
}
