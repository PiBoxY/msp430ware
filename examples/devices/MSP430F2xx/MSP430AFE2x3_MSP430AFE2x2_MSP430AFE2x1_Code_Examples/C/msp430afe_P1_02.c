//******************************************************************************
//  MSP430AFE25x Demo - Software Port Interrupt Service on P1.4 from LPM4
//
//  Description: A hi/low transition on P1.4 will trigger P1_ISR which,
//  toggles P1.0. Normal mode is LPM4 ~ 0.1uA. LPM4 current can be measured
//  with the LED removed, all unused P1.x/P2.x configured as output or inputs
//  pulled high or low, and ensure the P1.4 interrupt input does not float.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430AFE25x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P1.4         P1.0|-->LED
//     \|/
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
  P1IE |= BIT4;                             // P1.4 interrupt enabled
  P1IES |= BIT4;                            // P1.4 Hi/lo edge
  P1IFG &= ~BIT4;                           // P1.4 IFG cleared

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= BIT0;                            // P1.0 = toggle
  P1IFG &= ~BIT4;                           // P1.4 IFG cleared
}
