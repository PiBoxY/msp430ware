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
 *      |   I2C         |   USCIB0      |
 *      |   I2C_SDA     |    P1.7       |
 *      |   I2C_SCL     |    P1.6       |
 *      |   SMBAlert    |    P2.5       |
 *      |   Control1    |    P2.4       |
 *      |   Control2    |    P2.3       |
 *      |   LED0        |    P1.0       |
 *      |   LED1        |    P2.0       |
 *      =================================
 * ---------------------------------------------------------------------------*/
#include <stdint.h>
#include "msp430.h"
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
    if(CALBC1_1MHZ == 0xFF)                    // If calibration constant erased
    {
        while(1)
        {
            ;                                   // do not load, trap CPU!!
        }
    }
    DCOCTL = 0;                                 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                      // Set range
    DCOCTL = CALDCO_1MHZ;                       // Set DCO step + modulation */
#elif (MCLK_MHZ == 8)
    if(CALBC1_8MHZ == 0xFF)                    // If calibration constant erased
    {
        while(1)
        {
            ;                                  // do not load, trap CPU!!
        }
    }
    DCOCTL = 0;                                // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ;                     // Set range
    DCOCTL = CALDCO_8MHZ;                      // Set DCO step + modulation */
#else
#error "Frequency not supported, change project setttings"
#endif

    BCSCTL3 = LFXT1S_2;                         // VLO for ACLK (~12Khz)

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
    // PMBus Alert (P2.5)
    // Note that PMBusControl lines (P2.3-P2.4) are initialized as output low
    P1OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT6 | BIT7);
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P1DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT6 | BIT7);
    P3DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

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
    // Set P1[6:7]pins for I2C functionality
    P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    P1SEL2 |= BIT6 + BIT7;                    // Assign I2C pins to USCI_B0
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
    // G2xx implementation doesn't use base address, the module is hardcoded
    // in smbuslib PHY
    return(NULL);
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
    // PMBus Alert (P2.5) as input with pullup
    // PMBusControl lines (P2[4:3] are already initialized as output low
    P2DIR &= ~BIT5; // Pin as Input
    P2OUT |= BIT5;  // Enable pull-up
    P2REN |= BIT5;  //      "
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
    // SMBAlert is in P2.5
    if((P2IN & BIT5) == 0x00)
    {
        return(true); // If line is low, the Alert is asserted
    }
    else
    {
        return(false); // Otherwise...
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
    if(CtrlLine == CtrlLine1)
    {
        // ControlLine1 = P2.4
        if(LogicState == true)
        {
            P2OUT |= BIT4;
        }
        else
        {
            P2OUT &= ~BIT4;
        }
    }
    else
    {
        // ControlLine2 = P2.3
        if(LogicState == true)
        {
            P2OUT |= BIT3;
        }
        else
        {
            P2OUT &= ~BIT3;
        }
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
    if(CtrlLine == CtrlLine1)
    {
        // ControlLine1 = P2.4
        if((P2IN & BIT4) == 0x00)
        {
            return(false);
        }
        else
        {
            return(true);
        }
    }
    else
    {
        // ControlLine2 = P2.3
        if((P2IN & BIT3) == 0x00)
        {
            return(false);
        }
        else
        {
            return(true);
        }
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
        P1OUT |= BIT0;
    }
    else if(LED == LED1)
    {
        P2OUT |= BIT0;  // Not that P2.0 is not connected to LED in Launchpad
    }
    else
    {
        P1OUT |= BIT0;
        P2OUT |= BIT0;
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
        P1OUT &= ~BIT0;
    }
    else if(LED == LED1)
    {
        P2OUT &= ~BIT0; // Not that P2.0 is not connected to LED in Launchpad
    }
    else
    {
        P1OUT &= ~BIT0;
        P2OUT &= ~BIT0;
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
        P1OUT ^= BIT0;
    }
    else if(LED == LED1)
    {
        P2OUT ^= BIT0;  // Not that P2.0 is not connected to LED in Launchpad
    }
    else
    {
        P1OUT ^= BIT0;
        P2OUT ^= BIT0;
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
    // Configure timer sourced by ACLK
    TA1CTL = TASSEL_1 | MC_0 | TACLR;   // ACLK, stop, clear count

    // 1ms counter
    TA1CCR0 = (9400 / 1000);
    // Enable interrupt
    TA1CCTL0 = CCIE;

    Delay_ms_counter = ms;

    // Start timer in UP mode
    TA1CTL |= MC_1;

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
#pragma vector=USCIAB0TX_VECTOR, USCIAB0RX_VECTOR
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
//! Timeout Timer ISR
//!
//! Interrupt Service routine for Timeout Timer. Used to detect timeout from I2C.
//!
//! \return  None
//
// *****************************************************************************
#ifdef __MSP430_HAS_USCI__
#pragma vector=TIMER0_A0_VECTOR
__interrupt void HAL_TIMEOUT_ISR(void)
{
    // Call the SMBUS function to handle a timeout error and restart the SMBUS
    if(HAL_Timeout_Event() == true)
    {
        LPM3_EXIT;
    }
}

#endif

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
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    if(Delay_ms_counter > 0)
    {
        Delay_ms_counter--;
    }
    else
    {
        TA1CCTL0 &= ~CCIE;
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
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
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
#pragma vector=TRAPINT_VECTOR,PORT1_VECTOR,PORT2_VECTOR, \
    TIMER0_A1_VECTOR,WDT_VECTOR,COMPARATORA_VECTOR,TIMER1_A1_VECTOR, \
    NMI_VECTOR
__interrupt void Dummy_ISR(void)
{
    while(1)
    {
        ;
    }
}
