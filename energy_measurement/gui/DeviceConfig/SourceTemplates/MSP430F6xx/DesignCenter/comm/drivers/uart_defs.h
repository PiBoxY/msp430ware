/* --COPYRIGHT--,BSD
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * --/COPYRIGHT--*/
//*****************************************************************************
// uart_defs.h
//
//! This is the UART port driver definitions file.  This file sets
//! the configuration parameters for the UART port.
//
//! \author MSP Microcontroller Products
//
// TI Release: 0.80.00.31
// Release Date: Tue Aug 16 23:02:31 GMT-1 2016
//
//*****************************************************************************
//*****************************************************************************
//! \addtogroup UART
//! @{
//*****************************************************************************

#ifndef UART_DEFINITIONS_H_
#define UART_DEFINITIONS_H_

#include "driverlib.h"
#include "EM_userConfig.h"

//
//! CAPT_CommConfig is used to control UART__ENABLE.
//
#include "comm_config.h"

//*****************************************************************************
//
//! def UART__ENABLE controls whether the UART port is included
//! during build.  To use the UART port, set this definition to true.
//
//*****************************************************************************
// NOTE: Rather than enabling UART here, the enable is controlled in
// CAPT_CommConfig.
// #define UART__ENABLE				                                     (true)

//*****************************************************************************
//
//! def UART__EUSCI_A_PERIPHERAL defines the MSP430 base address of the
//! eUSCI_A instance being used with this UART port.
//
//*****************************************************************************
#define UART__EUSCI_A_PERIPHERAL	                            (EUSCI_A0_BASE)

//*****************************************************************************
//
//! def I2CSLAVE__LPMx_bits defines the low power mode to enter
//! when pending on a resource.
//
//*****************************************************************************
#define UART__LPMx_bits				                                (LPM0_bits)

//*****************************************************************************
//
//! def UART__SAMPLING_MODE defines the eUSCI_A LF or HF mode.
//
//! def UART__PRESCALER defines the eUSCI_A pre-scaler.
//
//! def UART__FIRST_STAGE_MOD defines the eUSCI_A first stage modulation.
//
//! def UART__SECOND_STAGE_MOD defines the eUSCI_A second stage modulation.
//
//*****************************************************************************
#define UART__SAMPLING_MODE	   (EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION)


#if (EM_SMCLK_FREQ_IN_HZ == 25165824)

//250K Baud Rate @ 25MHz	
#define UART__PRESCALER                                                  (0x06)
#define UART__FIRST_STAGE_MOD                                            (0x03)
#define UART__SECOND_STAGE_MOD                                           (0x02)

#elif (EM_SMCLK_FREQ_IN_HZ == 16777216)

//250K Baud Rate @ 16MHz	
#define UART__PRESCALER                                                  (0x04)
#define UART__FIRST_STAGE_MOD                                            (0x03)
#define UART__SECOND_STAGE_MOD                                           (0x00)
	
#elif (EM_SMCLK_FREQ_IN_HZ == 8388608)

//250K Baud Rate @ 8MHz	
#define UART__PRESCALER                                                  (0x02)
#define UART__FIRST_STAGE_MOD                                            (0x01)
#define UART__SECOND_STAGE_MOD                                           (0x02)
	
#else

#error "SMCLK frequency not supported by the UART driver"

#endif


//*****************************************************************************
//! Close the doxygen group
//! @}
//*****************************************************************************

#endif /* UART_DEFINITIONS_H_ */
