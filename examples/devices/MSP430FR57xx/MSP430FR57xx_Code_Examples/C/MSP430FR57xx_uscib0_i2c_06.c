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
//  MSP430FR57xx Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the master code. It continuously
//  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
//  *** to be used with "MSP430FR57xx_uscib0_i2c_07.c" ***
//
//                                /|\  /|\
//                MSP430FR5739    10k  10k     MSP430FR5739
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
//            |                 |          |                 |
//
//   P. Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with CCS V4 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>

unsigned char TXData =0;                    // Pointer to TX data
unsigned char TXByteCtr;

int main(void)

{
    WDTCTL = WDTPW + WDTHOLD;
    // Init SMCLK = MCLk = ACLK = 1MHz
    CSCTL0_H = 0xA5;
    CSCTL1 |= DCOFSEL0 + DCOFSEL1;          // Set max. DCO setting = 8MHz
    CSCTL2 = SELA_3 + SELS_3 + SELM_3;      // set ACLK = MCLK = DCO
    CSCTL3 = DIVA_3 + DIVS_3 + DIVM_3;      // set all dividers to 1MHz

    // Configure Pins for I2C
    P1SEL1 |= BIT6 + BIT7;                  // Pin init

    UCB0CTLW0 |= UCSWRST;                   // put eUSCI_B in reset state
    UCB0CTLW0 |= UCMODE_3 + UCMST + UCSSEL_2;// I2C master mode, SMCLk 
    UCB0BRW = 0x8;                          // baudrate = SMCLK / 8
    UCB0I2CSA = 0x48;                       // address slave is 48hex
    UCB0CTLW0 &=~ UCSWRST;	            //clear reset register
    UCB0IE |= UCTXIE0 + UCNACKIE;           //transmit and NACK interrupt enable

    while(1)
    {
    __delay_cycles(1000);                   // Delay between transmissions
    TXByteCtr = 1;                          // Load TX byte counter              
    while (UCB0CTLW0 & UCTXSTP);            // Ensure stop condition got sent
    UCB0CTLW0 |= UCTR + UCTXSTT;            // I2C TX, start condition

    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
    TXData++;                               // Increment data byte
    }
	
}



	
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR 
__interrupt void USCIB0_ISR(void) 
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCIB0_ISR (void)
#else
#error Compiler not supported!
#endif
{	
  switch(__even_in_range(UCB0IV,0x1E)) 
  {
        case 0x00: break;                    // Vector 0: No interrupts break;
        case 0x02: break;
        case 0x04:
          UCB0CTLW0 |= UCTXSTT;             //resend start if NACK
          break;                            // Vector 4: NACKIFG break;
        case 0x18: 
	  if (TXByteCtr)                    // Check TX byte counter
          {
            UCB0TXBUF = TXData;             // Load TX buffer
            TXByteCtr--;                    // Decrement TX byte counter
          }
          else
          {
            UCB0CTLW0 |= UCTXSTP;           // I2C stop condition
            UCB0IFG &= ~UCTXIFG;            // Clear USCI_B0 TX int flag
            __bic_SR_register_on_exit(CPUOFF);// Exit LPM0
          }
          break;                            // Vector 26: TXIFG0 break;
        default: break;
  }
}
	
