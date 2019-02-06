//*******************************************************************************
//  MSP430AFE25x Demo - SVS, Toggle port 1.0 on Vcc < 2.8V
//
//  Description: The SVS feature is used to monitor VCC. LED toggles when Vcc 
//  drops below reference threshold of 2.8V.SVS flags need to be reset in
//  software.
//  
//  ACLK= n/a, MCLK= SMCLK= default DCO ~ 1.1MHz
//
//               MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//            |                 |
//
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

int i;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= BIT0;                            // P1.0 - Set as output
  SVSCTL = VLD3;                            // SVS enabled @ 2.8V, no POR
  while(1)
  {   
    while(SVSCTL&SVSFG)
    {
      P1OUT ^= BIT0;                        // Toggle LED
      for (i = 0; i < 0x4000; i++);         // SW Delay
    }
  }
}
