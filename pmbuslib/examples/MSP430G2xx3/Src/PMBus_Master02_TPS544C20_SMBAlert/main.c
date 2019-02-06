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
 * PMBus_Master_TPS544C20_SMBAlert
 *
 * This example configures the TPS544C20 and attends SMBAlert events
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
 *           |       /|\       |              |           |
 *   LED0 <--|Px.y*   |---Pb.c*|<-------------|SMBAlert   |
 *   LED1 <--|Pz.a*            |              |           |
 *
 *                          * Check HAL for I/Os used for each device
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "Master_HAL.h"
#include "pmbus.h"

#define TPS544C20_ADDRESS 0x1B      /*! Slave address of TPS544C20 device */
#define ENABLE_PEC                  /*! Uncomment to enable PEC */
#define ENABLE_ARA_CHECK            /*! Uncomment to send ARA packet */

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
/*! Used to store the TPS544C20 Status flags after the Alert event */
static volatile uint16_t Status_Flag;

//*****************************************************************************
//
//! Main Function
//!
//! This example configures the TPS544C20 and enables SMBAlert functionality.
//! When an alert is detected by the slave device, the MSP430 reads the fault
//! and clears the flag.
//! If enabled, MSP430 will send an ARA packet to double-check if the TPS544C20
//! is the device asserting the SMBAlert line.
//! The example can be tested by lowering the TPS544C20's VIN
//!
//! \return none
//
// *****************************************************************************
void main(void)
{
    uint32_t timeout = 100000;
    int8_t ret;
    uint8_t size;

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

    // Clear SMBAlert flags
    HAL_PMBus_ClearSMBAlertInterrupt();
    // Enable SMBAlert to generate interrupt
    HAL_PMBus_EnableSMBAlertInterrupt();

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

    // Write ON_OFF_CONFIG Mode: OPERATION Only
    Command_Buff[0] = 0x1A;
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_ON_OFF_CONFIG,
                         Command_Buff,
                         1,
                         timeout);
    CHECK_RET();

    // Write MFR_23 (MASK_SMBALERT)
    // No alert is masked.
    // Auto_ARA is enabled which will make the slave device clear the alert line
    // as soon as an ARA packet is received
    Command_Buff[0] = 0x00; //LSB
    Command_Buff[1] = 0x01; //MSB
    ret = PMBus_cmdWriteWord(TPS544C20_ADDRESS,
                             0xE7,
                             Command_Buff,
                             timeout);
    CHECK_RET();

    // Write OPERATION register to enable the output, margin off
    Command_Buff[0] = 0x80;
    ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                         PMB_OPERATION,
                         Command_Buff,
                         1,
                         timeout);
    CHECK_RET();

    while(1)
    {
        __disable_interrupt();
        // Sleep if SMBAlert is not asserted
        while(HAL_PMBus_GetSMBAlert() == false)
        {
            _BIS_SR(LPM3_bits + GIE);
        }
        __enable_interrupt();

        // The only wake-up source in this example is the SMBAlert line
        // Check if the line is asserted and stay in this loop while the Alert
        // is active
        while(HAL_PMBus_GetSMBAlert() == true)
        {
            HAL_LED_SetHigh(LED0);      // LED0 goes high to show the error

#ifdef ENABLE_ARA_CHECK
            // Send ReceiveByte packet to Alert Response Address to check if the
            // TPS544C20 was the device calling the Alert.
            // Note that this is useful when there are multiple slaves on the
            // bus, but it's not needed when there's only one device.
            // If MASK_SMBALERT.Auto_ARA is enabled, the SMBAlert line will be
            // released immediately after this command, check the TPS544C20
            // User's Guide for more details.
            if((PMBus_ReceiveByteARA(&Resp_Buff[0], timeout) != PMBUS_RET_OK) ||
               (Resp_Buff[0] >> 1 != TPS544C20_ADDRESS))
            {
                // Either the TPS544C20 didn't respond to ARA packet
                // or some other device pulled the Alert Line
                for(;; )
                {
                    HAL_LED_SetHigh(LED1);   // Turn LED1 one to signal error
                }
            }
#endif

            ret = PMBus_cmdRead(TPS544C20_ADDRESS,
                                PMB_STATUS_WORD,
                                Resp_Buff,
                                &size,
                                timeout);
            CHECK_RET();

            // Resp_Buff[0] and Resp_Buff[1] will have the faults.
            // Check TPS544C20 to decode the different errors. An application
            // can do different actions depending on the fault.
            Status_Flag = Resp_Buff[0] << 8 + Resp_Buff[1];

            // This example simply clears the fault flags and if the fault is
            // persistent, it will trigger the event again
            ret = PMBus_cmdWrite(TPS544C20_ADDRESS,
                                 PMB_CLEAR_FAULTS,
                                 NULL,
                                 0,
                                 timeout);
            CHECK_RET();

            // Clear the edge detection flag (note that the line will be checked
            // again by the while() loop.
            HAL_PMBus_ClearSMBAlertInterrupt();
        }

        HAL_LED_SetLow(LED0);      // LED0 goes low after the flag is clear

        // Once the SMBAlert is de-asserted, we re-enable the interrupt.
        // Note that a new edge in between these 2 events will still be detected
        // by the device and it will trigger an event as soon as we enable
        // the interrupt.
        HAL_PMBus_EnableSMBAlertInterrupt();
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

//*****************************************************************************
//
//! SMBAlert Event
//!
//! Event called by HAL when SMBAlert is asserted
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
bool HAL_SMBAlert_Event(void)
{
    // Disable SMBAlert interrupt while check for the source of the Alert
    HAL_PMBus_DisableSMBAlertInterrupt();

    return (true);
}
