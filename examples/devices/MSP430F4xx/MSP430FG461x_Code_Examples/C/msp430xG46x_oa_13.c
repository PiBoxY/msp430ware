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
//   MSP430xG46x Demo - 3-Amp Differential Amplifier with OA1, OA2, and OA0
//
//   Description: Configure OA1, OA2, and OA0 as a 3-Amp Differential Amp.
//   In this configuration, the R2/R1 ratio sets the gain. The R ladders for
//   OA1 and OA0 form the R2/R1 dividers. The OAFBRx settings for both OA1
//   and OA0 must be equal.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO
//
//                |\
//                | \ OA1
//   V2-----------|+ \           R1             R2
//                |   |----+---/\/\/\/---+----/\/\/\/----|
//            ----|- /     |             |               |
//            |   | /      |             |  |\          GND
//            |   |/       |             |  | \ OA0
//            |____________|             ---|+ \
//                                          |   |--------+--------->
//                                       ---|- /         |
//                |\                     |  | /          | Vout = (V2-V1)xR2/R1
//                | \ OA2                |  |/           |        (Gain is 3)
//   V1-----------|+ \           R1      |      R2       |
//                |   |----+---/\/\/\/---+----/\/\/\/----|
//            ----|- /     |
//            |   | /      |
//            |   |/       |
//            |____________|
//
//
//                 MSP430xG461x
//              -------------------
//          /|\|                XIN|-
//           | |                   |
//           --|RST            XOUT|-
//             |                   |
//       V2 -->|P6.4/OA1I0         |
//       V1 -->|P6.6/OA2I0         |
//             |                   |
//             |          P6.1/OA0O|--> Diff Amp Output
//
//   K. Quiring / A. Dannenberg
//   Texas Instruments Inc.
//   June 2007
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  OA0CTL0 = OAN_3+OAP_3+OAPM_1+OAADC1;      // Select inputs, output
  OA0CTL1 = OAFC_6+OAFBR_4+OARRIP;          // Inverting PGA mode,
                                            // OAFBRx sets gain
                                            // limited range (see datasheet)

  OA1CTL0 = OAPM_1;                         // Select inputs, power mode
  OA1CTL1 = OAFC_1+OAFBR_4+OARRIP;          // Unity gain mode,
                                            // OAFBRx sets gain
                                            // limited range (see datasheet)

  OA2CTL0 = OAPM_1;                         // Select inputs, power mode
  OA2CTL1 = OAFC_7+OARRIP;                  // Differential amp mode
                                            // limited range (see datasheet)

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3
}
