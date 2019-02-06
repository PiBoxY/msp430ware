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
 * H5 Example
 *
 * High-Bandwidth Sending Using  USBHID_sendDataAndWaitTillDone Demo:
 *
 * The example implements large data transfer using  USBHID_sendDataAndWaitTillDone(), 
 * which doesn?t allow execution to proceed until all the data has been sent.  
 * It prompts for any key to be pressed, and when this happens, the application 
 * sends a large amount of data to the host.  
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
volatile uint8_t bHIDDataReceived_event = FALSE; // Indicates data has been rx'ed
                                              // without an open rx operation

// Application globals
char outString[32];
uint16_t x; // Dummy variable for use with abortSend()
uint16_t rounds = 0;

#define MEGA_DATA_LENGTH 2046
uint8_t dataBuf[MEGA_DATA_LENGTH];
uint16_t w;
uint8_t y = 0x21;

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

    // Pre-fill the buffers with visible ASCII characters (0x21 to 0x7E)
    for (w = 0; w < MEGA_DATA_LENGTH; w++)
    {
        dataBuf[w] = y++;
        if (y > 0x7E){
            y = 0x21;
        }
    }

    __enable_interrupt(); // Enable interrupts globally
    
    while (1)
    {
        // Check the USB state and directly main loop accordingly
        switch (USB_getConnectionState())
        {
            // This case is executed while your device is enumerated on the
            // USB host
            case ST_ENUM_ACTIVE:
            
                // Do this until a key is pressed
                if (!bHIDDataReceived_event){
                
                    // Prepare the outgoing string
                    strcpy(outString,"Press any key.\r");
                    
                    // Send it; no timeout
                    if ( USBHID_sendDataAndWaitTillDone((uint8_t*)outString,
                            strlen(outString),HID0_INTFNUM,0)){
                        // Operation may still be open; cancel it
                        USBHID_abortSend(&x,HID0_INTFNUM);
                        break;
                    }
                } else {
                    bHIDDataReceived_event = FALSE;
                    USBHID_rejectData(HID0_INTFNUM);
                    for (rounds = 0; rounds < 600; rounds++)
                    {
                        // Send all of dataBuf
                        if ( USBHID_sendDataAndWaitTillDone(dataBuf,MEGA_DATA_LENGTH,
                            HID0_INTFNUM, 0)){
                            // Operation probably still open; cancel it
                            USBHID_abortSend(&x,HID0_INTFNUM);
                            break;
                        }
                    }
                    
                    // Prepare the outgoing string
                    strcpy(outString,"\r\n\r\n\r\nThe test is completed.\r\n");
                    
                    // Send it; no timeout
                    if ( USBHID_sendDataAndWaitTillDone((uint8_t*)outString,
                            strlen(outString),HID0_INTFNUM,0)){
                        // Operation may still be open; cancel it
                        USBHID_abortSend(&x,HID0_INTFNUM);
                        break;
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
