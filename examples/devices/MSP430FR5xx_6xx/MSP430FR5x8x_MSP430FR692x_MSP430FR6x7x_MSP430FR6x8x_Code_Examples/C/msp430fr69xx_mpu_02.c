/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//  MSP430FR69xx Demo - MPU Write protection violation - PUC
//
//  Description: The MPU segment boundaries are defined by:
//  Border 1 = 0x6000 [MPUSEGB1 = 0x0600]
//  Border 2 = 0x8000 [MPUSEGB2 = 0x0800]
//  Segment 1 = 0x4400 - 0x5FFF
//  Segment 2 = 0x6000 - 0x7FFF
//  Segment 3 = 0x8000 - 0x23FFF
//  Segment 2 is write protected. Any write to an address in the segment 2 range
//  causes a PUC. The LED toggles after reset once MPUSEG2IFG is set. A delay is included
//  so the debugger can gain access via JTAG.
//
//  ACLK = n/a, MCLK = default DCO = 1MHz
//
//
//           MSP430FR6989
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

unsigned int *ptr = 0;
unsigned int Data =0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;                            // Configure P1.0 for LED

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  while (MPUCTL1 & MPUSEG2IFG)              // has reset occurred due to Seg2
  {
    P1OUT ^= BIT0;                          // Toggle LED
    __delay_cycles(30000);                  // Delay to see toggle
  }

  // Configure MPU
  MPUCTL0 = MPUPW;                          // Write PWD to access MPU registers
  MPUSEGB1 = 0x0600;                        // B1 = 0x6000; B2 = 0x8000
  MPUSEGB2 = 0x0800;                        // Borders are assigned to segments
  
  //  Segment 1 - Execute, Read
  //  Segment 2 - Violation, Execute, Read
  //  Segment 3 - Execute, Read
  MPUSAM =             MPUSEG1RE | MPUSEG1XE |
           MPUSEG2VS | MPUSEG2RE | MPUSEG2XE |
                       MPUSEG3RE | MPUSEG3XE |
                       MPUSEGIRE | MPUSEGIXE | MPUSEGIWE;
  MPUCTL0 = MPUPW | MPUENA;               // Enable MPU protection

  Data = 0x88;

  // Cause an MPU violation by writing to segment 2
  ptr = (unsigned int  *)0x6002;
  *ptr = Data;

  while(1);                                 // Code never gets here
}
