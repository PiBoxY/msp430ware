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
 * HID to HID Chat Demo:
 *
 * This application is just like the preceding ones, except it shows a composite
 * HID+HID device.   Run the example and enumerate the device.  Since both 
 * interfaces are HID, there will be no "Found New Hardware" boxes ? both 
 * interfaces will load silently.  
 * Run two instances of the HID Demo App.  Set the VID in each one to 0x2047, 
 * and the PID to 0x0314.  Press "Set VID/PID" in each instance?s window.  
 * After a few seconds, a value should appear in the "Serial Number" menu, and 
 * "HID0" should appear in the "Interface" menu, of both windows.  In one of 
 * the windows, select interface "HID1" instead.  Then press the "Connect" 
 * button in each instance window, to initiate connections with the HID 
 * interfaces. Now, type text into one window and press "Send"; the data 
 * appears in the other window, being echoed by the MSP430 from one interface
 * to the other.
 *
 * ----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 * ---------------------------------------------------------------------------*/
#include <string.h>

#include "driverlib.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"                 // USB-specific functions
#include "USB_API/USB_HID_API/UsbHid.h"
#include "USB_app/usbConstructs.h"

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"

// Global flags set by events
volatile uint8_t bDataReceived_event0 = FALSE; // Indicates data has been rx'ed
volatile uint8_t bDataReceived_event1 = FALSE; // without an open rx operation,
                                            // on respective HID0/1 interfaces

// Application globals
#define BUFFER_SIZE 130
#define MAX_STR_LENGTH 60
char dataBuffer[BUFFER_SIZE];
char wholeString[MAX_STR_LENGTH] = "";

/*  
 * ======== main ========
 */
void main (void)
{
    WDT_A_hold(WDT_A_BASE); // Stop watchdog timer

    // Minimum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
    PMM_setVCore(PMM_CORE_LEVEL_2);
    USBHAL_initPorts();           // Config GPIOS for low-power (output low)
    USBHAL_initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    USB_setup(TRUE,TRUE);  // Init USB & events; if a host is present, connect

    __enable_interrupt();  // Enable interrupts globally

    while (1)
    {
        uint8_t ReceiveError = 0, SendError = 0;
        uint16_t count;
        
        // Check the USB state and directly main loop accordingly
        switch (USB_getConnectionState())
        {
            // This case is executed while your device is enumerated on the
            // USB host
            case ST_ENUM_ACTIVE:
            
                // Enter LPM0 until awakened by an event handler
                __bis_SR_register(LPM0_bits + GIE);
                __no_operation();

                // Exit LPM because of a data-receive event, & fetch rx'ed data
                
                if (bDataReceived_event0){ // HID0
                    bDataReceived_event0 = FALSE;
                    count = USBHID_receiveDataInBuffer((uint8_t*)dataBuffer,
                        BUFFER_SIZE,
                        HID0_INTFNUM);
                    strncat(wholeString,(char*)dataBuffer,count);
                    
                    // Send message to other HID App
                    if (USBHID_sendDataInBackground((uint8_t*)wholeString,
                            strlen(wholeString),HID1_INTFNUM,1)){
                        SendError = 0x01;
                        break;
                    }
                    memset(wholeString,0,MAX_STR_LENGTH);   // Clear wholeString
                }
                if (bDataReceived_event1){ // HID1
                    bDataReceived_event1 = FALSE;
                    count = USBHID_receiveDataInBuffer((uint8_t*)dataBuffer,
                        BUFFER_SIZE,
                        HID1_INTFNUM);
                    strncat(wholeString,(char*)dataBuffer,count);
                    
                    // Send message to other HID App
                    if (USBHID_sendDataInBackground((uint8_t*)wholeString,
                            strlen(wholeString),HID0_INTFNUM,1)){
                        SendError = 0x01;
                        break;
                    }
                    memset(wholeString,0,MAX_STR_LENGTH);   // Clear wholeString
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

        if (ReceiveError || SendError){
            // TO DO: User can place code here to handle error
        }
    }  // while(1)
} // main()


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

//Released_Version_5_20_06_02
