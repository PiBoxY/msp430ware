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
//*****************************************************************************
//  MSP430xG46x Demo - OA1, Inverting PGA Mode
//
//  Description: Configure OA1 for Inverting PGA mode. In this mode, the
//  "+" terminal must be supplied with an offset, since the OA is a
//  single-supply opamp, and the input must be positive. If an offset is
//  not supplied, the opamp will try to drive its output negative, which
//  cannot be done. In this example, the offset is provided by DAC120
//  and is 1.5V. The "-" terminal is connected to the R ladder tap and
//  the OAFBRx bits select the gain. The input signal should be AC coupled.
//  ACLK = 32.768kHz, MCLK = SMCLK = default DCO
//
//                   MSP430xG461x
//                -------------------
//            /|\|                XIN|-
//             | |                   |
//             --|RST            XOUT|-
//               |                   |
//    "-" --||-->|P6.4/OA1I0         |
//               |                   |
//               |          P6.3/OA1O|-->  OA1 Output
//               |                   |     Gain is -7
//
// K. Quiring / A. Dannenberg
// Texas Instruments Inc.
// June 2007
// Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//*****************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  ADC12CTL0 = REFON + REF2_5V;              // Reference = 2.5V for DAC0
  DAC12_0CTL = DAC12IR + DAC12AMP_2 + DAC12ENC;
  DAC12_0DAT = 0x099A;                      // Offset level = 1.5V

  OA1CTL0 = OAP_2+OAPM_1+OAADC1;            // Select inputs, power mode
  OA1CTL1 = OAFC_6+OAFBR_6 + OARRIP;        // Inverting PGA mode,
                                            // amplifier gain is -7,
                                            // limited range (see datasheet)

  __bis_SR_register(LPM3_bits + GIE);       // LPM3
}
