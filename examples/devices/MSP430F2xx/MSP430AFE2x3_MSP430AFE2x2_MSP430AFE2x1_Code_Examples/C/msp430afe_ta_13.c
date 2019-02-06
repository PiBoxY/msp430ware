//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, Toggle P1.2/TA0, Up/Down Mode, DCO SMCLK
//
//  Description: Toggle P1.2 using hardware TA0 output. Timer_A is configured
//  for up/down mode with CCR0 defining period, TA0 also output on P1.2. In
//  this example, CCR0 is loaded with 250 and TA0 will toggle P1.2 at
//  TACLK/2*250. Thus the output frequency on P1.2 will be the TACLK/1000.
//  No CPU or software resources required.
//  ACLK = n/a, SMCLK = MCLK = TACLK = default DCO
//  As coded with TACLK = SMCLK, P1.2 output frequency is ~1000000/1000
//
//           MSP430AFE25x
//         -----------------
//     /|\|              XIN|-
//      | |                 |
//      --|RST          XOUT|-
//        |                 |
//        |         P1.2/TA0|--> SMCLK/1000
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
  P1DIR |= BIT2;                            // P1.2 output
  P1SEL |= BIT2;                            // P1.2 option select

  CCTL0 = OUTMOD_4;                         // CCR0 toggle mode
  CCR0 = 250;  
  TACTL = TASSEL_2 + MC_3;                  // SMCLK, up-downmode

  __bis_SR_register(CPUOFF);                // CPU off
}
