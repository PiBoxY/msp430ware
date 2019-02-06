//******************************************************************************
//  MSP430AFE25x Demo - WDT+ Failsafe Clock, DCO SMCLK
//
//  Description; Allow WDT+ in watchdog to timeout. Toggle P1.0 in main
//  funnction. LPM4 is entered, this example will demonstrate WDT+ feature
//  of preventing WDT+ clock to be disabled.
//  The WDT+ will not allow active WDT+ clock to be disabled by software, the
//  LED continues to Flash because the WDT times out normally even though
//  software has attempted to disable WDT+ clock source.
//  The MSP430AFE25x will stop code execution when software disables WDT+ clock
//  source.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//		  MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

void main(void)
{
  P1DIR |= BIT0;                            // Set P1.0 to output
  P1OUT ^= BIT0;                            // Toggle P1.0
  __bis_SR_register(LPM4_bits);             // Stop all clocks
}
