/*
 * MSP430_FET.h
 *
 * API for maintaing MSP-FET430UIF (TI USB FET) hardware and firmware.
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
 \file MSP430_FET.h

 \brief       This file contains the DLL function headers and definitions
              to maintain the MSP-FET430UIF (TI USB FET) hardware and firmware.

 \par         Project:
              MSP-FET430UIF (TI USB FET) Maintenance API

 \par         Developed using:
              MS Visual C++ 2003/2010

 \par         Supported API calls:
              - MSP430_FET_SelfTest()
              - MSP430_FET_SetSignals()
              - MSP430_FET_Reset()
              - MSP430_FET_I2C()
              - MSP430_FET_EnterBootloader()
              - MSP430_FET_ExitBootloader()
              - MSP430_FET_GetFwVersion()
              - MSP430_FET_FwUpdate()
              - MSP430_FET_GetHwVersion()
*/

#ifndef MSP430_FET_H
#define MSP430_FET_H

#include "MSP430.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef MSP430_FET_TYPES
#define MSP430_FET_TYPES

/**
\brief   Type definition for a callback function which could handle notify
         messages sent by the DLL to the calling application during USB FET
		 firmware update.
		 A handle to the callback function is passed to the DLL by calling
		 MSP430_FET_FwUpdate().
		 See enumeration UPDATE_STATUS_MESSAGES for details on possible
		 notify messages.
*/
typedef void (* DLL430_FET_NOTIFY_FUNC) (uint32_t MsgId,
                                         uint32_t wParam,
                                         uint32_t lParam,
                                         int32_t clientHandle);

/// Status messages during USB FET firmware update
typedef enum UPDATE_STATUS_MESSAGES {
	BL_INIT = 0, /**< Initializing Update Bootloader */
	BL_ERASE_INT_VECTORS = 1, /**< Erasing mapped interrupt vectors */
	BL_ERASE_FIRMWARE = 2, /**< Erasing firmware memory section */
	BL_PROGRAM_FIRMWARE = 3, /**< Program new firmware */
	BL_DATA_BLOCK_PROGRAMMED = 4, /**< One data block of the new firmware was successfully programmed */
	BL_EXIT = 5, /**< Exit Update Bootlader and reboot firmware */
	BL_UPDATE_DONE = 6, /**< Update was successfully finished */
	BL_UPDATE_ERROR = 7, /**< An error occured during firmware update */
	BL_WAIT_FOR_TIMEOUT = 8 /**< An error occured during firmware update */
} UPDATE_STATUS_MESSAGES_t;

#endif /* MSP430_FET_TYPES */

