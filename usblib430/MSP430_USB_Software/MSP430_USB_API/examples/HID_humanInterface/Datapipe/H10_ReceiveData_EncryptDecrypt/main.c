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
 * ======== main.c ========
 * H2 Example
 *
 * Receive 1K bytes from Host Demo:
 *
 * This example implements a device whose only purpose is to receive a 1K chunk 
 * of data from the host.  Establish a connection with the HID Demo App.  
 * The program prompts the user to press any key.  When the user does so, it 
 * asks for 1K of data.  Any data received after that point will count toward 
 * the 1K (1024 byte) goal.  When 1K has been received, the program thanks the 
 * user, and the process repeats.  
 +----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 *----------------------------------------------------------------------------*/
#include <string.h>

#include "driverlib.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"                 // USB-specific functions
#include "USB_API/USB_HID_API/UsbHid.h"
#include "USB_app/usbConstructs.h"
#include "aes128lib/TI_aes.h"

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"

void decryptData(uint8_t* dataString, uint16_t dataSize);
void encryptData(uint8_t* dataString, uint16_t dataSize);
uint8_t verifyData(uint8_t* dataString, uint16_t dataSize);
#define MAX_STR_LENGTH 1024  //total data size expected in increments of 16.  If data size expected
                           //is 33, MAX_STR_LENGTH should be 48

uint16_t expectedDataSize = 49; //has to be divisible by 16. count also includes 1 byte that lists the actual data size.

// Global flags set by events
volatile uint8_t bHIDDataReceived_event = FALSE;
volatile uint8_t bDataReceiveCompleted_event = FALSE;

// Application globals
uint8_t dataBuff[MAX_STR_LENGTH];  //dataBuff has to large enough to hold entire padded data size + 1
volatile uint16_t usbEvents = USB_VBUSON_EVENTMASK + USB_VBUSOFF_EVENTMASK +
    USB_DATARECEIVED_EVENTMASK + USB_USBSUSPEND_EVENTMASK +
    USB_USBRESUME_EVENTMASK +
    USB_USBRESET_EVENTMASK;
char outString[64] = "";
uint16_t x;
uint8_t ret;
uint16_t strlength = 0;
static uint8_t datavalid = 0;
unsigned char key[]   = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                           0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

/*  
 * ======== main ========
 */
