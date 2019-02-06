//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, PWM TA1, Up/Down Mode, DCO SMCLK
//
//  Description: This program generates one PWM output on P1.1 using
//  Timer_A configured for up/down mode. The value in CCR0, 128, defines the PWM
//  period/2 and the value in CCR1 the PWM duty cycles.
//  A 75% duty cycle is on P1.1.
//  SMCLK = MCLK = TACLK = default DCO
//
//               MSP430AFE25x
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.1/TA1|--> CCR1 - 75% PWM
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
  P1DIR |= BIT3 + BIT1;                     // P1.1 and P1.3 output
  P1SEL |= BIT3 + BIT1;                     // P1.1 and P1.3 TA1/2 options
  CCR0 = 128;                               // PWM Period/2
  CCTL1 = OUTMOD_6;                         // CCR1 toggle/set
  CCR1 = 32;                                // CCR1 PWM duty cycle
  TACTL = TASSEL_2 + MC_3;                  // SMCLK, up-down mode

  __bis_SR_register(LPM0_bits);             // Enter LPM0
}


