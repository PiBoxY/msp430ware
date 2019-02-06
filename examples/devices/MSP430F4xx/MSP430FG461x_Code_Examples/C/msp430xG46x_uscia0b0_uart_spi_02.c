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
//   MSP430xG41x Demo - USCI_A0 UART RX single byte and USCI_B0 SPI Slave RX 
//                      single byte from MSP430 Master
//
//   Description: This demo connects two MSP430's via SPI and UART. The master
//   transmits single bytes to the slave. This is the slave code. This 
//   demonstrates how to implement an SPI slave using the USCI_B0 RX interrupt
//   and UART reception using the USCI_A0 RX interrupt simultaneously.
//
//
//
//	***to be used with msp430xG46x_uscia0b0_uart_spi_01.c***
//
//                MSP430xG461x                 MSP430xG461x
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
//   Tim Love/Brandon Elliott
//   Texas Instruments Inc.
//   March 2008
//   Built with IAR Embedded Workbench Version: 5.11
//******************************************************************************
#include <msp430.h>

void UART_Initialization(void);
void SPI_Initialization(void);

unsigned int UART_Data;
unsigned int SPI_Data;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  UART_Initialization();                    // Initialize UART
  SPI_Initialization();                     // Initialize SPI
  
  while (1)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupt        
                                            // Remain in LPM0 until master
                                            // finishes TX
    __no_operation();                       // Set breakpoint >>here<< and read
  }                                         // read out the UART_Data and 
                                            // SPI_Data variables
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it stores received UART data on 
// USCI_A0 and SPI data on USCI_B0.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if(IFG2&UCA0RXIFG)
    UART_Data = UCA0RXBUF;                  // Store RX'ed UART data
  
  if(IFG2&UCB0RXIFG)
    SPI_Data = UCB0RXBUF;                   // Store RX'ed SPI data
}

void UART_Initialization(void)
{
  P4SEL |= 0x0C0;                           // P4.7,6 = USCI_A0 RXD/TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 0x09;                           // 1MHz 115200
  UCA0BR1 = 0x00;                           // 1MHz 115200
  UCA0MCTL = 0x02;                          // Modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void SPI_Initialization(void)
{
  while(!(P3IN&0x08));                      // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode
  P3SEL |= 0x0E;                            // P3.3,2,1 option select
  UCB0CTL1 = UCSWRST;                       // **Put state machine in reset**
  UCB0CTL0 |= UCSYNC+UCCKPL+UCMSB;          // 3-pin, 8-bit SPI master
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCB0RXIE;                          // Enable USCI_B0 RX interrupt
}
