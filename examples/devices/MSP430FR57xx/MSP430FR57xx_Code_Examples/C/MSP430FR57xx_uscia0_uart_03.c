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
//  MSP430FR57xx Demo - USCI_A0 UART echo at 9600 baud
//
//  Description: This demo echoes back characters received via a PC serial portt.
//  Note that level shifter hardware is needed to shift between RS232 and MSP
//  voltage levels.
//  The example code shows proper initialization of registers 
//  and interrupts to receive and transmit data.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1MHz
//
//                                
//                MSP430FR5739 
//                       
//             -----------------   
//       RST -|     P2.0/UCA0TXD|----> PC (echo)
//            |                 |    
//           -|                 |    
//            |     P2.1/UCA0RXD|<---- PC
//            |                 |  
//
//   P. Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with CCS V4 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>

unsigned int i;
unsigned char RXData = 0;
unsigned char TXData = 0;
unsigned char check = 0;
int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // stop watchdog
 
  // XT1 Setup 
  
  PJSEL0 |= BIT4 + BIT5; 
  
  CSCTL0_H = 0xA5;
  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers 
  CSCTL4 |= XT1DRIVE_0; 
  CSCTL4 &= ~XT1OFF;
  
  do
  {
    CSCTL5 &= ~XT1OFFG;
                                            // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG; 
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
  // Configure UART pins
  P2SEL1 |= BIT0 + BIT1;
  P2SEL0 &= ~(BIT0 + BIT1);
  // Configure UART 0
  UCA0CTL1 |= UCSWRST; 
  UCA0CTL1 = UCSSEL_1;                      // Set ACLK = 32768 as UCBRCLK
  UCA0BR0 = 3;                              // 9600 baud
  UCA0BR1 = 0; 
  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
                                            // UCBRSx value = 0x53 (See UG)
  UCA0CTL1 &= ~UCSWRST;                     // release from reset
  UCA0IE |= UCRXIE;                         // Enable RX interrupt
  
   __bis_SR_register(LPM0_bits + GIE);      // LPM3 + Enable interrupt
}


// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,0x08))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;  
  }
}
