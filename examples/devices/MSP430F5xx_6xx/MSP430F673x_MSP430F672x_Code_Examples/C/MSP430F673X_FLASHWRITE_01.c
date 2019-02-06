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
//  MSP430F673x Demo - Flash In-System Programming, Single-Byte Copy SegC to SegD
//
//  Description: This program first erases flash segment C, then it writes
//  incrementing values in segment C, then it erases segment D, then copies
//  segment C to segment D. Starting addresses of segments defined in this code:
//  Segment C-0x1880, Segment D-0x1800. RESET the device to re-execute code.
//  This is implemented to prevent stressing of Flash unintentionally.  Run
//  program until it reaches the breakpoint indicated below, then use the
//  debugger's memory browser to view segment C and segment D.
//  ACLK = REFO = 32kHz, MCLK = SMCLK = default DCO 1048576Hz
//
//               MSP430F673x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

char value;                                 // 8-bit value to write to segment C

// Function prototypes
void write_SegC(char value);
void copy_C2D(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    value = 0;                              // Initialize value

    write_SegC(value);                      // Write segment C, increment value
    copy_C2D();                             // Copy segment C to D

    while (1) ;                             // Loop forever, SET BREAKPOINT HERE
}

//------------------------------------------------------------------------------
// Input = value, holds value to write to Seg C
//------------------------------------------------------------------------------
void write_SegC(char value)
{
    unsigned int i;
    char *Flash_ptr;                        // Initialize Flash pointer

    Flash_ptr = (char *)0x1880;

    FCTL3 = FWKEY;                          // Clear Lock bit
    FCTL1 = FWKEY | ERASE;                  // Set Erase bit
    *Flash_ptr = 0;                         // Dummy write to erase Flash seg
    FCTL1 = FWKEY | WRT;                    // Set WRT bit for write operation

    for (i = 0; i < 128; i++)
    {
        *Flash_ptr++ = value++;             // Write value to flash
    }

    FCTL1 = FWKEY;                          // Clear WRT bit
    FCTL3 = FWKEY | LOCK;                   // Set LOCK bit
}

//------------------------------------------------------------------------------
// Copy Seg C to Seg D
//------------------------------------------------------------------------------
void copy_C2D(void)
{
    unsigned int i;
    char *Flash_ptrC;
    char *Flash_ptrD;

    Flash_ptrC = (char *) 0x1880;           // Initialize Flash segment C ptr
    Flash_ptrD = (char *) 0x1800;           // Initialize Flash segment D ptr

    FCTL3 = FWKEY;                          // Clear Lock bit
    FCTL1 = FWKEY | ERASE;                  // Set Erase bit
    *Flash_ptrD = 0;                        // Dummy write to erase Flash seg D
    FCTL1 = FWKEY | WRT;                    // Set WRT bit for write operation

    for (i = 0; i < 128; i++)
    {
        *Flash_ptrD++ = *Flash_ptrC++;      // copy value segment C to seg D
    }

    FCTL1 = FWKEY;                          // Clear WRT bit
    FCTL3 = FWKEY | LOCK;                   // Set LOCK bit
}

