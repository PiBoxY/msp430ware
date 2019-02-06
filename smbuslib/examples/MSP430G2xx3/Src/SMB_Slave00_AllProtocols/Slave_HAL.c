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
 *      |   I2C         |    USCIB0     |
 *      |   I2C_SDA     |    P1.7       |
 *      |   I2C_SCL     |    P1.6       |
 *      |   LED0        |    P1.0       |
 *      |   LED1        |    N/A        |
 *      |   ADC         |    P1.4/A4    |
 *      |   ADC trigger |    TA1.0      |
 *      |   IOPort      |    P1         |
 *      |   Switch      |    P1.3       |
 *      =================================
 * ---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "msp430.h"
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
    P1OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P1DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P2DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
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
//! Turn on a LED
//!
//! \param LED LED0 for LED0, LED1 for LED1, any other value for both LEDs
//!
//! \return None
//
// *****************************************************************************
void HAL_LED_SetHigh(t_LED LED)
{
    // Only LED0 is implemented
    P1OUT |= BIT0;
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
    // Only LED0 is implemented
    P1OUT &= ~BIT0;
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
    // Only LED0 is implemented
    P1OUT ^= BIT0;
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
    // Initialize ADC in P1.4/A4
    // No need to initialize P1SEL
    ADC10AE0 |= BIT4;                       // Enable Analog input for A4

    // ADC10ON, S&H=16 ADC clks, Enable interrupt
    ADC10CTL0 |= ADC10SHT_2 | ADC10ON | ADC10IE;
    // ADCCLK = ADC10OSC; Pulse-sample,Software trigger,
    //  single-channel, Convert CH4
    ADC10CTL1 = SHS_0 | CONSEQ_0 | INCH_4;
    // Start sampling
    ADC10CTL0 |= ENC;

    // Initialize TA1.0 for ADC updates
    TA1CCR0 = (12000 / 10);                   // Frequency of conversion ~10Hz (VLO/10)
    TA1CTL = TACLR;                         // Clear flags and counter
    TA1CTL = TASSEL_1 + MC_1 + TAIE;        // Enable Conversions
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
    // Port 1 used as IOPort, all outputs by default, except P1.3

    // Enable P1.3 as input with pull-up to read switch
    P1DIR &= ~BIT3;
    P1OUT |= BIT3;
    P1REN |= BIT3;
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
    return((uint8_t *) &P1IN);
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
    P1OUT = val;
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
    return(P1IN);
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
    uint8_t CRC = 0x00;

    extern void SMBus_NWK_addCrc8(uint8_t *crc,
                                  uint8_t newData);

    // Calculate CRC for each byte
    for(i = 0; i < size; i++)
    {
        SMBus_NWK_addCrc8(&CRC, data[i]);
    }
    return(CRC);
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
//! ADC Interrupt Service Routine
//!
//! ISR for ADC module
//!   This ISR generates an event which should be declared in application
//!
//! \return None
//
// *****************************************************************************
#pragma vector=ADC10_VECTOR
__interrupt void HAL_ADC_ISR(void)
{
    if(HAL_ADC_Event(ADC10MEM) == true)
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
//! TImer trigger for ADC Service Routine
//!
//! ISR for Timer triggering ADC
//!   This ISR generates an event which should be declared in application
//!
//! \return None
//
// *****************************************************************************
#pragma vector=TIMER1_A1_VECTOR
__interrupt void HAL_ADCTrigger_ISR(void)
{
    switch(TA1IV)
    {
    case  2:  break;                            // CCR1 not used
    case  4:  break;                            // CCR2 not used
    case 10:
        ADC10CTL0 |= ADC10SC;               // Start ADC conversion
        break;
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
    TIMER0_A1_VECTOR,WDT_VECTOR,COMPARATORA_VECTOR, \
    NMI_VECTOR
__interrupt void Dummy_ISR(void)
{
    while(1)
    {
        ;
    }
}
