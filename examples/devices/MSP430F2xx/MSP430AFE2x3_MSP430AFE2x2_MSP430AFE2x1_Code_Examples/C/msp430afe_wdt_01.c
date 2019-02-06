//******************************************************************************
//  MSP430AFE25x Demo - WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software timed by the WDT ISR. Toggle rate
//  is approximately 30ms based on default DCO/SMCLK clock source
//  used in this example for the WDT.
//  ACLK = n/a, MCLK = SMCLK = default
//
//		 MSP430AFE25x
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
  WDTCTL = WDT_MDLY_32;                     // Set Watchdog Timer interval to ~30ms
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR |= BIT0;                            // Set P1.0 to output direction

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= BIT0;                            // Toggle P1.0 using exclusive-OR
}

