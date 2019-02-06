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
//  MSP430F543xA Demo - USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
//						using a repeated restart.
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave, issues a repeated restart, and then receives data
//  from the slave. This is the MASTER CODE. It continuously transmits an array
//  of data and demonstrates how to implement an I2C master transmitter sending
//  multiple bytes using the USCI_B0 TX interrupt followed by a master receiver 
//  receiving multiple bytes using the USCI_B0 RX Interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
//      ***to be used with msp430x54xA_uscib0_i2c_13.c***
//
//                                /|\  /|\
//                MSP430F5438A     10k  10k      MSP430F5438A
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P3.1/UCB0SDA|<-|----+->|P3.1/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P3.2/UCB0SCL|<-+------>|P3.2/UCB0SCL     |
//            |                 |          |                 |
//
//   P. Liska
//   Texas Instruments Inc.
//   June 2012
//   Built with CCS Version: 5.1.0
//******************************************************************************

#include <msp430.h>

unsigned char *PTxData;                     // Pointer to TX data
unsigned char TXByteCtr;

const unsigned char TxData[] =              // Table of data to transmit
{
  0x11,
  0x22,
  0x33,
  0x44,
  0x55
};

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x48;                         // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCTXIE + UCRXIE;                // Enable TX and RX interrupt

  PTxData = (unsigned char *)TxData;      // TX array start address
                                          // Place breakpoint here to see each
                                          // transmit operation.
  TXByteCtr = sizeof TxData;              // Load TX byte counter

  PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
  RXByteCtr = 5;                          // Load RX byte counter

  UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

  while (1)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
  }
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count. Also, TXData
// points to the next byte to transmit.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6: break;   		                // Vector  6: STTIFG
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                  // Vector 10: RXIFG
    RXByteCtr--;                            // Decrement RX byte counter
    if (RXByteCtr)
    {
      *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
    }
    else
    {
      *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
      UCB0CTL1 |= UCTR + UCTXSTT;           // I2C TX, start condition
      PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
      RXByteCtr = 5;                          // Load RX byte counter
    }
    break;
  case 12:                                  // Vector 12: TXIFG
    if (TXByteCtr)                          // Check TX byte counter
    {
      UCB0TXBUF = *PTxData++;               // Load TX buffer
      TXByteCtr--;                          // Decrement TX byte counter
    }
    else
    {
        PTxData = (unsigned char *)TxData;  // TX array start address
                                            // Place breakpoint here to see each
                                            // transmit operation.
        TXByteCtr = sizeof TxData;          // Load TX byte counter

        UCB0CTL1 &= ~UCTR;         			// I2C RX
        UCB0CTL1 |= UCTXSTT;         		// I2C start condition

        UCB0TXBUF = *PTxData++;             // Load TX buffer
        TXByteCtr--;                        // Decrement TX byte counter
    }
  default: break;
  }
}
