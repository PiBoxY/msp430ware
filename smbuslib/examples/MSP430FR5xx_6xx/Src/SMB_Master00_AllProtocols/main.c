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
 * SMB_Master00_AllProtocols
 *
 * This example implements a Master testing all SMBus 2.0 protocols one-by-one
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
 * IOPort </>|Pxx*             |          |                 |
 *   ADC  -->|Ayy*             |          |                 |
 *
 *                          * Check HAL for I/Os used for each device
 *+----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 * ---------------------------------------------------------------------------*/

#include <stdint.h>
#include "Master_HAL.h"
#include "smbus.h"
#include "SMB_Demo.h"

//
// Macros and definitions
//
#define SLAVE_ADDRESS 0x43          /*! Address of slave device */
#define SMB_DEFAULT_PEC_ENABLED     /*! Define to enable PEC by default */

//
//  Global variables
//
static SMBus sSMBMaster;                             /*! SMBus Master Struct  */
static SMBus_State sSMBState;                        /*! SMBus state          */
static uint8_t Command_Buff[SMB_MAX_PACKET_SIZE + 4];  /*! Buffer for command \
                                                           to slave   */
static uint8_t Resp_Buff[SMB_MAX_PACKET_SIZE + 4];     /*! Buffer for response \
                                                           from slave device    */
static char const TestString[32] = "this is a test"; /*! String used to test \
                                                         block write */

