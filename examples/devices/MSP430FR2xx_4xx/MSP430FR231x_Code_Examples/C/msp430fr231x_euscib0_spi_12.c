/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//   MSP430FR231x Demo - eUSCI_B0, SPI 4-Wire Slave Data Echo
//                       eUSCI_B0 port remapped to P2.2-P2.5.
//   Description: SPI slave talks to SPI master using 4-wire mode. Data received
//   from master is echoed back.
//   eUSCI_B0 remap functionality is controlled by the USCIBRMP bit of SYSCFG2
//   register. Set USCIBRMP bit to remap eUSCI_B0 port to P2.2-P2.5.
//   ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//   Note: Ensure slave is powered up before master to prevent delays due to
//   slave init.
//
//
//                   MSP430FR2311
//                -----------------
//            /|\|                 |
//             | |                 |
//             --|RST              |
//               |                 |
//               |             P2.2|-> Slave Select (UCB0STE)
//               |             P2.3|-> Serial Clock Out (UCB0CLK)
//               |             P2.5|-> Data Out (UCB0SOMI)
//               |             P2.4|<- Data In (UCB0SIMO)
//
//
//   Darren Lu
//   Texas Instruments Inc.
//   Oct. 2016
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1
//******************************************************************************
#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer
  
  P2SEL0 |= BIT2 | BIT3 | BIT4 | BIT5;      // set 4-SPI pin as second function
  SYSCFG2 |= USCIBRMP;                      // eUSCI_B0 port remapped to P2.2-P2.5
  
  UCB0CTLW0 |= UCSWRST;                     // **Put state machine in reset**
                                            // 4-pin, 8-bit SPI slave
  UCB0CTLW0 |= UCSYNC|UCCKPL|UCMSB|UCMODE_1|UCSTEM;
                                            // Clock polarity high, MSB
  UCB0CTLW0 &= ~UCSSEL;
  UCB0CTLW0 |= UCSSEL__SMCLK;               // Select SMCLK
  UCB0BR0 = 0x02;                           // BRCLK = SMCLK/2
  UCB0BR1 = 0;                              //
  UCB0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
  UCB0IE |= UCRXIE;                         // Enable USCI_B0 RX interrupt
  
  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

  __bis_SR_register(LPM0_bits | GIE);       // Enter LPM0, enable interrupts
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    while (!(UCB0IFG&UCTXIFG));             // USCI_B0 TX buffer ready?
    UCB0TXBUF = UCB0RXBUF;                  // Echo received data
}
