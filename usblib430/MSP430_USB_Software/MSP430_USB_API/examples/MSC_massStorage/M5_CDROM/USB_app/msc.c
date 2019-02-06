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
 * ======== msc.c ========
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
#include "FatFs/ff.h"
#include "USB_app/msc.h"
const uint16_t BYTES_PER_BLOCK = 512;

/*
 * Every application using this MSC API must define an instance of this
 * structure.  It establishes compile-time information about the storage media.
 */
extern struct config_struct USBMSC_config;


//Data-exchange buffer between the API and the application.
//The application allocates it, and then registers it with the API.
//Later, the API will pass it back to the application when it
//needs the application to "process" it (exchange data with the media).
// In this case each cluster has one block so this buffer is 512 bytes.
uint8_t RWbuf[512];


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

//Global flag by which the timer ISR will trigger main() to check the
//media status
uint8_t bDetectCard = 0x00;
FATFS fs;            // File System Handle
FIL isoFile;         // ISO File Handle
FRESULT fres;        // FatFS result
uint16_t br;             // Bytes Read

/*
 * This function initializes the MSC data variables
 */
void USBMSC_initMSC(void)
{
    //SD-cards must go through a setup sequence after powerup.
    //This FatFs call does this.
    disk_initialize(0);

    //The API maintains an instance of the USBMSC_RWbuf_Info structure.This is
    // a shared resource between the API and application; the application must
    //request the pointer.

    RWbuf_info = USBMSC_fetchInformationStructure();


    //USBMSC_updateMediaInformation() must be called prior to USB connection.  We
    //check if the card is present, and if so, pull its size and bytes per
    //block.
    if (detectCard()){
        mediaInfo.mediaPresent = USBMSC_MEDIA_PRESENT;
    }
	else {
	    mediaInfo.mediaPresent = USBMSC_MEDIA_NOT_PRESENT;
	}
    mediaInfo.mediaChanged = 0x00;
    mediaInfo.writeProtected = 0x00;
    //Returns the number of blocks (sectors) in the media.
    disk_ioctl(0,GET_SECTOR_COUNT,&mediaInfo.lastBlockLba);
    //Block size will always be 512
    mediaInfo.bytesPerBlock = BYTES_PER_BLOCK;
    USBMSC_updateMediaInformation(0, &mediaInfo);

    //The data interchange buffer (used when handling SCSI READ/WRITE) is
    //declared by the application, and registered with the API using this
    //function.  This allows it to be assigned dynamically, giving
    //the application more control over memory management.
    USBMSC_registerBufferInformation(0, &RWbuf[0], NULL, sizeof(RWbuf));

    // Mounting our FileSystem and opening the ISO
    f_mount(0, &fs);
    fres = f_open(&isoFile, "0:mount.iso", FA_OPEN_EXISTING | FA_READ);
}

void USBMSC_processMSCBuffer(void)
{
    //Call USBMSC_pollCommand() to initiate handling of any received SCSI commands.
    //Disable interrupts
    //during this function, to avoid conflicts arising from SCSI commands
    //being received from the host
    //AFTER decision to enter LPM is made, but BEFORE it's actually entered
    //(in other words, avoid
    //sleeping accidentally).

	__disable_interrupt();
	if ((USBMSC_pollCommand() == USBMSC_OK_TO_SLEEP) && (!bDetectCard)){
	     //Enable interrupts atomically with LPM0 entry
		__bis_SR_register(LPM0_bits + GIE);
	}
	__enable_interrupt();


    //If the API needs the application to process a buffer, it will keep the
    //CPU awake by returning USBMSC_PROCESS_BUFFER
    //from USBMSC_pollCommand().  The application should then check the 'operation'
    //field of all defined USBMSC_RWbuf_Info
    //structure instances.  If any of them is non-null, then an operation
    //needs to be processed.  A value of
    //USBMSC_READ indicates the API is waiting for the application to fetch
    //data from the storage volume, in response
    //to a SCSI READ command from the USB host.  After the application does
    //this, it must indicate whether the
    //operation succeeded, and then close the buffer operation by calling
    //USBMSC_processBuffer().

	while (RWbuf_info->operation == USBMSC_READ)
	{
		/* Seeking to the requested LBA */
       f_lseek(&isoFile, RWbuf_info->lba * BYTES_PER_BLOCK);
       fres = f_read(&isoFile, RWbuf_info->bufferAddr,
    		   RWbuf_info->lbCount * BYTES_PER_BLOCK, &br);

      /* Making sure the read was sucessful */
      if(!fres)
      	RWbuf_info->returnCode = USBMSC_RW_SUCCESS;
      else
      	RWbuf_info->returnCode = USBMSC_RW_MEDIA_NOT_PRESENT;

   	  USBMSC_processBuffer();
    }
}

/*
 * ======== USBMSC_checkMSCInsertionRemoval ========
 *
 * This function checks for insertion/removal of the card.  If either is
 * detected, it informs the API by calling USBMSC_updateMediaInformation().  Whether
 * it detects it or not, it returns non-zero if the card is present, or zero if
 * not present
 */
uint8_t USBMSC_checkMSCInsertionRemoval (void)
{
    //Check card status -- there or not?
    uint8_t newCardStatus = detectCard();

    if ((newCardStatus) &&
        (mediaInfo.mediaPresent == USBMSC_MEDIA_NOT_PRESENT)){
        //An insertion has been detected -- inform the API
        mediaInfo.mediaPresent = USBMSC_MEDIA_PRESENT;
        mediaInfo.mediaChanged = 0x01;
        //Get the size of this new medium
        DRESULT SDCard_result = disk_ioctl(0,
            GET_SECTOR_COUNT,
            &mediaInfo.lastBlockLba);
        USBMSC_updateMediaInformation(0, &mediaInfo);
    }

    if ((!newCardStatus) && (mediaInfo.mediaPresent == USBMSC_MEDIA_PRESENT)){
        //A removal has been detected -- inform the API
        mediaInfo.mediaPresent = USBMSC_MEDIA_NOT_PRESENT;
        mediaInfo.mediaChanged = 0x01;
        USBMSC_updateMediaInformation(0, &mediaInfo);
    }

    return ( newCardStatus) ;
}
//Released_Version_5_20_06_02
