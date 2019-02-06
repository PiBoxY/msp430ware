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
//  MSP430F22x4 Demo - USCI_B0 I2C Master TX/RX multiple bytes from MSP430 Slave
//                     with a repeated start in between TX and RX operations.
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave, then a repeated start is generated followed by a
//  receive operation. This is the master code. This code demonstrates how to
//  implement an I2C repeated start with the USCI module using polling for a TX
//  operation and the USCI_B0 TX interrupt for an RX operation
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~8MHz (for SCL = 400 kHz).
//
//  Note: This example is specific for transmitting one byte and receiving two bytes
//  Transmitting more bytes needs to modify some lines of code
//    ***to be used with msp430x22x4_uscib0_i2c_13.c***
//
//                                /|\  /|\
//               MSP430F24x      10k  10k     MSP430F22x4
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P3.1/UCB0SDA|<-|---+->|P3.1/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P3.2/UCB0SCL|<-+----->|P3.2/UCB0SCL     |
//            |                 |         |                 |
//
//  F. Chen/D. Szmulewicz
//  Texas Instruments Inc.
//  Jan. 2013
//  Built with CCS Version: 5.3.0.00090
//******************************************************************************
#include <msp430.h>

#define NUM_BYTES_TX 1                      // Bytes to write, 
                                            //this value must equal to "NUM_BYTES" that define in msp430x22x4_uscib0_i2c_13.c
#define NUM_BYTES_RX 2                      // Bytes to read

int RXByteCtr, RPT_Flag = 0;                // enables repeated start when 1
volatile unsigned int counter = 0;
volatile unsigned char RxBuffer[128];       // Allocate 128 byte of RAM
unsigned char *PTxData;                     // Pointer to TX data
unsigned char *PRxData;                     // Pointer to RX data
unsigned char TXByteCtr;
unsigned char MSData = 0x55;

void Setup_TX(void);
void Setup_RX(void);
void Transmit(void);
void Receive(void);
void I2CWriteInit(void);
void I2CReadInit(void);

int main(void)
{
    unsigned int j;

    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
    if (CALBC1_8MHZ==0xFF)                  // If calibration constant erased
    {
        while(1);                           // do not load, trap CPU!!
    }
    DCOCTL = 0;                             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ;                  // Set DCO to 8MHz. This is needed for SCL = 400 KHz
    DCOCTL = CALDCO_8MHZ;

    P3SEL |= 0x06;                          // Assign I2C pins to USCI_B0

    UCB0CTL1 |= UCSWRST;                    // Enable SW reset
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;   // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 | UCSWRST;          // Use SMCLK, keep SW reset
    UCB0BR0 = 18;                           // fSCL = SMCLK/18 = ~400kHz
    UCB0BR1 = 0;
    UCB0I2CSA = 0x48;                       // Slave Address is 048h
    UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation


    while(1)
    {
/*---------------------------------------------------------------------------*/
//  Start communication with write operation. Send a start with address and
//  send NUM_BYTES_TX bytes.
/*---------------------------------------------------------------------------*/
        while (UCB0CTL1 & UCTXSTP);         // Ensure stop condition got sent
        I2CWriteInit();                     // Init I2C module for a write operation
        UCB0CTL1 |= UCTXSTT;                // start condition generation

        __delay_cycles(400);                // Delay. This is needed when running at 400 KHz

        for(j = 0; j < NUM_BYTES_TX; j++)   //  send NUM_BYTES_TX bytes.
        {
            if(UCB0TXIFG & IFG2)
            {
                UCB0TXBUF = MSData++;       // Load TX buffer
                IFG2 &= ~UCB0TXIFG;         // Clear USCI_B0 TX interrupt flag
                while (UCB0STAT & UCBUSY);  // wait until I2C module serve for this transmit,arbitration is not lost

               // __delay_cycles(150);          // Delay between two bytes

            }

        }

/*---------------------------------------------------------------------------*/
//  Send a RESTART condition with a Read operation. Send start with address and
//  read NUM_BYTES_RX bytes.
/*---------------------------------------------------------------------------*/
        PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
        counter=0;
        I2CReadInit();                      // Init I2C module for a read operation
        UCB0CTL1 |= UCTXSTT;                // start condition generation
        while(UCB0CTL1 & UCTXSTT);          // Start condition sent?

        __bis_SR_register(LPM0_bits | GIE); // Enter LPM0 w/ interrupts. Stay in LPM until all expected bytes are read

        while(UCB0CTL1 & UCTXSTP);          // Stop condition sent?
        __delay_cycles(50);                 // Delay between STOP and new START condition
    }
}


/*---------------------------------------------------------------------------*/
// Description:
//   Initialization of the I2C Module for Write operation.
/*---------------------------------------------------------------------------*/
void I2CWriteInit(void)
{
    UCB0CTL1 |= UCTR;                       // UCTR=1 => Transmit Mode (R/W bit = 0)
    IFG2 &= ~UCB0TXIFG;                     // Clear TXIFG flag
    IE2 &= ~UCB0RXIE;                       // disable Receive ready interrupt
    IE2 &= ~UCB0TXIE;                       // enable Transmit ready interrupt
}

/*----------------------------------------------------------------------------*/
// Description:
//   Initialization of the I2C Module for Read operation.
/*----------------------------------------------------------------------------*/
void I2CReadInit(void)
{
    UCB0CTL1 &= ~UCTR;                      // UCTR=0 => Receive Mode (R/W bit = 1)
    IFG2 &= ~UCB0RXIFG;                     // Clear RXIFG flag
    IE2 &= ~UCB0TXIE;                       // disable Transmit ready interrupt
    IE2 |= UCB0RXIE;                        // enable Receive ready interrupt
}

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any  number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(UCB0RXIFG & IFG2)                    // Make sure it is an RX interrupt
    {
        *PRxData++ = UCB0RXBUF;               // store received data in buffer
        counter++;                          // Increase counter = # of bytes received

        if(counter == NUM_BYTES_RX - 1)     // Stop condition must be sent before the last byte is read
        UCB0CTL1 |= UCTXSTP;

        if(counter == NUM_BYTES_RX)         // Last expected byte was read. Disable interrupts and clear flags
        {
            IFG2 &= ~UCB0RXIFG;             // Clear RXIFG flag
            IFG2 &= ~UCB0TXIFG;             // Clear TXIFG flag
            IE2 &= ~UCB0TXIE;               // Disable Transmit ready interrupt
            IE2 &= ~UCB0RXIE;               // Disable Receive ready interrupt
            counter = 0;                    // Reset counter for next RX operation
            __bic_SR_register_on_exit(LPM0_bits | GIE);   // Exit LPM0, we are done reading
        }
    }
}
