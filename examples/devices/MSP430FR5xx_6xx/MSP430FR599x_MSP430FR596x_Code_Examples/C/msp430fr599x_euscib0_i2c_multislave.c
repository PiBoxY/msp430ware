/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2015, Texas Instruments Incorporated
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
//  MSP430FR5x9x Demo - eUSCI_B2 I2C 4 Hardware I2C slaves
//
//  Description: This demo connects two MSP430's via the I2C bus.
//  This code configures the MSP430 USCI to be addressed as 4 independent I2C
//  slaves. Each slave has its owm interrupt flag and data variable to store
//  incoming data.
//  Use with MSP430FR5x9x_eusciB2_i2c_master_multislave.c
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
//
//                                /|\  /|\
//                MSP430FR5994    10k  10k     MSP430FR5994
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P7.0/UCB2SDA|<-|----+->|P7.0/UCB2SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P7.1/UCB2SCL|<-+------>|P7.1/UCB2SCL     |
//            |                 |          |                 |
//            |                 |          |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   October 2015
//   Built with IAR Embedded Workbench V6.30 & Code Composer Studio V6.1
//******************************************************************************
#include <msp430.h>

volatile unsigned char RXData0 = 0;
volatile unsigned char RXData1 = 0;
volatile unsigned char RXData2 = 0;
volatile unsigned char RXData3 = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;

    // Configure GPIO
    P7SEL0 |= BIT0 | BIT1;
    P7SEL1 &= ~(BIT0 | BIT1);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Configure USCI_B2 for I2C mode
    UCB2CTLW0 = UCSWRST;                    // Software reset enabled
    UCB2CTLW0 |= UCMODE_3 | UCSYNC;         // I2C mode, sync mode
    UCB2I2COA0 = 0x0A | UCOAEN;             // SLAVE0 own address is 0x0A+ enable
    UCB2I2COA1 = 0x0B | UCOAEN;             // SLAVE1 own address is 0x0B+ enable
    UCB2I2COA2 = 0x0C | UCOAEN;             // SLAVE2 own address is 0x0C+ enable
    UCB2I2COA3 = 0x0D | UCOAEN;             // SLAVE3 own address is 0x0D+ enable
    UCB2CTLW0 &= ~UCSWRST;                  // clear reset register

    UCB2IE |= UCRXIE0 | UCRXIE1 | UCRXIE2 | UCRXIE3;//receive interrupt enable

    __bis_SR_register(LPM0_bits | GIE);     // Enter LPM0 w/ interrupts
    __no_operation();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = EUSCI_B2_VECTOR
__interrupt void USCI_B2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_B2_VECTOR))) USCI_B2_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(UCB2IV, USCI_I2C_UCBIT9IFG))
    {
        case USCI_NONE:          break;     // Vector 0: No interrupts
        case USCI_I2C_UCALIFG:   break;     // Vector 2: ALIFG
        case USCI_I2C_UCNACKIFG: break;     // Vector 4: NACKIFG
        case USCI_I2C_UCSTTIFG:  break;     // Vector 6: STTIFG
        case USCI_I2C_UCSTPIFG:  break;     // Vector 8: STPIFG
        case USCI_I2C_UCRXIFG3:             // Vector 10: RXIFG3
            RXData3 = UCB2RXBUF;            // SLAVE3 - Get RX data
            break;
        case USCI_I2C_UCTXIFG3:  break;     // Vector 12: TXIFG3
        case USCI_I2C_UCRXIFG2:             // Vector 14: RXIFG2
            RXData2 = UCB2RXBUF;            // SLAVE2 - Get RX data
            break;
        case USCI_I2C_UCTXIFG2:  break;     // Vector 16: TXIFG2
        case USCI_I2C_UCRXIFG1:             // Vector 18: RXIFG1
            RXData1 = UCB2RXBUF;            // SLAVE1 - Get RX data
            break;
        case USCI_I2C_UCTXIFG1:  break;     // Vector 20: TXIFG1
        case USCI_I2C_UCRXIFG0:             // Vector 22: RXIFG0
            RXData0 = UCB2RXBUF;            // SLAVE0 - Get RX data
            break;
        case USCI_I2C_UCTXIFG0:  break;     // Vector 24: TXIFG0
        case USCI_I2C_UCBCNTIFG: break;     // Vector 26: BCNTIFG
        case USCI_I2C_UCCLTOIFG: break;     // Vector 28: clock low timeout
        case USCI_I2C_UCBIT9IFG: break;     // Vector 30: 9th bit
        default: break;
    }
}
