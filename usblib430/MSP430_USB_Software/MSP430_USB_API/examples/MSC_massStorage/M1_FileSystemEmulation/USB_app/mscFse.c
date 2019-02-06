/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 * ======== mscFse.c ========
 *
 *
 */
#include <string.h>
#include <stdint.h>
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"
#include "USB_config/descriptors.h"

#include "FatFs/diskio.h"
#include "FatFs/mmc.h"
#include "USB_app/mscFse.h"
#include "storageVolume.h"     // Holds the emulated storage volume

/*
 * Every application using this MSC API must define an instance of this
 * structure.  It establishes compile-time information about the storage media.
 */
extern struct config_struct USBMSC_config;

/******* Globals for Mass Storage Interface **********/
//The number of bytes per block.  In FAT, this is 512
const uint16_t BYTES_PER_BLOCK = 512;


//Data-exchange buffer between the API and the application.
//The application allocates it, and then registers it with the API.
//Later, the API will pass it back to the application when it
//needs the application to "process" it (exchange data with the media).
// In this case each cluster has one block so this buffer is 512 bytes.
__no_init uint8_t RW_dataBuf[512];


//The API allocates an instance of structure type USBMSC_RWbuf_Info to hold all
//information describing buffers to be processed.  The structure instance is a
//shared resource between the API and application.
//During initialization, we'll call USBMSC_fetchInformationStructure() to obtain the
//pointer from the API.
USBMSC_RWbuf_Info *RWbuf_info;

//The application must tell the API about the media.  This information is
//conveyed in a call to USBMSC_updateMediaInformation(), passing an instance of
//USBMSC_RWbuf_Info.
struct USBMSC_mediaInfoStr mediaInfo;

/*
 * This function initializes the MSC data variables
 */
void USBMSC_initFSE (void)
{
    //The API maintains an instance of the USBMSC_RWbuf_Info structure.This is
    // a shared resource between the API and application; the application must
    //request the pointer.
    RWbuf_info = USBMSC_fetchInformationStructure();


    //The application must tell the API about the media.  Since the media isn't
    //removable, this is only called once, at the beginning of execution.
    //If the media were removable, the application must call this any time
    //the status of the media changes.
    mediaInfo.mediaPresent = 0x01; // internal flash is non-removable.
    mediaInfo.mediaChanged = 0x00; //It can't change, it's in internal memory.
    mediaInfo.writeProtected = 0x00; //It's not write-protected
    // 84 blocks in the volume. (This number is also found twice in the volume
    // itself; see mscFseData.c. They should match.)
    mediaInfo.lastBlockLba = NUMBER_OF_SECTORS_IN_FLASH;
    //512 bytes per block. (This number is also found in the
    //volume itself; see mscFseData.c. They should match.)
    mediaInfo.bytesPerBlock = BYTES_PER_BLOCK;
    USBMSC_updateMediaInformation(0, &mediaInfo);

    //The data interchange buffer (used when handling SCSI READ/WRITE) is
    //declared by the application, and registered with the API using this
    //function.  This allows it to be assigned dynamically, giving
    //the application more control over memory management.
    USBMSC_registerBufferInformation(0, &RW_dataBuf[0], NULL, sizeof(RW_dataBuf));
}

void USBMSC_processFSEBuffer(void)
{
	/************* Mass Storage portion ************************/
	//Call USBMSC_pollCommand() to initiate handling of any received
	//SCSI commands.  Disable interrupts
	//during this function, to avoid conflicts arising from SCSI
	//commands being received from the host
	//AFTER decision to enter LPM is made, but BEFORE it's
	//actually entered (in other words, avoid
	//sleeping accidentally).
    __disable_interrupt();
    if (USBMSC_pollCommand() == USBMSC_OK_TO_SLEEP){
        __bis_SR_register(LPM0_bits + GIE);
    }
    __enable_interrupt();

	//If the API needs the application to process a buffer, it
	//will keep the CPU awake by returning USBMSC_PROCESS_BUFFER
	//from USBMSC_pollCommand().  The application should then check the
	//'operation' field of all defined USBMSC_RWbuf_Info
	//structure instances.  If any of them is non-null, then an
	//operation needs to be processed.  A value of
	//USBMSC_READ indicates the API is waiting for the
	//application to fetch data from the storage volume, in
	//response
	//to a SCSI READ command from the USB host.  After the
	//application does this, it must indicate whether the
	//operation succeeded, and then close the buffer operation
	//by calling USBMSC_processBuffer().
    while (RWbuf_info->operation == USBMSC_READ)
    {
        RWbuf_info->returnCode = disk_read(0,
            RWbuf_info->bufferAddr,
            RWbuf_info->lba,
            RWbuf_info->lbCount);  //Fetch a block from the medium
        USBMSC_processBuffer();  //Close the buffer operation
    }

	//Same as above, except for WRITE.  If operation ==
	//USBMSC_WRITE, then the API is waiting for us to
	//process the buffer by writing the contents to the storage
	//volume.
    while (RWbuf_info->operation == USBMSC_WRITE)
    {
        RWbuf_info->returnCode = disk_write(0,
            RWbuf_info->bufferAddr,
            RWbuf_info->lba,
            RWbuf_info->lbCount);  //Write the block to the medium
        USBMSC_processBuffer();  //Close the buffer operation
    }
}


//Released_Version_5_20_06_02
