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
//  MSP430F673x Demo - USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The slave
//  transmits to the master. This is the MASTER CODE. It continuously
//  receives an array of data and demonstrates how to implement an I2C
//  master receiver receiving multiple bytes using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
//  Note that the UCB0SDA and UCB0SCL lines are not assigned to a dedicated port
// and need to be assigned to PMAP ports
//
//     ***to be used with "MSP430F673X_USCIB0_I2C_11.c" ***
//
//
//                                /|\  /|\
//                MSP430F673x     10k  10k     MSP430F673x
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P2.1/UCB0SDA|<-|----+->|P2.1/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P2.0/UCB0SCL|<-+------>|P2.0/UCB0SCL     |
//            |                 |          |                 |
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

unsigned char *PRxData;                               // Pointer to RX data
unsigned char RXByteCtr;
volatile unsigned char RxBuffer[5];                   // Allocate 5 byte of RAM

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                         // Stop WDT

    // Setup P2.0 UCB0SCL, P2.1 UCB0SDA
    P2SEL |= BIT0 | BIT1;                             // Set P2.0,P2.1 to UCB0SCL, UCB0SDA

    // Setup eUSCI_B0
    UCB0CTLW0 |= UCSWRST;                             // Enable SW reset
    UCB0CTLW0 |= UCMST | UCMODE_3 | UCSSEL_2;         // I2C Master, synchronous mode
    UCB0BRW_L = 12;                                   // fSCL = SMCLK/12 = ~100kHz
    UCB0BRW_H = 0;
    UCB0I2CSA = 0x48;                                 // Slave Address is 048h
    UCB0CTLW0 &= ~UCSWRST;                            // Clear SW reset, resume operation
    UCB0IE |= UCRXIE0;                                // Enable RX interrupt

    while (1)
    {
        PRxData = (unsigned char *)RxBuffer;          // Start of RX buffer
        RXByteCtr = 5;                                // Load RX byte counter
        while (UCB0CTLW0 & UCTXSTP) ;                 // Ensure stop condition got sent
        UCB0CTLW0 |= UCTXSTT;                         // I2C start condition

        __bis_SR_register(LPM0_bits | GIE);           // Enter LPM0, enable interrupts
                                                      // Remain in LPM0 until all data
                                                      // is RX'd
        __no_operation();                             // for debugger
        __no_operation();                             // SET BREAKPOINT HERE
    }
}

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(UCB0IV, 30))
    {
        case USCI_NONE: break;                        // No interrupts
        case USCI_I2C_UCALIFG: break;                 // ALIFG
        case USCI_I2C_UCNACKIFG: break;               // NACKIFG
        case USCI_I2C_UCSTTIFG: break;                // STTIFG
        case USCI_I2C_UCSTPIFG: break;                // STPIFG
        case USCI_I2C_UCRXIFG3: break;                // RXIFG3
        case USCI_I2C_UCTXIFG3: break;                // TXIFG3
        case USCI_I2C_UCRXIFG2: break;                // RXIFG2
        case USCI_I2C_UCTXIFG2: break;                // TXIFG2
        case USCI_I2C_UCRXIFG1: break;                // RXIFG1
        case USCI_I2C_UCTXIFG1: break;                // TXIFG1
        case USCI_I2C_UCRXIFG0:                       // RXIFG0
            RXByteCtr--;                              // Decrement RX byte counter
            if (RXByteCtr)
            {
                *PRxData++ = UCB0RXBUF;               // Move RX data to address PRxData
                if (RXByteCtr == 1)                   // Only one byte left?
                    UCB0CTLW0 |= UCTXSTP;             // Generate I2C stop condition
            }
            else
            {
                *PRxData = UCB0RXBUF;                 // Move final RX data to PRxData
                __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
            }
            break;
        case USCI_I2C_UCTXIFG0: break;                // TXIFG0
        case USCI_I2C_UCBCNTIFG: break;               // CNTIFG
        case USCI_I2C_UCCLTOIFG: break;               // LTOIFG
        case USCI_I2C_UCBIT9IFG: break;               // BIT9IFG
        default: break;
    }
}

