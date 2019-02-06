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
//  MSP-FET430P140 Demo - I2C, Master Interface to DAC8571, Read/Write
//
//  Description: I2C communication with a DAC8571 in read and write mode is
//  demonstrated. MSP430 writes a 0x00 to the DAC to initialize communication
//  and then reads the value from the DAC, increments it by 1, and Transmits
//  it back to the DAC8571 in a repetitive manner to generate a ramp
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k, SCL = SMCLK/10
//  //* MSP430F169 Device Required *//
//
//                                 /|\  /|\
//                  DAC8571        10k  10k     MSP430F169
//                    slave         |    |        master
//              -----------------|  |    |  -----------------
//             |             SDA |<-|---+->|P3.1             |
//             |                 |  |      |                 |
//             |                 |  |      |                 |
//      GND <--|A0           SCL |<-+----->|P3.3             |
//             |                 |         |                 |
//             |                 |         |                 |
//             |              Vdd|<---+--->|Vcc              |
//             |             Vref|<---|    |                 |
//             |              GND|<------->|GND              |
//             |                 |         |                 |
//
//
//   DAC8571 I2C address = 0x4C (A0 = GND)
//
//
//  H. Grewal
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

void Commstart (void);
void RXBytes (void);
void TXBytes (void);
volatile unsigned int i;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Disable the Watchdog.
  P3SEL |= 0x0a;                            // Assign I2C pins to module
  U0CTL |= I2C + SYNC;                      // Switch USART0 to I2C mode
  U0CTL &= ~I2CEN;                          // Recommended init procedure
  I2CTCTL = I2CSSEL_2;                      // SMCLK
  I2CNDAT = 0x03;                           // Transmit Three byte
  I2CSA = 0x4C;                             // Slave address
  U0CTL |= I2CEN;                           // Enable I2C, 7 bit addr,
  Commstart();                              // MSP430 sends a 0x00 to the DAC
  while(1)
  {
    RXBytes();                              // RX 3 Bytes (MSB, LSB, Control)
    TXBytes();                              // TX 3 Bytes (Control, MSB, LSB)
  }
}


void Commstart (void)
{
  U0CTL |= MST;                             // Master mode
  I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;          // Initiate transfer
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = 0x10;                            // Load  Control Byte
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = 0x00;                            // Load  MSByte
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = 0x00;                            // Load  LSByte
  while ((I2CTCTL & I2CSTP) == 0x02);       // To prevent Arbitration Lost
}

void RXBytes (void)
{
  volatile unsigned char ctlbyte;
  U0CTL |= MST;                             // Master mode
  I2CTCTL = I2CSTT+I2CSTP;                  // Initiate transfer
  while ((I2CIFG & RXRDYIFG) == 0);         // Wait for Receiver to be ready
  i = I2CDRB;                               // Receive MSByte from DAC
  i = i << 8;
  while ((I2CIFG & RXRDYIFG) == 0);         // Wait for Receiver to be ready
  i = i + I2CDRB;                           // Receive LSByte from DAC
  while ((I2CIFG & RXRDYIFG) == 0);         // Wait for Receiver to be ready
  ctlbyte = I2CDRB;                         // Receive ControlByte from DAC
  while ((I2CTCTL & I2CSTP) == 0x02);       // Wait for Stop Condition
}

void TXBytes (void)
{
  i = i + 1;
  U0CTL |= MST;                             // Master mode
  I2CTCTL |= I2CSTT+I2CSTP+I2CTRX;          // Initiate transfer
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = 0x10;                            // Load  Control Byte
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = i >> 8;                          // Load  MSByte
  while ((I2CIFG & TXRDYIFG) == 0);         // Wait for transmitter to be ready
  I2CDRB = i & 0x00FF;                      // Load  LSByte
  while ((I2CTCTL & I2CSTP) == 0x02);       // Wait for Stop Condition
}
