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
//  MSP430F543xA Demo - USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
//						using a repeated restart.
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave, issues a repeated restart, and then receives data
//  from the slave. This is the SLAVE CODE. The interrupt driven data reception
//  is demonstrated using the USCI_B0 RX interrupt.  The interrupt driven data
//  transmission is demonstrated using the USCI_B0 TX interrupt.  The USCI_B0
//  STT interrupt is used to load the transmit buffer if the slave is configured
//  as a transmitter.
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
//
//      ***to be used with msp430x54xA_uscib0_i2c_12.c***
//
//                                /|\  /|\
//                MSP430F5438A     10k  10k     MSP430F5438A
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

unsigned char *PRxData;                     // Pointer to RX data
unsigned char RXByteCtr;
volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM
unsigned char temp = 0;

unsigned char *PTxData;                     // Pointer to TX data
const unsigned char TxData[] =              // Table of data to transmit
{
  0x66,
  0x77,
  0x88,
  0x99,
  0xAA
};

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB0I2COA = 0x48;                         // Own Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCSTPIE + UCSTTIE + UCRXIE + UCTXIE;
                                            // Enable STT, STP, RX & TX interrupt

  PRxData = (unsigned char *)RxBuffer;      // Start of RX buffer
  RXByteCtr = 0;                            // Clear RX byte count

  PTxData = (unsigned char *)TxData;        // Start of TX buffer

  while (1)
  {
    __bis_SR_register(LPM0_bits + GIE);     // Enter LPM0, enable interrupts
  }
}

//------------------------------------------------------------------------------
// The USCI_B0 data ISR RX vector is used to move received data from the I2C
// master to the MSP430 memory.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// The USCI_B0 state ISR TX vector is used to wake up the CPU from LPM0 in order
// to process the received data in the main program. LPM0 is only exit in case
// of a (re-)start or stop condition when actual data was received.
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
  case  2: break;							// Vector  2: ALIFG
  case  4: break;		                    // Vector  4: NACKIFG
  case  6:                                  // Vector  6: STTIFG
    if (UCB0CTL1 & UCTR){
    	PTxData = (unsigned char *)TxData;  // Start of TX buffer
		UCB0TXBUF = *PTxData++;			    // Transmit data at address PTxData
    }
    break;
  case  8:                                  // Vector  8: STPIFG
    break;
  case 10:                                  // Vector 10: RXIFG
	*PRxData++ = UCB0RXBUF;                 // Get RX'd byte into buffer
	RXByteCtr++;
    break;
  case 12:                                  // Vector 12: TXIFG
    UCB0TXBUF = *PTxData++;                 // Transmit data at address PTxData
    break;
  default: break;
  }  
}
