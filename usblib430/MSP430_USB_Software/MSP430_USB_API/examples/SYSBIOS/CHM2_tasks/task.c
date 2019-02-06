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
 *  MSP430 specific SYSBIOS example which uses tasks
 */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

#include "USB_API/USB_Common/device.h"         
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/usb.h"
#include <string.h>

// MSC #includes
#include "USB_API/USB_MSC_API/UsbMscScsi.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"
#include "USB_API/USB_MSC_API/UsbMscStateMachine.h"
#include "USB_app/LUN0.h"
#include "USB_app/LUN0_data.h"

#include "USB_app/usbConstructs.h"

/* Semaphore handle defined in task.cfg */
extern const Semaphore_Handle semLun0;
extern const Semaphore_Handle semCdc0;
extern const Semaphore_Handle semHid0;

extern USBMSC_RWbuf_Info *RWbuf_info;
extern uint8_t bDetectCard;

uint8_t checkInsertionRemoval(void);
void read_LUN1(void);
void write_LUN1(void);

#define MAX_STR_LENGTH 60
#define BUFFER_SIZE 130

char wholeString[MAX_STR_LENGTH] = "";
char pieceOfString[MAX_STR_LENGTH] = "";

char dataBuffer[BUFFER_SIZE];
uint16_t count;
       
// This function returns true if there's an 0x0D character in the string; and if so, 
// it trims the 0x0D and anything that had followed it.  
uint8_t retInString(char* string)
{
  uint8_t retPos=0,i,len;
  char tempStr[MAX_STR_LENGTH] = "";
  
  strncpy(tempStr,string,strlen(string));     // Make a copy of the string
  len = strlen(tempStr);    
  while((tempStr[retPos] != 0x0A) && (tempStr[retPos] != 0x0D) && (retPos++ < len)); // Find 0x0D; if not found, retPos ends up at len
  
  if((retPos<len) && (tempStr[retPos] == 0x0D))                          // If 0x0D was actually found...
  {
    for(i=0;i<MAX_STR_LENGTH;i++)             // Empty the buffer
      string[i] = 0x00;
    strncpy(string,tempStr,retPos);           // ...trim the input string to just before 0x0D
    return TRUE;                              // ...and tell the calling function that we did so
  }
  
  else if((retPos<len) && (tempStr[retPos] == 0x0A))                                // If 0x0D was actually found...
  {
    for(i=0;i<MAX_STR_LENGTH;i++)             // Empty the buffer
      string[i] = 0x00;
    strncpy(string,tempStr,retPos);           // ...trim the input string to just before 0x0D
    return TRUE;                              // ...and tell the calling function that we did so
  }
  else if (tempStr[retPos] == 0x0D)
  {
    for(i=0;i<MAX_STR_LENGTH;i++)             // Empty the buffer
      string[i] = 0x00;
    strncpy(string,tempStr,retPos);           // ...trim the input string to just before 0x0D
    return TRUE;                              // ...and tell the calling function that we did so
  }
    
  else if (retPos<len)
  {
    for(i=0;i<MAX_STR_LENGTH;i++)             // Empty the buffer
      string[i] = 0x00;
    strncpy(string,tempStr,retPos);           // ...trim the input string to just before 0x0D
    return TRUE;                              // ...and tell the calling function that we did so
  }
 
  return FALSE;                               // Otherwise, it wasn't found
}

void lun0TaskFxn(void) 
{                                             
    while(1) {
    	Semaphore_pend(semLun0, BIOS_WAIT_FOREVER);
    	
	    // If the API needs the application to process a buffer, it will keep the CPU awake by returning USBMSC_PROCESS_BUFFER
	    // from USBMSC_pollCommand().  The application should then check the 'operation' field of all defined USBMSC_RWbuf_Info
	    // structure instances.  If any of them is non-null, then an operation needs to be processed.  A value of 
	    // USBMSC_READ indicates the API is waiting for the application to fetch data from the storage volume, in response 
	    // to a SCSI READ command from the USB host.  After the application does this, it must indicate whether the 
	    // operation succeeded, and then close the buffer operation by calling USBMSC_processBuffer().  
	    
		while(RWbuf_info->operation == USBMSC_READ)
	    {
	      	RWbuf_info->returnCode = LUN0_read(RWbuf_info->lba, RWbuf_info->bufferAddr, RWbuf_info->lbCount); // Fetch a block from the medium, using file system emulation
	      	USBMSC_processBuffer();                           // Close the buffer operation
	    }
	    
	    // Everything in this section is analogous to READs.  Reference the comments above.   
	    while(RWbuf_info->operation == USBMSC_WRITE)
	    {                  
	    	RWbuf_info->returnCode = LUN0_write(RWbuf_info->lba, RWbuf_info->bufferAddr, RWbuf_info->lbCount); // Write the block to the medium, using file system emulation
	        USBMSC_processBuffer();                            // Close the buffer operation
	    }
    }                
}

void cdc0TaskFxn(void) 
{
    while(1) {
    	Semaphore_pend(semCdc0, BIOS_WAIT_FOREVER);
    	
    	memset(pieceOfString,0,MAX_STR_LENGTH);			// Clear pieceOfString
      	USBCDC_receiveDataInBuffer((uint8_t*)pieceOfString,MAX_STR_LENGTH,CDC0_INTFNUM);
      	strncat(wholeString,pieceOfString,strlen(pieceOfString));   
      	if(retInString(wholeString))                     //Wait for enter key to be pressed
      	{
      		strcat(wholeString,"\r\r\n");
      		if(USBCDC_sendDataInBackground((uint8_t*)wholeString,strlen(wholeString),CDC0_INTFNUM,1))   // Send message to other App
      		{
        		  break;
      		}
      		memset(wholeString,0,MAX_STR_LENGTH);		   // Clear wholeString
      	} 
    }	
}

void hid0TaskFxn(void) 
{
    while(1) {
    	Semaphore_pend(semHid0, BIOS_WAIT_FOREVER);

    	count = USBHID_receiveDataInBuffer((uint8_t*)dataBuffer,BUFFER_SIZE,HID0_INTFNUM);
      	strncat(wholeString,(char*)dataBuffer,count);
    	if(USBHID_sendDataInBackground((uint8_t*)wholeString,strlen(wholeString),HID0_INTFNUM,1)) // Send message to other CDC App
    	{
    		break;
    	}
    	memset(wholeString,0,MAX_STR_LENGTH);           // Clear wholeString     	
    }	
}


void myIdleFxn(void)
{	
  while(1) {
	  switch(USB_getConnectionState())
	  {
	    case ST_USB_DISCONNECTED:
	             __bis_SR_register(LPM3_bits + GIE); 	       // Enter LPM3 until VBUS-on event
	            break;
	    case ST_USB_CONNECTED_NO_ENUM:
	            break;
	      
	    case ST_ENUM_ACTIVE:                
	            __disable_interrupt();
                if(USBMSC_pollCommand() == USBMSC_OK_TO_SLEEP)
                {
                    __bis_SR_register(LPM0_bits + GIE);  // Enable interrupts atomically with LPM0 entry
                }
                __enable_interrupt();
	            
	                            
	            break;
	            
	        case ST_ENUM_SUSPENDED:
	            __bis_SR_register(LPM3_bits + GIE);            // Enter LPM3, until a resume or VBUS-off event
	            break;
	          
	      case ST_ENUM_IN_PROGRESS:
	            break;
	          
	      case ST_ERROR:
	            break;           
	      default:;  
	    }
  }
}

//Released_Version_5_20_06_02
