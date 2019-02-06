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
 * ======== main.c ========
 * PMBus_Master_TPS544C20_Control
 *
 * This example configures the TPS544C20 using the Control line to turn on/off
 * the device's output
 *
 *
 *                               /|\  /|\
 *                               Rp    Rp
 *                  Master        |    |        Slave
 *                  MSP430        |    |        TPS544C20
 *            -----------------   |    |      -------------
 *           |             SDA*|<-|----+----> |DATA       |
 *           |                 |  |           |           |--> VOUT
 *           |             SCL*|<-+---------> |CLK        |
 *           |                 |              |           |
 *   LED0 <--|Px.y*   Control2*|------------->|CNTL       |
 *   LED1 <--|Pz.a*            |              |           |
 *
 *                          * Check HAL for I/Os used for each device
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "Master_HAL.h"
#include "pmbus.h"

#define TPS544C20_ADDRESS 0x1B      /*! Slave address of TPS544C20 device */
#define ENABLE_PEC                  /*! Uncomment to enable PEC */

/*! Macro used to check the output of each command.
 *  For demo purposes, the device will stay in a loop with both LEDs ON if an
 *  error was detected (i.e. timeout, NACK, etc)
 */
#define CHECK_RET() {if(ret != PMBUS_RET_OK) \
                     {HAL_LED_SetHigh(LED_BOTH); while(1) {; } \
                     } \
}

/*! Buffer for command to slave */
static uint8_t Command_Buff[PMB_MAX_PACKET_SIZE + 4];
/*! Buffer for response from slave device */
static uint8_t Resp_Buff[PMB_MAX_PACKET_SIZE + 4];

//*****************************************************************************
//
//! Main Function
//!
//! This example configures the TPS544C20 using the CONTROL line.
//! The CONTROL line turns the output ON/OFF together or independently of the
//! OPERATION command. The signal is optional per PMBus spec but it's
//! implemented in many PMBus devices.
//!
//! \return none
//
// *****************************************************************************
void main(void)
{
    uint32_t timeout = 100000;
    int8_t ret;

    HAL_MCU_Init();             // Initialize default CLK and GPIO settings
    HAL_I2C_InitGPIO();         // Initialize GPIOs for I2C functionality

    // Initialize PMBus interface
    PMBus_init(HAL_I2C_GetBaseAddr(), (MCLK_MHZ * 1000000));
    // Initialize other GPIOs used for PMBus. This example uses SMBAlert
    // in addition to the I2C lines.
    HAL_PMBus_InitGPIO();

#ifdef ENABLE_PEC
    // If desired, enable PEC. The TPS544C20 device will accept commands with or
    // without PEC, but it will only check PEC if it's sent/requested
    PMBus_enablePEC();
#endif

    __enable_interrupt();

    // Verify the ID of the device (0x0153 for TPS544C20 Rev 3)
    if((PMBus_cmdReadWord(TPS544C20_ADDRESS,
                          0xFC,
                          Resp_Buff,
                          timeout) != PMBUS_RET_OK)
       || (Resp_Buff[0] != 0x53)
       || (Resp_Buff[1] != 0x01))
    {
        // Stay in loop with LEDs ON if device didn't respond OK
        HAL_LED_SetHigh(LED_BOTH); //Set LEDs high
        while(1)
        {
            ;
        }
    }

    // Write VIN_OFF 4.00 V: Minimum allowed value for VIN_OFF
    Command_Buff[0] = 0x10; //LSB
    Command_Buff[1] = 0xF0; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_VIN_OFF,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write VIN_ON 4.25 V
    Command_Buff[0] = 0x11; //LSB
    Command_Buff[1] = 0xF0; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_VIN_ON,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write IOUT_OC_WARN_LIMIT 4.0 A
    Command_Buff[0] = 0x08; //LSB
    Command_Buff[1] = 0xF8; //MSB
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_IOUT_OC_WARN_LIMIT,
                         Command_Buff,
                         2,
                         timeout);
    CHECK_RET();

    // Write ON_OFF_CONFIG Mode: CONTROL Pin Only; Control: Active High
    // The CONTROL line will be used to turn on/off the device independently of
    // the OPERATIOn command
    Command_Buff[0] = 0x16;
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_ON_OFF_CONFIG,
                         Command_Buff,
                         1,
                         timeout);
    CHECK_RET();

    // The control line is a simple GPIO. In this example, the GPIO is
    // configured and controlled by the HAL
    // The output of the device should be ON after this function
    HAL_PMBus_SetControl(CtrlLine2, true);

    while(1)
    {
        // toggle LED if OK
        HAL_LED_Toggle(LED0);
        HAL_Timer_Delay_ms(200);
    }
}

//
// Events
//
//*****************************************************************************
//
//! I2C Event
//!
//! Event called by HAL when I2C TX, RX, Start, Stop and errors are detected
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
bool HAL_I2C_Event(void)
{
    PMBus_processInt();
    return(false);
}

//*****************************************************************************
//
//! Timer Timeout Event
//!
//! Event called by HAL when Timeout timer is detected for devices without eUSCI
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
#ifdef __MSP430_HAS_USCI__
bool HAL_Timeout_Event(void)
{
    PMBus_processTimeoutInt();
    return(false);
}

#endif
