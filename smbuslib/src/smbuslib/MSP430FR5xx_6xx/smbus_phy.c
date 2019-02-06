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

#include <stdint.h>
#include <msp430.h>
#include "smbus.h"
#include "smbus_nwk.h"
#include "smbus_phy.h"

#include "inc/hw_types.h"
#include "eusci_b_i2c.h"
#if (SMB_MANUAL_ACK_ENABLE == 1)
#include "dma.h"
#endif
#ifdef  __IAR_SYSTEMS_ICC__
#include "deprecated/IAR/msp430fr5xx_6xxgeneric.h"
#else
#include "deprecated/CCS/msp430fr5xx_6xxgeneric.h"
#endif

#if (SMB_MANUAL_ACK_ENABLE == 1)
/* eUSCI doesn't support Manual ACK, a workaround needs to be implemented
 * The workaround uses DMA to stretch SCL low immediately after a byte is
 * received. The DMA ISR is attended and the application then decides if it
 * needs to ACK or NACK the byte. The following Hardware definitions are used
 * in order to use GPIOs and DMA:
 */
#define SMB_SWACK_I2CPORT_BASE  (__MSP430_BASEADDRESS_PORT1_R__)
#define SMB_SWACK_SDA_PIN       (BIT6)
#define SMB_SWACK_SCL_PIN       (BIT7)
#define SMB_SWACK_PxSEL_OFSET   (OFS_P1SEL1)    // P1SEL1
#define SMB_SWACK_DEFAULT_STATE (BIT6 | BIT4)
#define SWB_SWACK_DMA_SOURCE    (DMA_TRIGGERSOURCE_18)
const uint8_t u8DefaultDMA_State = SMB_SWACK_DEFAULT_STATE;
#endif

void SMBus_PHY_disable(SMBus *smbus)
{
    // Reset the eUSCI module
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |= UCSWRST;
    smbus->ctrl.bits.phyEn = 0;      // Update control flag
}

void SMBus_PHY_enable(SMBus *smbus)
{
    if(smbus->ctrl.bits.master)
    {
        SMBus_PHY_masterEnable(smbus);
    }
    else
    {
        SMBus_PHY_slaveEnable(smbus);
    }
}

void SMBus_PHY_slaveEnable(SMBus *smbus)
{
#if (SMB_MANUAL_ACK_ENABLE == 1)
    uint8_t GPIO_Mask;
#endif

    // Make sure the PHY is disabled
    SMBus_PHY_disable(smbus);

    // Write the Slave Address
    HWREG(smbus->phy.SMBus_Phy_i2cBase +
          OFS_UCBxI2COA0) = smbus->ownSlaveAddr + UCOAEN;

#if (SMB_MANUAL_ACK_ENABLE == 1)
    // If Manual ACK is enabled, make sure pins have I2C functionality
    if(smbus->ctrl.bits.swackEn == 1)
    {
        GPIO_Mask = (SMB_SWACK_SCL_PIN | SMB_SWACK_SDA_PIN);
        // Set pins for I2C functionality
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1SEL1) |= GPIO_Mask;
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1SEL0) &= ~GPIO_Mask;
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1OUT) &= ~GPIO_Mask;
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1DIR) |= GPIO_Mask;
    }
#endif

    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIFG) = 0x00;        // Clear all flags
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) &= ~UCSWRST; // Start USCI I2C

    // Re-enable Interrupts since the Reset will clear them
    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_slaveEnableInt(smbus);
    }

    smbus->ctrl.bits.phyEn = 1;      // Set global flag
}

void SMBus_PHY_slaveEnableInt(SMBus *smbus)
{
    // Clear all flags
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIFG) = 0x00;

#if (SMB_MANUAL_ACK_ENABLE == 1)
    // Enable eUSCI Interrupts (Start, Stop, TX, RX, Timeout)
    HWREG(smbus->phy.SMBus_Phy_i2cBase +
          OFS_UCBxIE) = UCSTTIE | UCSTPIE | UCTXIE0 |
                        UCCLTOIE;

    if(smbus->ctrl.bits.swackEn == 0)
    {
        // If SWACK is disabled, handle RX in eUSCI ISR
        HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIE) |= UCRXIE0;
    }
    else
    {
        // Else, handle RX in DMA interrupt
        HWREG(DMA_BASE + OFS_DMA0CTL) |= DMAIE;
    }
#else
    // Enable eUSCI Interrupts (Start, Stop, TX, RX, Timeout)
    HWREG(smbus->phy.SMBus_Phy_i2cBase +
          OFS_UCBxIE) = UCSTTIE | UCSTPIE | UCTXIE0 |
                        UCRXIE0 | UCCLTOIE;
#endif

    // Set global flag
    smbus->ctrl.bits.intEn = 1;
}

