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
//  MSP430F22x4 Demo - OA1, Comparator Mode
//
//  Description: Configure OA1 for Comparator mode. In this example, OA1
//  is configured as a comparator. The comparator reference is connected to
//  the "-" input and comes from the integrated R ladder. The reference level
//  is selected with the OAFBR bits. The "+" terminal is connected to OA1I0.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430F22x4
//             -------------------
//         /|\|                XIN|-
//          | |                   |
//          --|RST            XOUT|-
//            |                   |
//     "+" -->|P2.4/A4/OA1I0      |
//            |                   |
//            |       P2.3/A3/OA1O|--> OA1 Output
//            |                   |    Output goes high when
//            |                   |    Input > (0.25 * AVcc)
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  March 2006
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  OA1CTL0 = OAPM_1 + OAADC1;                // Slow slew rate,
                                            // "+" connected to OA1I0 (default),
                                            // Output connected to A3/OA1O
  OA1CTL1 = OAFBR_4 + OAFC_3 + OARRIP;      // "-" connected to AVcc * 4/16,
                                            // Comparator mode,
                                            // Reverse resistor connection

  ADC10AE0 = 0x18;                          // P2.4/3 analog function select
  LPM3;                                     // Enter LPM3
}
