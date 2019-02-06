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
//  MSP430FR69xx Demo - MPU Write protection violation - Interrupt notification
//
//  Description: The MPU segment boundaries are defined by:
//  Border 1 = 0x6000 [MPUSEGB1 = 0x0600]
//  Border 2 = 0x8000 [MPUSEGB2 = 0x0800]
//  Segment 1 = 0x4400 - 0x5FFF
//  Segment 2 = 0x6000 - 0x7FFF
//  Segment 3 = 0x8000 - 0x23FFF
//  Segment 2 is write protected. Any write to an address in the segment 2 range
//  causes an NMI interrupt. The LED toggles after accessing SYS NMI ISR.
//
//  ACLK = n/a, MCLK = SMCLK = default DCO
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

unsigned char SYSNMIflag = 0;
unsigned int *ptr = 0;
unsigned int Data = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

  // Configure GPIO
  P1DIR |= BIT0;                            // Configure P1.0 for LED

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure MPU
  MPUCTL0 = MPUPW;                          // Write PWD to access MPU registers
  MPUSEGB1 = 0x0600;                        // B1 = 0x6000; B2 = 0x8000
  MPUSEGB2 = 0x0800;                        // Borders are assigned to segments
  
  //  Segment 1 - Execute, Read
  //  Segment 2 - Execute, Read
  //  Segment 3 - Execute, Read
  MPUSAM = MPUSEG1RE | MPUSEG1XE |
           MPUSEG2RE | MPUSEG2XE |
           MPUSEG3RE | MPUSEG3XE |
           MPUSEGIRE | MPUSEGIWE| MPUSEGIXE;
  MPUCTL0 = MPUPW | MPUENA | MPUSEGIE;    // Enable MPU protection
                                          // MPU registers locked until BOR
  Data = 0x88;

  // Cause an MPU violation by writing to segment 2+
  ptr = (unsigned int *)0x6002;
  *ptr = Data;

  __delay_cycles(100);

  while(SYSNMIflag)                         // Has violation occurred due to Seg2
  {
    P1OUT ^= BIT0;                          // Toggle LED
    __delay_cycles(100000);                 // Delay to see toggle
  }

  // No violation - trap here
  while(1);
}

// System NMI vector
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = SYSNMI_VECTOR
__interrupt void SYSNMI_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(SYSNMI_VECTOR))) SYSNMI_ISR (void)
#else
#error Compiler not supported!
#endif

{
  switch (__even_in_range(SYSSNIV, SYSSNIV_CBDIFG))
  {
  case SYSSNIV_NONE: break;
  case SYSSNIV_RES02: break;
  case SYSSNIV_UBDIFG: break;
  case SYSSNIV_RES06: break;
  case SYSSNIV_MPUSEGPIFG: break;
  case SYSSNIV_MPUSEGIIFG: break;
  case SYSSNIV_MPUSEG1IFG: break;
  case SYSSNIV_MPUSEG2IFG:
    MPUCTL1 &= ~MPUSEG2IFG;                 // Clear violation interrupt flag
    SYSNMIflag = 1;                         // Set flag
    break;
  case SYSSNIV_MPUSEG3IFG: break;
  case SYSSNIV_VMAIFG: break;
  case SYSSNIV_JMBINIFG: break;
  case SYSSNIV_JMBOUTIFG: break;
  case SYSSNIV_CBDIFG: break;
  default: break;
  }
}
