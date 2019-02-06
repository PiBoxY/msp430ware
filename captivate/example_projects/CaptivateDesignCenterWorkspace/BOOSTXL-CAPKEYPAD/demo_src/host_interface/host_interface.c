/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
//#############################################################################
//
//! \file   host_interface.c
//!
//! \brief  This is the board support package for the BOOSTXL-CAPKEYPAD.
//
//  Group:          MSP
//  Target Device:  MSP430FR2522
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include "driverlib.h"
#include "rom_map_driverlib.h"
#include "system.h"
#include "host_interface.h"

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \def The interrupts used for I2C slave operation
//!
#define I2C_INTERRUPT_MASK (EUSCI_B_I2C_STOP_INTERRUPT      |\
                            EUSCI_B_I2C_RECEIVE_INTERRUPT0  |\
                            EUSCI_B_I2C_TRANSMIT_INTERRUPT0 )

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief A boolean for indicating whether the current I2C activity is a
//! new transaction
volatile bool HostInterface_newTransaction;

//! \brief The requested host interface register to be read/written
//!
volatile uint8_t HostInterface_reg;

//! \brief the host interface I2C receive buffer for storing the received
//! byte from the I2C slave interface
volatile uint8_t HostInterface_receiveBuffer;

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void HostInterface_init(void)
{
    EUSCI_B_I2C_initSlaveParam HostInterface_I2CSlaveParams =
    {
        .slaveAddress = HOST_INTERFACE_I2C_SLAVE_ADDRESS,
        .slaveAddressOffset = EUSCI_B_I2C_OWN_ADDRESS_OFFSET0,
        .slaveOwnAddressEnable = EUSCI_B_I2C_OWN_ADDRESS_ENABLE
    };

    HostInterface_reg = 0;
    HostInterface_newTransaction = true;

    HOST_IRQ_DIR &= ~HOST_IRQ_PIN;
    HOST_IRQ_OUT &= ~HOST_IRQ_PIN;

    MAP_EUSCI_B_I2C_initSlave(EUSCI_B0_BASE, &HostInterface_I2CSlaveParams);
    MAP_EUSCI_B_I2C_enable(EUSCI_B0_BASE);
    MAP_EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE, I2C_INTERRUPT_MASK);
}

#pragma vector = USCI_B0_VECTOR
__interrupt void HostInterface_I2CHandler(void)

{
    switch (__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:           // Vector 0: No interrupts
            break;
        case USCI_I2C_UCALIFG:    // Vector 2: ALIFG
            break;
        case USCI_I2C_UCNACKIFG:  // Vector 4: NACKIFG
            break;
        case USCI_I2C_UCSTTIFG:   // Vector 6: STTIFG
            break;
        case USCI_I2C_UCSTPIFG:   // Vector 8: STPIFG
            UCB0IFG &= ~UCSTPIFG;
//          HostInterface_reg = 0; // Include this to only allow repeated start reads.
            HostInterface_newTransaction = true;
            __bic_SR_register_on_exit(LPM4_bits);
            break;
        case USCI_I2C_UCRXIFG3:   // Vector 10: RXIFG3
            break;
        case USCI_I2C_UCTXIFG3:   // Vector 14: TXIFG3
            break;
        case USCI_I2C_UCRXIFG2:   // Vector 16: RXIFG2
            break;
        case USCI_I2C_UCTXIFG2:   // Vector 18: TXIFG2
            break;
        case USCI_I2C_UCRXIFG1:   // Vector 20: RXIFG1
            break;
        case USCI_I2C_UCTXIFG1:   // Vector 22: TXIFG1
            break;
        case USCI_I2C_UCRXIFG0:   // Vector 24: RXIFG0
            HostInterface_receiveBuffer = UCB0RXBUF;
            if (HostInterface_newTransaction == true)
            {
                HostInterface_reg = HostInterface_receiveBuffer;
                HostInterface_newTransaction = false;
            }
            else
            {
                if (HostInterface_reg < SYSTEM_CTRL_BYTE_REGISTER_COUNT)
                {
                    System_topLevel.regFile[HostInterface_reg++] = HostInterface_receiveBuffer;
                }
            }
            break;
        case USCI_I2C_UCTXIFG0:   // Vector 26: TXIFG0
            if (HostInterface_reg < SYSTEM_BYTE_REGISTER_COUNT)
            {
                UCB0TXBUF = System_topLevel.regFile[HostInterface_reg++];
            }
            else
            {
                UCB0TXBUF = HOST_INTERFACE_I2C_SLAVE_INVALID_RETURN;
            }
            break;
        case USCI_I2C_UCBCNTIFG:  // Vector 28: BCNTIFG
            break;
        case USCI_I2C_UCCLTOIFG:  // Vector 30: clock low timeout
            break;
        case USCI_I2C_UCBIT9IFG:  // Vector 32: 9th bit
            break;
        default:
            break;
    }
}