void main (void)
{
    WDT_A_hold(WDT_A_BASE); // Stop watchdog timer

    // Minumum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
    PMM_setVCore(PMM_CORE_LEVEL_2);
    USBHAL_initPorts();           // Config GPIOS for low-power (output low)
    USBHAL_initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    USB_setup(TRUE, TRUE); // Init USB & events; if a host is present, connect

    __enable_interrupt();  // Enable interrupts globally

    while (1)
    {
        // Check the USB state and directly main loop accordingly
        switch (USB_getConnectionState())
        {
            // This case is executed while your device is enumerated on the
            // USB host
            case ST_ENUM_ACTIVE:
                // Enter LPM0 until an event occurs.
                __bis_SR_register(LPM0_bits + GIE);

                // This flag is set by the handleDataReceived event; this 
                // event is only enabled when waiting for 'press any key'
                if (bHIDDataReceived_event){
                    bHIDDataReceived_event = FALSE;

                    // Change the event flags, in preparation for receiving 1K
                    // data. No more data-received.  We only used this for
                    // 'press any key'
                    usbEvents &= ~USB_DATARECEIVED_EVENTMASK;

                    // But enable receive-completed; we want to be prompted when
                    // 1K data has been received
                    usbEvents |= USB_RECEIVECOMPLETED_EVENTMASK;

                    USB_setEnabledEvents(usbEvents);
                    // We don't care what char the key-press was so we reject it
                    USBHID_rejectData(HID0_INTFNUM);

                    // Prompt user for 1K data.  
                   strcpy(outString,"\r\nI'm ready to receive 1K of data.\r\n\r\n");

                    strlength = strlen(outString);
                    encryptData((uint8_t*)outString, strlength);

                    // Send it over USB. If it failed for some reason; abort and
                    // leave the main loop
                    if ( USBHID_sendDataAndWaitTillDone((uint8_t*)outString,
                    		strlength,HID0_INTFNUM,0)){
                        USBHID_abortSend(&x,HID0_INTFNUM);
                        break;
                    }

                    // Set up rcv operation for 1K data
                    // If USBHID_receiveData fails because of surprise removal
                    // or suspended by host abotr and leave main loop
                    if (USBHID_receiveData(dataBuff,expectedDataSize, HID0_INTFNUM) ==
                        USBHID_BUS_NOT_AVAILABLE){
                        USBHID_abortReceive(&x,HID0_INTFNUM);
                        break;
                    }
                }

                // This flag would have been set by the handleReceiveCompleted
                // event; this event is only enabled while receiving 1K data,
                // and signals that all 1K has been received
                if (bDataReceiveCompleted_event){
                    bDataReceiveCompleted_event = FALSE;
                    //decrypted data after all expected data have been received
                    decryptData(dataBuff, expectedDataSize);
                    datavalid = verifyData(dataBuff, expectedDataSize);

                    if (datavalid == 0)
                    {
                    	strcpy(outString,"\r\nData valid.\r\n\r\n");
                    }
                    else
                    {
                    	strcpy(outString,"\r\nData is not valid.\r\n\r\n");
                    }
                    // Prepare the outgoing string

                //    strcpy(outString,"\r\nThanks for the data.\r\n\r\n");
                    strlength = strlen(outString);
                    encryptData((uint8_t*)outString, strlength);
                    // Send the response over USB.  If it failed for some reason
                    // abort and leave the main loop
                    if (USBHID_sendDataInBackground((uint8_t*)outString,
                    		strlength,HID0_INTFNUM,0)){
                        USBHID_abortSend(&x,HID0_INTFNUM);
                        break;
                    }

                    // Change the event flags, in preparation for 'press any key'
                    // No more receive-completed.
                    usbEvents &= ~USB_RECEIVECOMPLETED_EVENTMASK;
                    
                    // This will tell us that data -- any key -- has arrived
                    usbEvents |= USB_DATARECEIVED_EVENTMASK;
                    USB_setEnabledEvents(usbEvents);
                }

                break;


            // These cases are executed while your device is disconnected from
            // the host (meaning, not enumerated); enumerated but suspended
            // by the host, or connected to a powered hub without a USB host
            // present.
            case ST_PHYS_DISCONNECTED:
            case ST_ENUM_SUSPENDED:
            case ST_PHYS_CONNECTED_NOENUM_SUSP:
                __bis_SR_register(LPM3_bits + GIE);
                _NOP();
                break;

            // The default is executed for the momentary state
            // ST_ENUM_IN_PROGRESS.  Usually, this state only last a few
            // seconds.  Be sure not to enter LPM3 in this state; USB
            // communication is taking place here, and therefore the mode must
            // be LPM0 or active-CPU.
            case ST_ENUM_IN_PROGRESS:
            default:;
        }

    }  //while(1)
} //main()



void decryptData(uint8_t* dataString, uint16_t dataSize)
{
    uint8_t i, j;
    uint8_t aes128Len=16;  //AES-128 data array length
    uint8_t numWords = 0;
    uint8_t bufPtr = 1;
    uint8_t ptr = 0;
    uint8_t actualSize = 0;
    uint8_t padBytes = 0;
    uint8_t tempBuf[16];  //temporary buffer used to do decryption
    uint8_t bytesLeft;


    unsigned char tempStr[MAX_STR_LENGTH] = "";

    // Make a copy of the string
    strncpy((char*)tempStr,(char*)dataString,dataSize);

    //get the actual data size sent by user.  This size is not padded.
    actualSize = tempStr[0];

    numWords = actualSize / aes128Len;

    bytesLeft = actualSize - (numWords * aes128Len);

    if (bytesLeft > 0)
    {
    	//Any bytes not divisible by 16 are padded as sent by user's GUI
    	numWords = numWords + 1;
    }

    for (i = 0; i < numWords; i++)
    {
    	for (j = 0; j < aes128Len; j++)
		{
    		//copy up to 16 bytes of encrypted data to temporary array
    		tempBuf[j] = tempStr[bufPtr + j];
		}
    	//decrypt the data
		aes_decrypt(tempBuf, key);

		//copy back the decrypted data into temporary array
		for (j = 0; j < aes128Len; j++)
		{
			tempStr[ptr + j] = tempBuf[j];

		}
        bufPtr += 16;
        ptr += 16;

    }
    //calculate the number of bytes used for padding the data string
	padBytes = aes128Len - bytesLeft;

    //copy back the decrypted data into original array
   	for (i = 0; i < actualSize; i++)
   	{
   		dataString[i] = tempStr[i];
   	}
	if (padBytes < 16)
	{
	   	for (i = 0; i < padBytes + 1; i++)
	   	{
	   		dataString[actualSize + i] = 0;
	   	}
	}

}

