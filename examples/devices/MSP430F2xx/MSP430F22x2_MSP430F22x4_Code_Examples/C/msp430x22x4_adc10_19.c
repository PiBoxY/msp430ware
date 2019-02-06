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
//   MSP430F22x4 Demo - ADC10, DTC Sample A0 64x, AVcc, HF XTAL
//
//   Description: For non-low power applications HF XTAL can source ACLK and be
//   used also for CPU MCLK, as in this example. DTC always uses MCLK for data
//   transfer. Though not required, ADC10 can also clock from HF XTAL as in this
//   example using ACLK.
//   MCLK = ACLK = ADC10CLK = HF XTAL in this example.
//   DTC Used to sample A0 64 times with reference to AVcc. Software writes to
//   ADC10SC to trigger sample burst. In Mainloop MSP430 waits in LPM0 to save
//   power until ADC10 conversion burst complete, ADC10_ISR(DTC) will force
//   exit from LPM0 in Mainloop on reti. In this example ACLK times sample
//   period (16x) and conversion (13x). DTC transfers conversion code to RAM
//   200h - 280h. P1.0 set at start of conversion burst, reset on completion.
//   //* HF XTAL NOT INSTALLED ON FET *//
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 | HF XTAL (3 – 16MHz crystal or resonator)
//          --|RST          XOUT|-
//            |                 |
//        >---|P2.0/A0      P1.0|-->LED
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   April 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 |= XTS;                           // LFXT1 = HF XTAL
  BCSCTL3 |= LFXT1S1;                       // LFXT1S1 = 3-16Mhz
  do
  {
    IFG1 &= ~OFIFG;                         // Clear OSC fault flag
    i = 0xFF;                               // i = Delay
    while (i--);                            // Additional delay to ensure start
  }
  while (OFIFG & IFG1);                     // OSC fault flag set?
  BCSCTL2 |= SELM_3;                        // MCLK = LFXT1

  ADC10CTL1 = ADC10SSEL_1 + CONSEQ_2;
  ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
  ADC10AE0 |= 0x01;                         // P2.0 ADC option select
  ADC10DTC1 = 0x040;                        // 64 conversions
  P1DIR |= 0x001;                           // P1.0 output

  while(1)
  {
    P1OUT |= 0x01;                          // P1.0 = 1
    ADC10CTL0 &= ~ENC;
    while (BUSY & ADC10CTL1);
    ADC10SA = 0x0200;                       // Data buffer start
    ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
    __bis_SR_register(LPM0_bits + GIE);     // LPM0, ADC12 ISR will force exit
    P1OUT &= ~0x01;                         // P1.0 = 0
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM0_bits);     // Exit LPM0, interrupts enabled
}

