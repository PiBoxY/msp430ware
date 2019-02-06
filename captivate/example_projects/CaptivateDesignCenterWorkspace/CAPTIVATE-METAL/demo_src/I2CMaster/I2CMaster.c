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
//! \file   I2CMaster.c
//
//  Group:          MSP
//  Target Devices: MSP430FR2xx/4xx/5xx/6xx
//
//  (C) Copyright 2015, Texas Instruments, Inc.
//#############################################################################
// TI Release: 1.70.00.03
// Release Date: July 30, 2018
//#############################################################################

//*****************************************************************************
// Includes
//*****************************************************************************

#include <driverlib.h>
#include "I2CMaster.h"

#if (I2CMASTER__ENABLE==true)

//*****************************************************************************
// Definitions
//*****************************************************************************

//! \brief A short-cut to the eUSCI_B CTLW0 register.
//!
#define I2CMASTER__CTLW0 HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxCTLW0)

//! \brief A short-cut to the eUSCI_B CTLW1 register.
//!
#define I2CMASTER__CTLW1 HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxCTLW1)

//! \brief A short-cut to the eUSCI_B BRW register.
//!
#define I2CMASTER__BRW   HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxBRW)

//! \brief A short-cut to the eUSCI_B TBCNT register.
//!
#define I2CMASTER__TBCNT HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxTBCNT)

//! \brief A short-cut to the eUSCI_B I2CSA register.
//!
#define I2CMASTER__I2CSA HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxI2CSA)

//! \brief A short-cut to the eUSCI_B STATW register.
//!
#define I2CMASTER__STATW HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxSTATW)

//! \brief A short-cut to the eUSCI_B IE register.
//!
#define I2CMASTER__IE    HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxIE)

//! \brief A short-cut to the eUSCI_B IFG register.
//!
#define I2CMASTER__IFG   HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxIFG)

//! \brief A short-cut to the eUSCI_B TXBUF register.
//!
#define I2CMASTER__TXBUF HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxTXBUF)

//! \brief A short-cut to the eUSCI_B RXBUF register.
//!
#define I2CMASTER__RXBUF HWREG16(I2CMASTER__EUSCI_B_PERIPHERAL + OFS_UCBxRXBUF)

//! \brief A short-cut to clear the reset bit
//!
#define I2CMASTER__CLEAR_RST                     (I2CMASTER__CTLW0 &= ~UCSWRST)

//! \brief A short-cut to set the reset bit
//!
#define I2CMASTER__SET_RST                        (I2CMASTER__CTLW0 |= UCSWRST)

//! \brief A short-cut to set the write mode bit
//!
#define I2CMASTER__WRITEMODE                         (I2CMASTER__CTLW0 |= UCTR)

//! \brief A short-cut to clear the write mode bit
//!
#define I2CMASTER__READMODE                         (I2CMASTER__CTLW0 &= ~UCTR)

//! \brief A short-cut to set the start bit
//!
#define I2CMASTER__START                          (I2CMASTER__CTLW0 |= UCTXSTT)

//! \brief A short-cut to set the start bit
//!
#define I2CMASTER__RESTART                                    (I2CMASTER_START)

//! \brief A short-cut to set the stop bit
//!
#define I2CMASTER__STOP                           (I2CMASTER__CTLW0 |= UCTXSTP)

//! \brief Defines the interrupt vector location of the selected
//!        eUSCI_B I2C peripheral.
//!
#if (I2CMASTER__EUSCI_B_PERIPHERAL == EUSCI_B0_BASE)
#define I2CMASTER__EUSCI_VECTOR     	(USCI_B0_VECTOR)
#else
#error INVALID_EUSCI_B_SELECTION
#endif

//! \brief Defines the interrupt vector of the selected
//!        eUSCI_B I2C peripheral.
//!
#if (I2CMASTER__EUSCI_B_PERIPHERAL == EUSCI_B0_BASE)
#define I2CMASTER__EUSCI_IV    	 	    (UCB0IV)
#else
#error INVALID_EUSCI_B_SELECTION
#endif

//*****************************************************************************
// Type Definitions
//*****************************************************************************

//! \brief This enumeration defines he states that the I2C master driver may be 
//!        in during operation.
//!
enum
{
	I2CMaster_isClosed = 0,
	I2CMaster_isIdle,
	I2CMaster_isCommunicating
};
typedef uint8_t tI2CMaster_state;

