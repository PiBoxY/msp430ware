/*
 * MSP430_HIL.h
 *
 * API for low level HIL access.
 *
 * Copyright (c) 2004 - 2016 Texas Instruments Incorporated - http://www.ti.com/
 *
 * All rights reserved not granted herein.
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free,
 * non-exclusive license under copyrights and patents it now or hereafter
 * owns or controls to make, have made, use, import, offer to sell and sell ("Utilize")
 * this software subject to the terms herein.  With respect to the foregoing patent
 * license, such license is granted  solely to the extent that any such patent is necessary
 * to Utilize the software alone.  The patent license shall not apply to any combinations which
 * include this software, other than combinations with devices manufactured by or for TI (“TI Devices”).
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license (including the
 * above copyright notice and the disclaimer and (if applicable) source code license limitations below)
 * in the documentation and/or other materials provided with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided that the following
 * conditions are met:
 *
 *	* No reverse engineering, decompilation, or disassembly of this software is permitted with respect to any
 *     software provided in binary form.
 *	* any redistribution and use are licensed by TI for use only with TI Devices.
 *	* Nothing shall obligate TI to provide you with source code for the software licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the source code are permitted
 * provided that the following conditions are met:
 *
 *   * any redistribution and use of the source code, including any resulting derivative works, are licensed by
 *     TI for use only with TI Devices.
 *   * any redistribution and use of any object code compiled from the source code and any resulting derivative
 *     works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers may be used to endorse or
 * promote products derived from this software without specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI’S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
\file MSP430_HIL.h

 \brief       This file contains the DLL function headers and definitions
              for low level HIL access.

 \par         Project:
              MSP-FET430UIF (TI USB FET) HIL API

 \par         Developed using:
              MS Visual C++ 2003/2010

 \par         Supported API calls:
              - MSP430_HIL_Open()
			  - MSP430_HIL_Configure();
			  - MSP430_HIL_Connect()
			  -	MSP430_HIL_Bsl()
			  -	MSP430_HIL_Close()

			  -	MSP430_HIL_JTAG_IR()
			  -	MSP430_HIL_JTAG_DR()
			  -	MSP430_HIL_TST()
			  -	MSP430_HIL_TCK()
			  -	MSP430_HIL_TMS()
			  -	MSP430_HIL_TDI()
			  -	MSP430_HIL_RST()
			  -	MSP430_HIL_ResetJtagTap()
*/


#ifndef MSP430_HIL_H
#define MSP430_HIL_H

#include "MSP430.h"

#if defined(__cplusplus)
extern "C" {
#endif

/// Configurations values for CONFIG_MODE INTERFACE_MODE
typedef enum HIL_ENTRY_STATE {
	HIL_ENTRY_RSTHIGH = 0, /**< RST high during entry sequence*/
	HIL_ENTRY_RSTLOW = 1, /**< RST low during entry sequence*/
} IHIL_ENTRY_STATE_t;


// Functions. -----------------------------------------------------------------
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Open(void);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Configure(int32_t mode, int32_t value);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Connect();
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Connect_Entry_State(int32_t value);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Bsl(void);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_Close(int32_t vccOff);

DLL430_SYMBOL int32_t WINAPI MSP430_HIL_JTAG_IR(int32_t instruction);
DLL430_SYMBOL int32_t WINAPI MSP430_HIL_JTAG_IR4(int32_t instruction);
DLL430_SYMBOL int32_t WINAPI MSP430_HIL_JTAG_DR(int32_t data, int32_t bits);
DLL430_SYMBOL int64_t WINAPI MSP430_HIL_JTAG_DRX(int64_t data, int32_t bits);
DLL430_SYMBOL int64_t WINAPI MSP430_HIL_JTAG_IR_DRX(int32_t instruction, int64_t data, int32_t bits);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_TST(int32_t state);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_TCK(int32_t state);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_TMS(int32_t state);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_TDI(int32_t state);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_RST(int32_t state);
DLL430_SYMBOL void WINAPI MSP430_HIL_ResetJtagTap(void);
DLL430_SYMBOL void WINAPI MSP430_HIL_FuseCheck(void);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_DPACC(uint8_t address, uint32_t *data, uint16_t RdnWr);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_APACC(uint8_t portNum, uint8_t address, uint32_t *data, uint16_t RdnWr);
DLL430_SYMBOL STATUS_T WINAPI MSP430_HIL_MEMAP(uint8_t portNum, uint32_t address, uint32_t *data, uint16_t RdnWr);

#if defined(__cplusplus)
}
#endif

#endif // MSP430_HIL_H
