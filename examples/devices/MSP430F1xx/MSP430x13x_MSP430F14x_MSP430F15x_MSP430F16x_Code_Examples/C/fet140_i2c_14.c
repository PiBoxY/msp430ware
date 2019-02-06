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
//   MSP-FET430P140 Demo - Switch Between UART0 & Master I2C Transmitter
//
//   Description: This code shows proper switching between UART and I2C
//   UART0 Puts out A-Z, CR, LF on the Hyperterminal Screen
//   and then Transmits 0x5A over I2C. This happens continuously
//   This is the I2C Master Code. For Slave, use fet140_i2c_07.c
//   ACLK = n/a, MCLK = SMCLK = I2CCLOCK = DCO ~ 800kHz
//   //* MSP430F169 Device Required *//
//
//                                 /|\  /|\
//                  MSP430F169     10k  10k     MSP430F169
//                    slave         |    |        master
//              -----------------|  |    |  -----------------
//             |             P3.1|<-|---+->|P3.1             |
//             |                 |  |      |             P3.4|--------->
//             |                 |  |      |                 |2400 - 8N1
//             |             P3.3|<-+----->|P3.3         P3.5|<---------
//             |                 |         |                 |
//
//
//
//  H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>


void InitSerialPort(void);
void I2C_TX(void);                          // Function prototypes
void InitI2C(void);


int main(void)
{
 unsigned char i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;
  P1OUT = 0;
  P3SEL |= 0x3A;                            // UART and I2C pin option select
  P3DIR |= 0x10;                            // P3.4 = output direction
  P3OUT |= 0x10;                            // P3.4 = 1, when in I2C mode

  for (;;)
  {
  InitSerialPort();
  P3SEL |= 0x10;                            // Port 3.4 set for UART

  i = 'A';
  while (i != '\n' + 1 )
  {
    if (i==('Z'+1))
      i = '\r';
    else if (i==('\r'+1))                   // CR
      i = '\n';                             // LF

    while (!(IFG1 & UTXIFG0));              // UART TX buffer ready?
    TXBUF0 = i++;                           // Output a character
    while (!(U0TCTL & TXEPT));              // UART TX buffer empty?
  }

  P3SEL &= ~ 0x10;                          // Clear UART 3.4 Port setting for I2C
  InitI2C();
  I2C_TX();                                 // Transmit char over I2C
  }
}

void InitSerialPort(void)
{
  I2CIE = 0x00;
  U0CTL = 0;                                // **clear U0CTL register**
  U0CTL = SWRST;                            // **Reset UART state machine**
  UCTL0 |= CHAR;                            // 8-bit character
  UTCTL0 |= SSEL0;                          // UCLK = ACLK
  UBR00 = 0x0D;                             // 32k/2400 - 13.65
  UBR10 = 0x00;                             //
  UMCTL0 = 0x6B;                            // Modulation
  UMCTL0 = 0x00;
  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  U0CTL &= ~SWRST;                          // Initialize USART state machine
}

void I2C_TX(void)
{
  while (I2CBUSY & I2CDCTL);                // I2C ready?
  I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;          // Initiate transfer
  I2CDRB = 0x5A;                            // Copy data to I2CDRB
  while (I2CBUSY & I2CDCTL);                // I2C ready?
  while (I2CBB & I2CDCTL);                  // I2C ready?
  P1OUT = 0x01;
  P1OUT = 0;
}

void InitI2C(void)
{
  U0CTL |= SWRST;                           // Reset USART state machine
  U0CTL &= ~CHAR;
  U0CTL |= I2C+SYNC;                        // Recommended I2C init procedure
  U0CTL &= ~I2CEN;                          // Recommended I2C init procedure
  I2CTCTL |= I2CSSEL1;                      // SMCLK for I2C
  I2CSA = 0x0048;                           // Address of Slave
  U0CTL |= MST;                             // Master mode
  I2CNDAT = 0x01;                           // Write one byte
  U0CTL |= I2CEN;                           // Enable I2C
}