//! \brief This structure contains the status flags that are used by the driver.
//!
typedef union
{
	uint8_t ui8AllFlags;
	struct
	{
		uint8_t ui8ErrorFlags : 2;
		uint8_t ui8StatusFlags : 2;
	};
	struct
	{
		uint8_t bSlaveNack : 1;
		uint8_t bClockLowTimeout : 1;
		uint8_t bStop : 1;
		uint8_t bStart : 1;
	};
} tI2CMaster_flags;

//! \brief This structure contains all of the variables needed by an instance
//! of the driver.
//!
typedef struct
{
	tI2CMaster_state state;
	tI2CMaster_flags flags;
	uint8_t *pData;
} tI2CMaster_driver;

//*****************************************************************************
// Global Variables
//*****************************************************************************

//! \brief This is the instance of the I2C Master driver.
//!
static volatile tI2CMaster_driver g_driver =
{
	.state = I2CMaster_isClosed
};

//*****************************************************************************
// Local Function Prototypes
//*****************************************************************************

//! \brief Checks for any error flags and returns the appropriate error code.
//! \return the error code corresponding to any error flags that are set.
static tI2CMaster_error I2CMaster_checkForErrors(void);

//! \brief Prepare the peripheral for an I2C bus transaction.
//! \param ui8Addr is the address of the slave device
//! \param pData is a pointer to the data buffer space to used
//! \param ui8Cnt is the size of the data transaction
static inline void I2CMaster_prep(uint8_t ui8Addr, uint8_t *pData,
		uint8_t ui8Cnt);

//! \brief Clean-up the peripheral after an I2C bus transaction.
static inline void I2CMaster_cleanup(void);

//! \brief Finish a I2C bus transaction.
static inline tI2CMaster_error I2CMaster_finish(void);

//*****************************************************************************
// Function Implementations
//*****************************************************************************

void I2CMaster_open(void)
{
	I2CMASTER__SET_RST;
	I2CMASTER__CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK;
	I2CMASTER__CTLW1 = UCASTP_2 | UCCLTO_2;
	I2CMASTER__BRW = I2CMASTER__PRESCALER;
	g_driver.state = I2CMaster_isIdle;
}

void I2CMaster_close(void)
{
	I2CMASTER__SET_RST;
	g_driver.state = I2CMaster_isClosed;
}

tI2CMaster_error I2CMaster_checkForErrors(void)
{
	if (g_driver.flags.bClockLowTimeout == true)
	{
		return eI2CMaster_busTimeoutError;
	}
	else if (g_driver.flags.bSlaveNack == true)
	{
		return eI2CMaster_slaveNackError;
	}
	else
	{
		return eI2CMaster_noError;
	}
}

inline void I2CMaster_prep(uint8_t ui8Addr, uint8_t *pData,
		uint8_t ui8Cnt)
{
	I2CMASTER__SET_RST;
	g_driver.state = I2CMaster_isCommunicating;
	g_driver.flags.ui8AllFlags = 0;
	g_driver.pData = pData;
	I2CMASTER__I2CSA = ui8Addr;
	I2CMASTER__TBCNT = ui8Cnt;
	I2CMASTER__CLEAR_RST;
	I2CMASTER__IE = UCNACKIE | UCCLTOIE | UCSTPIFG;
}

inline void I2CMaster_cleanup(void)
{
	I2CMASTER__IE = 0;
	g_driver.state = I2CMaster_isIdle;
}

inline tI2CMaster_error I2CMaster_finish(void)
{
	while (g_driver.flags.bStop == false)
	{
		if (g_driver.flags.ui8ErrorFlags != 0)
		{
			I2CMaster_cleanup();
			return I2CMaster_checkForErrors();
		}
	}
	I2CMaster_cleanup();
	return I2CMaster_checkForErrors();
}

tI2CMaster_error I2CMaster_writeBuffer(uint8_t ui8Addr, uint8_t *pData,
		uint8_t ui8Cnt)
{
	if (g_driver.state != I2CMaster_isIdle)
	{
		return eI2CMaster_unavailableError;
	}
	I2CMaster_prep(ui8Addr, pData, ui8Cnt);
	I2CMASTER__WRITEMODE;
	I2CMASTER__START;
	while (!(I2CMASTER__IFG & UCTXIFG))
	{
		if (g_driver.flags.ui8ErrorFlags != 0)
		{
			I2CMaster_cleanup();
			return I2CMaster_checkForErrors();
		}
	}
	I2CMASTER__IE |= UCTXIE;
	return I2CMaster_finish();
}

