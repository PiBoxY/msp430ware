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
//   MSP430F5529LP:  emulStorageKeyboard example
//
//   Description:   Demonstrates an HID+MSC composite USB device, housing
//                  an emulated keyboard and storage volume, respectively.
//                  Uses the MSP430 USB API, aided by the Descriptor Tool.
//
//   Texas Instruments Inc.
//   August 2013
//******************************************************************************
/*  
 * ======== main.c ========
 *
 * This software enumerates as a composite USB device (defined in \USB_config)
 * HID (Human Interface device):  an emulated keyboard
 * MSC (Mass Storage class):  an emulated hard drive with FAT volume
 *
 * The content of the hard drive can be viewed with a file browser on the PC:
 * 1) Button1.txt:  the contents of this file are "typed out" to the PC, using
 *    the emulated keyboard, upon button S1 press
 * 2) Button2.txt:  the contents of this file are "typed out" to the PC, using
 *    the emulated keyboard, upon button S2 press
 * 3) MSP430 USB LaunchPad.url:  when you double-click, your browser launches
 *    the MSP-EXP430F5529LP landing page
 * 4) README.txt- a README that describes this example
 *
 * The LaunchPad's buttons S1 and S2 can be used to send ASCII strings to the
 * PC as if they came from a keyboard. These strings are stored in Button1.txt
 * and Button2.txt, respectively; and these files can be modified to change
 * the strings.  The text string is limited to 2048 characters, so even though
 * you can make the file contents longer, be aware that the string will be
 * truncated to 2048.
 *
 * The default strings are:
 * Button 1:  "Hello world"
 * Button 2:  an ASCII-art picture of the LaunchPad rocket
 *
 * For the rocket picture, please note that the display can be affected by
 * settings of the application receiving the typed characters.  On Windows,
 * the basic Notepad.exe is recommended.  (Go to "Run..." in the start menu,
 * and type "Notepad".)
 *
 * For more explanation of the code, see the F5529 LaunchPad User's Guide, and
 * the USB API Programmer's Guide inside the USB Developers Package.  Also see
 * the 20+ other USB examples in the USB Dev Package.
 */


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
#include "USB_API/USB_HID_API/UsbHid.h"
#include "USB_API/USB_MSC_API/UsbMsc.h"

// Application #includes
#include "USB_app/mscIntf.h"
#include "USB_app/keyboard.h"
#include "USB_app/FatFs/ff.h"

#include "hal.h"              // Modify hal.h to select your hardware!


#define BTNSTR_SIZE 2048      // Size of the string buffer

void prepSendingStr(char* path);


/*********** Application specific globals **********************/

// Boolean flags
BYTE bButton1Pressed = FALSE;
BYTE bButton2Pressed = FALSE;
BYTE bKeyDirIsDown = TRUE;    // "Keypress direction is down (or up)"
                              // TRUE if next keypress is down, FALSE if up
BYTE bUsbSendComplete = TRUE; // Only FALSE while waiting for a USB report to
                              // get fetched by the host

// Buffer holding outgoing strings associated with the pushbuttons
BYTE btnStr[BTNSTR_SIZE];
WORD btnStrLen;

WORD charLeftToSend = 0;


/*
 * ======== main ========
 */
