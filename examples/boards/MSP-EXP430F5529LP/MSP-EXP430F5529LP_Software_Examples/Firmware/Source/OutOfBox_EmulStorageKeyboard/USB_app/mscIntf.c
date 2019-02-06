/* --COPYRIGHT--,BSD
 * Copyright (c) 2012, Texas Instruments Incorporated
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

// USB API includes
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_config/descriptors.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"


// Application includes
#include "USB_app/mscIntf.h"
#include "storageVolume.h"     // Holds the emulated storage volume
#include "USB_app/FatFs/mmc.h" // FatFs low-level volume access functions
#include <string.h>


/******* Globals for Mass Storage Interface **********/

// In FAT, this is 512.
const WORD BYTES_PER_BLOCK = 512;


// Data-exchange buffer between the API and the application.
// The application allocates it, and then registers it with the API.
__no_init BYTE RW_dataBuf[512];


// The API allocates an instance of this structure to hold all information
// describing buffers to be processed.  It's a shared resource between the API
// and application.  The API allocates it, and we'll call USBMSC_fetchInfoStruct()
// to obtain the pointer.
USBMSC_RWbuf_Info *RWbuf_info;


// The application must tell the API about the media.  This information is
// conveyed in a call to USBMSC_updateMediaInfo(), passing an instance of
// USBMSC_mediaInfoStr.
struct USBMSC_mediaInfoStr mediaInfo;



void initMscIntf (void)
{
    // Obtain the pointer
    RWbuf_info = USBMSC_fetchInfoStruct();

    // The application must tell the API about the media.  This must be done at
    // init; and also, if the media were removable, like a memory card, this
    // would need to be done anytime the media was changed.  In our case the
    // media is in internal flash and will always be present.
    mediaInfo.mediaPresent = 0x01;   // Currently (and always) present
    mediaInfo.mediaChanged = 0x00;   // Hasn't changed (now or ever)
    mediaInfo.writeProtected = 0x00;
    mediaInfo.lastBlockLba = NUMBER_OF_SECTORS_IN_FLASH;
    mediaInfo.bytesPerBlock = BYTES_PER_BLOCK;
    USBMSC_updateMediaInfo(0, &mediaInfo);

    // Register the data interchange buf.  Registering allows it to be assigned
    // dynamically, giving the app more control over memory management.
    USBMSC_registerBufInfo(0, &RW_dataBuf[0], NULL, sizeof(RW_dataBuf));
}


// After a call to USBMSC_poll(), this function checks RWbuf_info->operation
// to see if the API is waiting on the application to process
// READ/WRITE operations from the host.  If so, it accesses the media.
void mscProcessBuffer(void)
{
    while (RWbuf_info->operation == kUSBMSC_READ)
    {
    	// Use the FatFs library to fetch data from the storage volume
        // and put it into the buffer.
        RWbuf_info->returnCode = disk_read(0,
            RWbuf_info->bufferAddr,
            RWbuf_info->lba,
            RWbuf_info->lbCount);

        // Tell the API the operation is finished
        USBMSC_bufferProcessed();
    }

    while (RWbuf_info->operation == kUSBMSC_WRITE)
    {
    	// Use the FatFs library to write the contents of the buffer
        // to the storage volume.
        RWbuf_info->returnCode = disk_write(0,
            RWbuf_info->bufferAddr,
            RWbuf_info->lba,
            RWbuf_info->lbCount);

        // Tell the API the operation is finished
        USBMSC_bufferProcessed();
    }
}