tI2CMaster_error I2CMaster_readBuffer(uint8_t ui8Addr, uint8_t *pData,
		uint8_t ui8Cnt)
{
	if (g_driver.state != I2CMaster_isIdle)
	{
		return eI2CMaster_unavailableError;
	}
	I2CMaster_prep(ui8Addr, pData, ui8Cnt);
	I2CMASTER__READMODE;
	I2CMASTER__START;
	I2CMASTER__IE |= UCRXIE;
	return I2CMaster_finish();
}

tI2CMaster_error I2CMaster_writeReg8(uint8_t ui8Addr, uint8_t ui8Reg,
		uint8_t *pData, uint8_t ui8Cnt)
{
	tI2CMaster_error error;

	if (g_driver.state != I2CMaster_isIdle)
	{
		return eI2CMaster_unavailableError;
	}
	I2CMaster_prep(ui8Addr, pData, ui8Cnt+1);
	I2CMASTER__WRITEMODE;
	I2CMASTER__START;
	while (!(I2CMASTER__IFG & UCTXIFG))
	{
		if (g_driver.flags.ui8ErrorFlags != 0)
		{
			I2CMaster_cleanup();
			return I2CMaster_checkForErrors();
		}
	}
	I2CMASTER__TXBUF = ui8Reg;
	I2CMASTER__IE |= UCTXIE;
	error = I2CMaster_finish();
	return error;
}

tI2CMaster_error I2CMaster_readReg8(uint8_t ui8Addr, uint8_t ui8Reg,
		uint8_t *pData, uint8_t ui8Cnt)
{
	tI2CMaster_error error;

	if (g_driver.state != I2CMaster_isIdle)
	{
		return eI2CMaster_unavailableError;
	}
	I2CMaster_prep(ui8Addr, pData, ui8Cnt);

	// Start communication by sending a start condition
	// and the slave bus address, and ensuring the slave
	// does not nack.
	I2CMASTER__WRITEMODE;
	I2CMASTER__START;
	while (!(I2CMASTER__IFG & UCTXIFG))
	{
		if (g_driver.flags.ui8ErrorFlags != 0)
		{
			I2CMaster_cleanup();
			return I2CMaster_checkForErrors();
		}
	}

	// Transmit the slave sub-register to read from
	I2CMASTER__TXBUF = ui8Reg;
	while (!(I2CMASTER__IFG & UCTXIFG))
	{
		// Handle the i8Cnt==1 case, where the stop bit is sent prematurely
		if (g_driver.flags.bStop == true)
		{
			break;
		}
		if (g_driver.flags.ui8ErrorFlags != 0)
		{
			I2CMaster_cleanup();
			return I2CMaster_checkForErrors();
		}
	}

	// Read out the data from the slave
	g_driver.flags.ui8StatusFlags = 0;
	I2CMASTER__READMODE;
	I2CMASTER__START;
	I2CMASTER__IE |= UCRXIE;
	error = I2CMaster_finish();

	return error;
}

#pragma vector=I2CMASTER__EUSCI_VECTOR
__interrupt void I2CMaster_ISR(void)
{
	switch(__even_in_range(I2CMASTER__EUSCI_IV, USCI_I2C_UCBIT9IFG))
	{
		case USCI_I2C_UCALIFG:  break;
		case USCI_I2C_UCNACKIFG:
			I2CMASTER__STOP;
			g_driver.flags.bSlaveNack = true;
			break;
		case USCI_I2C_UCSTTIFG:
			g_driver.flags.bStart = true;
			break;
		case USCI_I2C_UCSTPIFG:
			g_driver.flags.bStop = true;
			break;
		case USCI_I2C_UCRXIFG0:
			*g_driver.pData++ = I2CMASTER__RXBUF;
			break;
		case USCI_I2C_UCTXIFG0:
			I2CMASTER__TXBUF = *g_driver.pData++;
			break;
		case USCI_I2C_UCBCNTIFG: break;
		case USCI_I2C_UCCLTOIFG:
			g_driver.flags.bClockLowTimeout = true;
			break;
		case USCI_I2C_UCBIT9IFG: break;
		default: break;
	}
	__bic_SR_register_on_exit(LPM3_bits);
}

#endif /* I2CMASTER__ENABLE==true */
