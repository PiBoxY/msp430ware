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
//  MSP-FET430P140 Demo - USART0, SPI Interface to TLV5616 DAC
//
//  Description: Demonstrate USART0 in SPI mode interface to TLV5616 DAC.
//  USART0 is used to transmit data to DAC, software generated frame sync
//  pulse, DAC is updated inside CCR0 ISR operating in continuos mode.
//  ACLK= n/a, MCLK= SMCLK= UCLK= default DCOCLK
//  //* SWRST** please see MSP430x1xx Users Guide for description *//
//
//               MSP430F149
//            -----------------
//        /|\|              XIN|-
//         | |                 |           TLV5616
//         --|RST          XOUT|-       -------------
//           |             P3.0|------>|FS        OUT|--> ~ 390Hz
//           |       SIMO0/P3.1|------>|DIN          |
//           |       UCLK0/P3.3|------>|SCLK       CS|-|
//           |                 |       |             | v
//
//
//  M. Buccini
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

unsigned int pointer;                       // 16-bit value to write

//------------------------------------------------------------------------------
// 12-bit Sine Lookup table with 32 steps
//------------------------------------------------------------------------------
static unsigned int Sin_tab[32] = {
        2048,
        2447,
        2831,
        3185,
        3495,
        3750,
        3939,
        4056,
        4095,
        4056,
        3939,
        3750,
        3495,
        3185,
        2831,
        2447,
        2048,
        1648,
        1264,
        910,
        600,
        345,
        156,
        39,
        0,
        39,
        156,
        345,
        600,
        910,
        1264,
        1648
};

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0xA;                             // P3.1,3 SPI option select
  P3DIR |= 0xB;                             // P3.0,1,3 output direction
  P3OUT &= ~0x1;                            // FS reset
  ME1 |= USPIE0;                            // Enable USART0 SPI
  UCTL0 |= CHAR + SYNC + MM;                // 8-bit SPI Master **SWRST**
  UTCTL0 = CKPH + CKPL + SSEL1 + STC;       // Inv. delayed, SMCLK, 3-pin
  UBR00 = 0x2;                              // ACLK/2 for baud rate
  UBR10 = 0x0;                              // ACLK/2 for baud rate
  UMCTL0 = 0x0;                             // Clear modulation
  UCTL0 &= ~SWRST;                          // Initialize USART state machine
  pointer = 0;                              // Clear pointer
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 64-1;                              // ~ 390Hz Clock period
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, Up-mode

  __bis_SR_register(LPM0_bits + GIE);  	    // Enter LPM0 w/ interrupt
}

// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  P3OUT |= 0x1;                             // FS set
  P3OUT &= ~0x1;                            // FS reset
  TXBUF0 = Sin_tab[pointer] >> 8;
  TXBUF0 = Sin_tab[pointer];
  pointer++;
  pointer &= 0x1F;
}
