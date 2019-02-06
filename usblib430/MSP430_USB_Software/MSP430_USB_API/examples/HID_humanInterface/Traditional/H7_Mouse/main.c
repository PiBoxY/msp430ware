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
 * Mouse HID Demo:
 *
 * This example functions as a mouse on the host.  It causes the mouse pointer 
 * to move in a circular pattern on the screen.  Simply build and run the 
 * example.  To re-gain control of the mouse, unplug USB.  
 * Unlike the HID-Datapipe examples, this one does not communicate with the 
 * HID Demo Application
 *
 +----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details.
 *----------------------------------------------------------------------------*/
#include <string.h>

#include "driverlib.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"                 // USB-specific functions
#include "USB_API/USB_HID_API/UsbHid.h"

/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"


#define LED_PORT    GPIO_PORT_P1
#define LED_PIN     GPIO_PIN0

typedef struct {
    uint8_t buttons;
    uint8_t dX;
    uint8_t dY;
    uint8_t dZ;
} MOUSE_REPORT;

MOUSE_REPORT mouseReport = { 0, 0, 0, 0 };          // HID report, to be sent to the PC.
const int16_t tableSinCosLookUp[93][2];                 // Lookup table for mouse data;
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
uint8_t index = 1;                                     // Index for lookup table
#endif
volatile uint8_t sendNewMousePosition = FALSE;        // Flag by which timer tells main
                                                    // loop to send a new report
Timer_A_initUpModeParam Timer_A_params = {0};

void initTimer (void);

/*  
 * ======== main ========
 */
void main (void)
{
#if defined(__GNUC__) && (__MSP430__)
uint8_t index = 1;
#endif
    WDT_A_hold(WDT_A_BASE); // Stop watchdog timer

    // Minumum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
    PMM_setVCore(PMM_CORE_LEVEL_2);
    USBHAL_initPorts();           // Config GPIOS for low-power (output low)
    USBHAL_initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    initTimer();
    USB_setup(TRUE, TRUE); // Init USB & events; if a host is present, connect

    __enable_interrupt();                           // Enable interrupts globally
    
    while (1)
    {
        // Check the USB state and directly main loop accordingly
        switch (USB_getConnectionState())
        {
            // This case is executed while your device is enumerated on the
            // USB host
            case ST_ENUM_ACTIVE:

                // Start Timer
                Timer_A_startCounter(TIMER_A0_BASE,
                    TIMER_A_UP_MODE);

                // Enter LPM0, until the timer wakes the CPU
                __bis_SR_register(LPM0_bits + GIE);

                // Timer has awakened the CPU.  Proceed with main loop...
                if (sendNewMousePosition){
                
                    // Build the report
                    mouseReport.dX =
                        (tableSinCosLookUp[index][0] -
                         tableSinCosLookUp[index - 1][0]) >> 1;
                    mouseReport.dY =
                        (tableSinCosLookUp[index][1] -
                         tableSinCosLookUp[index - 1][1]) >> 1;

                    // Send the report
                    USBHID_sendReport((void *)&mouseReport, HID0_INTFNUM);

                    // Toggle LED on P1.0
                    GPIO_toggleOutputOnPin(LED_PORT, LED_PIN);

                    if (index++ >= 90){
                        index = 1;
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
                TA0CTL &= ~MC_1;
                P1OUT &= ~BIT0;
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

/*  
 * ======== TIMER0_A0_ISR ========
 */
#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
#elif defined(__GNUC__) && (__MSP430__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) TIMER0_A0_ISR (void)
#else
#error Compiler not found!
#endif
{
sendNewMousePosition = TRUE;                 // Set flag telling main loop to send a report
__bic_SR_register_on_exit(LPM0_bits);        // Keep CPU awake after returning;
                                             // enables a run through the main loop
}

// Lookup table for mouse position values.  "const" indicates it will be stored
// in flash.
const int16_t tableSinCosLookUp[93][2] = {
    0,200,
    14,200,
    28,198,
    42,196,
    55,192,
    68,188,
    81,183,
    94,177,
    106,170,
    118,162,
    129,153,
    139,144,
    149,134,
    158,123,
    166,112,
    173,100,
    180,88,
    185,75,
    190,62,
    194,48,
    197,35,
    199,21,
    200,7,
    200,-7,
    199,-21,
    197,-35,
    194,-48,
    190,-62,
    185,-75,
    180,-88,
    173,-100,
    166,-112,
    158,-123,
    149,-134,
    139,-144,
    129,-153,
    118,-162,
    106,-170,
    94,-177,
    81,-183,
    68,-188,
    55,-192,
    42,-196,
    28,-198,
    14,-200,
    0,-200,
    -14,-200,
    -28,-198,
    -42,-196,
    -55,-192,
    -68,-188,
    -81,-183,
    -94,-177,
    -106,-170,
    -118,-162,
    -129,-153,
    -139,-144,
    -149,-134,
    -158,-123,
    -166,-112,
    -173,-100,
    -180,-88,
    -185,-75,
    -190,-62,
    -194,-48,
    -197,-35,
    -199,-21,
    -200,-7,
    -200,7,
    -199,21,
    -197,35,
    -194,48,
    -190,62,
    -185,75,
    -180,88,
    -173,100,
    -166,112,
    -158,123,
    -149,134,
    -139,144,
    -129,153,
    -118,162,
    -106,170,
    -94,177,
    -81,183,
    -68,188,
    -55,192,
    -42,196,
    -28,198,
    -14,200,
    0,200
};

/*
 * ======== setTimer_A_Parameters ========
 */
// This function sets the timer A parameters
void setTimer_A_Parameters()
{
    Timer_A_params.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
	Timer_A_params.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    Timer_A_params.timerPeriod = 547;  // 547/32768 = a period of 16.7ms
	Timer_A_params.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
	Timer_A_params.captureCompareInterruptEnable_CCR0_CCIE =
		       TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
	Timer_A_params.timerClear = TIMER_A_DO_CLEAR;
	Timer_A_params.startTimer = false;
}



/*
 * ======== initTimer ========
 */
void initTimer (void)
{
    setTimer_A_Parameters();
    
    // Start timer
    Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

    Timer_A_initUpMode(TIMER_A0_BASE, &Timer_A_params);
}
//Released_Version_5_20_06_02
