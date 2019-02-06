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
//   MSP430xG46x Demo - USART1, SPI 3-Wire Slave Data Echo
//
//   Description: SPI slave talks to SPI master using 3-wire mode. Data received
//   from master is echoed back.  USART1 RX ISR is used to handle communication,
//   CPU normally in LPM4.  Prior to initial data exchange, master pulses
//   slaves RST for complete reset.
//   ACLK = 32.768kHz, MCLK = SMCLK = DCO = ~1048kHz
//
//   Use with SPI Master Incremented Data code example.  If the slave is in
//   debug mode, the reset signal from the master will conflict with slave's
//   JTAG; to work around, use IAR's "Release JTAG on Go" on slave device.  If
//   breakpoints are set in slave RX ISR, master must stopped also to avoid
//   overrunning slave RXBUF.
//
//                MSP430xG461x
//             -----------------
//         /|\|              XIN|-
//          | |                 |   32kHz xtal
//          | |             XOUT|-
// Master---+-|RST              |
//            |             P4.3|<- Data In (SIMO1)
//            |                 |
//            |             P4.4|-> Data Out (SOMI1)
//            |                 |
//            |             P4.5|<- Serial Clock In (UCLK1)
//
//
//   K. Quiring/ M. Mitchell
//   Texas Instruments Inc.
//   March 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************
#include <msp430.h>

int main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  FLL_CTL0 |= XCAP14PF;                     // Configure load caps

  // Wait for xtal to stabilize
  do
  {
  IFG1 &= ~OFIFG;                           // Clear OSCFault flag
  for (i = 0x47FF; i > 0; i--);             // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  for(i=2100;i>0;i--);                      // Now with stable ACLK, wait for
                                            // DCO to stabilize.
  while(!(P4IN&0x20));                      // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode
  P4SEL |= 0x038;                           // P4.5,4,3 SPI option select
  U1CTL = CHAR+SYNC+SWRST;                  // 8-bit, SPI, Slave
  U1TCTL |= CKPL+STC;                       // Polarity, UCLK, 3-wire
  ME2 |= USPIE1;                            // Module enable
  U1CTL &= ~SWRST;                          // SPI enable
  IE2 |= URXIE1;                            // Receive interrupt enable

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}

// Echo character
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void USART1RX_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1RX_VECTOR))) USART1RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG2 & UTXIFG1));                // USART1 TX buffer ready?
  U1TXBUF = U1RXBUF;
}
