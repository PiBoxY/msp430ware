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
//   MSP-FET430P140 Demo - I2C, Master Interface to DAC8571 in Repeat Mode
//   Master = '169, Slave = DAC8571
//
//   Description: Using the I2C interrupt TXRDY, a continuous
//   sine wave is output to the external DAC using a 16-point look-up table.
//   Only one start condition is executed. Data is handled in bytes by the I2C
//   module and the CPU is normally in LPM0.
//   ACLK = n/a, MCLK = SMCLK = I2CCLOCK = DCO ~ 800kHz
//  //* MSP430F169 Device Required *//
//
//                    MSP430x169                         DAC8571
//                  ---------------                   ------------
//                 |            SDA|<--------------->|SDA         |
//                 |     I2C    SCL|---------------->|SCL  I2C    |
//                 |   MASTER      |                 |    SLAVE   |
//                 |               |              GND|A0          |
//
//
//   DAC8571 I2C address = 0x4C (A0 = GND)
//
//  Z. Albus
//  Texas Instruments Inc.
//  Feb 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.21A
//******************************************************************************

#include <msp430.h>

//------------------------------------------------------------------------------
// Flash - 16-bit Sine Lookup table with 16 steps
//------------------------------------------------------------------------------
const char Sin_tab[32] = {
       0xFF,
       0xFF,
       0xF6,
       0x40,
       0xDA,
       0x81,
       0xB0,
       0xFA,
       0x7F,
       0xFF,
       0x4F,
       0x03,
       0x25,
       0x7C,
       0x09,
       0xBD,
       0x00,
       0x00,
       0x09,
       0xBD,
       0x25,
       0x7C,
       0x4F,
       0x03,
       0x7F,
       0xFF,
       0xB0,
       0xFA,
       0xDA,
       0x81,
       0xF6,
       0x40
};

static char i = 0;                       // Data Pointer

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;              // Stop watchdog
  P3SEL |= 0x0A;                         // Assign I2C pins to module
  U0CTL |= I2C + SYNC;                   // Switch USART0 to I2C mode
  U0CTL &= ~I2CEN;                       // Disable I2C
  I2CTCTL = I2CRM + I2CSSEL1;            // S/W controls data flow, SMCLK
  I2CSA = 0x4C;                          // Slave address
  I2CIE = TXRDYIE;                       // Enable I2C interrupts
  U0CTL |= I2CEN;                        // Enable I2C

  U0CTL |= MST;                          // Master
  I2CTCTL |= I2CTRX + I2CSTT;            // Write, ST
  I2CDRB = 0x10;                         // Write DAC control byte

  __bis_SR_register(LPM0_bits + GIE);    // Enter LPM0 w/ interrupt
}

// USART0 I2C interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0TX_VECTOR
__interrupt void I2C_IV (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0TX_VECTOR))) I2C_IV (void)
#else
#error Compiler not supported!
#endif
{
  switch( I2CIV )
  {
  case 0x02: break;                      // ALIFG: n/a
  case 0x04: break;                      // NACKIFG: n/a
  case 0x06: break;                      // OAIFG: n/a
  case 0x08: break;                      // ARDYIFG: n/a
  case 0x0A: break;                      // RXRDYIFG: n/a
  case 0x0C: I2CDRB = Sin_tab[i];        // TXRDYIFG: Write next byte
             i = (i+1) & 0x1F;           // Increment & maintain pointer
             break;
  case 0x0E: break;                      // GCIFG: n/a
  case 0x10: break;                      // STTIFG: n/a
 }
}
