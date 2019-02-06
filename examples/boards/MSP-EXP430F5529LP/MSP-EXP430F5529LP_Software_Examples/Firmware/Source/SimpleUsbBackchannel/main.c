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
//******************************************************************************
//   MSP430F5529LP:  simpleUsbBackchannel example
//
//   Description: 	Demonstrates simple sending over USB, as well as the F5529's
//                  backchannel UART.
//
//   Texas Instruments Inc.
//   August 2013
//******************************************************************************

// Basic MSP430 and driverLib #includes
#include "msp430.h"
#include "driverlib/MSP430F5xx_6xx/wdt_a.h"
#include "driverlib/MSP430F5xx_6xx/ucs.h"
#include "driverlib/MSP430F5xx_6xx/pmm.h"
#include "driverlib/MSP430F5xx_6xx/sfr.h"

// USB API #includes
#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/types.h"
#include "USB_API/USB_Common/usb.h"

#include "USB_app/usbConstructs.h"

// Application #includes
#include "BCUart.h"           // Include the backchannel UART "library"
#include "hal.h"              // Modify hal.h to select your hardware


/* You have a choice between implementing this as a CDC USB device, or a HID-
 * Datapipe device.  With CDC, the USB device presents a COM port on the host;
 * you interact with it with a terminal application, like Hyperterminal or
 * Docklight.  With HID-Datapipe, you interact with it using the Java HID Demo
 * App available within the MSP430 USB Developers Package.
 *
 * By default, this app uses CDC.  The HID calls are included, but commented
 * out.
 *
 * See the F5529 LaunchPad User's Guide for simple instructions to convert
 * this demo to HID-Datapipe.  For deeper information on CDC and HID-Datapipe,
 * see the USB API Programmer's Guide in the USB Developers Package.
 */


// Global variables
WORD rxByteCount;                        // Momentarily stores the number of bytes received
BYTE buf_bcuartToUsb[BC_RXBUF_SIZE];     // Same size as the UART's rcv buffer
BYTE buf_usbToBcuart[128];               // This can be any size


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Halt the dog

    // MSP430 USB requires a Vcore setting of at least 2.  2 is high enough
	// for 8MHz MCLK, below.
    PMM_setVCore(PMM_CORE_LEVEL_2);

    initPorts();           // Config all the GPIOS for low-power (output low)
    initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    bcUartInit();          // Init the back-channel UART
    USB_setup(TRUE,TRUE);  // Init USB; if a USB host (PC) is present, connect
    __enable_interrupt();  // Enable interrupts globally

    while(1)
    {
       // Look for rcv'ed bytes on the backchannel UART. If any, send over USB.
       rxByteCount = bcUartReceiveBytesInBuffer(buf_bcuartToUsb);
       if(rxByteCount)
       {
           cdcSendDataInBackground(buf_bcuartToUsb, rxByteCount, CDC0_INTFNUM, 1000);
           //hidSendDataInBackground(buf_bcuartToUsb, rxByteCount, HID0_INTFNUM, 1000);
       }

       // Look for received bytes over USB. If any, send over backchannel UART.
       rxByteCount = cdcReceiveDataInBuffer(buf_usbToBcuart, sizeof(buf_usbToBcuart), CDC0_INTFNUM);
       //rxByteCount = hidReceiveDataInBuffer(buf_usbToBcuart, sizeof(buf_usbToBcuart), HID0_INTFNUM);
       if(rxByteCount)
       {
           bcUartSend(buf_usbToBcuart, rxByteCount);
       }
    }
}



