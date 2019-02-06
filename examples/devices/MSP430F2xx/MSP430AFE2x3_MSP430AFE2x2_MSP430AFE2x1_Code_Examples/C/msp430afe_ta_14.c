//******************************************************************************
//  MSP430AFE25x Demo - Timer_A, Toggle P1.2/TA0, Up/Down Mode, VLO ACLK 
//
//  Description: Toggle P1.2 using hardware TA0 output. Timer_A is configured
//  for up/down mode with CCR0 defining period, TA0 also output on P1.2. In
//  this example, CCR0 is loaded with 5 and TA0 will toggle P1.2 at TACLK/2*5.
//  Thus the output frequency on P1.2 will be the TACLK/20. No CPU or software
//  resources required. Normal operating mode is LPM3.
//  ACLK = VLO, MCLK = default DCO
//  As coded with TACLK = ACLK, P1.2 output frequency = 12 Khz/20 = ~600 Hz
//
//            MSP430AFE25x
//         -----------------
//     /|\|              XIN|-
//      | |                 | 
//      --|RST          XOUT|-
//        |                 |
//        |         P1.2/TA0|--> ACLK/20
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
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLOCLK
  CCTL0 = OUTMOD_4;                         // CCR0 toggle mode
  CCR0 = 5;
  TACTL = TASSEL_1 + MC_3;                  // ACLK, up-downmode

  __bis_SR_register(LPM3_bits);             // Enter LPM3
}