void main (void)
{
  // Hold the watchdog timer.
  WDT_A_hold(__MSP430_BASEADDRESS_WDT_A__);

  // MSP430 USB requires a Vcore setting of at least 2.
  PMM_setVCore(PMM_CORE_LEVEL_3);

  initPorts();           // Config all the GPIOS for low-power (output low)
  initButtons();         // Config the GPIOS for the buttons
  initClocks(8000000);   // Config clocks. MCLK=SMCLK=FLL=8MHz; ACLK=REFO=32kHz

  KB_init();             // Initialize the keyboard report
  initMscIntf();         // Initialize the mass storage interface
  USB_setup(TRUE,TRUE);  // Init USB; if a USB host (PC) is present, connect
  __enable_interrupt();  // Enable interrupts globally


   while (1)             // Main loop
  {
      switch(USB_connectionState())
      {
          // This case is executed while the device is enumerated on the USB
          // host.
          case ST_ENUM_ACTIVE:

              // USBMSC_poll() handles any SCSI cmds from the USB host. If a
              // SCSI READ/WRITE is received, it returns kUSBMSC_processBuffer,
              // avoiding LPM0 and proceeding to mscProcessBuffer().  Also
        	  // check flags related to sending strings.
              // We disable ints to avoid a situation where we decide to sleep,
        	  // but then receive a SCSI command before we actually enter sleep
              __disable_interrupt();
              if ((USBMSC_poll() == kUSBMSC_okToSleep) && !charLeftToSend &&
            		                !bButton1Pressed && !bButton2Pressed)
              {
                  // Enter LPM0.  Active USB requires we go no lower than LPM0.
                  __bis_SR_register(LPM0_bits + GIE);
              }
              __enable_interrupt();

              // In this app, LPM0 is exited if a USB event occurs and returns
              // TRUE, if a SCSI command is received, or if a pushbutton is
              // pressed.  The most significant USB event for this app is
              // handleSendCompleted(), meaning a USB report previously sent
              // has been fetched by the host.

              mscProcessBuffer();  // Handle any MSC READ/WRITE's from the host

              // Handle a press of button 1, if it happened
              if (bButton1Pressed && !charLeftToSend)
              {
                  // Initiate string sending.  "0:Button1.txt" is the name of
                  // the file containing the string, not the string itself
                  prepSendingStr("0:Button1.txt");
                  bButton1Pressed = FALSE;
              }

              // Handle a press of button 2, if it happened
              if (bButton2Pressed && !charLeftToSend)
              {
                  // Initiate string sending.  "0:Button2.txt" is the name of
                  // the file containing the string, not the string itself
                  prepSendingStr("0:Button2.txt");
                  bButton2Pressed = FALSE;
              }

              // Send (or, 'type') the string to the host, one char at a time.
              // Each char gets a down-press and then an up-press, otherwise
              // the host thinks the key is still held down.  Only send a new
              // report if no previous reports are pending to be fetched by the
              // host, and if there are more chars left to send
              if (bUsbSendComplete && charLeftToSend)
              {
                  if(bKeyDirIsDown)
                  {
                      KB_addKeypressToReport(btnStr[btnStrLen-charLeftToSend]);
                      bKeyDirIsDown = FALSE;
                  }
                  else
                  {
                      KB_addKeyReleaseToReport(btnStr[btnStrLen-charLeftToSend]);
                      bKeyDirIsDown = TRUE;
                      charLeftToSend--;
                  }
                  bUsbSendComplete = FALSE;
                  USBHID_sendReport(KB_getReportPtr(), HID0_INTFNUM);

                  // Note that sendReport() may return a fail code if the USB
                  // host has become disconnected or unresponsive.  And even if
                  // the return code is good, the host may still become
                  // disconnected before it can fetch the report.  If a bad
                  // return value occurs, the call has no effect, and execution
                  // continues.  If the host becomes disconnected, execution
                  // will soon arrive at USB_connectionState(), and the sending
                  // will shut down.
              }
              break;

             // These cases are executed while your device is:
             // physically disconnected from the host;
             // enumerated on the host but suspended; or
             // connected to a powered hub without a USB host present
             case ST_PHYS_DISCONNECTED:
             case ST_ENUM_SUSPENDED:
             case ST_PHYS_CONNECTED_NOENUM_SUSP:
                 // Shut down sending and enter LPM3, which is OK when USB is
                 // in suspend or disconnected.
                 bButton1Pressed = FALSE;
                 bButton2Pressed = FALSE;
                 charLeftToSend = 0;
                 bUsbSendComplete = TRUE;
                 __bis_SR_register(LPM3_bits + GIE);
                 _NOP();
                 break;

             case ST_ENUM_IN_PROGRESS:
             default:;
        }
    }  //while(1)
} //main()



/*  
 * ======== UNMI_ISR ========
 */
#pragma vector = UNMI_VECTOR
__interrupt void UNMI_ISR (void)
{
    switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
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
            SYSBERRIV = 0; // Clear bus error flag
            USB_disable(); // Disable
    }
}



void prepSendingStr(char* path)
{
    FIL fileObj;       // FatFs file object
    FATFS fileSysObj;  // FatFs handle for a volume
    FRESULT result;    // Stores FatFS result codes
    WORD bytesRead;    // Returns the number of bytes actually read

    // We fetch the string each button press, in case the user changed it via
    // the host OS, which they can do at any time.

    // Mount the volume and open the file
    f_mount(0, &fileSysObj);
    result = f_open(&fileObj, (TCHAR*)path, FA_OPEN_EXISTING | FA_READ);

    // It's possible the user could delete the file, so we check for this.
    if (result == FR_OK)
    {
        // Get the size of the file, which is also the size of the string
        btnStrLen = f_size(&fileObj);

        // If it's >BTNSTR_SIZE, truncate the upcoming read to BTNSTR_SIZE
        if (btnStrLen >= BTNSTR_SIZE)
        {
            btnStrLen = BTNSTR_SIZE;
        }

        // Read the file and close it
        result = f_read(&fileObj, btnStr, btnStrLen, &bytesRead);
        f_close(&fileObj);

        if (result == FR_OK)
        {
            // Start the sending process.  Setting charLeftToSend to a non-zero
            // value will begin the sending of USB reports, one char's
            // down/up-press at a time.
            charLeftToSend = btnStrLen;  // Start the char countdown
            bKeyDirIsDown = TRUE;        // First press will be a downpress
        }
    }
}
