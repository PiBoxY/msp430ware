/* --COPYRIGHT--,BSD_EX
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
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR57x Demo - Secure JTAG access using a password
//
//  Description: The FR5739 JTAG interface can be secured to allow only password
//  protected access to the device. This is done by saving a special JTAG 
//  signature, the length of the password and the actual password in the JTAG 
//  signature location. The code example shows how this is done. 
//  Once this example is executed NO FURTHER ACCESS to the device is possible 
//  unless the password is proivded via the tool chain. 
//  IAR V5.10 or greater is required to provide the tool chain support for the 
//  password check. 
//  After the first time, use the JTAG password 0x22221111 via the tool chain to
//  access the device. The password can be supplied at the IAR project options 
//  menu by going to Project --> Options --> FET Debugger --> Download and 
//  entering the password in the 'JTAG password' window. 
//
//           MSP430FR5739
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2011
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************
#include <msp430.h>

//pointer init 

unsigned int *JTAG1 = 0;
unsigned int *JTAG2 = 0;
unsigned long *JTAGPWD = 0;

int main(void)
{  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT  


  JTAG1 = (unsigned int *)0xFF80;          // JTAG signature 1
  JTAG2 = (unsigned int *)0xFF82;          // JTAG signature 2
  JTAGPWD = (unsigned long *)0xFF88;       // JTAG password location
   
  *JTAG1 = 0xAAAA;                          // Enable JTAG lock/unlock with pwd
  *JTAG2 = 0x0002;                          // Pwd length = 2 words
  *JTAGPWD = 0x11112222;                    // Pwd = 0x22221111
  
  // To disable the password mechanism write 0xFFFF to JTAG1 and JTAG2 locations
  // Also erase JTAGPWD location

  
  while(1);  

}
