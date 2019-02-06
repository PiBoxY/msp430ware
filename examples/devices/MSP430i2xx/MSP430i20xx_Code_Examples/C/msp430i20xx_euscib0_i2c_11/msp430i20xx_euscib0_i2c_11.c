/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
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
//  MSP430i20xx Demo - eUSCI_B0 I2C Slave TX Multiple Bytes to MSP430 Master
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  reads from the slave. This is the SLAVE code. The TX data begins at 0
//  and is incremented each time it is sent. A stop condition
//  is used as a trigger to initialize the outgoing data.
//                * used with "msp430i20xx_euscib0_i2c_10.c *
//
//  ACLK = 32kHz, MCLK = SMCLK = Calibrated DCO = 16.384MHz
//  * Ensure low_level_init.c is included when building/running this example *
//
//                                /|\  /|\
//               MSP430i2041      10k  10k     MSP430i2041
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//            |     P1.7/UCB0SDA|<-|----+->|P1.7/UCB0SDA     |
//            |                 |  |       |                 |
//            |                 |  |       |                 |
//            |     P1.6/UCB0SCL|<-+------>|P1.6/UCB0SCL     |
//            |                 |          |             P1.4|--> LED
//
//  T. Witt
//  Texas Instruments, Inc
//  September 2013
//  Built with Code Composer Studio v5.5
//******************************************************************************
#include "msp430.h"

volatile unsigned char TXData;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop Watchdog Timer

    P1SEL0 |=   BIT6 | BIT7;                // I2C Pins
    P1SEL1 &= ~(BIT6 | BIT7);

    // Configure USCI_B0 for I2C mode
    UCB0CTLW0 |= UCSWRST;                   // Software reset enabled
    UCB0CTLW0 |= UCMODE_3 | UCSYNC;         // I2C slave mode
    UCB0I2COA0 = 0x48 | UCOAEN;             // Address is 0x48 + enable
    UCB0CTL1 &= ~UCSWRST;
    UCB0IE |= UCTXIE0 | UCSTPIE;            // Transmit, Stop interrupt enable

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
    __no_operation();                       // For debugger
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt void USCIB0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCIB0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG)) {
        case USCI_NONE: break;
        case USCI_I2C_UCALIFG: break;
        case USCI_I2C_UCNACKIFG: break;
        case USCI_I2C_UCSTTIFG: break;
        case USCI_I2C_UCSTPIFG:
                 TXData = 0;
                 UCB0IFG &= ~UCSTPIFG;      // Clear IFG
                 break;
        case USCI_I2C_UCRXIFG3: break;
        case USCI_I2C_UCTXIFG3: break;
        case USCI_I2C_UCRXIFG2: break;
        case USCI_I2C_UCTXIFG2: break;
        case USCI_I2C_UCRXIFG1: break;
        case USCI_I2C_UCTXIFG1: break;
        case USCI_I2C_UCRXIFG0: break;
        case USCI_I2C_UCTXIFG0:
                  UCB0TXBUF = TXData++;
                 break;
        case USCI_I2C_UCBCNTIFG: break;
        case USCI_I2C_UCCLTOIFG: break;
        case USCI_I2C_UCBIT9IFG: break;
        default: break;
    }
}