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
 * SMB_Slave00_AllProtocols
 *
 * This example implements a Slave testing all SMBus 2.0 protocols one-by-one
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
 * Switch -->|Pxx.c*           |          |                 |
 *
 *                          * Check HAL for I/Os used for each device
 *+----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 * ---------------------------------------------------------------------------*/

#include "Slave_HAL.h"
#include "smbus.h"
#include "SMBus_Commands_List.h"

//
// Macros and definitions
//
#define SMB_I2C_SLAVE_OWN_ADDR   (0x43) /*! Own Slave address */
#define SMB_DEFAULT_PEC_ENABLED         /*! Define to enable PEC by default */

//
//  Global variables
//
//  Variables used for SMBus functionality
static SMBus sSMBusSlave;                       /*! SMBus Structure           */
static uint8_t au8RxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Reception Buffer      */
static uint8_t au8TxBuff[SMB_MAX_PACKET_SIZE];  /*! SMB Transmission Buffer   */
//  Variables used for demo
static volatile uint16_t au16ADC_shadow;        /*! ADC value                 */
/*! String written/read on block Read/Write commands */
static uint8_t au8String_buf[SMB_MAX_PAYLOAD_SIZE] =
    "Default string for SMBus430 Demo";
static const uint8_t u8Version = 0x15;           /*! Demo u8Version            */
static int8_t i8current_handler;                /*! Current command handler   */

//
//  Function prototypes
//
static int8_t Demo_isCmd_valid(uint8_t cmd);
static int8_t Demo_CmdComplete(SMBus *SMB);