void SMBus_PHY_slaveInit(SMBus *smbus,
                         uint16_t i2cAddr)
{
    SMBus_Phy *SMBusPHY = &smbus->phy;

    // Set the I2C Base Address
    smbus->phy.SMBus_Phy_i2cBase = i2cAddr;
    // Software reset enabled
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) = UCSWRST;
    // Use SMCLK, I2C mode, sync mode
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |=
        UCSSEL__SMCLK | UCMODE_3 + UCSYNC;
    // Select deglitch = 50ns, SW Acknownedge
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW1) = UCGLIT_0 | UCSWACK;
    // Enable Clock low timeout (150,000 MODCLK cycles = ~31ms)
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW1) |= UCCLTO_2;

    // Slave Address not initialized by default
    smbus->ownSlaveAddr = 0;

    // Enable the I2C module
    SMBus_PHY_slaveEnable(smbus);
}

SMBus_State SMBus_PHY_slaveProcessInt(SMBus *smbus)
{
    SMBus_State ret_state = SMBus_State_Slave_NTR;
    SMBus_Phy *SMBusPHY = &smbus->phy;
    uint8_t data, addr;

    // Check received address
    addr = HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxADDRX);

    // Check if there is a RX pending and RX Interrupts are enabled
    if((HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIE) & UCRXIE0) &&
       (HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCRXIFG0))
    {
        // RXIFG0 is set when a byte is received for slave address
        // Note that this ISR will be disabled and DMA will handle RX when
        // the ACK/NACK workaround is enabled
        // Get the data from the buffer
        data = HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxRXBUF);
        // Pass data to NWK layer
        ret_state = SMBus_NWK_slaveProcessRx(smbus, data);
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCSTPIFG)
    {
        // Check if a stop is pending
        // Inform the NWK later, so it can process the whole packet
        ret_state = SMBus_NWK_slaveProcessStop(smbus);
        // Clear flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCSTPIFG;
        if((HWREG(SMBusPHY->SMBus_Phy_i2cBase +
                  OFS_UCBxIFG) & (UCTXIFG0 | UCSTTIFG)) ==
           UCTXIFG0)
        {
            // Clear TX flag if set and no start was received
            HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCTXIFG0;
        }
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCSTTIFG)
    {
        // Get received address
        data = addr << 1;

        if((HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) & UCTR))
        {
            // If Master is requesting data, add Read bit to Address+RW
            data |= I2C_READ;
        }
        else
        {
            // Clear TX flag if set in case it's still set from previous transfer
            HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCTXIFG0;
        }
        // Pass information to NWK
        ret_state = SMBus_NWK_slaveProcessStart(smbus, data);
        // ACKnowledge the address
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |= UCTXACK;

        // Clear start flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCSTTIFG;
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCTXIFG0)
    {
        // TXIFG0 is set when host is requesting a byte from our slave address
        // Inform the NWK layer and get the data which will be transmitted
        ret_state = SMBus_NWK_slaveProcessTx(smbus, &data);
        // Send the data
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxTXBUF) = data;
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCCLTOIFG)
    {
        // This flag is set when a clock timeout is detected
        // Inform the NWK of the error
        ret_state = SMBus_NWK_slaveProcessTimeout(smbus);
        // Clear flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCCLTOIFG;
    }

    return(ret_state);
}

#if (SMB_MANUAL_ACK_ENABLE == 1)
SMBus_State SMBus_PHY_slaveProcessIntDMA(SMBus *smbus)
{
    SMBus_State ret_state = SMBus_State_Slave_NTR;
    SMBus_Phy *SMBusPHY = &smbus->phy;
    uint8_t data;

    // I2C bus should be stalled when we get here because pin is output low
    // We can process the data in the buffer and decide if we need to:
    // ACK (release SCL and let the USCI module send the ACK), or
    // NACK (force a NACK by setting I2C pins as inputs)

    if(DMA0CTL & DMAIFG)
    {
        // Get data from buffer
        data = HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxRXBUF);
        // Send to NWK for processing
        ret_state = SMBus_NWK_slaveProcessRx(smbus, data);
        DMA0CTL &= ~DMAIFG;     // Clear flag
    }

    return(ret_state);      // Update the state for application processing
}

