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
//  MSP430F552x Demo - Bank Erase from a Block while Executing Code from Another
//                    Block.
//
//  Description: This program first writes to Bank 1 from 0x10000 to 0x10100.
//  Then a bank erase is done on Bank 1. During Bank erase, code is still
//  executing from Bank 0 by toggling P1.0. Toggling can only be seen by using
//  an oscilloscope.
//  Ensure Large memory model is selected.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = default DCO 1048576Hz
//  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
//
//               MSP430F552x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |             P1.0|- Oscilloscope
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************

#include <msp430.h>

// Function prototypes
void erase_Seg(void);
void wrt_Bank1(void);

char value;                                 // 8-bit value to write to Bank 1

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  P1DIR |= 0x01;                            // Set P1.0 as output
  while(1)
  {
    P1OUT &= ~ 0x01;                        // clear P1.0
    value = 0;                              // Loop forever, SET BREAKPOINT HERE
    wrt_Bank1();                            // Write to Bank 1
    erase_Seg();                            // Call erase function
    while(BUSY & FCTL3)                     // Check for erase completion
    {
      P1OUT ^= 0x01;                        // toggle LED if still erasing
    }                                       // Else, Exit if erase is done
  }
}

//------------------------------------------------------------------------------
// Erases Memory Bank 1
//------------------------------------------------------------------------------
void erase_Seg(void)
{
  char *Flash_ptr = (char *)0x10000;        // Initialize Flash pointer
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+MERAS;                      // Set Bank Erase bit
  *Flash_ptr = 0;                           // Dummy erase byte
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}

//------------------------------------------------------------------------------
// Writes incremented Value to Bank 1
//------------------------------------------------------------------------------
void wrt_Bank1(void)
{
  unsigned int i;
  char *Flash_ptr = (char *)0x10000;        // Initialize Flash pointer

  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+WRT;                        // Set WRT bit for write operation
  for(i = 0; i < 256; i++)
  {
    *Flash_ptr++ = value++;                   // Write a word to flash
  }
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
}
