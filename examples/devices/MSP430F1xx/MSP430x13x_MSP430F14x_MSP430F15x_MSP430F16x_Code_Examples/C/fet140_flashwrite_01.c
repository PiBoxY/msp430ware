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
//****************************************************************************
//  MSP-FET430P140 Demo - Flash In-System Programming, Copy SegA to SegB
//
//  Description: This program first erases flash seg A, then it increments all
//  values in seg A, then it erases seg B, then  copies seg A to seg B.
//  Assumed MCLK 550kHz - 900kHz.
//  //* Set Breakpoint on NOP in the Mainloop to avoid Stressing Flash *//
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
//  M. Mitchell
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

char  value;                                // 8-bit value to write to segment A

// Function prototypes
void  write_SegA (char value);
void  copy_A2B (void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  FCTL2 = FWKEY + FSSEL0 + FN0;             // MCLK/2 for Flash Timing Generator
  value = 0;                                // Initialize value

  while(1)                                  // Repeat forever
  {
    write_SegA(value++);                    // Write segment A, increment value
    copy_A2B();                             // Copy segment A to B
    __no_operation();                       // SET BREAKPOINT HERE
  }
}

void write_SegA (char value)
{
  char *Flash_ptr;                          // Flash pointer
  unsigned int i;

  Flash_ptr = (char *) 0x1080;              // Initialize Flash pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptr = 0;                           // Dummy write to erase Flash segment

  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<128; i++)
  {
    *Flash_ptr++ = value;                   // Write value to flash
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}


void copy_A2B (void)
{
  char *Flash_ptrA;                         // Segment A pointer
  char *Flash_ptrB;                         // Segment B pointer
  unsigned int i;

  Flash_ptrA = (char *) 0x1080;             // Initialize Flash segment A pointer
  Flash_ptrB = (char *) 0x1000;             // Initialize Flash segment B pointer
  FCTL1 = FWKEY + ERASE;                    // Set Erase bit
  FCTL3 = FWKEY;                            // Clear Lock bit
  *Flash_ptrB = 0;                          // Dummy write to erase Flash segment B
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  for (i=0; i<128; i++)
  {
    *Flash_ptrB++ = *Flash_ptrA++;           // Copy value segment A to segment B
  }

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
}
