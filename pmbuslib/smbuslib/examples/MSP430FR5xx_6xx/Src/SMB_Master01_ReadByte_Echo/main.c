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
 * SMB_Master01_ReadByte_Echo
 *
 * This example sends a ReadByte command to a Slave
 *
 *                               /|\  /|\
 *                               Rp    Rp
 *                  slave         |    |        master
 *            -----------------   |    |   -----------------
 *           |             SDA*|<-|----+->|SDA*             |
 *           |                 |  |       |                 |
 *           |                 |  |       |                 |
 *           |             SCL*|<-+------>|SCL*             |
 *   LED0 <--|Px.y*            |          |            Px.y*|--> LED0
 *   LED1 <--|Pz.a*            |          |            Pz.a*|--> LED1
 *
 *                          * Check HAL for I/Os used for each device
 *+----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "Master_HAL.h"
#include "smbus.h"

//
// Macros and definitions
//
#define SLAVE_ADDRESS 0x43          /*! Address of slave device */
#define SMB_DEFAULT_PEC_ENABLED     /*! Define to enable PEC */

//
//  Global variables
//
static SMBus sSMBMaster;                             /*! SMBus Master Struct  */
static SMBus_State sSMBState;                        /*! SMBus state          */
static uint8_t Resp_Buff[SMB_MAX_PACKET_SIZE + 4];     /*! Buffer for response \
                                                           from slave device    */

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Master demo sending a ReadByte command to a slave
//!   Check response using logic analyzer or set breakpoints to debug
//!
//! \return none
//
// *****************************************************************************
void main(void)
{
    uint8_t cmd = 0;
    int8_t ret;

    HAL_MCU_Init();             // Initialize default CLK and GPIO settings
    HAL_I2C_InitGPIO();         // Initialize GPIOs for I2C functionality

    // Initialize SMBus Master always at 100kbps per SMBus spec
    SMBus_masterInit(&sSMBMaster, HAL_I2C_GetBaseAddr(), (MCLK_MHZ * 1000000));

 #ifdef SMB_DEFAULT_PEC_ENABLED
    // Call if PEC is enabled by default
    SMBus_enablePEC(&sSMBMaster);
 #endif

    // Enable SMBus Interrupts, after initializing I2C
    SMBus_masterEnableInt(&sSMBMaster);

    __enable_interrupt();

    while(1)
    {
        // Send Read byte, the slave should respond with an echo of the command
        ret = SMBus_masterReadByteWord(&sSMBMaster,     // SMB struct
                                       SLAVE_ADDRESS,   // Slave Addr
                                       cmd,            // SMB Command
                                       Resp_Buff,       // ResponsePtr
                                       1);              // 1 byte expected

        // After sending the command, wait for completion and check response
        if(ret == SMBUS_RET_OK)
        {
            // If the command was sent to slave, wait for completion
            if(SMBus_masterWaitUntilDone (&sSMBMaster, 100000) != SMBUS_RET_OK)
            {
                ret = -1;
            }
            else
            {
                ret = sSMBState;
            }

            // If we are waiting for a response, check if we got it
            if(ret == SMBus_State_OK)
            {
                if(SMBus_getRxPayloadAvailable(&sSMBMaster) != 1)
                {
                    ret = -1;             // RX Data size Error
                }
                else
                {
                    // Check if we got the echo
                    if(Resp_Buff[0] != cmd)
                    {
                        ret = -1;
                    }
                }
            }
        }

        if(ret != SMBus_State_OK)
        {
            HAL_LED_SetHigh(LED0);  // Set LED0 if error
            SMBus_masterReset(&sSMBMaster); // Restart the interface
        }
        else
        {
            HAL_LED_SetLow(LED0);  // clear LED0 if OK
        }
        sSMBMaster.status.u8byte = 0x00; // Clear status
        cmd++;  // Increment command
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
    // Process SMBus ISR as Master, store result flags
    // Note that the Master starts transfers in Main loop and this function
    // handles the SMBus State machine. The main loop just waits for completion
    sSMBState = SMBus_masterProcessInt(&sSMBMaster);

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
    // Call the SMBUS function to handle a timeout error and restart the SMBUS
    sSMBState = SMBus_masterProcessTimeoutInt(&sSMBMaster);
    return(false);
}

#endif
