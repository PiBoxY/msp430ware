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
//  MSP-FET430P120 Demo - ADC10, DTC Sample A10 32x to Flash, Int Ref, DCO
//
//  Description: Use DTC to sample A10 32x, Int reference, and transfer
//  code directly to Flash. Software writes to ADC10SC to trigger sample
//  burst. Timing for Flash programming is important and must meet the device
//  f(FTG) datasheet specification ~ (257kHz)/35 - (476kHz)/35
//  Assume default DCO = SMCLK ~ 800kHz.
//  As programmed;
//  Each ADC10 sample & convert = (SMCLK)/(64+13) = SMCLK/77 = 96us
//  Flash write per word = (SMCLK/2)/35 = SMCLK/70 = 88us
//  Enough time is provided between ADC10 conversions for each word moved by
//  the DTC to Flash to program. DTC transfers ADC10 code to Flash 1080h-10BEh.
//  In the Mainloop, MSP430 waits in LPM0 durring conversion and programming,
//  ADC10_ISR(DTC) will force exit from any LPMx in Mainloop on reti.
//  //* MSP430F12x2 or MSP430F11x2 Device Required *//
//
//               MSP430F1232
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |A10              |
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL1 = INCH_10 + ADC10SSEL_3 + CONSEQ_2;
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + MSC + REFON + ADC10ON + ADC10IE;
  __enable_interrupt();                     // Enable interrupts.
  TACCR0 = 1500;                            // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  LPM0;                                     // Wait for delay.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  __disable_interrupt();
  ADC10DTC1 = 0x20;                         // 32 conversions
  FCTL2 = FWKEY + FSSEL_2 + FN0;            // SMCLK/2

  for (;;)
  {
   FCTL3 = FWKEY;                           // Lock = 0
   FCTL1 = FWKEY+ERASE;                     // Erase bit = 1
   *(unsigned int *)0x1080 = 0;             // Dummy write to erase Flash segment

    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = 0x1080;                       // Data buffer start
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready

    FCTL1 = FWKEY + WRT;                    // Write bit = 1
    ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    FCTL3 = FWKEY + LOCK;                   // Lock = 1
    __no_operation();                       // << SET BREAKPOINT HERE
  }
}


// ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(CPUOFF);      // Clear CPUOFF bit from 0(SR)
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void ta0_isr(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) ta0_isr (void)
#else
#error Compiler not supported!
#endif
{
  TACTL = 0;
  LPM0_EXIT;                                // Exit LPM0 on return
}// ta0_isr()
