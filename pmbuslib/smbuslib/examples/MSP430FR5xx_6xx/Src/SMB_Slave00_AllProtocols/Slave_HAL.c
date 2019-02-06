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
 *      |   LED0        |    P1.0       |
 *      |   LED1        |    P4.6       |
 *      |   ADC         |    P2.4/A7    |
 *      |   ADC trigger |    TA0.1      |
 *      |   IOPort      |    P4         |
 *      |   Switch      |    P4.5       |
 *      =================================
 * ---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "driverlib.h"
#include "Slave_HAL.h"

//
// Local functions
//
static void MCU_Clock_Init(void);
static void MCU_GPIO_Init(void);

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
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 |
                           GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 |
                           GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 |
                           GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 |
                           GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 |
                           GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4 | GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 |
                        GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
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
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, BIT6 + BIT7,
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
//! Initialize a Timer-triggered ADC
//!
//! The corresponding GPIOs, ADC and a Timer is enabled to trigger continuous
//! conversions automatically
//!
//! \return None
//
// *****************************************************************************
void HAL_ADCwTimer_Init(void)
{
    // Initialize ADC in P2.4/A7
    P2SEL0 |= BIT4;
    P2SEL1 |= BIT4;
    // ADC12ON, S&H=16 ADC clks
    ADC12CTL0 |= ADC12SHT0_2 + ADC12ON;
    // ADCCLK = MODOSC; Pulse-sample, TA0.1 as trigger, repeated-single-channel
    ADC12CTL1 |= ADC12SHP | ADC12SHS_1 | ADC12CONSEQ_2;
    // convert A7 channel
    ADC12MCTL0 = ADC12INCH_7;
    // Enable ADC conv complete interrupt
    ADC12IER0 |= ADC12IE0;
    // Start sampling
    ADC12CTL0 |= ADC12ENC;

    // Initialize TimerA0.1 for ADC updates
    TA0CCR0 = (8300 / 10);                    // Frequency of conversion ~10Hz (VLO/10)
    TA0CCTL1 = OUTMOD_3;                    // TACCR1 in set/reset mode
    TA0CCR1 = TA0CCR0 / 2;                    // Set TA0.1 duty cycle
    TA0CTL = TASSEL__ACLK + MC_1;           // Enable Conversions
}

//*****************************************************************************
//
//! Initialize the Port used for SMBus demo IO operations
//!
//! This port can be read/write using SMBus commands
//!
//! \return None
//
// *****************************************************************************
void HAL_IOPort_Init(void)
{
    // Port 4 used as IOPort, all outputs by default, except P4.5
    // Enable P4.5 as input with pull-up to read switch
    P4DIR &= ~BIT5;
    P4OUT |= BIT5;
    P4REN |= BIT5;
}

//*****************************************************************************
//
//! Get address of IOPort Read register
//!
//! \return Pointer to IOPort
//
// *****************************************************************************
uint8_t * HAL_IOPort_GetAddrR(void)
{
    return((uint8_t *) &P4IN);
}

//*****************************************************************************
//
//! Write to the IO Port
//
//! \param val Value to be written to IO Port
//!
//! \return None
//
// *****************************************************************************
void HAL_IOPort_Write(uint8_t val)
{
    P4OUT = val;
}

//*****************************************************************************
//
//! Read value of IO Port
//!
//! \return value of IO Port
//
// *****************************************************************************
uint8_t HAL_IOPort_Read(void)
{
    return(P4IN);
}

//*****************************************************************************
//
//! Calculate CRC of a packet
//!
//! A CRC is implemented using CRC16 module if available, if not
//! a CRC8 is returned using the same algorithm as PEC
//!
//! \param data Pointer to data array
//! \param size Number of bytes in data array
//!
//! \return CRC result
//
// *****************************************************************************
uint16_t HAL_Calculate_CRC(uint8_t *data,
                           uint8_t size)
{
    uint8_t i;
    CRCINIRES = 0x0000;

    // Calculate CRC for each byte
    for(i = 0; i < size; i++)
    {
        CRCDIRB_L = data[i];
    }
    return(CRCINIRES);
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
//! DMA Interrupt Service Routine
//!
//! ISR for DMA module
//!   This ISR generates an event which should be declared in application
//!
//! \return None
//
// *****************************************************************************
#if (SMB_MANUAL_ACK_ENABLE == 1)
#pragma vector=DMA_VECTOR
__interrupt void HAL_DMA_ISR(void)
{
    // This event should be declared in Application
    if(HAL_DMA_Event() == true)
    {
        LPM3_EXIT;
    }
}

#endif

//*****************************************************************************
//
//! ADC Interrupt Service Routine
//!
//! ISR for ADC module
//!   This ISR generates an event which should be declared in application
//!
//! \return None
//
// *****************************************************************************
#pragma vector=ADC12_VECTOR
__interrupt void HAL_ADC_ISR(void)
{
    switch(__even_in_range(ADC12IV,12))
    {
    case  0: break;                              // No interrupt
    case  2: break;                              // conu8Version result overflow
    case  4: break;                              // conu8Version time overflow
    case  6: break;                              // ADC12HI
    case  8: break;                              // ADC12LO
    case 10: break;                              // ADC12IN
    case 12:
        // This event should be declared in Application
        if(HAL_ADC_Event(ADC12MEM0) == true)
        {
            LPM3_EXIT;
        }
        break;
    default: break;
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
    USCI_A0_VECTOR,TIMER0_B1_VECTOR,TIMER0_B0_VECTOR, \
    SYSNMI_VECTOR,AES256_VECTOR, PORT3_VECTOR,TIMER3_A0_VECTOR, \
    TIMER1_A0_VECTOR, TIMER2_A0_VECTOR, COMP_E_VECTOR, UNMI_VECTOR
__interrupt void Dummy_ISR(void)
{
    while(1)
    {
        ;
    }
}
