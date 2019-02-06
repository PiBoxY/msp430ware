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
//  MSP430FR69xx Demo - eUSCI_B0 I2C Master TX bytes to Multiple Slaves
//
//  Description: This demo connects two MSP430's via the I2C bus.
//  The master transmits to 4 different I2C slave addresses 0x0A,0x0B,0x0C&0x0D.
//  Each slave address has a specific related data in the array TXData[].
//  At the end of four I2C transactions the slave address rolls over and begins
//  again at 0x0A.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//  Use with MSP430FR69xx_uscib0_i2c_multislave.c
//
//                                /|\  /|\
//                MSP430FR6989    10k  10k     MSP430FR6989
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
//            |                 |          |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>

const unsigned char TXData[] = { 0xA1, 0xB1, 0xC1, 0xD1 };
const unsigned char SlaveAddress[] = { 0x0A, 0x0B, 0x0C, 0x0D };
volatile unsigned char TXByteCtr;
volatile unsigned char SlaveFlag = 0;

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;

  // Configure GPIO
  P1SEL0 |= BIT6 | BIT7;                    // I2C pins

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  // Configure USCI_B0 for I2C mode
  UCB0CTLW0 = UCSWRST;                      // put eUSCI_B in reset state
  UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK; // I2C master mode, SMCLK
  UCB0BRW = 0x8;                            // baudrate = SMCLK / 8
  UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
  UCB0IE |= UCTXIE0 | UCNACKIE;             // transmit and NACK interrupt enable

  SlaveFlag = 0;                            // Initialize SlaveFlag

  while(1)
  {
    __delay_cycles(1000);                   // Delay between transmissions
    UCB0I2CSA = SlaveAddress[SlaveFlag];    // configure slave address
    TXByteCtr = 1;                          // Load TX byte counter
    while (UCB0CTLW0 & UCTXSTP);            // Ensure stop condition got sent

    UCB0CTLW0 |= UCTR | UCTXSTT;            // I2C TX, start condition

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
    // Change Slave address
    SlaveFlag++;
    if (SlaveFlag > 3)                      // Roll over slave address
    {
      SlaveFlag = 0;
    }
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE:          break;         // Vector 0: No interrupts
    case USCI_I2C_UCALIFG:   break;         // Vector 2: ALIFG
    case USCI_I2C_UCNACKIFG:                // Vector 4: NACKIFG
      UCB0CTLW0 |= UCTXSTT;                 // resend start if NACK
      break;
    case USCI_I2C_UCSTTIFG:  break;         // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:  break;         // Vector 8: STPIFG
    case USCI_I2C_UCRXIFG3:  break;         // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3:  break;         // Vector 12: TXIFG3
    case USCI_I2C_UCRXIFG2:  break;         // Vector 14: RXIFG2
    case USCI_I2C_UCTXIFG2:  break;         // Vector 16: TXIFG2
    case USCI_I2C_UCRXIFG1:  break;         // Vector 18: RXIFG1
    case USCI_I2C_UCTXIFG1:  break;         // Vector 20: TXIFG1
    case USCI_I2C_UCRXIFG0:  break;         // Vector 22: RXIFG0
    case USCI_I2C_UCTXIFG0:                 // Vector 24: TXIFG0
      if (TXByteCtr)                        // Check TX byte counter
      {
        UCB0TXBUF = TXData[SlaveFlag];      // Load TX buffer
        TXByteCtr--;                        // Decrement TX byte counter
      }
      else
      {
        UCB0CTLW0 |= UCTXSTP;               // I2C stop condition
        UCB0IFG &= ~UCTXIFG;                // Clear USCI_B0 TX int flag
        __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
      }
      break;
    default: break;
  }
}
