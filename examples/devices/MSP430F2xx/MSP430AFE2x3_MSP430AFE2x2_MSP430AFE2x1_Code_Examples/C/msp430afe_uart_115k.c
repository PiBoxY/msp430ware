//******************************************************************************
//   MSP430AFE25x Demo - USART0, 115200 UART Echo ISR, DCO SMCLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USART0 RX interrupt triggers TX Echo.
//   Baud rate divider with 1.1Mhz = 1.1MHz/115200 = ~10 (00Ah|0h)
//   MCLK = SMCLK = default DCO  
//
//                MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 | 
//          --|RST          XOUT|-
//            |                 |
//            |       P1.3/UTXD0|------------>
//            |                 | 115200 - 8N1
//            |       P1.4/URXD0|<------------
//
//  K. Naveen/ K. Venkat
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include "msp430.h"

void main(void)
{
  volatile unsigned int i;

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1SEL |= BIT3+BIT4;                       // P1.3,1.4 = USART0 TXD/RXD
  do
  {
      IFG1 &= ~OFIFG;                       // Clear OSCFault flag
      for (i = 0x47FF; i > 0; i--);         // Time for flag to set
  }
  while ((IFG1 & OFIFG));                   // OSCFault flag still set?

  ME1 |= UTXE0 + URXE0;                     // Enable USART0 TXD/RXD
  U0CTL |= CHAR;                            // 8-bit character
  U0TCTL |= SSEL1;                          // UCLK= SMCLK
  U0BR0 = 10;                               // 1MHz 115200
  U0BR1 = 0x00;                             // 1MHz 115200
  U0MCTL = 0x00;                            // 1MHz 115200 modulation
  U0CTL &= ~SWRST;                          // Initialize USART state machine
  IE1 |= URXIE0;                            // Enable USART0 RX interrupt
  P1SEL2 |= BIT0;                           // Set SMCLK at P1.0
  
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART0RX_VECTOR
__interrupt void USART0_RX (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART0RX_VECTOR))) USART0_RX (void)
#else
#error Compiler not supported!
#endif
{
  while (!(IFG1 & UTXIFG0));                // USART0 TX buffer ready?
  TXBUF0 = RXBUF0;                          // RXBUF0 to TXBUF0
}