/*! Step-by-step Tests performed by this demo */
enum Tests
{
    QUICK_CMD_TEST = 0,      /*! Test Quick Command                      */
    SEND_BYTE_TEST,         /*! Test Send Byte command                  */
    RECEIVE_BYTE_TEST,      /*! Test Receive Byte command               */
    WRITE_BYTE_TEST,        /*! Test Write Byte Command                 */
    READ_BYTE_TEST,         /*! Test Read Byte command                  */
    READ_WORD_TEST,         /*! Test Read Word command                  */
    PROCESS_CALL_TEST,      /*! Test Process Call command               */
    BLOCK_WRITE_TEST,       /*! Test Block Write command                */
    BLOCK_READ_TEST,        /*! Test Block Read command                 */
    BLOCK_PROC_CALL_TEST,   /*! Test BlockWBlockR Process Call command  */
    END_OF_TESTS            /*! delimiter                               */
} Tests_e;

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Master demo testing all SMBus 2.0 protocols
//!   Each command is sent to slave one-by-one
//!   Check response using logic analyzer or set breakpoints to debug
//!
//! \return None
//
// *****************************************************************************
void main(void)
{
    uint8_t ret, RespLen;
    bool RXExpected;

    HAL_MCU_Init();             // Initialize default CLK and GPIO settings
    HAL_I2C_InitGPIO();         // Initialize GPIOs for I2C functionality

    // Initialize SMBus Master always at 100kbps per SMBus spec
    SMBus_masterInit(&sSMBMaster, HAL_I2C_GetBaseAddr(), (MCLK_MHZ * 1000000));

 #ifdef SMB_DEFAULT_PEC_ENABLED
    SMBus_enablePEC(&sSMBMaster);       // Call if PEC is enabled by default
 #endif

    // Enable SMBus Interrupts, after initializing I2C
    SMBus_masterEnableInt(&sSMBMaster);

    __enable_interrupt();

    Tests_e = QUICK_CMD_TEST;    // Start with Quick Command test

    while(1)
    {
        switch(Tests_e)
        {
        case QUICK_CMD_TEST:
            // Send QuickCommand (W), no data nor command is needed
            //  Slave is expected to simply toggle LED1
            ret = SMBus_masterQuickCommand(&sSMBMaster,         // SMB struct
                                           SLAVE_ADDRESS,       // Slave Addr
                                           true);               // QuickCMD W
            RXExpected = false;      // Response not expected
            break;
        case SEND_BYTE_TEST:
            // Send Sendbyte, command is needed
            //  Slave will change its register pointer:
            //  0x00 = Reg0 = SMBus Status
            //  0x01 = Reg1 = SMBus Control
            //  0x02 = Reg2 = Port Input
            //  0x03 = Reg3 = Demo version  <- Used in demo
            ret = SMBus_masterSendByte(&sSMBMaster,             // SMB struct
                                       SLAVE_ADDRESS,           // Slave Addr
                                       0x03);                   // SMB Command
            RXExpected = false;      // No response expected

            break;
        case RECEIVE_BYTE_TEST:
            // Send Receive byte, no data nor command is needed
            //  Slave is expected to send the contents of its register
            //  pointer (set by Send_Byte command)
            ret = SMBus_masterReceiveByte(&sSMBMaster,       // SMB struct
                                          SLAVE_ADDRESS,     // Slave Addr
                                          Resp_Buff);        // Response
            RXExpected = true;      // Response expected (version)
            break;
        case WRITE_BYTE_TEST:
            // Send Write byte, Command and data are needed
            //  Slave will write directly to the corresponding register
            //  0x10 = Reg0 = SMBus Status
            //  0x11 = Reg1 = SMBus Control
            //  0x12 = Reg2 = Port Output   <- Used in demo
            //  0x13 = Reg3 = Demo version
            Command_Buff[0] = 0xAA;         // Write 0xAA to the port
            ret = SMBus_masterWriteByteWord(&sSMBMaster,        // SMB struct
                                            SLAVE_ADDRESS,      // Slave Addr
                                            0x12,               // SMB Command
                                            &Command_Buff[0],    // DataPtr
                                            1);                 // 1 byte data
            RXExpected = false;      // No response expected
            break;
        case READ_BYTE_TEST:
            // Send Read byte, Command is needed
            //  Slave will read directly from the corresponding register
            //  0x20 = Reg0 = SMBus Status
            //  0x21 = Reg1 = SMBus Control
            //  0x22 = Reg2 = Port input    <- Used in demo
            //  0x23 = Reg3 = Demo version
            ret = SMBus_masterReadByteWord(&sSMBMaster,         // SMB struct
                                           SLAVE_ADDRESS,       // Slave Addr
                                           0x22,                // SMB Command
                                           Resp_Buff,           // ResponsePtr
                                           1);                  // 1 byte expected
            RXExpected = true;      // Response expected (port input)
            break;
        case READ_WORD_TEST:
            // Send Read Word, Command is needed
            //  0x30 = Slave will send contents of latest ADC conversion
            ret = SMBus_masterReadByteWord(&sSMBMaster,         // SMB struct
                                           SLAVE_ADDRESS,       // Slave Addr
                                           0x30,                // SMB Command
                                           Resp_Buff,           // ResponsePtr
                                           2);                  // 2 bytes expected
            RXExpected = true;      // Response expected (ADC result)
            break;
        case PROCESS_CALL_TEST:
            // Send Process Call, Command and 2 bytes of data are needed
            //  0x40 = Multiply 2 bytes
            Command_Buff[0] = 0x02;         // Multiplier 1
            Command_Buff[1] = 0x10;         // Multiplier 2
            ret = SMBus_masterProcessCall(&sSMBMaster,          // SMB struct
                                          SLAVE_ADDRESS,        // Slave Addr
                                          0x40,                 // SMB Command
                                          &Command_Buff[0],     // DataPtr
                                          Resp_Buff);           // ResponsePtr
            RXExpected = true;      // Response expected (result of multiplication)
            break;
        case BLOCK_WRITE_TEST:
            // Send Block Write with CMD, Len and Data
            //  0x50 = Write a string
            ret = SMBus_masterWriteBlock(&sSMBMaster,               // SMB struct
                                         SLAVE_ADDRESS,             // Slave Addr
                                         0x50,                      // SMB Command
                                         (uint8_t *) &TestString[0],       // DataPtr
                                         sizeof(TestString));       // Data Len
            RXExpected = false;      // No response expected
            break;
        case BLOCK_READ_TEST:
            // Send Read Block, Command is needed
            //  0x51 = Read a string from slave
            ret = SMBus_masterReadBlock(&sSMBMaster,            // SMB struct
                                        SLAVE_ADDRESS,          // Slave Addr
                                        0x51,                   // SMB Command
                                        Resp_Buff);             // ResponsePtr
            RXExpected = true;      // Response expected (string)
            break;
        case BLOCK_PROC_CALL_TEST:
            // Send Block Write with CMD, Men and Data
            //  0x60 = Calculate CRC of bytes
            Command_Buff[0] = 0x01;         // Data1
            Command_Buff[1] = 0x02;         // Data2
            Command_Buff[2] = 0x03;         // Data3
            ret = SMBus_masterProcessCallBlock(&sSMBMaster,        // SMB struct
                                               SLAVE_ADDRESS,      // Slave Addr
                                               0x60,                // SMB Command
                                               &Command_Buff[0],     // DataPtr
                                               3,                   // Data Len
                                               Resp_Buff);         // ResponsePtr
            RXExpected = true;      // Response expected (CRC)
            break;
        default:
            Tests_e = QUICK_CMD_TEST;   // Unexpected case, restart demo
            break;
        }

        // After sending the command, wait for completion and check response
        if(ret == SMBUS_RET_OK)
        {
            // If the command was sent to slave, wait for completion
            if(SMBus_masterWaitUntilDone (&sSMBMaster,
                                          DEMO_TIMEOUT) != SMBUS_RET_OK)
            {
                ret = DEMO_TIMEOUT_ERROR;
                // Timeout detected in App but not by SCL line, restart interface
                SMBus_masterReset(&sSMBMaster);
            }
            else
            {
                ret = sSMBState;
            }

            // If we are waiting for a response, check if we got it
            if((ret == SMBus_State_OK) && (RXExpected == true))
            {
                // Get the lenght of payload for response
                RespLen = SMBus_getRxPayloadAvailable(&sSMBMaster);

                if(RespLen >= 1)
                {
                    ret = DEMO_NO_ERROR_DATA_AVAIL; // Data Received OK
                }
                else
                {
                    ret = DEMO_RX_ERROR;             // RX Data size Error
                }
            }
        }

        sSMBMaster.status.u8byte = 0x00; // Clear status
        Tests_e++;                       // Run next test
        if(Tests_e >= END_OF_TESTS)
        {
            Tests_e = QUICK_CMD_TEST;
        }
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
