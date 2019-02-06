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
 * SMB_Slave01_ReadByte_Echo
 *
 * This example responds to a ReadByte command with an echo
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

#include "Slave_HAL.h"
#include "smbus.h"

#if (SMB_MANUAL_ACK_ENABLE == 1)
#error "This example doesn't implement DMA ISR for Manual ACK"
#endif

//
// Macros and definitions
//
#define SMB_I2C_SLAVE_OWN_ADDR   (0x43) /*! Own Slave address */
#define SMB_DEFAULT_PEC_ENABLED         /*! Define to enable PEC by default */

//
// Global variables
//
static SMBus sSMBusSlave;                       /*! SMBus Structure           */
static uint8_t au8RxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Reception Buffer      */
static uint8_t au8TxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Transmission Buffer   */

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Slave demo responding to a ReadByte command from a Master
//!  The example responds to all commands by sending an echo
//!
//! \return None
//
// *****************************************************************************
void main(void)
{
    HAL_MCU_Init();         // Initialize default CLK and GPIO settings
    HAL_I2C_InitGPIO();         // Initialize GPIOs for I2C functionality

    // Initialize SMBus Master using USCI_B0 always at 100kbps per SMBus spec
    SMBus_slaveInit(&sSMBusSlave, HAL_I2C_GetBaseAddr());
    SMBus_slaveSetAddress(&sSMBusSlave, SMB_I2C_SLAVE_OWN_ADDR);

    // Set the RX and TX buffers for SMBus layers
    SMBus_slaveSetRxBuffer(&sSMBusSlave, au8RxBuff, sizeof(au8RxBuff));
    SMBus_slaveSetTxBuffer(&sSMBusSlave, au8TxBuff, sizeof(au8TxBuff));

    SMBus_slaveEnableInt(&sSMBusSlave);

    #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBusSlave);  // Enable Packet Error Check
    #endif

    while(1)
    {
        // Wait for packets from master
        __disable_interrupt();
        {
            _BIS_SR(LPM3_bits + GIE); // Go to sleep
        }
        __enable_interrupt();
    } // While (1)
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
    // Check the state of SMBus
    switch(SMBus_slaveProcessInt(&sSMBusSlave))
    {
    case SMBus_State_Slave_QCMD:
        // Quick command is not implemented
        break;
    case SMBus_State_Slave_CmdComplete:
        // Send echo of the command
        sSMBusSlave.nwk.txBuffPtr[0] = sSMBusSlave.nwk.rxBuffPtr[0];
        sSMBusSlave.nwk.txLen = 1;
        break;
    case SMBus_State_Slave_FirstByte:
        // this example doesn't validate the command
        break;
    case SMBus_State_DataSizeError:
    case SMBus_State_PECError:
    case SMBus_State_Slave_Error:
    case SMBus_State_TimeOutError:
        // This demo simply updates the Status register when an error is
        // detected. The Host can check this register
        break;
    default:
        break;
    }
    // Clear flags to be ready for next packet
    SMBus_processDone(&sSMBusSlave);

    return(false); // No need to wake-up
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
    SMBus_slaveProcessTimeoutInt(&sSMBusSlave);
    return(false);
}

#endif
