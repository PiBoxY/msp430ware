//******************************************************************************
// MSP430AFE25x Demo - Basic Clock, LPM3 Using WDT ISR, VLO ACLK
//
//   Description: This program operates MSP430 normally in LPM3, pulsing P1.0
//   ~ 5 second intervals. WDT ISR used to wake-up system. 
//   ACLK = VLO/2, MCLK = SMCLK = default DCO ~1.1MHz
//
//                MSP430AFE25x
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
#include "msp430.h"

volatile unsigned int i;

void main(void)
{
  WDTCTL = WDT_ADLY_1000;                   // WDT 1s*4 interval timer, ACLK
  BCSCTL1 |= DIVA_1;                        // ACLK/2
  BCSCTL3 |= LFXT1S_2;                      // ACLK = VLO
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT |= 0x03;                            // All P1.x reset
  while(1)
  {
    __bis_SR_register(LPM3_bits + GIE);     // Enter LPM3, enable interrupts
    P1OUT |= BIT0;                          // Set P1.0 LED on
    for (i = 20000; i > 0; i--);            // Delay
    P1OUT &= ~BIT0;                         // Clear P1.0 LED off
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}