//*****************************************************************************
//
//! Main Function
//!
//! SMBus Slave demo testing all SMBus 2.0 protocols
//! Waits for command from the SMBus Master and processes it.
//! The commands supported are:
//! | Function         | CMD   |Type              | Description
//! | Quick_Led_toggle |  -    |QUICK_COMMAND     | Toggle LED
//! | Read_Reg_Ptr     |  -    |RECEIVE_BYTE      | Read Register pointer contents
//! | Set_Reg_Ptr      | 00-03 |SEND_BYTE         | Change Register pointer
//! | Write_Reg        | 10-13 |WRITE_BYTE        | Write Register
//! | Read_Reg         | 20-23 |READ_BYTE         | Read Register
//! | Read_ADC_Ch      | 30    |READ_WORD         | Read ADC
//! | Mult_Bytes       | 40    |PROCESS_CALL      | Multiplies 2 bytes
//! | Write_String     | 50    |BLOCK_WRITE       | Write a string
//! | Read_String      | 51    |BLOCK_READ        | Read the string
//! | Calc_CRC         | 60    |BLW_BLR_PROC_CALL | Multiplies 2 bytes
//!
//! A timer triggered ADC gets continuous samples automatically
//! A GPIO switch is enabled in IOPort to read it using Read_Reg or Read_Reg_Ptr
//!
//! \return None
//
// *****************************************************************************
void main(void)
{
    HAL_MCU_Init();         // Initialize default CLK and GPIO settings

    HAL_ADCwTimer_Init();   // Initialize timer-triggered ADC (for demo)
    HAL_IOPort_Init();      // Initialize IOPort (for demo)

    HAL_I2C_InitGPIO();         // Initialize GPIOs for I2C functionality

    // Initialize SMBus Master using USCI_B0 always at 100kbps per SMBus spec
    SMBus_slaveInit(&sSMBusSlave, HAL_I2C_GetBaseAddr());
    SMBus_slaveSetAddress(&sSMBusSlave, SMB_I2C_SLAVE_OWN_ADDR);

    // Set the RX and TX buffers for SMBus layers
    SMBus_slaveSetRxBuffer(&sSMBusSlave, au8RxBuff, sizeof(au8RxBuff));
    SMBus_slaveSetTxBuffer(&sSMBusSlave, au8TxBuff, sizeof(au8TxBuff));

    #if (SMB_MANUAL_ACK_ENABLE == 1)
    SMBus_slaveEnableManualACK (&sSMBusSlave);
    #endif

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

//*****************************************************************************
//
//! Quick Command
//!
//! \param Addr_RW  Received address + R/W bit
//!
//! This demo toggles LED0 when a Quick Command is received
//! Note that we can detect QuickCommand(W), but not QuickCommand(R)
//!   Format:
//!     S + Slave_Addr+R/W*+A + P
//!
//! \return SMBUS_RET_OK
//
// *****************************************************************************
int8_t SMBus_App_Quickcommand(uint8_t Addr_RW)
{
    HAL_LED_Toggle(LED0);

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write Register Pointer
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command is received on a SEND_BYTE
//! The contents of this register pointer will be sent when a RECEIVE_BYTE
//! command is sent by the host.
//! Format:
//!   S + Slave_Addr+W+A + CMD +A + {PEC+A} + P
//!   CMD = 0x00-0x03, sets pointer to Register0-3
//!       Reg0 = SMBus Status
//!       Reg1 = SMBus Control
//!       Reg2 = PxIN
//!       Reg3 = Demo u8Version
//!
//! \return  SMBUS_RET_OK if pointer was updated correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_Reg_Ptr_hdlr(SMBus * SMB){
    // Change the register pointer to the new address
    // Implemented registers are 0-3
    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Point to SMBus status register
        SMB->nwk.recByteTxPtr = &(SMB->status.u8byte);
        break;
    case 0x01:
        // Point to Control Reg
        SMB->nwk.recByteTxPtr = &(SMB->ctrl.u8byte);
        break;
    case 0x02:
        // Get address of IOPort
        SMB->nwk.recByteTxPtr = HAL_IOPort_GetAddrR();
        break;
    case 0x03:
        // u8Version
        SMB->nwk.recByteTxPtr = (uint8_t *) &u8Version;
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write Register
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command writes a register directly when a WRITE_BYTE command is
//! received. Note that some bits in some registers are read-only.
//!
//! Format:
//!     S + Slave_Addr+W+A + CMD+A + VAL+A + {PEC+A} + P
//!    CMD = 0x10-0x13 Writes Register 0-3
//!        Reg0 = SMBus Status  (clear only)
//!        Reg1 = SMBus Control (some bits are read-only)
//!        Reg2 = PxOUT
//!        Reg3 = Demo u8Version (read-only)
//!     VAL = New value being written to register
//!
//! \return  SMBUS_RET_OK if pointer was updated correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_Reg_hdlr(SMBus * SMB){
    // Implemented registers are 0-3, some are Read-only
    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Clear Status Register
        SMB->status.u8byte =
            SMBus_slaveClearStatusReg(SMB, SMB->nwk.rxBuffPtr[1]);
        break;
    case 0x01:
        // Writes to Control Register (only to r/w bits)
        SMB->ctrl.u8byte =
            SMBus_slaveWriteCtrlReg(SMB, SMB->nwk.rxBuffPtr[1]);
        break;
    case 0x02:
        // Write to IOPort
        HAL_IOPort_Write(SMB->nwk.rxBuffPtr[1]);
        break;
    case 0x03:
        // Register is read-only, ignore request
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads Register
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads a register directly when a READ_BYTE command is
//! received.
//! Format:
//!   S + Slave_Addr+W+A + CMD+A +
//!   Sr + Slave_Addr+R+A + VAL+A + {PEC+A} + P
//!   CMD = 0x20-0x23 Reads Register 0-3
//!       Reg0 = SMBus Status
//!       Reg1 = SMBus Control
//!       Reg2 = PxIN
//!       Reg3 = Demo u8Version
//!   VAL = Value sent as response from MSP430
//!
//!
//! \return  SMBUS_RET_OK if register read correctly,
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_Reg_hdlr(SMBus * SMB){
    // Change the register pointer to the new address
    // Implemented registers are 0-7
    switch((SMB->nwk.rxBuffPtr[0]) & 0x03)
    {
    case 0x00:
        // Point to SMBus status register
        SMB->nwk.txBuffPtr[0] = SMB->status.u8byte;
        break;
    case 0x01:
        // Point to Control Reg
        SMB->nwk.txBuffPtr[0] = SMB->ctrl.u8byte;
        break;
    case 0x02:
        // Read IOPort
        SMB->nwk.txBuffPtr[0] = HAL_IOPort_Read();
        break;
    case 0x03:
        // u8Version
        SMB->nwk.txBuffPtr[0] = u8Version;
        break;
    default:
        // Invalid register
        return(SMBUS_RET_ERROR);
    }

    SMB->nwk.txLen = 1;
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Reads ADC
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command reads the latest result from ADC conversions using READ_WORD
//  protocol.
//! ADC conversions are performed automatically by the device, using timer
//! as a trigger.
//!   Format:
//!    S + Slave_Addr+W+A + CMD+A +
//!    Sr + Slave_Addr+R+A + VAL_L+A + VAL_H+A + {PEC+A} + P
//!   CMD = 0x30
//!   VAL_L = Low byte of ADC
//!   VAL_H = High byte of ADC
//!
//!
//! \return  SMBUS_RET_OK if ADC read successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_ADC_Ch_hdlr(SMBus * SMB){
    // Get the ADC value which is updated by ADC Timer
    uint16_t temp;

    temp = au16ADC_shadow;

    // Send 2 bytes of ADC as response
    SMB->nwk.txBuffPtr[0] = (temp & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((temp & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Multiply 2 bytes
//!
//! \param SMB  SMBus structure which received the command
//!
//! This command gets 2 bytes and returns the result of their multiplication
//!   using PROCESS_CALL format.
//!   Format:
//!     S + Slave_Addr+W+A + CMD+A + VAL_1+A + VAL_2+A +
//!      Sr + Slave_Addr+R+A + RES_L+A + RES_H+A + {PEC+A} + P
//!   CMD = 0x40
//!   VAL_1 = First byte (multiplicand)
//!   VAL_2 = Second byte (multiplier)
//!   RES_L = Low byte of result of VAL_1*VAL_2
//!   RES_H = High byte of result of VAL_1*VAL_2
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Mult_Bytes_hdlr(SMBus * SMB){
    uint16_t mult;

    // Multiply the 2 received values
    mult = SMB->nwk.rxBuffPtr[1] * SMB->nwk.rxBuffPtr[2];
    // Set the response as a word
    SMB->nwk.txBuffPtr[0] = (mult & 0x00FF);
    SMB->nwk.txBuffPtr[1] = ((mult & 0xFF00) >> 8);
    SMB->nwk.txLen = 2;

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Write a string
//!
//! \param SMB  SMBus structure which received the command
//!
//! Gets a block of data and stores in string using BLOCK_WRITE protocol.
//!   Format:
//!      S + Slave_Addr+W+A + CMD+A + COUNT+A + DATA1+A +...+ DATAN+A + {PEC+A} +P
//!   CMD = 0x50
//!   COUNT = Number of bytes being received
//!   DATA1-DATAN = Received values 1-N
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Write_String_hdlr(SMBus * SMB){
    uint8_t i;

    // Simply write the received data to local buffer
    for(i = 0; i < SMB->nwk.rxBuffPtr[1]; i++)
    {
        au8String_buf[i] = SMB->nwk.rxBuffPtr[2 + i];
    }

    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Read a string
//!
//! \param SMB  SMBus structure which received the command
//!
//! Returns a string using BLOCK_READ protocol.
//!   Format:
//!    S + Slave_Addr+W+A + CMD+A +
//!    Sr + Slave_Addr+R+A + COUNT+A + DATA1+A +...+ DATAN+A + {PEC+A} +P
//!   CMD = 0x51
//!   COUNT = Number of bytes being sent (32)
//!   DATA1-DATAN = Sent values 1-N
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Read_String_hdlr(SMBus * SMB){
    uint8_t i;

    // Set the lenght of payload
    SMB->nwk.txBuffPtr[0] = SMB_MAX_PAYLOAD_SIZE;
    // Now send the data
    for(i = 0; i < SMB_MAX_PAYLOAD_SIZE; i++)
    {
        SMB->nwk.txBuffPtr[i + 1] = au8String_buf[i];
    }
    // Number of bytes to send (including LEN)
    SMB->nwk.txLen = (SMB_MAX_PAYLOAD_SIZE + 1);
    return(SMBUS_RET_OK);
}

//*****************************************************************************
//
//! Calculate CRC
//!
//! \param SMB  SMBus structure which received the command
//!
//! Calculates the CRC of a packet using BLOCKWRITE_BLOCKREAD PROCESS CALL
//!  protocol.
//!   Format:
//!    S + Slave_Addr+W+A + CMD+A + COUNT+A + DATA1+A +...+ DATA2+A +
//!    Sr + Slave_Addr+R+A + COUNT_TX+A + RES_L+A +RES_H+A + {PEC+A} + P
//!   CMD = 0x60
//!   COUNT = Number of received bytes
//!   DATA1-DATAN = Received values 1-N
//!    COUNT_TX = Number of bytes to transmit (2)
//!    RES_L = Low Byte of CRC Result
//!    RES_H = High Byte of CRC Result
//!
//! \return  SMBUS_RET_OK if executed OK
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
int8_t Calc_CRC_hdlr(SMBus * SMB){
    uint16_t CRC;

    // Calculate the CRC of the packet
    CRC = HAL_Calculate_CRC(&SMB->nwk.rxBuffPtr[2], SMB->nwk.rxBuffPtr[1]);
    SMB->nwk.txBuffPtr[0] = 2;    // Response lenght is 2 bytes
    SMB->nwk.txBuffPtr[1] = (CRC & 0x00FF);
    SMB->nwk.txBuffPtr[2] = ((CRC & 0xFF00) >> 8);
    SMB->nwk.txLen = 3; // Number of bytes to send (including COUNT_TX)

    return(SMBUS_RET_OK);
}

//
// Local functions
//
//*****************************************************************************
//
//! Validates a command
//!
//! \param cmd  Received command
//!
//! This function is called when a command is received and the slave needs to
//! check if the command is valid or not.
//! The function checks a list of valid commands (SMB_CMD_List) in order to
//! validate the current command, and if the command is found, it gets its
//! associated handler.
//!
//! \return  SMBUS_RET_OK if command was found
//!          SMBUS_RET_ERROR if command is invalid
//
// *****************************************************************************
static int8_t Demo_isCmd_valid(uint8_t cmd)
{
    uint8_t i;

    i8current_handler = -1; // Handler is invalid by default

    for(i = 0; i < (CMD_LIST_LENGHT); i++)
    {
        if((cmd | SMB_CMD_List[i].mask) ==
           (SMB_CMD_List[i].cmd | SMB_CMD_List[i].mask))
        {
            // If the command is valid, save the handler which will be executed
            // if the command is received completely in Demo_CmdComplete
            i8current_handler = i;
            return(SMBUS_RET_OK);
        }
    }

    return(SMBUS_RET_ERROR);
}

//*****************************************************************************
//
//! Command complete callback
//!
//! \param SMB  SMBus structure which received the command
//!
//! This function is called when a received packet is ready to process
//! after a STOP or RE-START.
//! The function executes the corresponding function if it was found by
//! Demo_isCmd_valid().
//! This demo implements a list of commands in SMB_CMD_List but the function
//! can be customized according to particular needs (i.e. switch statement or
//! hardcoded response).
//!
//! \return  SMBUS_RET_OK if command executed successfully
//!          SMBUS_RET_ERROR if unsuccessful
//
// *****************************************************************************
static int8_t Demo_CmdComplete(SMBus *SMB)
{
    int8_t ret;
    // i8current_handler is set by Demo_isCmd_valid when a command is received
    if(i8current_handler != -1)
    {
        // If the command was found as valid, call the corresponding handler
        ret = SMB_CMD_List[i8current_handler].handler(SMB);
        i8current_handler = -1;   // Invalidate the handler after execution
    }
    return(ret);
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
        // If a Quick command was detected, execute function (if any)
        SMBus_App_Quickcommand(sSMBusSlave.nwk.currentAddr & 0x01);
        break;
    case SMBus_State_Slave_CmdComplete:
        // If any other command besides Quick Command is detected, process it
        if(Demo_CmdComplete(&sSMBusSlave) == SMBUS_RET_ERROR)
        {
            SMBus_slaveReportError(&sSMBusSlave, SMBus_ErrorCode_Packet);
        }
        return(true);         // Exit to main loop if required
        //break;
#if (SMB_MANUAL_ACK_ENABLE != 1)
    case SMBus_State_Slave_FirstByte:
        // If Manual ACK is disabled , validate the command here
        if(Demo_isCmd_valid(SMBus_slaveGetCommand(&sSMBusSlave)) !=
           SMBUS_RET_OK)
        {
            SMBus_slaveReportError(&sSMBusSlave, SMBus_ErrorCode_Cmd);
        }
        break;
#endif
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
//! DMA ISR
//!
//! Interrupt Service routine for DMA. Used only when Manual ACK is enabled
//! to ACK/NACK bytes manually
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
#if (SMB_MANUAL_ACK_ENABLE == 1)
bool HAL_DMA_Event(void)
{
    // Process the DMA interrupt
    SMBus_State Status = SMBus_slaveProcessInt(&sSMBusSlave);

    if(Status == SMBus_State_Slave_FirstByte)
    {
        // If a Command with FirstByte is received, validate command
        if(Demo_isCmd_valid(SMBus_slaveGetCommand(&sSMBusSlave)) !=
           SMBUS_RET_OK)
        {
            SMBus_slaveReportError(&sSMBusSlave, SMBus_ErrorCode_Cmd);
            SMBus_slaveSendACK(&sSMBusSlave, false); // Send manual NACK
            uint8_t u8SMB_Status_Shadow = sSMBusSlave.status.u8byte;
        }
        else
        {
            SMBus_slaveSendACK(&sSMBusSlave, true);
        }
    }
    else if(Status == SMBus_State_Slave_ByteReceived)
    {
        // Note that this example doesn't check each byte
        // A PEC Error is recognized after packet completion but is not NACKed
        // If desired, the PEC can be checked here based on each command
        SMBus_slaveSendACK(&sSMBusSlave, true);
    }
    else
    {
        // Invalid state, NACK the byte
        SMBus_slaveSendACK(&sSMBusSlave, false);
    }
    SMBus_processDone(&sSMBusSlave);

    return(false);
}

#endif

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

//*****************************************************************************
//
//! ADC ISR
//!
//! Reads current ADC result and stores it
//!
//! \param   value has the result of latest ADC conversion
//!
//! \return  true to wake-up MCU, false to stay in LPMx
//
// *****************************************************************************
bool HAL_ADC_Event(uint16_t value)
{
    au16ADC_shadow = value;
    return(false);
}
