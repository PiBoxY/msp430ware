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
 * During USB execution, the USB interrupts all have short latency except the
 * ones for USB resume and VBUS-on, because these enable the XT2 oscillator and
 * PLL, which can take a few milliseconds.  This happens inside an interrupt
 * context, so your app won't receive other interrupts during that time.  If
 * this is a problem for your app, v4.0 and later of the API provide a way
 * to break the USB resume process into three pieces:
 * -> one starts the XT2 oscillator stabilization
 * -> one starts the PLL
 * -> one finishes the process.
 * While XT2 stabilizes, and while the PLL locks, your app can be responsive
 * to interrupts.  The application is responsible for knowing when these events
 * are completed; it can use a timer to wait fixed delays.
 *
 * With this feature in place, all USB interrupts finish very quickly (see
 * the Release Notes).
 *
 * This example demonstrates using this feature.  Its token USB functionality
 * is to send "Hello World" repeatedly, but the real point of the example is
 * in the timer ISR and the event handlers USB_handleCrystalStartedEvent()
 * and USB_handlePLLStartedEvent().
 *
 * Note that to use this feature, you must define USE_TIMER_FOR_RESUME in the
 * project settings.  This internal #define causes the API to break  the resume
 * event into the three phases above.  After defining USE_TIMER_FOR_RESUME,
 * call the special functions shown in the timer ISR.
 *
 *
 * SEQUENCE OF EVENTS:
 1) USB resume event occurs; some initial internal USB API code executes
 2) USB_handleCrystalStartedEvent() event occurs; calls application function startTimerForOscStabilize()
 3) startTimerForOscStabilize() starts the timer
 4) Timer expires; ensures osc stabilization, then calls USB_enable_PLL()
 5) USB_handlePLLStartedEvent() event occurs, calls application function startTimerforPLLlock()
 6) startTimerforPLLlock() starts timer
 7) Timer expires, calls USB_enable_final()
 8) PLL is now stable, and USB functionality resumes as normal
 *
 */
/*
 * ----------------------------------------------------------------------------+
 * Please refer to the Examples Guide for more details..
 * ---------------------------------------------------------------------------*/
#include <string.h>

#include "driverlib.h"

#include "USB_config/descriptors.h"
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/usb.h"                 // USB-specific functions
#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "USB_app/usbConstructs.h"


/*
 * NOTE: Modify hal.h to select a specific evaluation board and customize for
 * your own board.
 */
#include "hal.h"


//Indicates data has been received without an open rcv operation
volatile uint8_t bCDCDataReceived_event = FALSE;

// Defines/variables for token USB functionality (send "Hello world")
#define OUTPUT_STRING_LENGTH    13
char outString[13] = "Hello World\r\n";
Timer_A_initUpModeParam Timer_A_params = {0};


// Variable & state #defines, for the state of USB enabling
#define NONE					0
#define WAIT_FOR_XTAL_STABILIZE	1
#define WAIT_FOR_PLL_LOCK	2
volatile uint8_t usbEnableState =  NONE;
void setTimer_A_Parameters(void);

/*  
 * ======== main ========
 */
void main (void)
{
    WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__); //Stop watchdog timer

    // Minimum Vcore setting required for the USB API is PMM_CORE_LEVEL_2 .
    PMM_setVCore(PMM_CORE_LEVEL_2);
    USBHAL_initPorts();           // Config GPIOS for low-power (output low)
    USBHAL_initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz
    USB_setup(TRUE,TRUE);  // Init USB & events; if a host is present, connect

    __enable_interrupt();  // Enable interrupts globally
    
    while (1)
    {
        //Check the USB state and directly main loop accordingly
        switch (USB_getConnectionState())
        {
            case ST_ENUM_ACTIVE:
            
                // Send some data over USB, while USB is active
                USBCDC_sendDataAndWaitTillDone((uint8_t *)outString, OUTPUT_STRING_LENGTH,
                    CDC0_INTFNUM, 0);
                break;

            case ST_PHYS_DISCONNECTED:
            case ST_ENUM_SUSPENDED:
            case ST_PHYS_CONNECTED_NOENUM_SUSP:
                __bis_SR_register(LPM3_bits + GIE);
                _NOP();
                break;

            case ST_ENUM_IN_PROGRESS:
            default:;
        }

    }  //while(1)
} //main()


// Sets a timer to wait for XT2 osc startup; updates usbEnableState
void startTimerForOscStabilize(void)
{
    setTimer_A_Parameters();
    Timer_A_params.timerPeriod = 64;  // 64/32768Hz = 2ms, where 32kHz 
                                      //is the REFO=ACLK

	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

	// Set up Timer_A in UP mode, for 2ms.  2ms is very conservative for
	// stabilization of XT2 using a crystal, while also considering that if
	// this is a USB resume situation, the USB device has a total of 10ms to
	// stabilize XT2, the PLL, and re-enable any functionality that had been
	// turned off during suspend.
	//
	// Ultimately, you should characterize your XT2 oscillator, using the
	// crystal/resonator and capacitors you chose, to understand its startup
	// time under all operating conditions; and then adjust this value
	// accordingly.
    Timer_A_initUpMode(TIMER_A0_BASE, &Timer_A_params);

	// Start the timer
	Timer_A_startCounter(TIMER_A0_BASE,
		TIMER_A_UP_MODE);

	usbEnableState = WAIT_FOR_XTAL_STABILIZE;
}


// Sets a timer to wait for PLL lock; updates usbEnableState
void startTimerforPLLlock(void)
{
    setTimer_A_Parameters();
    Timer_A_params.timerPeriod = 32;

	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

	// Set up Timer_A in UP mode, for 2ms.  2ms is chosen because the datasheet
	// indicates a 2ms max PLL lock time
	Timer_A_initUpMode(TIMER_A0_BASE, &Timer_A_params);

	Timer_A_startCounter(TIMER_A0_BASE,
		TIMER_A_UP_MODE);

	usbEnableState = WAIT_FOR_PLL_LOCK;
}

/*
 * ======== setTimer_A_Parameters ========
 */
// This function sets the timer A parameters
void setTimer_A_Parameters()
{
	Timer_A_params.clockSource = TIMER_A_CLOCKSOURCE_ACLK;
	Timer_A_params.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
	Timer_A_params.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
	Timer_A_params.captureCompareInterruptEnable_CCR0_CCIE =
		       TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
	Timer_A_params.timerClear = TIMER_A_DO_CLEAR;
	Timer_A_params.startTimer = false;
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
	// Is this the expiration of the XT osc stabilization period, or PLL lock
	// period?
	if (usbEnableState == WAIT_FOR_XTAL_STABILIZE) {
		Timer_A_stop(TIMER_A0_BASE);

		// There's always a chance the osc didn't stabilize.  So, clear the
		// fault flags and see if they stay cleared.
	    UCSCTL7 &= ~(XT2OFFG);
	    SFRIFG1 &= ~OFIFG;
	    if (UCSCTL7 & XT2OFFG) {   // Oscillator failed to stabilize!
			USB_disable();         // Disable USB
			usbEnableState = NONE;
		}
		else {
			USB_enable_PLL();      // Now wait for PLL
		}
	}

	// PLL lock period is completed.
	else if (usbEnableState == WAIT_FOR_PLL_LOCK) {
		usbEnableState = NONE;
		Timer_A_stop(TIMER_A0_BASE);
	    USB_enable_final();
	}
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

//Released_Version_5_20_06_02
