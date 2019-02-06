/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//   MSP430F461x Demo - USCI_A0 UART TX single byte and USCI_B0 SPI Master TX 
//                       single byte to MSP430 Slave
//
//   Description: This demo connects two MSP430's via SPI and UART. The master
//   transmits to the slave. This is the master code. It continuously
//   transmits a single byte of data starting at 0 and incrementing from both 
//   SPI and UART. This demonstrates how to implement an SPI master using the 
//   USCI_B0 TX interrupt and UART transmission using the USCI_A0 TX interrupt 
//   simultaneously.
//
//	***to be used with msp430xG46x_uscia0b0_uart_spi_02.c***
//
//                                 
//                MSP430F461x                 MSP430F461x
//                    slave                       master
//              -----------------           -----------------
//            -|XIN              |         |              XIN|-
//       32kHz |                 |         |                 | 32kHz
//            -|XOUT             |         |             XOUT|-
//             |    P3.1/UCB0SIMO|<--------|P3.1/UCB0SIMO    |
//             |                 |         |                 |
//             |    P3.2/UCB0SOMI|-------->|P3.2/UCB0SOMI    |
//             |                 |         |                 |
//             |     P3.3/UCB0CLK|<--------|P3.3/UCB0CLK     |
//             |                 |         |                 |
//             |     P4.6/UCA0TXD|<------->|P4.7/UCA0RXD     |
//             |                 |         |                 |
//             |     P4.7/UCA0RXD|<------->|P4.6/UCA0TXD     |
//             |                 |         |                 |
//
//
//   Y. Yang
//   Texas Instruments Inc.
//   January 2013
//   Built with CCE Version: 5.2.1 and IAR Embedded Workbench Version: 6.4
//******************************************************************************
#include <msp430.h>

void UART_Initialization(void);
void SPI_Initialization(void);

unsigned int UART_Data = 0;
unsigned int SPI_Data = 0;

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                 // Stop WDT
  
  UART_Initialization();                    // Initialize UART
  SPI_Initialization();                     // Initialize SPI

  while (1)
  {
    __bis_SR_register(CPUOFF|GIE);          // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
    __no_operation();                                 // Set breakpoint >>here<< and read
                                            // read out the UART_Data and 
                                            // SPI_Data variables
  }
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it transmits UART data on USCI_A0 
// and SPI data on USCI_B0. 
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if(IFG2&UCA0TXIFG)                        // Check for UART TX
    UCA0TXBUF = UART_Data++;                // Load TX buffer
  
  if(IFG2&UCB0TXIFG)                        // Check for SPI TX
    UCB0TXBUF = SPI_Data++;                 // Load TX buffer
  
  __bic_SR_register_on_exit(CPUOFF|GIE);      // Exit LPM0
}

void UART_Initialization(void)
{
  P4SEL |= 0x0C0;                           // Set P4.7,6 as USCI_A0 RXD/TXD
  UCA0CTL1 |= UCSSEL_2|UCSWRST;             // SMCLK, Reset
  UCA0BR0 = 0x09;                           // 1MHz 115200
  UCA0BR1 = 0x00;                           // 1MHz 115200
  UCA0MCTL = 0x02;                          // Modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0TXIE;                          // Enable USCI_A0 TX interrupt
}

void SPI_Initialization(void)
{ 
  P3SEL |= 0x0E;                            // Select P3.3,2,1 as peripheral module
  UCB0CTL1 = UCSWRST;                       // Reset
  UCB0CTL0 |= UCMST|UCSYNC|UCCKPL|UCMSB;    // 3-pin, 8-bit SPI master
  UCB0CTL1 |= UCSSEL_2|UCSWRST;             // SMCLK, keep reset
  UCB0BR0 = 0x02;                           // SMCLK/2 Baud rate
  UCB0BR1 = 0;                              //
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCB0TXIE;                          // Enable USCI_A0 TX interrupt 
}
