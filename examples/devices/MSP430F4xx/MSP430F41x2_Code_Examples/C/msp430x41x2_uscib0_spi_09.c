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
//   msp430x41x2 Demo - USCI_B0, SPI 3-Wire Master Incremented Data
//
//   Description: SPI master talks to SPI slave using 3-wire mode. Incrementing
//   data is sent by the master starting at 0x01. Received data is expected to
//   be same as the previous transmission.  USCI RX ISR is used to handle
//   communication with the CPU, normally in LPM0. If high, P5.1 indicates
//   valid data reception.  Because all execution after LPM0 is in ISRs,
//   initialization waits for DCO to stabilize against ACLK.
//   ACLK = n/a, MCLK = SMCLK = DCO ~ 1048kHz.  BRCLK = SMCLK/2
//
//   Use with SPI Slave Data Echo code example.  
//
//                    MSP430F41x2
//                 -----------------
//             /|\|              XIN|-
//              | |                 |  
//              --|RST          XOUT|-
//                |                 |
//                |             P6.2|-> Data Out (UCB0SIMO)
//                |                 |
//          LED <-|P5.1         P6.1|<- Data In (UCB0SOMI)
//                |                 |
//  Slave reset <-|P1.1         P6.4|-> Serial Clock Out (UCB0CLK)
//
//
//  P. Thanigai
//  Texas Instruments Inc.
//  January 2009
//  Built with CCE Version: 3.1 and IAR Embedded Workbench Version: 4.11
//******************************************************************************
#include <msp430.h>

unsigned char MST_Data,SLV_Data;

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  FLL_CTL0 |= XCAP11PF;                     // Configure load caps

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?
                                       
  P1OUT |= 0x02;                            // P1 setup for slave reset
  P1DIR |= 0x02;                            //
  P5OUT |= 0x02;                            // P5 setup for LED
  P5DIR |= 0x02;                            //

  P6SEL |= BIT1 + BIT2 + BIT4;              // P6.1,2,4 option select
  UCB0CTL0 |= UCMST+UCSYNC+UCCKPL+UCMSB;    //3-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0x02;                           // /2
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCB0RXIE;                          // Enable USCI_A0 RX interrupt

  P1OUT &= ~0x02;                           // Now with SPI signals initialized,
  P1OUT |= 0x02;                            // reset slave

  for(i=50;i>0;i--);                        // Wait for slave to initialize

  MST_Data = 0x001;                         // Initialize data values
  SLV_Data = 0x000;                         //

  UCB0TXBUF = MST_Data;                     // Transmit first character

  __bis_SR_register(LPM0_bits + GIE);       // CPU off, enable interrupts
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIA0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  volatile unsigned int i;

  while (!(IFG2 & UCB0TXIFG));              // USCI_B0 TX buffer ready?
  if (UCB0RXBUF==SLV_Data)                  // Test for correct character RX'd
    P5OUT |= 0x02;                          // If correct, light LED
  else
    P5OUT &= ~0x02;                         // If incorrect, clear LED

  MST_Data++;                               // Increment data
  SLV_Data++;
  UCB0TXBUF = MST_Data;                     // Send next value; "TX/RX of master happens here"

  for(i=30;i>0;i--);                        // Add time between transmissions to
}                                           // make sure slave can keep up
