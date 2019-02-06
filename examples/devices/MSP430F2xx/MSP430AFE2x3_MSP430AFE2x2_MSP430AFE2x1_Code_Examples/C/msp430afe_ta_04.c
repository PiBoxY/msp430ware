//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, Toggle P1.0, Overflow ISR, VLO ACLK 
//
//  Description: Toggle P1.0 using software and the Timer_A overflow ISR.
//  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is ~0.2Hz. Proper use of the TAIV interrupt
//  vector generator is demonstrated.
//  ACLK = VLO , MCLK = SMCLK = default DCO
//
//          MSP430AFE25x
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
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLOCLK
  TACTL = TASSEL_1 + MC_2 + TAIE;           // ACLK, contmode, interrupt

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
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
    case  2:  break;                        // CCR1 not used
    case  4:  break;                        // CCR2 not used
    case 10:  P1OUT ^= BIT0;                // overflow
              break;
  }
}