void SMBus_PHY_slaveEnableManualACK(SMBus *smbus)
{
    // This workaround uses DMA to force the SCL low in order to evaluate the
    // received byte before ACK
    // Port_val has the Value of I2C PSEL1 used to clear SCL functionality
    // Make sure this doesn't affect the rest of the port GPIOs

    // Initialize DMA for single transfers using Bytes, triggered by USCI_RX
    __data16_write_addr((uint16_t)(DMA_BASE + DMA0SAL),
                        (unsigned long) &u8DefaultDMA_State);
    __data16_write_addr((uint16_t)(DMA_BASE + DMA0DAL),
                        (unsigned long) (SMB_SWACK_I2CPORT_BASE +
                                         SMB_SWACK_PxSEL_OFSET));

    HWREG(DMA_BASE + OFS_DMA0SZ) = 1;       // Only 1 byte
    HWREG(DMA_BASE + OFS_DMACTL0) = SWB_SWACK_DMA_SOURCE;
    HWREG(DMA_BASE + OFS_DMA0CTL) = DMADT_4 + DMASRCINCR_0 + DMADSTINCR_0 +
                                    DMADSTBYTE + DMASRCBYTE; // Repeated Single transfer

    HWREG(DMA_BASE + OFS_DMA0CTL) |= DMAEN;     // Enable the DMA

    smbus->ctrl.bits.swackEn = 1;     // Set global flag

    // Re-enable Interrupts in order to enable RX for DMA
    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_slaveEnableInt(smbus);
    }
}

void SMBus_PHY_slaveDisableManualACK(SMBus *smbus)
{
    //Enable USCI RX Interrupts
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIE) |= UCRXIE0;
    HWREG(DMA_BASE + OFS_DMA0CTL) &= ~DMAEN;     // Disable the DMA

    smbus->ctrl.bits.swackEn = 0;     // Clear global flag

    // Re-enable Interrupts in order to enable RX for eUSCI
    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_slaveEnableInt(smbus);
    }
}

void SMBus_PHY_slaveSendACK(SMBus *smbus,
                            bool sendAck)
{
    if(sendAck == true)
    {
        // Return functionality of the SCL pin to I2C
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1SEL1) |= SMB_SWACK_SCL_PIN;
    }
    else
    {
        // NACK is sent manually using GPIO control
        // Force NACK by releasing SDA (goes high)
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1DIR) &= ~SMB_SWACK_SDA_PIN;
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1SEL1) &= ~SMB_SWACK_SDA_PIN;
        // Release SCL
        HWREGB(SMB_SWACK_I2CPORT_BASE + OFS_P1DIR) &= ~SMB_SWACK_SCL_PIN;
        // Restart the I2C bus
        SMBus_PHY_disable(smbus);
        SMBus_PHY_slaveEnable(smbus);
        smbus->nwk.eState = SMBus_NwkState_Idle;
    }
}

#endif  //SMB_MANUAL_ACK_ENABLE

void SMBus_PHY_masterEnable(SMBus *smbus)
{
    // Make sure the PHY is disabled
    SMBus_PHY_disable(smbus);

    // Write the Slave Address
    HWREG(smbus->phy.SMBus_Phy_i2cBase +
          OFS_UCBxI2COA0) = smbus->ownSlaveAddr + UCOAEN;

    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIFG) = 0x00;              // Clear all flags
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) &= ~UCSWRST;      // Start USCI I2C

    // Re-enable Interrupts since the Reset will clear them
    if(smbus->ctrl.bits.intEn == 1)
    {
        SMBus_masterEnableInt(smbus);
    }

    smbus->ctrl.bits.phyEn = 1;      // Set global flag
}

void SMBus_PHY_masterEnableInt(SMBus *smbus)
{
    // Clear all flags
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxIFG) = 0x00;

    // Enable eUSCI Interrupts (Start, Stop, TX, RX, Timeout, NACK, Arbitration Lost)
    HWREG(smbus->phy.SMBus_Phy_i2cBase +
          OFS_UCBxIE) = UCSTTIE | UCSTPIE | UCTXIE0 |
                        UCRXIE0 | UCCLTOIE |
                        UCNACKIE |
                        UCALIE;

    // Set global flag
    smbus->ctrl.bits.intEn = 1;
}

void SMBus_PHY_masterInit(SMBus *smbus,
                          uint16_t i2cAddr,
                          uint32_t busClk)
{
    SMBus_Phy *SMBusPHY = &smbus->phy;
    uint16_t Prescaler;

    // Set the I2C Base Address
    smbus->phy.SMBus_Phy_i2cBase = i2cAddr;

    //Software reset of I2C
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) = UCSWRST;
    // Use SMCLK, I2C mode, sync mode, Multi-Master mode
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |= UCMST | UCMM |
                                                          UCSSEL__SMCLK |
                                                          UCMODE_3 + UCSYNC;
    // Select deglitch = 50ns, SW Acknownedge
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW1) = UCGLIT_0 | UCSWACK;
    // Enable Clock low timeout (150,000 MODCLK cycles = ~31ms)
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxCTLW1) |= UCCLTO_2;

    // Slave Address not initialized by default
    smbus->ownSlaveAddr = 0;

    // PreStop is cleared
    smbus->phy.SMBus_Phy_stop = SMBus_Stop_No;

    //Compute the prescaler. Per SMBus spec, the frequency is always 100Khz
    Prescaler = (uint16_t )(busClk / 100000);
    HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxBRW) = Prescaler;

    // Enable the I2C module
    SMBus_PHY_masterEnable(smbus);
}

