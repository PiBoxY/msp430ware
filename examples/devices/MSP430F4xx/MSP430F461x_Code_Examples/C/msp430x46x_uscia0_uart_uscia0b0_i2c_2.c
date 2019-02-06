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
//   MSP430F41x Demo - USCI_A0 UART RX single byte and USCI_B0 I2C Slave RX 
//                      single byte from MSP430 Master
//
//   Description: This demo connects two MSP430's via the I2C bus and UART. 
//   The master transmits single bytes to the slave. This is the slave code. 
//   This demonstrates how to implement an I2C slave receiver using the USCI_B0 
//   RX interrupt and UART reception using the USCI_A0 RX interrupt 
//   simultaneously.
//
//
//	***to be used with msp430xG46x_uscia0_uart_uscia0b0_i2c_1.c***
//
//                                 /|\  /|\
//                MSP430F461x     10k  10k    MSP430F461x
//                    slave         |    |        master
//              -----------------   |    |  -----------------
//            -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
//       32kHz |                 |  |      |                 | 32kHz
//            -|XOUT             |  |      |             XOUT|-
//             |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
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
void I2C_Initialization(void);

unsigned char I2C_ByteCtr;
unsigned int UART_Data;
unsigned int I2C_Data;

int main(void)
{
  WDTCTL = WDTPW|WDTHOLD;                   // Stop WDT
  
  UART_Initialization();                    // Initialize UART
  I2C_Initialization();                     // Initialize I2C
  
  while (1)
  {
    I2C_ByteCtr = 0;                        // Clear I2C byte count
    __bis_SR_register(LPM0_bits|GIE);       // Enter LPM0 w/ interrupts enabled       
                                            // Remain in LPM0 until master
                                            // finishes TX
    __no_operation();                       // Set breakpoint >>here<< and read
  }                                         // read out the UART_Data and 
                                            // I2C_Data buffers
}

//------------------------------------------------------------------------------
// The USCIB0 data ISR is used to move received data from the I2C master
// to the I2C_Data buffer.
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
  I2C_Data = UCB0RXBUF;                     // Store RX'ed I2C data
  I2C_ByteCtr++;                            // Increment I2C byte count
}

//------------------------------------------------------------------------------
// The USCIB0 state ISR is used to wake up the CPU from LPM0 in order to
// process the I2C received data in the main program and storing received  
// UART data. LPM0 is only exit in caseof a (re-)start or stop condition when 
// actual data was received. 
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
    UART_Data = UCA0RXBUF;                     // Store RX'ed UART data
  
  if (I2C_ByteCtr)                             // Check I2C byte counter
  {
    UCB0STAT &= ~(UCSTPIFG|UCSTTIFG);          // Clear interrupt flags
    __bic_SR_register_on_exit(LPM0_bits|GIE);  // Exit LPM0 if data was received
  }
}

void UART_Initialization(void)
{
  P4SEL |= 0x0C0;                           // Set P4.7,6 as USCI_A0 RXD/TXD
  UCA0CTL1 = UCSWRST;                        // Reset
  UCA0CTL1 |= UCSSEL_2|UCSWRST;             // SMCLK, keep reset
  UCA0BR0 = 0x09;                           // 1MHz 115200
  UCA0BR1 = 0x00;                           // 1MHz 115200
  UCA0MCTL = 0x02;                          // Modulation
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void I2C_Initialization(void)
{
  P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3|UCSYNC;               // I2C Slave, synchronous mode
  UCB0I2COA = 0x48;                         // Own Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0I2CIE |= UCSTPIE;                     // Enable STP condition interrupt
  IE2 |= UCB0RXIE;                          // Enable RX interrupt
}