/**
\fn   STATUS_T MSP430_FET_SelfTest(int32_t count, uint8_t* buffer);

\brief   Call the hardware self test function of the USB JTAG adaptor.

\note    1. MSP430_Initialize() must have been called prior to calling this function.

\param   count:  size of the buffer in bytes.
\param   buffer: pointer to a memory buffer of at least count bytes.

\return  STATUS_OK:    Hardware self test was executed successfully.
\n       STATUS_ERROR: Hardware self test was not executed successfully.

\par     Error codes:
         COMM_ERR
\n       SELFTEST_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_SelfTest(int32_t count, uint8_t* buffer);

/**
\fn   STATUS_T MSP430_FET_SetSignals(int32_t SigMask, int32_t SigState);

\brief   This function is only meant for testing.

\note	 1. Not supported by MSP430.DLL v3

\param   SigMask:  mask of which signal to set/reset.
\param   SigState: mask of the corresponding signal states.

\return  STATUS_OK:    Signals were set/reset.
\n       STATUS_ERROR: Signals were not set/reset.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_SetSignals(int32_t SigMask, int32_t SigState);

/**
\fn   STATUS_T MSP430_FET_Reset(void);

\brief   Perform a reset of the USB JTAG adaptor's firmware.

\note	 1. Not supported by MSP430.DLL v3
\note    2. MSP430_Initialize() must have been called prior to calling this function.

\return  STATUS_OK:    Firmware reset was performed.
\n       STATUS_ERROR: Firmware reset was not performed.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_Reset(void);

/**
\fn   STATUS_T MSP430_FET_I2C(int32_t address, uint8_t* buffer, int32_t count, int32_t rw);

\brief   Read and write to EEPROM via I2C.

\note	 1. Not supported by MSP430.DLL v3
\note    2. MSP430_Initialize() must have been called prior to calling this function.

\param   address: The starting address of the EEPROM memory to be read or written.
\param   buffer:  The buffer into which EEPROM memory is read, or from which EEPROM memory is written.
\param   count:   The number of bytes of EEPROM memory read or written.
\param   rw:      Specify a read (READ) or write (WRITE) operation.

\return  STATUS_OK:    The EEPROM operation encountered no errors.
\n       STATUS_ERROR: The EEPROM operation encountered errors.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_I2C(int32_t address, uint8_t* buffer, int32_t count, int32_t rw);

#define MSP430_FET_Read_I2C(ADDRESS, BUFFER, COUNT) MSP430_FET_I2C(ADDRESS, BUFFER, COUNT, READ)
#define MSP430_FET_Write_I2C(ADDRESS, BUFFER, COUNT) MSP430_FET_I2C(ADDRESS, BUFFER, COUNT, WRITE)

/**
\fn   STATUS_T MSP430_FET_EnterBootloader(void);

\brief   Enter the bootloader.

\note	 1. Not supported by MSP430.DLL v3
\note    2. MSP430_Initialize() must have been called prior to calling this function.

\return  STATUS_OK:    The bootloader is active.
\n       STATUS_ERROR: The bootloader could not be entered.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_EnterBootloader(void);


/**
\fn   STATUS_T MSP430_FET_ExitBootloader(void);

\brief   Exit the bootloader.

\note	 1. Not supported by MSP430.DLL v3
\note    2. MSP430_Initialize() must have been called prior to calling this function.

\return  STATUS_OK:    The firmware is active.
\n       STATUS_ERROR: Could not exit the bootloader.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_ExitBootloader(void);

/**
\fn   STATUS_T MSP430_FET_GetFwVersion(int32_t* version);

\brief   Returns the version number of the MSP430-FET430UIF USB FET firmware.

\note    1. MSP430_Initialize() must have been called prior to calling this function.

\param   version: The version number of the MSP430-FET430UIF firmware is returned.

\return  STATUS_OK:    Firmware version was returned.
\n       STATUS_ERROR: Firmware version was NOT returned.

\par     Error codes:
         COMM_ERR
\n       INTERFACE_SUPPORT_ERR
\n		 INTERNAL_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_GetFwVersion(int32_t* version);

/**
\fn   STATUS_T WINAPI MSP430_FET_GetHwVersion(uint8_t** version, int32_t* count);

\brief   Returns the version number of the MSP430-FET430UIF USB FET hardware.

\note    1. MSP430_Initialize() must have been called prior to calling this function.

\param   version: Pointer to version number buffer of the MSP430-FET430UIF is returned.
\param   count:   Number of valid bytes in the version number buffer is returned.

\code    Desription of the Dll's internal hardware version buffer.
         The hardware revision of the MSP430-FET430UIF is retrieved internally
         by calling MSP430_Initialize(). MSP430 DLL stores the hardware revision
         number in a byte array. On calling MSP430_FET_GetHwVersion() a pointer
         to the internal buffer as well as the number of valid bytes in the buffer
         are returned.

         Current buffer implementation:
         [0] // designator letter: 'U' for USB FET, 'E' for eZ430 demo tool
         [1] // unused
         [2] // minor version number
         [3] // major version number
\endcode

\return  STATUS_OK:    Hardware version was returned.
\n       STATUS_ERROR: Hardware version was NOT returned.

\par     Error codes:
         COMM_ERR
\n		 PARAMETER_ERR
\n		 INTERNAL_ERR
\n       INTERFACE_SUPPORT_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_GetHwVersion(uint8_t** version, int32_t* count);

/**
\fn   STATUS_T MSP430_FET_FwUpdate(const char* lpszFileName,
					DLL430_FET_NOTIFY_FUNC callback,
					int32_t clientHandle);

\brief   Synchronizes the firmware version of the MSP-FET430UIF (TI USB FET)
         with the version of the Dll.

\note    1. MSP430_Initialize() must have been called prior to calling this function.
\note	 2. When calling MSP430_FET_FwUpdate() in response the MSP430_Initialize()
			returning a version of -3, the file CDC.log must exist
			in the directory of the executed binary and the content must be the string "True"
			without a newline. This file signals that a CDC driver is installed and prevents
			the update from making the MSP-FET430UIF unusable.
\note    3. The actual firmware update of the MSP-FET430UIF might require a long time
			to finish. Therefore a callback mechanism was implemented to report
            back status information to the calling application during update operation.
\note    4. Please have a look at the provided example application (UifUpdate) on how
            to apply this function!!!

\param   lpszFileName: Pointer to a file of TI TXT or INTEL HEX format.
                       It is recommended to call MSP430_FET_FwUpdate() with paramter
                       lpszFileName set to NULL. In that case an internally stored firmware
                       image will be used by MSP430.dll to perform the update. By following
                       this recommondation you make sure that the MSP-FET430UIF interface gets
                       updated with the correct firmware matching version number and features
                       of the currently used MSP430.dll Library.
\param   callback:     Pointer to a callback function of the type DLL430_FET_NOTIFY_FUNC
                       that might handle notify messages send by the DLL during firmware
                       update. This parameter can be set to NULL if one does not want to use
                       the callback mechanism.
\param   clientHandle: Handle of the calling application. Can be set to FALSE in case the
                       callback mechanism is not applied and parameter callback is set to NULL.

\return  STATUS_OK:    Firmware update was done successfully.
\n       STATUS_ERROR: Errors occured during firmware update.

\par     Error codes:
         COMM_ERR
*/
DLL430_SYMBOL STATUS_T WINAPI MSP430_FET_FwUpdate(const char* lpszFileName,
                                           DLL430_FET_NOTIFY_FUNC callback,
                                           int32_t clientHandle);

#if defined(__cplusplus)
}
#endif

#endif // MSP430_FET_H
