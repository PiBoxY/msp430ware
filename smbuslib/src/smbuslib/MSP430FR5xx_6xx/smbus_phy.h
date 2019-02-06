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
#ifndef __SMBUS_PHY_H__
#define __SMBUS_PHY_H__

//*****************************************************************************
//
//! \addtogroup smbus_MSP430FR5xx_6xx_phy MSP430 PHY layer
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "smbus_phy.h"
#include "smbus.h"

//! R/W state when write is received
#define I2C_WRITE      0
//! R/W state when read is received
#define I2C_READ       1

//*****************************************************************************
//
//! \brief   Disables the PHY and Data Link layer
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_disable(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Enables the PHY and Data Link layer
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_enable(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Enables the PHY and Data Link layer for slave operation
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveEnable(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Enables the I2C interrupts
//
//!  This function enables the eUSCI Start,Stop, RX,TX, Timeout interrupts.
//!  If Manual_ACK is enabled, it enables DMA to handle the RX
//!  SMBus_PHY_slaveInit() must be called before this function.
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveEnableInt(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Initializes the I2C Slave module supporting SMBus functionality
//
//!  - Resets and then configures the I2C for SMBus support
//!  - I2C is enabled using Automatic ACK and Slave address is initialized to 0x00
//!  - Call SMBus_slaveSetAddress() in order to set the Slave address
//!  - Call SMBus_slaveEnableManualACK() in order to enable Manual ACK
//
//! \param *smbus     Pointer to SMBus structure
//! \param i2cAddr    Base address of I2C module
//
// \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveInit(SMBus *smbus,
                                uint16_t i2cAddr);

//*****************************************************************************
//
//! \brief   I2C Interrupt Service routine
//
//!  Handles the interrupts from eUSCI module and passes the information to
//!  the network layer. Should be called by application when an eUSCI
//!  interrupt is detected.
//
//! \param smbus    Pointer to SMBus structure
//
//! \return  The new state of Slave (see SMBus_slaveProcessInt())
//
//*****************************************************************************
extern SMBus_State SMBus_PHY_slaveProcessInt(SMBus *smbus);

//*****************************************************************************
//
//! \brief   This function is not used in the MSP430FR5xx_6xx implementation.
//
//*****************************************************************************
extern SMBus_State SMBus_PHY_slaveProcessTimeoutInt(SMBus *smbus);

#if (SMB_MANUAL_ACK_ENABLE == 1)
//*****************************************************************************
//
//! \brief   DMA Interrupt Service routine
//
//! Used when SW ACK/NACK is enabled since the double-buffer mechanism of eUSCI
//! doesn't allow us to perform manual ACK/NACK
//
//! \param smbus    Pointer to SMBus structure
//
//! \return new state of Slave (see SMBus_slaveProcessInt())
//
//*****************************************************************************
extern SMBus_State SMBus_PHY_slaveProcessIntDMA(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Enables functionality to perform manual ACK/NACK
//
//!  DMA will be used to stretch SCL
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveEnableManualACK(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Disables functionality to perform manual ACK/NACK
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveDisableManualACK(SMBus *smbus);

//******************************************************************************
//
//! \brief   Send a manual ACK
//
//! \param *smbus    Pointer to SMBus structure
//! \param sendAck
//!                 - true: sends ACK
//!                 - false: sends NACK
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_slaveSendACK(SMBus *smbus,
                                   bool sendAck);
#endif

//*****************************************************************************
//
//! \brief   Enables the PHY and Data Link layer
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterEnable(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Enables the I2C interrupts
//
//!  This function enables the eUSCI Start,Stop, RX,TX, Timeout interrupts.
//!  If Manual_ACK is enabled, it enables DMA to handle the RX
//!  SMBus_PHY_slaveInit() must be called before this function.
//
//! \param *smbus     Pointer to SMBus structure
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterEnableInt(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Initializes the I2C Master module supporting SMBus functionality
//
//! - Resets and then configures the I2C for SMBus support
//! - I2C is enabled using Automatic ACK and Slave address is initialized to 0x00
//
//! \param smbus      Pointer to SMBus structure
//! \param i2cAddr    Base address of I2C module
//! \param busClk     SMCLK Frequency (used for eUSCI)
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterInit(SMBus *smbus,
                                 uint16_t i2cAddr,
                                 uint32_t busClk);

//*****************************************************************************
//
//! \brief   Generate Stop condition if it hasn't been sent
//
//! \param smbus      Pointer to SMBus structure
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterSendStop(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Prepare to send stop at next byte
//
//! \param smbus      Pointer to SMBus structure
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterSendPreStop(SMBus *smbus);

//*****************************************************************************
//
//! \brief   Start a TX transfer
//
//! \param smbus      Pointer to SMBus structure
//! \param targetaddr Slave target address
//! \param stopFlag   Indicates the stop condition
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterStartTx(SMBus *smbus,
                                    uint8_t targetaddr,
                                    SMBus_Stop stopFlag);

//*****************************************************************************
//
//! \brief   Start a RX transfer
//! \param smbus      Pointer to SMBus structure
//! \param targetaddr Slave target address
//! \param stopFlag   Indicates the stop condition
//
//! \return  None
//
//*****************************************************************************
extern void SMBus_PHY_masterStartRx(SMBus *smbus,
                                    uint8_t targetaddr,
                                    SMBus_Stop stopFlag);

//*****************************************************************************
//
//! \brief   I2C Interrupt Service routine
//
//!  Handles the interrupts from eUSCI module and passes the information to
//!  the network layer. Should be called by application when an eUSCI
//!  interrupt is detected.
//
//! \param smbus    Pointer to SMBus structure
//
//! \return  The new state of master (see SMBus_masterProcessInt())
//
//*****************************************************************************
extern SMBus_State SMBus_PHY_masterProcessInt(SMBus *smbus);

//*****************************************************************************
//
//! \brief   This function is not used in the MSP430FR5xx_6xx implementation.
//
//*****************************************************************************
extern SMBus_State SMBus_PHY_masterProcessTimeoutInt(SMBus *smbus);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

#endif //__SMBUS_PHY_H__
