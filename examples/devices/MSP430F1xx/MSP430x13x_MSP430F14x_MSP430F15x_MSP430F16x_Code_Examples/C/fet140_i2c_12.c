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
//  MSP-FET430P140 Demo - I2C, Master Interface to DAC8571, Write
//
//  Description: I2C communication with a DAC8571 write mode is
//  demonstrated. MSP430 writes to the DAC and increments it by
//  256 in a repetitive manner to generate a ramp
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 800k, SCL = SMCLK/10
//  //* MSP430F169 Device Required *//
//
//                                 /|\  /|\
//                  DAC8571        10k  10k     MSP430F169
//                    slave         |    |        master
//              -----------------|  |    |  -----------------
//             |             SDA |<-|---+->|P3.1         Xin |---
//             |                 |  |      |             Xout|---
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

unsigned char ctlbyte = 0x10;
unsigned int DAC_Data = 0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Disable the Watchdog.
  P3SEL |= 0x0A;                            // Assign pins to module function
  U0CTL |= I2C + SYNC;                      // Switch USART0 to I2C mode
  U0CTL &= ~I2CEN;                          // Recommended init procedure
  I2CTCTL = I2CSSEL_2;                      // SMCLK
  I2CNDAT = 0x03;                           // Transmit Three bytes
  I2CSA = 0x4C;                             // Slave address
  U0CTL |= I2CEN;                           // Enable I2C, 7 bit addr
  I2CIE = TXRDYIE;                          // I2C receive ready interrupt enable
  U0CTL |= MST;                             // Master
  I2CTCTL |= I2CTRX + I2CSTT + I2CSTP;      // Transmit, ST, SP (clears MST)

  while(1)
  {
   __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
   DAC_Data = DAC_Data + 256;
   while ((I2CTCTL & I2CSTP) == 0x02);       // I2C Stopped?
   U0CTL |= MST;                             // Master
   I2CTCTL |= I2CTRX + I2CSTT + I2CSTP;      // Transmit, ST, SP (clears MST)
  }
}

// I2C Interrupt Vector (I2CIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0TX_VECTOR
__interrupt void USART0 (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0TX_VECTOR))) USART0 (void)
#else
#error Compiler not supported!
#endif
{
  static unsigned int count = 0;            // Counter to determine CtlByte
                                            // or MSByte or LSByte Transfer
  switch( I2CIV )
  {
   case 0x0C:                               // TXRDYIFG
   {
    if (count == 0)
    {
     I2CDRB = ctlbyte;                      // Transmit Control Byte
     count ++;
    }
    else if (count == 1)
    {
     I2CDRB = DAC_Data >> 8;                // Transmit MSByte to DAC
     count ++;
    }
    else
    {
     I2CDRB = (DAC_Data & 0x00FF);          // Transmit LSByte to DAC
     count = 0;
     LPM0_EXIT;                             // CPU Active on ISR exit
    }
    break;
   }
  }
}
