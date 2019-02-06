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
 * H4 Example
 *
 * Packet Protocol Demo:
 *
 * This application emulates a simple packet protocol that receives packets 
 * like the following:
 * Establish a connection with the HID Demo App.  No text is initially 
 * displayed.  Send a single digit, a number between 1-9.  Then, send that 
 * number of bytes.  For example, send '3', and then send "abc".  The 
 * application responds by indicating it has received the packet, and waits for 
 * another.
 * ----------------------------------------------------------------------------+
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
volatile uint8_t bCommandBeingProcessed = FALSE;
volatile uint8_t bDataReceiveCompleted_event = FALSE;  // data receive completed event

// Application globals
uint16_t x,y;
uint8_t size;
char c[2] = "";

// The "size" byte can only be a single ASCII numerical digit, which means 1-9
#define bufferLen 11
uint8_t buffer[bufferLen];

// Holds outgoing strings to be sent
char outString[65];

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
            
                // If no command is being processed, then open a rx operation
                // to receive the start of the "packet"
                if (!bCommandBeingProcessed){
                
                    // Only open it if we haven't already done so
                    if (!(USBHID_getInterfaceStatus(HID0_INTFNUM,&x,
                              &y) & USBHID_WAITING_FOR_RECEIVE)){
                              
                        // Start a receive operation for a single byte-- the
                        // "size" byte of the "packet
                        if (USBHID_receiveData(buffer,1,
                                HID0_INTFNUM) == USBHID_BUS_NOT_AVAILABLE){
                            // Abort receive is BUS is not available
                            USBHID_abortReceive(&x,HID0_INTFNUM);
                            break;
                        }
                    }
                }

                // Wait in LPM0 until a receive operation has completed
                __bis_SR_register(LPM0_bits + GIE);

                if (bDataReceiveCompleted_event){
                    bDataReceiveCompleted_event = FALSE;
                    
                    // This means that the incoming byte is the start of the
                    // "packet" -- the "size" byte
                    if (!bCommandBeingProcessed){
                    
                        // It's in ASCII, so convert it to a number
                        if ((buffer[0] >= 0x31) &&  (buffer[0] <= 0x39)){
                            size = buffer[0] - 0x30;
                            
                            // And then open a rcv operation for that size
                            if (USBHID_receiveData(buffer,size,
                                    HID0_INTFNUM) == USBHID_BUS_NOT_AVAILABLE){
                                // Abort receive if bus is no longer available
                                USBHID_abortReceive(&x,HID0_INTFNUM);
                                break;
                            }
                            // Now we're waiting for the "data" part of the
                            // "packet"
                            bCommandBeingProcessed = TRUE;
                        } else {
                        
                            // Prepare the outgoing string
                            strcpy( outString,
                            "\r\nEnter a valid number between 1 and 9\r\n\r\n");
                            
                            // Send the response over USB
                            if (USBHID_sendDataInBackground((uint8_t*)outString,
                                    strlen(outString),HID0_INTFNUM,0)){
                                    
                                // Operation may still be open; cancel it
                                // If the send fails, escape the main loop
                                USBHID_abortSend(&x,HID0_INTFNUM);
                                break;
                            }
                            // Now we're back to waiting for the "size" byte
                            bCommandBeingProcessed = FALSE;
                        }
                    // This means that the incoming data is the "data" part of
                    // the "packet"
                    } else {
                        // Prepare the outgoing string
                        strcpy(outString,
                            "\r\nI received your packet with size of ");
                        buffer[bufferLen - 1] = 0;
                        c[0] = (char)(size + '0');
                        c[1] = 0;
                        
                        // Convert the size back to ASCII
                        outString[64] = 0;
                        strcat(outString,c);
                        strcat(outString," bytes.\r\n\r\n");
                        
                        // Send the response over USB
                        if (USBHID_sendDataInBackground((uint8_t*)outString,
                                strlen(outString),HID0_INTFNUM,0)){
                            // Operation may still be open; cancel it if the
                            // send fails, escape the main loop
                            USBHID_abortSend(&x,HID0_INTFNUM);
                            break;
                        }
                        // Now we're back to waiting for the "size" byte
                        bCommandBeingProcessed = FALSE;
                    }
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