void encryptData(uint8_t* dataString, uint16_t dataSize)
{
	 uint8_t bytesForPad = 0, i, j;
	 uint8_t numWords=0;
	 uint8_t bufPtr = 0;
	 uint8_t tempBuf[16];
	 uint8_t bytesLeft = 0;
	 uint8_t ptr = 1;
	 uint8_t aes128DataSize = 16;

	 unsigned char tempStr[MAX_STR_LENGTH] = "";


    //calculate number of 16byte words in data string
	numWords = dataSize / aes128DataSize;
	//calculate the extra bytes
	bytesLeft = dataSize - (numWords * aes128DataSize);

    if (numWords > 0)
    {
		for (i = 0; i < numWords; i++)
		{
			for (j = 0; j < aes128DataSize; j++)
			{
			//copy data into temporary buffer array
			   tempBuf[j] = dataString[bufPtr + j];

			}
			//encrypt data in 16 byte increments
			aes_encrypt(tempBuf, key);
			for (j = 0; j< aes128DataSize; j++)
			{
				//copy encrypted data into temporary array
				tempStr[ptr + j] = tempBuf[j];

			}
			bufPtr += 16;
			ptr += 16;
		}

    }
    //add bytes for padding data
	if (bytesLeft > 0)
	{
		//increment number of 16 byte word by 1
        numWords = numWords + 1;
        //number of bytes used for padding data
		bytesForPad = aes128DataSize - bytesLeft;

		for (i = 0; i < bytesLeft; i++)
	    {
			//copy un-padded data to temporary buffer array
		   	tempBuf[i] = dataString[bufPtr + i];
	    }
		for (j = 0; j < bytesForPad; j++)
		{
			//add the pads for a total data length of 16 bytes
            tempBuf[bytesLeft + j] = 0;

		}
		//encrypt the data
	    aes_encrypt(tempBuf, key);
	    for (i = 0; i < aes128DataSize; i++)
	    {
	    	//copy the encrypted data into temporary array
	    	tempStr[ptr + i] = tempBuf[i];
	    }
	}
	//include actual data length
	tempStr[0] = dataSize;
	//padded data length
	strlength = aes128DataSize * numWords + 1;
	//copy the entire encrypted data with padding to original data string
	for (i = 0; i < strlength; i++)
	{
		dataString[i] = tempStr[i];
	}

}


uint8_t verifyData(uint8_t* dataString, uint16_t dataSize)
{

    char tempBuffer[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHI";
    char copyOfData[64] = "";
    strncpy(copyOfData, (char*)dataString,dataSize);
	datavalid = strcmp(tempBuffer, copyOfData);

    return datavalid;
}


/*  
 * ======== UNMI_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(UNMI_VECTOR))) UNMI_ISR (void)
#else
#error Compiler not found!
#endif
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG ))
    {
        case SYSUNIV_NONE:
            __no_operation();
            break;
        case SYSUNIV_NMIIFG:
            __no_operation();
            break;
        case SYSUNIV_OFIFG:
            UCS_clearFaultFlag(UCS_XT2OFFG);
            UCS_clearFaultFlag(UCS_DCOFFG);
            SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
            break;
        case SYSUNIV_ACCVIFG:
            __no_operation();
            break;
        case SYSUNIV_BUSIFG:
            // If the CPU accesses USB memory while the USB module is
            // suspended, a "bus error" can occur.  This generates an NMI.  If
            // USB is automatically disconnecting in your software, set a
            // breakpoint here and see if execution hits it.  See the
            // Programmer's Guide for more information.
            SYSBERRIV = 0; //clear bus error flag
            USB_disable(); //Disable
    }
}

//Released_Version_5_10_00_17
//Released_Version_5_20_06_02
