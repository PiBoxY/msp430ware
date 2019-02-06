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
//  MSP430x26x Demo - Basic Clock, VLO-Driven Timer with VLO Compensation
//
//  This example toggles P1.0 approx. every second, using a timer driven by the
//  VLO.  The VLO freq can vary between 4kHz-20kHz, so the timer compare value 
//  must be adjusted to compensate, in order to produce a one-second interval.  
//  So in effect, this code performs a kind of "calibration" of the VLO.  This 
//  is done by counting the number of 1MHz DCO pulses in one ACLK=VLO/8 period, 
//  assigned to dco_delta.  VLO is /8 in order to increase the number of DCO 
//  pulses in one ACLK pulse, increasing the resolution of the measurement, 
//  and therefore the accuracy of the one-second interval.  Since 1,000,000 
//  DCO pulses at 1MHz are required to produce a one-second interval, the 
//  number of VLO/8 pulses in one second is given by 1M/dco_delta.  This value 
//  is loaded into TACCR0 as the timer compare value.  
 
//  MSP430x2xx devices are shipped with calibration values in info memory that,
//  when used by software, allow the DCO to operate within approx. +/-1% of 
//  common values such as 1MHz.  (See the datasheet for add'l information.)  
//  Through the algorithm shown here, this accuracy is extended to the VLO.  An
//  additional source of error in this algorithm is that the VLO/8 freq will 
//  most likely not be evenly divisible by the DCO freq, and therefore up to 
//  one DCO pulse may be lost.  At nominal values, there are 400 1MHz DCO 
//  pulses in one 2.5kHz VLO/8 pulse, so losing one clock pulse produces 
//  ~0.25% error, in addition to the error tolerance of the calibrated DCO.  
//  ACLK = VLO/8, MCLK = SMCLK = 1MHz
//
//                MSP430F261x/241x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |                 | 
//            |        P1.0/ACLK|-->LED 1 second interrupt
//
//  
//  B. Nisarga
//  Texas Instruments Inc.
//  September 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h> 

unsigned int dco_delta;

// Function Prototype
unsigned int  measureVLO( void );

int main(void)
{
  WDTCTL = WDTPW +WDTHOLD;                  // Stop Watchdog Timer
  P1DIR |= 0x01;                            // P1.0 output
 
  dco_delta = measureVLO();                 // dco delta = number of
                                            // 1MHz cycles in 8 VLO cycles
  //Clock settings: DCO freq = 1MHz; ACLK = VLO/8
  
  // Timer A interval interrupt setup
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = (1000000 / dco_delta);             
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode
  P1OUT = 0x01;
  
  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt

}

// Timer_A0 Interrupt Vector 
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A0 (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= 0x01;                            // Toggle P1.0
}

// This function measures the number of 1MHz clock pulses in 8 VLO cycles
unsigned int  measureVLO( void )
{
  static unsigned int start_count, stop_count;
  // Clock settings to achieve the good resolution for VLO freq measurement
  if (CALBC1_1MHZ==0xFF)					// If calibration constant erased
  {											
    while(1);                               // do not load, trap CPU!!	
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set range
  DCOCTL = CALDCO_1MHZ;                     // Set DCO step + modulation
  TACCTL2 = CM_1+CCIS_1+CAP;                // CAP, ACLK
  TACTL = TASSEL_2+MC_2+TACLR;              // SMCLK, cont-mode, clear
  BCSCTL3 |= LFXT1S_2;                      // ACLK = VLO  
  BCSCTL2 = 0x0;
  BCSCTL1 |= DIVA_3;                        // ACLK = VLO/8
  
  TACCTL2 &= ~CCIFG;                        // Clear capture flag
  while (!(CCIFG & TACCTL2));               // Test capture flag to skip 1st cycle
  TACCTL2 &= ~CCIFG;                        // Clear capture flag
  
  while (!(CCIFG & TACCTL2));               // Test capture flag to skip 2nd cycle
  start_count = TACCR2;                     // save hardware captured value
  TACCTL2 &= ~CCIFG;                        // Clear capture flag
  
  while (!(CCIFG & TACCTL2));               // Test capture flag to capture a good clock
  TACTL &= ~MC_3;                           // Stop timer
  stop_count = TACCR2;
  return(stop_count - start_count);         // return
}
