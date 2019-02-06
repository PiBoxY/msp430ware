//******************************************************************************
//  MSP430AFE25x Demo - Software Toggle P1.0, MCLK = VLO/8
//
//  Description; Pulse P1.1 with 50% active duty cycle using software.
//  Toggle rate is approx 1/10 of VLO frequency with 50% duty cycle 
//  
//  ACLK = VL0, MCLK = VLO/10 ~ 1kHz, SMCLK = default DCO ~1.1MHz
//
//             MSP430AFE25x
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |       P1.0/SMCLK|-->LED/SMCLK
//            |             P1.1|-->MCLK = ~VLO/10
//
//  K. Naveen/ K. Venkat
//  Texas Instruments, Inc
//  March 2011
//  Built with IAR Embedded Workbench Version: 5.20.1
//******************************************************************************
#include "msp430.h"

void main(void)
{
      WDTCTL = WDTPW + WDTHOLD;          // Stop WDT
      P1SEL |= BIT0;                     // Set SMCLK at P1.0
      P1SEL2 |= BIT0;                    // Set SMCLK at P1.0
      P1DIR |= BIT0+BIT1;                // P1.1 and P1.0 = output direction
      BCSCTL3 |= LFXT1S_2;               // LFXT1 = VLOCLK
      BCSCTL2 |= SELM_3;                 // MCLK = VLO
      for (;;)
        P1OUT ^= BIT1;                   // P1.1 Toggle
}
