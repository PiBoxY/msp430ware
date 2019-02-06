//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, Toggle P1.0, CCR1 Cont. Mode ISR, DCO SMCLK
//
//  Description: Toggle P1.0 using software and TA_1 ISR. Toggles every
//  50000 SMCLK cycles. SMCLK provides clock source for TACLK.
//  During the TA_1 ISR, P1.0 is toggled and 50000 clock cycles are added to
//  CCR0. TA_1 ISR is triggered every 50000 cycles. CPU is normally off and
//  used only during TA_ISR. Proper use of the TAIV interrupt vector generator
//  is demonstrated.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO
//
//           MSP430AFE25x
//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  K. Naveen
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include <msp430.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= BIT0;                            // P1.0 output
  CCTL1 = CCIE;                             // CCR1 interrupt enabled
  CCR1 = 50000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, Contmode

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

// Timer_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERA1_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERA1_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  switch( TAIV )
  {
  case  2:                                  // CCR1
    {
    P1OUT ^= BIT0;                          // Toggle P1.0
    CCR1 += 50000;                          // Add Offset to CCR1
    }
           break;
  case  4: break;                           // CCR2 not used
  case 10: break;                           // overflow not used
 }
}

