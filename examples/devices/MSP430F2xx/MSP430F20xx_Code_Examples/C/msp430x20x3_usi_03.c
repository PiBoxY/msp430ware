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
//  MSP430F20x2/3 Demo - SPI full-Duplex 3-wire Slave
//
//  Description: SPI Master communicates full-duplex with SPI Slave using
//  3-wire mode. The level on P1.4 is TX'ed and RX'ed to P1.0.
//  Master will pulse slave reset for synch start.
//  ACLK = n/a, MCLK = SMCLK = Default DCO
//
//                Slave                      Master
//               MSP430F20x2/3              MSP430F20x2/3
//             -----------------          -----------------
//            |              XIN|-    /|\|              XIN|-
//            |                 |      | |                 |
//            |             XOUT|-     --|RST          XOUT|-
//            |                 | /|\    |                 |
//            |          RST/NMI|--+<----|P1.2             |
//      LED <-|P1.0             |        |             P1.4|<-
//          ->|P1.4             |        |             P1.0|-> LED
//            |         SDI/P1.7|<-------|P1.6/SDO         |
//            |         SDO/P1.6|------->|P1.7/SDI         |
//            |        SCLK/P1.5|<-------|P1.5/SCLK        |
//
//  M. Buccini / L. Westlund
//  Texas Instruments Inc.
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
  P1OUT =  0x10;                        // P1.4 set, else reset
  P1REN |= 0x10;                        // P1.4 pullup
  P1DIR = 0x01;                         // P1.0 output, else input
  USICTL0 |= USIPE7 + USIPE6 + USIPE5 + USIOE; // Port, SPI slave
  USICTL1 |= USIIE;                     // Counter interrupt, flag remains set
  USICTL0 &= ~USISWRST;                 // USI released for operation
  USISRL = P1IN;                        // init-load data
  USICNT = 8;                           // init-load counter

  __bis_SR_register(LPM0_bits + GIE);   // Enter LPM0 w/ interrupt
}

// USI interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USI_VECTOR
__interrupt void universal_serial_interface(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USI_VECTOR))) universal_serial_interface (void)
#else
#error Compiler not supported!
#endif
{
  if (0x10 & USISRL)
    P1OUT |= 0x01;
  else
    P1OUT &= ~0x01;
  USISRL = P1IN;
  USICNT = 8;                           // re-load counter
}
