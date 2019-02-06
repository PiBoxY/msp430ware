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
//  MSP430x26x Demo - USCI_B0 to USCI_B1 I2C RX and TX single bytes
//
//  Description: This demo connects both USCI_B channels on one MSP430 via the 
//  I2C bus. The master (B0) reads from the slave(B1), then the slave (B1) reads
//  from the master (B0). 
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
//                                /|\              /|\
//               MSP430F261x      10k              10k     
//             -----------------   |                |
//           -|XIN  P5.1/UCB0SDA|<-|---+----|       |
//            |                 |           |       |
//           -|XOUT             |           |       |
//            |     P5.2/UCB0SCL|<-+------- | ------|
//            |                 |           |       |
//            |     P3.1/UCB0SDA|<----------|       |
//            |                 |                   |
//            |                 |                   |
//            |     P3.2/UCB0SCL|<------------------|
//            |                 |        
//
//  R. B. Elliott / H. Grewal
//  Texas Instruments Inc.
//  January 2008
//  Built with IAR Embedded Workbench Version: 3.42A
//******************************************************************************
#include <msp430.h>


unsigned char RXData, Master_RX, Slave_RX, TXByteCtr = 0;

void Slave_TX_to_RX_Master(void);
void Master_TX_to_RX_Slave(void);
void SlaveUSCIB1TX (void);
void SlaveUSCIB1RX (void);
void MasterUSCIB0TX (void);
void MasterUSCIB0RX (void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
  P5SEL |= 0x06;                            // Assign I2C pins to USCI_B1
  
   Master_TX_to_RX_Slave();
 __bic_SR_register(GIE);
  
  Slave_TX_to_RX_Master();
  __bic_SR_register(GIE);
  
 Master_TX_to_RX_Slave();
 __bic_SR_register(GIE);
    
while(1);                                   // SET BREAKPOINT HERE

}

// USCI_B0 Data ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if (Master_RX == 1)
  {
  RXData = UCB0RXBUF;                       // Get RX data
  __bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
  }
  else{
    if (TXByteCtr)                          // Check TX byte counter
  {
    UCB0TXBUF = 0x22;                       // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter
  }
  else
  {
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
  }
}

// USCI_B1 Data ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB1TX_VECTOR
__interrupt void USCIAB1TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB1TX_VECTOR))) USCIAB1TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  if (Slave_RX == 1) RXData = UCB1RXBUF;    //for slave RX
  else  UCB1TXBUF = 0x55;                   // TX data
  //__bic_SR_register_on_exit(CPUOFF);        
}

// USCI_B1 State ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB1RX_VECTOR
__interrupt void USCIAB1RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB1RX_VECTOR))) USCIAB1RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  UCB1STAT &= ~(UCSTPIFG + UCSTTIFG);       // Clear start condition int flag

}

void Slave_TX_to_RX_Master(void){
  SlaveUSCIB1TX();                          // Set up USCI B1 as slave TX
  MasterUSCIB0RX();                         // Set up USCI B1 as slave TX

   __bis_SR_register(GIE);
  
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
  
}

void Master_TX_to_RX_Slave(void){

    SlaveUSCIB1RX();
    MasterUSCIB0TX();
     
    __bis_SR_register(GIE);
    TXByteCtr = 1;                          // Load TX byte counter
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until dat is TX'ed
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

}

void SlaveUSCIB1TX (void){
  
  UC1IE &= ~UCB1RXIE;                       // Disable RX interrupt
  UCB1CTL1 |= UCSWRST;                      // Enable SW reset 
  UCB1CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB1I2COA = 0x48;                         // Own Address is 048h
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  Slave_RX = 0;
  UCB1I2CIE  |= UCSTPIE + UCSTTIE;          // Enable STT and STP interrupt
  UC1IE |= UCB1TXIE;                        // Enable TX interrupt
}

void SlaveUSCIB1RX (void){
  UCB1I2CIE &= ~(UCSTPIE + UCSTTIE);        // Disable STT and STP interrupt
  UC1IE &= ~UCB1TXIE;                       // Disable TX interrupt
  
  UCB1CTL1 |= UCSWRST;                      // Enable SW reset
  UCB1CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB1I2COA = 0x48;                         // Own Address is 048h
  UCB1CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  Slave_RX = 1;
  UC1IE |= UCB1RXIE;                        // Enable RX interrupt
}

void MasterUSCIB0TX (void){
  
  IE2 &= ~UCB0RXIE;                         // disable RX interrupt
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x48;                         // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  Master_RX = 0;
  IE2 |= UCB0TXIE;                          // Enable TX interrupt
}  


void MasterUSCIB0RX (void){
  IE2 &= ~UCB0TXIE;                         // disable TX interrupt
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x048;                        // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  Master_RX = 1;
  IE2 |= UCB0RXIE;                          // Enable RX interrupt
}