void SMBus_PHY_masterSendStop(SMBus *smbus)
{
    // Generate Stop condition if it hasn't been sent
    if(smbus->phy.SMBus_Phy_stop != SMBus_Stop_Sent)
    {
        HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTL1) |= UCTXSTP;
        smbus->phy.SMBus_Phy_stop = SMBus_Stop_Sent;
    }
}

void SMBus_PHY_masterSendPreStop(SMBus *smbus)
{
    smbus->phy.SMBus_Phy_stop = SMBus_Stop_PreRead;
}

void SMBus_PHY_masterStartTx(SMBus *smbus,
                             uint8_t targetaddr,
                             SMBus_Stop stopFlag)
{
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxI2CSA) = targetaddr;

    smbus->phy.SMBus_Phy_stop = SMBus_Stop_No;
    if(stopFlag == SMBus_Stop_Immediate)
    {
        HWREG(smbus->phy.SMBus_Phy_i2cBase +
              OFS_UCBxCTLW0) |= UCMST + UCTR + UCTXSTT + UCTXSTP;
        smbus->phy.SMBus_Phy_stop = SMBus_Stop_Sent;
    }
    else
    {
        HWREG(smbus->phy.SMBus_Phy_i2cBase +
              OFS_UCBxCTLW0) |= UCMST + UCTR + UCTXSTT;
    }
}

void SMBus_PHY_masterStartRx(SMBus *smbus,
                             uint8_t targetaddr,
                             SMBus_Stop stopFlag)
{
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxI2CSA) = targetaddr;
    HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) &= ~UCTR;

    smbus->phy.SMBus_Phy_stop = SMBus_Stop_No;
    if(stopFlag == SMBus_Stop_Immediate)
    {
        HWREG(smbus->phy.SMBus_Phy_i2cBase +
              OFS_UCBxCTLW0) |= UCMST + UCTXSTT + UCTXSTP;
        smbus->phy.SMBus_Phy_stop = SMBus_Stop_Sent;
    }
    else
    {
        HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |= UCMST + UCTXSTT;
    }

    if(stopFlag == SMBus_Stop_Addr)
    {
        // Wait for the Start to be sent
        while(HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) & UCTXSTT)
        {
            ;
        }
        HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTLW0) |= UCTXSTP;
        smbus->phy.SMBus_Phy_stop = SMBus_Stop_Sent;
    }

    if(stopFlag == SMBus_Stop_PreRead)
    {
        smbus->phy.SMBus_Phy_stop = SMBus_Stop_PreRead;
    }
}

SMBus_State SMBus_PHY_masterProcessInt(SMBus *smbus)
{
    uint8_t data;
    SMBus_State ret_state;
    SMBus_Phy *SMBusPHY = &smbus->phy;

    // Check for NACK
    if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCNACKIFG)
    {
        // Pass data to NWK layer
        ret_state = SMBus_NWK_masterProcessNACK(smbus);
        // Clear flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCNACKIFG;
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCALIFG)
    {
        // Arbitration lost
        // Clear start flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCALIFG;
    }     // STT condition
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCSTTIFG)
    {
        // Start is detected when we get a packet as Slave
        // Clear start flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCSTTIFG;
    }     // STT condition
    // Check if there is a RX pending
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCRXIFG0)
    {
        // Send stop before receiving
        if(smbus->phy.SMBus_Phy_stop == SMBus_Stop_PreRead)
        {
            HWREG(smbus->phy.SMBus_Phy_i2cBase + OFS_UCBxCTL1) |= UCTXSTP;
            smbus->phy.SMBus_Phy_stop = SMBus_Stop_Sent;
        }
        // RXIFG0 is set when a byte is received from slave
        data = HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxRXBUF);
        // Pass data to NWK layer
        ret_state = SMBus_NWK_masterProcessRx(smbus, data);
    }
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCTXIFG0)
    {
        // TXIFG0 is set when the TX buffer is empty and we need to send data to
        // slave
        ret_state = SMBus_NWK_masterProcessTx(smbus, &data);
        // Send the data
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxTXBUF) = data;
    }     // TXIFG
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCSTPIFG)
    {
        // Inform the NWK later
        ret_state = SMBus_NWK_masterProcessStop(smbus);
        // Clear flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCSTPIFG;
    }     // STP
    else if(HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) & UCCLTOIFG)
    {
        // This flag is set when a clock timeout is detected
        // Inform the NWK of the error
        ret_state = SMBus_NWK_masterProcessTimeout(smbus);
        // Clear flag
        HWREG(SMBusPHY->SMBus_Phy_i2cBase + OFS_UCBxIFG) &= ~UCCLTOIFG;
    }

    return(ret_state);
}
