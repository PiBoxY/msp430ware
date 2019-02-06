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

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"


// Global flags set by events
volatile uint8_t bHIDDataReceived_event = FALSE;
volatile uint8_t bDataReceiveCompleted_event = FALSE;

// Application globals
uint8_t dataBuff[1024];
volatile uint16_t usbEvents = USB_VBUSON_EVENTMASK + USB_VBUSOFF_EVENTMASK +
    USB_DATARECEIVED_EVENTMASK + USB_USBSUSPEND_EVENTMASK +
    USB_USBRESUME_EVENTMASK +
    USB_USBRESET_EVENTMASK;
char outString[40] = "";
uint16_t i;
uint16_t x,y;
uint8_t ret;

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
                    strcpy(outString,"I'm ready to receive 1K of data.\r\n");

                    // Send it over USB. If it failed for some reason; abort and
                    // leave the main loop
                    if ( USBHID_sendDataAndWaitTillDone((uint8_t*)outString,
                            strlen(outString),HID0_INTFNUM,0)){
                        USBHID_abortSend(&x,HID0_INTFNUM);
                        break;
                    }

                    // Set up rcv operation for 1K data
                    // If USBHID_receiveData fails because of surprise removal
                    // or suspended by host abotr and leave main loop
                    if (USBHID_receiveData(dataBuff,1024, HID0_INTFNUM) ==
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
                    // Prepare the outgoing string
                    strcpy(outString,"Thanks for the data.\r\n");
                    // Send the response over USB.  If it failed for some reason
                    // abort and leave the main loop
                    if (USBHID_sendDataInBackground((uint8_t*)outString,
                            strlen(outString),HID0_INTFNUM,0)){
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
