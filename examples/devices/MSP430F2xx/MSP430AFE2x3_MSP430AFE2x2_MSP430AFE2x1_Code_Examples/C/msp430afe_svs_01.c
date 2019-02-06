//*******************************************************************************
//   MSP430AFE25x Demo - SVS, POR @ 2.5V Vcc
//
//  Description: The SVS POR feature is used to disable normal operation that
//  toggles P1.0 by xor'ing P1.0 inside of a software loop.
//  In the example, when VCC is above 2.5V, the MSP430 toggles P1.0. When VCC is
//  below 2.5V, the SVS resets the MSP430, and no toggle is seen.
//  ACLK= n/a, MCLK= SMCLK= default DCO 
//  
//
//                MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
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
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // Set P1.0 to output direction
  SVSCTL = 0x60 + PORON;                    // SVS POR enabled @ 2.5V

  for (;;)
  {
    volatile unsigned int i;
    i = 50000;                              // Delay
    do (i--);
    while (i != 0);
    P1OUT ^= BIT0;                          // Toggle P1.0 using exclusive-OR
  }
}
