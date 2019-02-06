//******************************************************************************
//  MSP430AFE25x Demo - Basic Clock, Configure RST/NMI as NMI
//
//  Description: Configure RST/NMI as NMI, hi/lo edge. Flash P1.0  inside of
//  NMI_ISR if NMI occurs. General enable interrupt in status register does
//  not need to be set for NMI. NMIIE does need to be reset, as NMI_ISR
//  automatically clears NMI enable to prevent unintentional stack overflow
//  that could result from, bounce or uncontrolled NMI's.
//  ACLK = n/a, MCLK = SMCLK = DCO 
//
//              MSP430AFE25x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |            P1.0 |-->LED 
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD + WDTNMI + WDTNMIES;  // WDT off NMI hi/lo
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  P1OUT &= ~BIT0;                           // Clear P1.0 LED off
  IE1 |= NMIIE;                             // Enable NMI

  __bis_SR_register(LPM0_bits);             // Enter LPM0
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=NMI_VECTOR
__interrupt void nmi_ (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(NMI_VECTOR))) nmi_ (void)
#else
#error Compiler not supported!
#endif
{
  volatile unsigned int i;
  P1OUT |= BIT0;                            // Set P1.0 LED on
  for (i = 20000; i > 0; i--);              // Delay
  P1OUT &= ~BIT0;                           // Clear P1.0 LED off
  IFG1 &= ~NMIIFG;                          // Reclear NMI flag in case bounce
  IE1 |= NMIIE;                             // Enable NMI
}
