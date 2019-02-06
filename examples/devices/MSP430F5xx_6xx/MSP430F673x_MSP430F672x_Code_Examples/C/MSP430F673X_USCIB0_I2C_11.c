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
//  MSP430F673x Demo - USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
//
//  Description: This demo connects two MSP430's via the I2C bus. The slave
//  transmits to the master. This is the slave code. The interrupt driven
//  data transmission is demonstrated using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
//
//  Note that the UCB0SDA and UCB0SCL lines are not assigned to a dedicated port
// and need to be assigned to PMAP ports
//
//     ***to be used with "MSP430F673X_USCIB0_I2C_10.c" ***
//
//                                /|\  /|\
//               MSP430F673x      10k  10k     MSP430F673x
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//            |     P2.1/UCB0SDA|<-|----+->|P2.1/UCB0SDA     |
//            |                 |  |       |                 |
//            |                 |  |       |                 |
//            |     P2.0/UCB0SCL|<-+------>|P2.0/UCB0SCL     |
//            |                 |          |                 |
//
//  M. Swanson
//  Texas Instruments Inc.
//  December 2011
//  Built with CCS Version: 5.1.0 and IAR Embedded Workbench Version: 5.40.1
//******************************************************************************
#include <msp430.h>

unsigned char *PTxData;                     // Pointer to TX data
const unsigned char TxData[] =              // Table of data to transmit
{
    0x11,
    0x22,
    0x33,
    0x44,
    0x55
};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Setup P2.0 UCB0SCL, P2.1 UCB0SDA
    P2SEL |= BIT0 | BIT1;                   // Set P2.0,P2.1 to UCB0SCL, UCB0SDA

    // Seutp eUSCI_B0
    UCB0CTLW0 |= UCSWRST;                   // Enable SW reset
    UCB0CTLW0 = UCMODE_3;                   // I2C Slave
    UCB0I2COA0 = 0x48 | UCOAEN;             // Own Address is 048h
    UCB0CTLW0 &= ~UCSWRST;                  // Clear SW reset, resume operation
    UCB0IE |= UCSTPIE | UCSTTIE;            // Enable STT and STP interrupt
                                            // Enable TX interrupt in STT ISR

    while (1)
    {
        PTxData = (unsigned char *)TxData;  // Start of TX buffer

        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0, enable interrupts
                                            // Remain in LPM0 until master
                                            // finishes RX
        __no_operation();                   // For debugger
    }
}

//------------------------------------------------------------------------------
// The USCI_B0 data ISR TX vector is used to move data from MSP430 memory to the
// I2C master. PTxData points to the next byte to be transmitted, and TXByteCtr
// keeps track of the number of bytes transmitted.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// The USCI_B0 state ISR TX vector is used to wake up the CPU from LPM0 in order
// to do processing in the main program after data has been transmitted. LPM0 is
// only exited in case of a (re-)start or stop condition when actual data
// was transmitted.
//------------------------------------------------------------------------------
// USCI_B0 interrupt service routine
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
        case USCI_NONE: break;                    // No interrupts
        case USCI_I2C_UCALIFG: break;             // ALIFG
        case USCI_I2C_UCNACKIFG: break;           // NACKIFG
        case USCI_I2C_UCSTTIFG:                   // STTIFG
            UCB0IE |= UCTXIE0;                    // Enable TX interrupt
            UCB0IFG &= ~UCSTTIFG;                 // Clear start condition int flag
            break;
        case USCI_I2C_UCSTPIFG:                   // STPIFG
            UCB0IFG &= ~UCSTPIFG;                 // Clear stop condition int flag
            __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 if data was transmitted
            break;
        case USCI_I2C_UCRXIFG3: break;            // RXIFG3
        case USCI_I2C_UCTXIFG3: break;            // TXIFG3
        case USCI_I2C_UCRXIFG2: break;            // RXIFG2
        case USCI_I2C_UCTXIFG2: break;            // TXIFG2
        case USCI_I2C_UCRXIFG1: break;            // RXIFG1
        case USCI_I2C_UCTXIFG1: break;            // TXIFG1
        case USCI_I2C_UCRXIFG0: break;            // RXIFG0
        case USCI_I2C_UCTXIFG0:                   // TXIFG0
            UCB0TXBUF = *PTxData++;               // Transmit data at address PTxData
            break;
        case USCI_I2C_UCBCNTIFG: break;           // CNTIFG
        case USCI_I2C_UCCLTOIFG: break;           // LTOIFG
        case USCI_I2C_UCBIT9IFG: break;           // BIT9IFG
        default: break;
    }
}

