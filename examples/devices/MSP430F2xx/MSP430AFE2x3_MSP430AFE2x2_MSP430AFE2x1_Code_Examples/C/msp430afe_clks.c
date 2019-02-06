//******************************************************************************
//  MSP430AFE25x Demo - Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10
//
//  Description: Buffer SMCLK(DCO) on P1.0, MCLK/10 on P1.1 
//  ACLK =n/a, MCLK = SMCLK = Default DCO
//
//               MSP430AFE25x
//             -----------------
//         /|\|                   XIN|-
//          | |                      | 
//          --|RST               XOUT|-
//            |                      |
//            |            P1.0/SMCLK|-->SMCLK = DCO
//            |                  P1.1|-->MCLK/10 = DCO/10
//           
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

void main(void)
{
  WDTCTL = WDTPW +WDTHOLD;      // Stop Watchdog Timer
  P1DIR |= BIT0 + BIT1;         // P1.0, P1.1 outputs
  P1SEL |= BIT0;                // SMCLK Pin Function
  P1SEL2 |= BIT0;
  
  while(1)                                  
  {
    P1OUT |= BIT1;    	        // P1.1 = 1
    P1OUT &= ~BIT1;             // P1.1 = 0
  }
}

